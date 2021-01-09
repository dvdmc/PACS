#!/bin/bash
# g++ image_histogram_cpu.c -o image_histogram_cpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# hola=0
# echo $hola
# hola=$hola+1
# g++ image_histogram_ints_cpu.c -o image_histogram_ints_cpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_in_partition_cpu.c -o image_histogram_in_partition_cpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_out_partition_cpu.c -o image_histogram_out_partition_cpu  -lOpenCL -I . -lm -lpthread -lX11 -O2

# echo $hola
# hola=$hola+1
# g++ image_histogram_gpu.c -o image_histogram_gpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_ints_gpu.c -o image_histogram_ints_gpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_in_partition_gpu.c -o image_histogram_in_partition_gpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_out_partition_gpu.c -o image_histogram_out_partition_gpu  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_ints_gpu_vectors.c -o image_histogram_ints_gpu_vectors  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_in_partition_gpu_vectors.c -o image_histogram_in_partition_gpu_vectors  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_out_partition_gpu_vectors.c -o image_histogram_out_partition_gpu_vectors  -lOpenCL -I . -lm -lpthread -lX11 -O2



# echo $hola
# hola=$hola+1
# g++ image_histogram_image_assignment.c -o image_histogram_image_assignment  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_in_partition_heter.c -o image_histogram_in_partition_heter  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_out_partition_heter.c -o image_histogram_out_partition_heter  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_in_out_partition_heter.c -o image_histogram_in_out_partition_heter  -lOpenCL -I . -lm -lpthread -lX11 -O2
# echo $hola
# hola=$hola+1
# g++ image_histogram_out_out_partition_heter.c -o image_histogram_out_out_partition_heter  -lOpenCL -I . -lm -lpthread -lX11 -O2



# for program in "image_histogram_cpu" "image_histogram_ints_cpu" "image_histogram_in_partition_cpu" "image_histogram_out_partition_cpu" "image_histogram_gpu" "image_histogram_ints_gpu" "image_histogram_in_partition_gpu" "image_histogram_out_partition_gpu" "image_histogram_ints_gpu_vectors" "image_histogram_in_partition_gpu_vectors"  "image_histogram_out_partition_gpu_vectors" "image_histogram_image_assignment" "image_histogram_in_partition_heter" "image_histogram_out_partition_heter" "image_histogram_in_out_partition_heter" "image_histogram_out_out_partition_heter"
for program in "image_histogram_in_partition_heter" "image_histogram_out_partition_heter" "image_histogram_in_out_partition_heter" "image_histogram_out_out_partition_heter"
do
    echo "---------------------"
    echo $program
    # for file in "image5.ppm" "image10.ppm" "image15.ppm" "image20.ppm"
    for file in "image20.ppm"
    do
        ./$program $file 100
    done
done