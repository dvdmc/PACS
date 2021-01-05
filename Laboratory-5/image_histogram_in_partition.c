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
#include <iostream>
#include <chrono>
#include <iomanip>
#include <stdbool.h>
#include <sys/stat.h>
#include "CImg/CImg.h" // Path to the previous download directory
#ifdef __APPLE__
  #include <OpenCL/opencl.h>
#else
  #include <CL/cl.h>
#endif
  

using namespace cimg_library;
using namespace std;

// check error, in such a case, it exits

void cl_error(cl_int code, const char *string){
	if (code != CL_SUCCESS){
		printf("%d - %s\n", code, string);
	    exit(-1);
	}
}
////////////////////////////////////////////////////////////////////////////////

void draw_histograms(cl_int* hist, const int rows, const unsigned int columns, const unsigned int channels, 
const unsigned int bins){
  unsigned char color[3][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}};
  auto max = std::max_element(hist, hist + bins*channels);
  for (unsigned int i = 0; i < channels; i++){
    CImg<unsigned char> img(bins*4+10, *max/500, 1, 3); 
    //CImg<unsigned char> img(200, 200, 1, 3); 
    img.fill(0);
    unsigned int begin = bins*i;
    for (unsigned int j = 0; j < bins; j++){
      
      //if (hist[begin + j]< rows*columns && j%10 ==1){
         //printf("%d, %d\n", hist[begin + j], rows*columns);
         //printf("%d\n", hist[begin + j]);
         img.draw_line(5+4*j, *max/500 - hist[begin + j]/500, 5+4*j,*max/500, color[i]);
         //img.draw_line(10+4*j, 0, 14+4*j, hist[begin + j], color[i]);
     // }
    } 
    img.display("Histogram channel " + i);

  }
}

void usage(int argc, char** argv){
	if (argc != 2){
		printf("usage: %s <image_name>\n", argv[0]);
	  exit(-1);
	}
}

