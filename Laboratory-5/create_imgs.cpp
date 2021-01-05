#include "CImg/CImg.h" // Path to the previous download directory
#include <iostream>
#include <cstdlib>
#include <string>

using namespace cimg_library;

int main(int argc, char* argv[])
{
  for (int i = 5; i<26; i+=5){
    CImg<unsigned char> img(128*i, 128*i, 1, 3);
    cimg_forXYC(img, x, y, c){
      img(x, y, 0, c)=rand()%256;
    }
    std::string filename = "image" + std::to_string(i) + ".ppm";
    img.save(filename.c_str());
  }
  return 0;
}