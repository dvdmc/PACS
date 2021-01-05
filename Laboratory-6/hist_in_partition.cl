typedef struct{
    unsigned int begin;
    unsigned int end;
} pair;

pair split_evenly(unsigned int N, unsigned int groups, unsigned int i, unsigned int init)
{
    unsigned int chunks = N/(groups);
    unsigned int extra = N%groups;
    if (i < extra){
      pair p;
      p.begin = (chunks+1)*i +  init;
      p.end = (chunks+1)*(i+1) + init;
      return p;
    }
    else{
      pair p;
      p.begin = (chunks+1)*extra + chunks*(i-extra) + init;
      p.end = p.begin + chunks;
      return p;
    }
}


__kernel void histogram(
  __global unsigned int *img,
  __global int *hist,
  const unsigned int num_groups,
  const unsigned int group_size,
  const unsigned int total_pixels,
  __local unsigned int *loc_hist
  ){

  unsigned int i = get_local_id(0);
  unsigned int group = get_group_id(0);

  if(get_global_id(0) < total_pixels){
    // Get the pixels that correspond to the work group
    pair begin_end_group = split_evenly(total_pixels, num_groups, group, 0);
    // Get the pixels that correspond to the work item
    pair begin_end_work = split_evenly(begin_end_group.end -  begin_end_group.begin, group_size, i, begin_end_group.begin);
    
    for (unsigned int bin = i; bin<256*3; bin+=group_size){
      loc_hist[bin]=0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int p = begin_end_work.begin; p<begin_end_work.end; p++){
      unsigned int pixel = img[p];
      unsigned char red = pixel & 0xFF;
      unsigned char green = (pixel >> 8) & 0xFF;
      unsigned char blue = (pixel >> 16) & 0xFF;
      atomic_add(loc_hist + red, 1);
      atomic_add(loc_hist + 256 + green, 1);
      atomic_add(loc_hist + 512 + blue, 1);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int bin = i; bin<256*3; bin+=group_size){
      atomic_add(hist+bin, loc_hist[bin]);
    }
  }
}