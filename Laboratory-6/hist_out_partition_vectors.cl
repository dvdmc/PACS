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

  // returns the id for each work item
  // 0 as the argument as it is a 1 dimensional kernel

  unsigned int i = get_local_id(0);
  unsigned int group = get_group_id(0);

  if(get_global_id(0) < total_pixels){
    pair begin_end_group = split_evenly(256, num_groups, group, 0);
    
    pair begin_end_work = split_evenly(total_pixels, group_size, i, 0);
    
    for (unsigned int bin = i; bin<256*3; bin+=group_size){
      loc_hist[bin]=0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int p = begin_end_work.begin; p<begin_end_work.end; p++){
      uint4 pixel = img[p];
      uchar4 red = convert_uchar4(pixel & 0xFF);
      uchar4 green = convert_uchar4((pixel >> 8) & 0xFF);
      uchar4 blue = convert_uchar4((pixel >> 16) & 0xFF);
      if (red.x >= begin_end_group.begin && red.x < begin_end_group.end)
        atomic_add(hist + red.x, 1);
      if (green.x >= begin_end_group.begin && green.x < begin_end_group.end)
        atomic_add(hist + 256 + green.x, 1);
      if (blue.x >= begin_end_group.begin && blue.x < begin_end_group.end)
        atomic_add(hist + 512 + blue.x, 1);
      if (red.y >= begin_end_group.begin && red.y < begin_end_group.end)
        atomic_add(hist + red.y, 1);
      if (green.y >= begin_end_group.begin && green.y < begin_end_group.end)
        atomic_add(hist + 256 + green.y, 1);
      if (blue.y >= begin_end_group.begin && blue.y < begin_end_group.end)
        atomic_add(hist + 512 + blue.y, 1);
      if (red.z >= begin_end_group.begin && red.z < begin_end_group.end)
        atomic_add(hist + red.z, 1);
      if (green.z >= begin_end_group.begin && green.z < begin_end_group.end)
        atomic_add(hist + 256 + green.z, 1);
      if (blue.z >= begin_end_group.begin && blue.z < begin_end_group.end)
        atomic_add(hist + 512 + blue.z, 1);
      if (red.w >= begin_end_group.begin && red.w < begin_end_group.end)
        atomic_add(hist + red.w, 1);
      if (green.w >= begin_end_group.begin && green.w < begin_end_group.end)
        atomic_add(hist + 256 + green.w, 1);
      if (blue.w >= begin_end_group.begin && blue.w < begin_end_group.end)
        atomic_add(hist + 512 + blue.w, 1);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int bin = i+begin_end_group.begin; bin<begin_end_group.end; bin+=group_size){
      hist[bin] += loc_hist[bin];
      hist[bin+256] += loc_hist[bin+256];
      hist[bin+512] += loc_hist[bin+512];
    }
  }
}