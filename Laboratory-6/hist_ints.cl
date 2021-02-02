__kernel void histogram(
  __global unsigned int *img,
  __global int *hist,
  const unsigned int pixels
  ){
  
  int i = get_global_id(0);

  if(i < pixels){
    unsigned int pixel = img[i];
    unsigned char red = pixel & 0xFF;
    unsigned char green = (pixel >> 8) & 0xFF;
    unsigned char blue = (pixel >> 16) & 0xFF;
    atomic_add(hist + red, 1);
    atomic_add(hist + 256 + green, 1);
    atomic_add(hist + 512 + blue, 1);
  }
}