__kernel void rotate(
  __global unsigned char *img,
  __global unsigned char *result,
  const unsigned int rows,
  const unsigned int columns,
  const unsigned int spectrum,
  const float angle
  ){

  // returns the id for each work item
  // 0 as the argument as it is a 1 dimensional kernel
  
  int i = get_global_id(0);
  int j = get_global_id(1);
  int k = get_global_id(2);
  unsigned int total_pixels = columns*rows;

  if(i < rows && j < columns && k < spectrum){
    float x0 = columns/2;
    float y0 = rows/2;
    int x2 = convert_int_rte(cos(angle)*(j-x0) - sin(angle)*(i-y0) + x0);
    int y2 = convert_int_rte(sin(angle)*(j-x0) + cos(angle)*(i-y0) + y0);
    if (x2 < columns && x2 >= 0 && y2 < rows && y2 >=0){
      result[y2*columns + x2 + k*total_pixels] = img[i*columns + j + k*total_pixels];
    }
  }
}