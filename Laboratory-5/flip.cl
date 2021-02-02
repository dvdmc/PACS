__kernel void img_flip(
  __global unsigned char *img,
  const unsigned int rows,
  const unsigned int columns,
  const unsigned int spectrum
  ){

  // returns the id for each work item
  // 0 as the argument as it is a 1 dimensional kernel
  
  int i = get_global_id(0);
  int j = get_global_id(1);
  int k = get_global_id(2);
  unsigned int total_pixels = columns*rows;

  if(i < rows && j < columns / 2 && k < spectrum){
      unsigned char aux = img[i*columns + j + total_pixels*k];
      img[i*columns + j + total_pixels*k] = img[i*columns + columns - j - 1 + total_pixels*k];
      img[i*columns + columns - j - 1 + total_pixels*k] = aux;
  }
}