#ifndef __CAMERA_IF_H_
#define __CAMERA_IF_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/videodev2.h>


#include <zjpeg.h>


typedef struct {
	int video_height;
	int video_width;
	int buf_count;
}pixmap_set;
pixmap_set pixmap;
struct v4l2_requestbuffers reqbuff;
extern struct v4l2_buffer v4l2_buf;
extern struct videobuffer *video_buf;

int numBuf_;


void camera_start(int *camerafd, char *dev);
void push_queue(int *camerafd);
void capture_image(int *camerafd);
void camera_stop(int *camerafd);
extern int yuyv_to_rgb(unsigned char* yuv,unsigned char *a_rgb);
extern long rgb_to_jpeg(unsigned char *rgb_buf, unsigned char **jpeg_buf);

#endif 
