__kernel void histogram(
  __global unsigned char *img,
  __global int *hist,
  const unsigned int pixels,
  const unsigned int channels
  ){

  // returns the id for each work item
  // 0 as the argument as it is a 1 dimensional kernel
  
  int i = get_global_id(0);
  int k = get_global_id(1);

  if(i < pixels && k < channels){
    atomic_add(hist + k*256 + img[i + k*pixels], 1);
  }
}