int main(int argc, char** argv)
{
  usage(argc, argv);
  char* image_file = argv[1];
  auto start = std::chrono::steady_clock::now();
  pid_t pid = getpid();
  char* cmd = (char*)malloc(100);
  sprintf(cmd, "/bin/ps -p %d -o size | /bin/tail -1 | /bin/tr \'\n\' \' \'", pid);
  char* mem_kernel = (char*)malloc(100);
  char* mem_end = (char*)malloc(100);
  size_t mem_size = 100;

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
  FILE *fileHandler = fopen("hist_in_partition.cl", "r");
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

  // Build the systemutable and check errors
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
  cl_kernel kernel = clCreateKernel(program, "histogram", &err);
  cl_error(err, "Failed to create kernel from the program\n");

  CImg<unsigned char> img(image_file); 
  cl_uint rows = img.height();
  cl_uint columns = img.width();
  cl_uint channels = img.spectrum();
  cl_uint channel_size = rows*columns;
  cl_uint size = rows*columns*channels;
  cl_uchar* img_array = (cl_uchar*) malloc(rows*columns*(channels+1));
  cimg_forXYC(img,x,y,c) {  // Do 3 nested loops
     img_array[columns*y*4 + 4*x + c] = (unsigned char)img(x, y, c);
  }
  cl_uint n_bins = 256;
  cl_int hist[256*3] = {0};

  auto before_allocation = std::chrono::steady_clock::now();
  auto start_allocation = std::chrono::steady_clock::now();
  // Create OpenCL buffer visible to the OpenCl runtime
  cl_mem img_buffer  = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(cl_uint)*channel_size, NULL, &err);
  cl_error(err, "Failed to create memory buffer at device\n");
  cl_mem hist_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(cl_int)*n_bins*channels, NULL, &err);
  cl_error(err, "Failed to create memory buffer at device\n");
  clFinish(command_queue);
  auto end_allocation = std::chrono::steady_clock::now();

  auto start_copy = std::chrono::steady_clock::now();
  // Write date into the memory object 
  err = clEnqueueWriteBuffer(command_queue, img_buffer, CL_TRUE, 0, sizeof(cl_uint)*channel_size,
                            img_array, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a write command\n");
  err = clEnqueueWriteBuffer(command_queue, hist_buffer, CL_TRUE, 0, sizeof(cl_int)*n_bins*channels,
                            hist, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a write command\n");
  clFinish(command_queue);
  auto end_copy = std::chrono::steady_clock::now();

  cl_uint num_groups = 16;
  cl_uint group_size = 128*4;

  // Set the arguments to the kernel
  err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &img_buffer);
  cl_error(err, "Failed to set argument 0\n");
  err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &hist_buffer);
  cl_error(err, "Failed to set argument 1\n");
  err = clSetKernelArg(kernel, 2, sizeof(num_groups), &num_groups);
  cl_error(err, "Failed to set argument 2\n");
  err = clSetKernelArg(kernel, 3, sizeof(group_size), &group_size);
  cl_error(err, "Failed to set argument 3\n");
  err = clSetKernelArg(kernel, 4, sizeof(channel_size), &channel_size);
  cl_error(err, "Failed to set argument 4\n");
  err = clSetKernelArg(kernel, 5, 256*3*sizeof(unsigned int), NULL);
  cl_error(err, "Failed to set argument 5\n");

  // Launch Kernel
  size_t local_size = group_size;
  size_t global_size = group_size*num_groups;
  cl_event kernel_event;
  auto start_kernel = std::chrono::steady_clock::now();
  err = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_size, &local_size, 0, NULL, &kernel_event);
  cl_error(err, "Failed to launch kernel to the device\n");
  clFinish(command_queue);
  auto end_kernel = std::chrono::steady_clock::now();
  FILE* fp = popen(cmd, "r");
  getline(&mem_kernel, &mem_size, fp);
  pclose(fp);

  // Read data form device memory back to host memory
  auto start_merge = std::chrono::steady_clock::now();
  err = clEnqueueReadBuffer(command_queue, hist_buffer, CL_TRUE, 0, sizeof(cl_int)*n_bins*channels, hist, 0, NULL, NULL);
  cl_error(err, "Failed to enqueue a read command\n");
  
  clFinish(command_queue);
  auto end_merge = std::chrono::steady_clock::now();
  cl_ulong time_start;
  cl_ulong time_end;
  clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
  clGetEventProfilingInfo(kernel_event, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);


  //draw_histograms(hist, rows, columns, channels, n_bins);

  clReleaseMemObject(img_buffer);
  clReleaseMemObject(hist_buffer);
  clReleaseProgram(program);
  clReleaseKernel(kernel);
  clReleaseCommandQueue(command_queue);
  clReleaseContext(context);

  fp = popen(cmd, "r");
  getline(&mem_end, &mem_size, fp);
  pclose(fp);

  // auto stop = std::chrono::steady_clock::now();
  // std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::nanoseconds>(stop-start);
  // std::cout << std::setprecision(6) << time.count()/1000000.0 << std::endl;

  double nanoSeconds = time_end - time_start;
  // printf("%0.3f\n", nanoSeconds/1000000.0);
  // printf("Kernel time: %0.3f miliseconds \n", nanoSeconds/1000000.0);

  // time = std::chrono::duration_cast<std::chrono::nanoseconds>(before_allocation-start);
  // std::cout << std::setprecision(6) << time.count()/1000000.0 << std::endl;

  // time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_allocation-start_allocation);
  // std::cout << std::setprecision(6) << time.count()/1000000.0 << std::endl;

  auto time_copy = std::chrono::duration_cast<std::chrono::nanoseconds>(end_copy-start_copy);
  // std::cout << std::setprecision(6) << time_copy.count()/1000000.0 << std::endl;

  // time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_kernel-start_kernel);
  // std::cout << std::setprecision(6) << time.count()/1000000.0 << std::endl;

  auto time_merge = std::chrono::duration_cast<std::chrono::nanoseconds>(end_merge-start_merge);
  // std::cout << std::setprecision(6) << time_merge.count()/1000000.0 << std::endl;

  // double throughput = size/(nanoSeconds);
  // printf("%0.3f\n", throughput);
  // printf("Kernel throughput: %0.3f ops/s\n", throughput);

  // double bandwidthWrite = (sizeof(cl_int)*channel_size + sizeof(cl_int)*n_bins*channels)/((double)time_copy.count());
  // printf("%0.3f\n", bandwidthWrite);
  // printf("Write bandwidth: %0.3f GB/s\n", bandwidthWrite);
  // double bandwidthRead = (sizeof(cl_int)*n_bins*channels)/((double)time_merge.count());
  // printf("%0.3f\n", bandwidthRead);
  // printf("Read bandwidth: %0.3f GB/s\n", bandwidthRead);
  // double bandwidthKernel = (sizeof(cl_int)*channel_size + sizeof(cl_int)*n_bins*channels)/(nanoSeconds);
  // printf("%0.3f\n", bandwidthKernel);
  // printf("Kernel bandwidth: %0.3f GB/s\n", bandwidthKernel);

  // std::cout <<"Kernel: " << atoi(mem_kernel)-atoi(mem_end) << " KB" << std::endl;
  // std::cout <<atoi(mem_kernel)-atoi(mem_end) << std::endl;
  // std::cout <<"End: " << mem_end << " KB" << std::endl;
    std::cout <<mem_end << std::endl;

  return 0;
}

