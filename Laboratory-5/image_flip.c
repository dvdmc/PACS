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
#include "CImg/CImg.h" // Path to the previous download directory
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif
  

using namespace cimg_library;

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
  int err; 
	    

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

  // ***Task***: print on the screen the name, host_timer_resolution, vendor, versionm, ...
  // 2. Scan for devices in each platform
  for (int i = 0; i < n_platforms; i++ ){
    err = clGetDeviceIDs( platforms_ids[i], CL_DEVICE_TYPE_ALL, num_devices_ids, devices_ids[i], &(n_devices[i]));
    cl_error(err, "Error: Failed to Scan for Devices IDs");
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
  FILE *fileHandler = fopen("flip.cl", "r");
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
    size_t len = 2048;
    char buffer[2048];

    printf("Error: Some error at building process.\n");
    clGetProgramBuildInfo(program, devices_ids[0][0], CL_PROGRAM_BUILD_LOG, len, buffer, NULL);
    printf("%s\n", buffer);
    exit(-1);
  }
  // Create a compute kernel with the program we want to run
  cl_kernel kernel = clCreateKernel(program, "img_flip", &err);
  cl_error(err, "Failed to create kernel from the program\n");

  CImg<unsigned char> img("image.jpg"); 

  img.display("Before flip");
  cl_uint rows = img.height();
  cl_uint columns = img.width();
  cl_uint spectrum = img.spectrum();

  // Create OpenCL buffer visible to the OpenCl runtime
  cl_mem img_buffer  = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_uchar)*rows*columns*spectrum, NULL, &err);
  cl_error(err, "Failed to create memory buffer at device\n");
  // Write date into the memory object 
  err = clEnqueueWriteBuffer(command_queue, img_buffer, CL_TRUE, 0, sizeof(cl_uchar)*rows*columns*spectrum,
                            img.data(), 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a write command\n");
  // Set the arguments to the kernel
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &img_buffer);
  cl_error(err, "Failed to set argument 0\n");
  err = clSetKernelArg(kernel, 1, sizeof(rows), &rows);
  cl_error(err, "Failed to set argument 1\n");
  err = clSetKernelArg(kernel, 2, sizeof(columns), &columns);
  cl_error(err, "Failed to set argument 2\n");
  err = clSetKernelArg(kernel, 3, sizeof(spectrum), &spectrum);
  cl_error(err, "Failed to set argument 3\n");
  // Launch Kernel
  size_t local_size[] = {16, 8, 1};
  size_t global_size[] = {rows, columns/2, spectrum};
  err = clEnqueueNDRangeKernel(command_queue, kernel, 3, NULL, global_size, local_size, 0, NULL, NULL);
  cl_error(err, "Failed to launch kernel to the device\n");
  // Read data form device memory back to host memory
  err = clEnqueueReadBuffer(command_queue, img_buffer, CL_TRUE, 0, sizeof(cl_uchar)*rows*columns*spectrum, img.data(), 0, NULL, NULL);
  // err = clEnqueueReadBuffer(command_queue, out_device_object, CL_TRUE, count*sizeof(cl_float), sizeof(float), &out, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a read command\n");
  img.display("After flip");

  clReleaseMemObject(img_buffer);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  return 0;
}

