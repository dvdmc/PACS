#include "CImg.h" // Path to the previous download directory
#include <iostream>

using namespace cimg_library;

void first()
{
  CImg<unsigned char> img(640, 400, 1, 3);        // Define a 640x400 color image with 8 bits per color component
  img.fill(0);                                    // Set pixel values to 0 (color : black)
  unsigned char purple[] = {255, 0, 255};         // Define a purple color
  img.draw_text(100, 100, "Hello World", purple); // Draw a purple "Hello world" at coordinates (100,100)
  img.display("My first CImg code");              // Display the image in a display window
}

void second(const int i, const int j)
{
  CImg<unsigned char> img("image.jpg"); // Load image file "image.jpg" at object img

  std::cout << "Image width: " << img.width() << "Image height: " << img.height() << "Number of slices: "
            << img.depth() << "Number of channels: " << img.spectrum() << std::endl; //dump some characteristics of the loaded image

  std::cout << std::hex << (int)img(i, j, 0, 0)<< std::endl; //print pixel value for channel 0 (red)
  //std::cout << std::hex << img(i, j, 0, 1) << std::endl; //print pixel value for channel 1 (green)
  //std::cout << std::hex << img(i, j, 0, 2) << std::endl; //print pixel value for channel 2 (blue)

  img.display("My first CImg code"); // Display the image in a display window
}

void third(const int i, const int j)
{
  CImg<unsigned char> img("image.jpg"); // Load image file "image.jpg" at object img
  unsigned char blue[] = {0, 0, 255};         // Define a blue color
  std::cout << "Image width: " << img.width() << "Image height: " << img.height() << "Number of slices: "
            << img.depth() << "Number of channels: " << img.spectrum() << std::endl; //dump some characteristics of the loaded image

  
  img.draw_line(0,i,img.width(),i, blue);

  img.display("My first CImg code"); // Display the image in a display window
}

std::pair<int, int>
usage(int argc, char *argv[]) {
    // read the number of divisions from the command line
    if (!(argc == 3)) {
        std::cerr << "Invalid syntax: first <pixel_i> <pixel_j>" << std::endl;
        exit(1);
    }
    return std::make_pair(atoi(argv[1]), atoi(argv[2]));
}

int main(int argc, char* argv[])
{
  std::pair<int, int> pixel_coord = usage (argc, argv);
  third(pixel_coord.first, pixel_coord.second);
  return 0;
}