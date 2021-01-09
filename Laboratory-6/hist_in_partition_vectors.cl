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
  __global uint4 *img,
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
      uint4 pixel = img[p];
      uchar4 red = convert_uchar4(pixel & 0xFF);
      uchar4 green = convert_uchar4((pixel >> 8) & 0xFF);
      uchar4 blue = convert_uchar4((pixel >> 16) & 0xFF);
      atomic_add(loc_hist + red.x, 1);
      atomic_add(loc_hist + 256 + green.x, 1);
      atomic_add(loc_hist + 512 + blue.x, 1);
      atomic_add(loc_hist + red.y, 1);
      atomic_add(loc_hist + 256 + green.y, 1);
      atomic_add(loc_hist + 512 + blue.y, 1);
      atomic_add(loc_hist + red.z, 1);
      atomic_add(loc_hist + 256 + green.z, 1);
      atomic_add(loc_hist + 512 + blue.z, 1);
      atomic_add(loc_hist + red.w, 1);
      atomic_add(loc_hist + 256 + green.w, 1);
      atomic_add(loc_hist + 512 + blue.w, 1);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int bin = i; bin<256*3; bin+=group_size){
      atomic_add(hist+bin, loc_hist[bin]);
    }
  }
}