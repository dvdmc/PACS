#!/bin/bash
g++ image_histogram.c -o image_histogram  -lOpenCL -I . -lm -lpthread -lX11 -O2
g++ image_histogram_ints.c -o image_histogram_ints  -lOpenCL -I . -lm -lpthread -lX11 -O2
g++ image_histogram_in_partition.c -o image_histogram_in_partition  -lOpenCL -I . -lm -lpthread -lX11 -O2
g++ image_histogram_out_partition.c -o image_histogram_out_partition  -lOpenCL -I . -lm -lpthread -lX11 -O2

for program in "image_histogram" "image_histogram_ints" "image_histogram_in_partition" "image_histogram_out_partition"
do
    for file in "image5.ppm" "image10.ppm" "image15.ppm" "image20.ppm" "image25.ppm"
    do
        ./$program $file > /dev/null
        ./$program $file > /dev/null
        i=0
        while [ $i -le 5 ]
        do
            ./$program $file
            i=$(( $i + 1 ))
        done
        echo
    done
    echo "---------------------"
done