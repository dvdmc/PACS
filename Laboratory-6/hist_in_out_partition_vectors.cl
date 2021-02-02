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
  const unsigned int min,
  __local unsigned int *loc_hist
  ){

  unsigned int i = get_local_id(0);
  unsigned int group = get_group_id(0);

  if(get_global_id(0) < total_pixels){
    // Get the pixels that correspond to the work group
    pair begin_end_group = split_evenly(total_pixels, num_groups, group, 0);
    // Get the pixels that correspond to the work item
    pair begin_end_work = split_evenly(begin_end_group.end -  begin_end_group.begin, group_size, i, begin_end_group.begin);
    
    for (unsigned int bin = i; bin<(256/2)*3; bin+=group_size){
      loc_hist[bin]=0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int p = begin_end_work.begin; p<begin_end_work.end; p++){
      uint4 pixel = img[p];
      uchar4 red = convert_uchar4(pixel & 0xFF);
      uchar4 green = convert_uchar4((pixel >> 8) & 0xFF);
      uchar4 blue = convert_uchar4((pixel >> 16) & 0xFF);
      if (red.x >= min && red.x < min+256/2)
        atomic_add(hist -min + red.x, 1);
      if (green.x >= min && green.x < min+256/2)
        atomic_add(hist -min + 128 + green.x, 1);
      if (blue.x >= min && blue.x < min+256/2)
        atomic_add(hist -min + 256 + blue.x, 1);
      if (red.y >= min && red.y < min+256/2)
        atomic_add(hist -min + red.y, 1);
      if (green.y >= min && green.y < min+256/2)
        atomic_add(hist -min + 128 + green.y, 1);
      if (blue.y >= min && blue.y < min+256/2)
        atomic_add(hist -min + 256 + blue.y, 1);
      if (red.z >= min && red.z < min+256/2)
        atomic_add(hist -min + red.z, 1);
      if (green.z >= min && green.z < min+256/2)
        atomic_add(hist -min + 128 + green.z, 1);
      if (blue.z >= min && blue.z < min+256/2)
        atomic_add(hist -min + 256 + blue.z, 1);
      if (red.w >= min && red.w < min+256/2)
        atomic_add(hist -min + red.w, 1);
      if (green.w >= min && green.w < min+256/2)
        atomic_add(hist -min + 128 + green.w, 1);
      if (blue.w >= min && blue.w < min+256/2)
        atomic_add(hist -min + 256 + blue.w, 1);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int bin = i; bin<(256/2)*3; bin+=group_size){
      atomic_add(hist+bin, loc_hist[bin]);
    }
  }
}