#include <stdio.h> 

#include <camera_if.h> /**/

static void camera_open(int *camerafd, char *dev)
{
	*camerafd = open(dev, O_RDWR);
    if(-1 == *camerafd)
    {
        *camerafd = open(dev, O_RDWR);
        if(-1 == *camerafd)
        {
            perror("open camera fail");
            exit(EXIT_FAILURE);
        }
    }
    printf("open camera success!\n");
}

static void camera_find(int *camerafd)
{
    //vim /usr/include/linux/videodev2.h
    struct  v4l2_capability video_cap;//设备功能
    if(ioctl(*camerafd,VIDIOC_QUERYCAP,&video_cap) < 0)
    {//查询驱动功能
        perror("VIDIOC_QUERYCAP fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
    if(!(video_cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
    {//查询是否支持图片获取
        perror("V4L2_CAP_VIDEO_CAPTURE fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
    if(!(video_cap.capabilities & V4L2_CAP_STREAMING))
    {//查询是否支持视频流
        perror("V4L2_CAP_STREAMING fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
    struct v4l2_fmtdesc fmtdesc;//设备格式
    memset(&fmtdesc,0,sizeof(fmtdesc));
    fmtdesc.index=0;//格式序号从0开始
    fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    while((ioctl(*camerafd,VIDIOC_ENUM_FMT,&fmtdesc)) == 0)
    {//获取当前设备支持的视频格式
        fmtdesc.index++;
        printf("{pixelformat = %c%c%c%c, description=%s}\n",\
              fmtdesc.pixelformat & 0xFF,\
              (fmtdesc.pixelformat >> 8) & 0xFF,\
              (fmtdesc.pixelformat >> 16) & 0xFF,\
              (fmtdesc.pixelformat >> 24) & 0xFF,\
              fmtdesc.description);
    }
}

//设置视频捕获格式
static void set_camera(int *camerafd)
{
    struct v4l2_format fmt;
    memset(&fmt,0,sizeof(fmt));
    fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width=pixmap.video_width;
    fmt.fmt.pix.height=pixmap.video_height;
    fmt.fmt.pix.pixelformat=V4L2_PIX_FMT_YUYV;
    fmt.fmt.pix.field=V4L2_FIELD_INTERLACED;
    if(ioctl(*camerafd,VIDIOC_S_FMT,&fmt) == -1)
    {
        perror("VIDIOC_S_FMT fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
}

//申请缓存
static void req_image_buf(int *camerafd)
{
    memset(&reqbuff,0,sizeof(reqbuff));
    reqbuff.count=pixmap.buf_count;//缓存区中缓存帧数目
    reqbuff.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    reqbuff.memory=V4L2_MEMORY_MMAP;//内存映射
    if(ioctl(*camerafd,VIDIOC_REQBUFS,&reqbuff) == -1)
    {//分配内存
        perror("VIDIOC_REQBUFS fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
}



//获取物理内存
static void mmap_image(int *camerafd)
{
    video_buf = (struct videobuffer *)calloc(reqbuff.count,sizeof(*video_buf));
    if(video_buf == NULL)
    {
        perror("video calloc buffers failed");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
    for(numBuf_=0;numBuf_ < reqbuff.count;numBuf_++)
    {
        memset(&v4l2_buf,0,sizeof(struct v4l2_buffer));
        v4l2_buf.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buf.memory=V4L2_MEMORY_MMAP;
        v4l2_buf.index=numBuf_;
        if(ioctl(*camerafd,VIDIOC_QUERYBUF,&v4l2_buf) == -1)
        {//读取缓存
            perror("VIDIOC_QUERYBUF fail");
            close(*camerafd);
            exit(EXIT_FAILURE);
        }
        video_buf[numBuf_].length=v4l2_buf.length;
        //转换成相对地址
        video_buf[numBuf_].start=mmap(NULL,v4l2_buf.length,\
                                      PROT_READ | PROT_WRITE,\
                                     MAP_SHARED,*camerafd,\
                                      v4l2_buf.m.offset);
        if(video_buf[numBuf_].start == MAP_FAILED)
        {
            perror("MAP_FALLED");
            close(*camerafd);
            exit(EXIT_FAILURE);
        }
        push_queue(camerafd);
    }
}

void camera_start(int *camerafd, char *dev)
{
    camera_open(camerafd, dev);
    camera_find(camerafd);
    set_camera(camerafd);
    req_image_buf(camerafd);
    mmap_image(camerafd);
    enum v4l2_buf_type v4l2_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fd_set fd;
    struct timeval tv;
    if(ioctl(*camerafd,VIDIOC_STREAMON,&v4l2_type) == -1)
    {//启动视频采集命令
        perror("VIDIOC_STREAMON fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&fd);
    FD_SET(*camerafd,&fd);
    tv.tv_sec=2;
    tv.tv_usec=0;
    if(select(*camerafd+1,&fd,NULL,NULL,&tv) == -1)
    {
        perror("select fail");
        exit(EXIT_FAILURE);
    }
    printf("start camera success!\n");
}

//入队
void push_queue(int *camerafd)
{
    if(ioctl(*camerafd,VIDIOC_QBUF,&v4l2_buf) == -1)
    {//放入缓存队列
        perror("VIDIOC_QBUF fail");
        close(*camerafd);
        exit(EXIT_FAILURE);
    }
}

//出队
void capture_image(int *camerafd)
{
    if(ioctl(*camerafd,VIDIOC_DQBUF,&v4l2_buf) == -1)
    {//取数据
        perror("VIDIOC_DQBUF fail");
        exit(EXIT_FAILURE);
    }
}
//停止视频采集
void camera_stop(int *camerafd)
{
    enum v4l2_buf_type v4l2_type;
    v4l2_type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(*camerafd,VIDIOC_STREAMOFF,&v4l2_type))
    {
        perror("VIDIOC_STREAMOFF fail");
        if(ioctl(*camerafd,VIDIOC_STREAMOFF,&v4l2_type))
        {
            perror("VIDIOC_STREAMOFF fail");
            close(*camerafd);
            exit(EXIT_FAILURE);
        }
    }
}

