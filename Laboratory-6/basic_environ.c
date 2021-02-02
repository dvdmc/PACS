////////////////////////////////////////////////////////////////////
//File: basic_environ.c
//
//Description: base file for environment exercises with openCL
//
// 
////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <sys/stat.h>
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif
  
// check error, in such a case, it exits

void cl_error(cl_int code, const char *string){
	if (code != CL_SUCCESS){
		printf("%d - %s\n", code, string);
	    exit(-1);
	}
}
////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  int err;                            	// error code returned from api calls
  size_t t_buf = 50;			// size of str_buffer
  char str_buffer[t_buf];		// auxiliary buffer	
  size_t e_buf;				// effective size of str_buffer in use
	    
  size_t global_size;                      	// global domain size for our calculation
  size_t local_size;                       	// local domain size for our calculation

  const cl_uint num_platforms_ids = 10;				// max of allocatable platforms
  cl_platform_id platforms_ids[num_platforms_ids];		// array of platforms
  cl_uint n_platforms;						// effective number of platforms in use
  const cl_uint num_devices_ids = 10;				// max of allocatable devices
  cl_device_id devices_ids[num_platforms_ids][num_devices_ids];	// array of devices
  cl_uint n_devices[num_platforms_ids];				// effective number of devices in use for each platform
	
  cl_device_id device_id;             				// compute device id 
  cl_context context;                 				// compute context
  cl_command_queue command_queue;     				// compute command queue
    

  // 1. Scan the available platforms:
  err = clGetPlatformIDs (num_platforms_ids, platforms_ids, &n_platforms);
  cl_error(err, "Error: Failed to Scan for Platforms IDs");
  printf("Number of available platforms: %d\n\n", n_platforms);

  const char* attributeNames[6] = { "Name", "Vendor", "Version", "Profile", "Extensions", "Timer resolution"};
  const cl_platform_info attributeTypes[6] = {CL_PLATFORM_NAME, CL_PLATFORM_VENDOR,
                                              CL_PLATFORM_VERSION, 
                                              CL_PLATFORM_PROFILE, CL_PLATFORM_EXTENSIONS,
                                              CL_PLATFORM_HOST_TIMER_RESOLUTION };
  char* buff_attr;
  size_t buff_size;
  for (int i = 0; i < n_platforms; i++) {
      for (int j = 0; j < 5; j++) {
          // Attribute size
          err = clGetPlatformInfo(platforms_ids[i], attributeTypes[j], 0, NULL, &buff_size);
          cl_error (err, "Error: Failed to get info of the platform\n");
          buff_attr = (char*) malloc(buff_size);
          // Value
          err = clGetPlatformInfo(platforms_ids[i], attributeTypes[j], buff_size, buff_attr, NULL);
          cl_error (err, "Error: Failed to get info of the platform\n");
          printf("\t[%d]-%s: %s\n", i, attributeNames[j], buff_attr); 
          free(buff_attr);
      }
      cl_ulong resolution;
      err = clGetPlatformInfo(platforms_ids[i], attributeTypes[5], sizeof(resolution), &resolution, NULL);
      cl_error (err, "Error: Failed to get info of the platform\n");
      printf("\t[%d]-%s: %llu\n", 5, attributeNames[5], (unsigned long long)resolution); 
  }
  // for (int i = 0; i < n_platforms; i++ ){
  //   err= clGetPlatformInfo(platforms_ids[i], CL_PLATFORM_NAME, t_buf, str_buffer, &e_buf);
  //   cl_error (err, "Error: Failed to get info of the platform\n");
  //   printf( "\t[%d]-Platform Name: %s\n", i, str_buffer);
  // }
  printf("\n");
  // ***Task***: print on the screen the name, host_timer_resolution, vendor, versionm, ...
  // 2. Scan for devices in each platform
  for (int i = 0; i < n_platforms; i++ ){
    err = clGetDeviceIDs( platforms_ids[i], CL_DEVICE_TYPE_ALL, num_devices_ids, devices_ids[i], &(n_devices[i]));
    cl_error(err, "Error: Failed to Scan for Devices IDs");
    printf("\t[%d]-Platform. Number of available devices: %d\n", i, n_devices[i]);

    for(int j = 0; j < n_devices[i]; j++){
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_NAME, sizeof(str_buffer), &str_buffer, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device name");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_NAME: %s\n", i, j,str_buffer);

      cl_uint max_compute_units_available;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(max_compute_units_available), &max_compute_units_available, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device max compute units available");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_MAX_COMPUTE_UNITS: %d\n", i, j, max_compute_units_available);

      cl_ulong global_mem_cache_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(global_mem_cache_size), &global_mem_cache_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device size of global memory cache");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_GLOBAL_MEM_CACHE_SIZE: %llu\n", i, j, global_mem_cache_size);

      cl_ulong local_mem_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(local_mem_size), &local_mem_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device size of local memory");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_LOCAL_MEM_SIZE: %llu\n", i, j, local_mem_size);

      size_t max_work_group_size;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(max_work_group_size), &max_work_group_size, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device max work group size");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_MAX_WORK_GROUP_SIZE: %lu\n", i, j, max_work_group_size);

      size_t profiling_timer_resolution;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(profiling_timer_resolution), &profiling_timer_resolution, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device profiling timer resolution");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_PROFILING_TIMER_RESOLUTION: %lu\n", i, j, profiling_timer_resolution);

      cl_uint preferred_vector_width_int;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, sizeof(preferred_vector_width_int), &preferred_vector_width_int, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device preferred vector width int");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT: %d\n", i, j, preferred_vector_width_int);

      cl_uint preferred_vector_width_float;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, sizeof(preferred_vector_width_float), &preferred_vector_width_float, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device preferred vector width float");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT: %d\n", i, j, preferred_vector_width_float);

      cl_uint preferred_vector_width_double;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, sizeof(preferred_vector_width_double), &preferred_vector_width_double, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device preferred vector width double");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE: %d\n", i, j, preferred_vector_width_double);

      cl_uint native_vector_width_int;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_NATIVE_VECTOR_WIDTH_INT, sizeof(native_vector_width_int), &native_vector_width_int, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device native vector width int");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_NATIVE_VECTOR_WIDTH_INT: %d\n", i, j, native_vector_width_int);

      cl_uint native_vector_width_float;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT, sizeof(native_vector_width_float), &native_vector_width_float, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device native vector width float");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT: %d\n", i, j, native_vector_width_float);

      cl_uint native_vector_width_double;
      err = clGetDeviceInfo(devices_ids[i][j], CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE, sizeof(native_vector_width_double), &native_vector_width_double, NULL);
      cl_error(err, "clGetDeviceInfo: Getting device native vector width double");
      printf("\t\t [%d]-Platform [%d]-Device CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE: %d\n", i, j, native_vector_width_double);

    }
  }	
  printf("\n");
  // ***Task***: print on the screen the cache size, global mem size, local memsize, max work group size, profiling timer resolution and ... of each device


  // 3. Create a context, with a device
  cl_context_properties properties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platforms_ids[0], 0};
  context = clCreateContext(properties, n_devices[0], devices_ids[0], NULL, NULL, &err);
  cl_error(err, "Failed to create a compute context\n");

  // 4. Create a command queue
  cl_command_queue_properties proprt[] = { CL_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, 0 };
  command_queue = clCreateCommandQueueWithProperties( context, devices_ids[0][0], proprt, &err);
  cl_error(err, "Failed to create a command queue\n");

  // Calculate size of the file
  FILE *fileHandler = fopen("kernel.cl", "r");
  fseek(fileHandler, 0, SEEK_END);
  size_t fileSize = ftell(fileHandler);
  rewind(fileHandler);

  // read kernel source into buffer
  char * sourceCode = (char*) malloc(fileSize + 1);
  sourceCode[fileSize] = '\0';
  fread(sourceCode, sizeof(char), fileSize, fileHandler);
  fclose(fileHandler);

  // create program from buffer
  cl_program program = clCreateProgramWithSource(context, 1, (const char**)&sourceCode, &fileSize, &err);
  cl_error(err, "Failed to create program with source\n");
  free(sourceCode);


  // Build the executable and check errors
  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  if (err != CL_SUCCESS){
    size_t len = 30000;
    char buffer[30000];

    printf("Error: Some error at building process.\n");
    clGetProgramBuildInfo(program, devices_ids[0][0], CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
    printf("%s\n", buffer);
    exit(-1);
  }
  // Create a compute kernel with the program we want to run
  cl_kernel kernel = clCreateKernel(program, "pow_of_two", &err);
  cl_error(err, "Failed to create kernel from the program\n");

  cl_uint count = 4096;
  cl_float in[count], out[count];
  for (unsigned int i = 0; i < count ; i++){
    in[i] = i;
  }

  // Create OpenCL buffer visible to the OpenCl runtime
  cl_mem in_device_object  = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_float)*count, NULL, &err);
  cl_error(err, "Failed to create memory buffer at device\n");
  cl_mem out_device_object = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_float)*count, NULL, &err);
  cl_error(err, "Failed to create memory buffer at device\n");

  // Write date into the memory object 
  err = clEnqueueWriteBuffer(command_queue, in_device_object, CL_TRUE, 0, sizeof(cl_float) * count,
                             in, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a write command\n");
 
  // Set the arguments to the kernel
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &in_device_object);
  cl_error(err, "Failed to set argument 0\n");
  err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &out_device_object);
  cl_error(err, "Failed to set argument 1\n");
  err = clSetKernelArg(kernel, 2, sizeof(count), &count);
  cl_error(err, "Failed to set argument 2\n");

  // Launch Kernel
  local_size = 128;
  global_size = count;
  err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, NULL);
  cl_error(err, "Failed to launch kernel to the device\n");
 
  // Read data form device memory back to host memory
  err = clEnqueueReadBuffer(command_queue, out_device_object, CL_TRUE, 0, sizeof(cl_float)*count, &out, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a read command\n");

  // Check the correctness of the execution
  bool correct = true;
  for (unsigned int i = 0; i < count; i++){
    if (in[i]*in[i] != out[i]){
      printf("Wrong value at position %d", i);
      break;
    }
  }
  if (correct){
    printf("The execution was correct\n");
  }

  clReleaseMemObject(in_device_object);
  clReleaseMemObject(out_device_object);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  return 0;
}

