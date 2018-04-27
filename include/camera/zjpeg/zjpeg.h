#ifndef __ZJPEG_H_
#define __ZJPEG_H_
#include <linux/videodev2.h>

typedef unsigned long UL;
int yuyv_to_rgb(unsigned char* yuv,unsigned char *a_rgb);
long rgb_to_jpeg(unsigned char *rgb_buf, unsigned char **jpeg_buf);

struct v4l2_buffer v4l2_buf;
struct videobuffer {//内核映射地址
    void *start;//首地址
    UL length;//空间长度
}*video_buf;


#endif
