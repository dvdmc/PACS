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
  const unsigned int group_size,
  const unsigned int total_pixels,
  __local unsigned int *loc_hist,
  const unsigned int min
  ){

  // returns the id for each work item
  // 0 as the argument as it is a 1 dimensional kernel

  unsigned int i = get_local_id(0);
  unsigned int group = get_group_id(0);
  unsigned int num_groups = get_num_groups(0);

  if(get_global_id(0) < total_pixels){
    pair begin_end_group = split_evenly(min+128, num_groups, group, min);
    
    pair begin_end_work = split_evenly(total_pixels, group_size, i, 0);
    
    for (unsigned int bin = i; bin<256/2*3; bin+=group_size){
      loc_hist[bin]=0;
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    // printf("despues de barrera 1\n");
    for (unsigned int p = begin_end_work.begin; p<begin_end_work.end; p++){
      unsigned int pixel = img[p];
      unsigned char red = pixel & 0xFF;
      unsigned char green = (pixel >> 8) & 0xFF;
      unsigned char blue = (pixel >> 16) & 0xFF;
      if (red >= begin_end_group.begin && red < begin_end_group.end){
        atomic_add(loc_hist + red - min, 1);
      }
      if (green >= begin_end_group.begin && green < begin_end_group.end)
        atomic_add(loc_hist + 128 + green - min, 1);
      if (blue >= begin_end_group.begin && blue < begin_end_group.end){
        atomic_add(loc_hist + 256 + blue - min, 1);
      }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    for (unsigned int bin = i+begin_end_group.begin-min; bin<begin_end_group.end-min; bin+=group_size){
      hist[bin] += loc_hist[bin];
      hist[bin+128] += loc_hist[bin+128];
      hist[bin+256] += loc_hist[bin+256];
    }
  }
}