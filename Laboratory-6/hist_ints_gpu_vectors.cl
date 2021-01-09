__kernel void histogram(
  __global uint4 *img,
  __global int *hist,
  const unsigned int pixels
  ){
  
  int i = get_global_id(0);

  if(i < pixels){
    uint4 pixel = img[i];
    uchar4 red = convert_uchar4(pixel & 0xFF);
    uchar4 green = convert_uchar4((pixel >> 8) & 0xFF);
    uchar4 blue = convert_uchar4((pixel >> 16) & 0xFF);
    atomic_add(hist + red.x, 1);
    atomic_add(hist + 256 + green.x, 1);
    atomic_add(hist + 512 + blue.x, 1);
    atomic_add(hist + red.y, 1);
    atomic_add(hist + 256 + green.y, 1);
    atomic_add(hist + 512 + blue.y, 1);
    atomic_add(hist + red.z, 1);
    atomic_add(hist + 256 + green.z, 1);
    atomic_add(hist + 512 + blue.z, 1);
    atomic_add(hist + red.w, 1);
    atomic_add(hist + 256 + green.w, 1);
    atomic_add(hist + 512 + blue.w, 1);
  }
}