#include <stdio.h>
#include <string.h>

#include <camera.h>  /**/

void camera(void *parameter)
{
	int clientfd = *(int *)parameter;
	pthread_mutex_unlock(&g_mutex);  //mutex解锁  
	int camerafd;
	char dev[] = CAMERA_DEV;
	int write_read_len;
	printf("My is camera.c!\n");
	/***********test*code****************/
	camera_start(&camerafd, dev);
	while(1)
    {
		usleep(25000);
        capture_image(&camerafd);//取出一张图
        unsigned char *rgb_buf=(unsigned char *)malloc(video_buf[v4l2_buf.index].length*1.5);
        if(NULL == rgb_buf)
        {
            perror("rgb_buf");
            exit(EXIT_FAILURE);
        }
        yuyv_to_rgb(video_buf[v4l2_buf.index].start,rgb_buf);
        unsigned char *jpeg_buf=NULL;
        long size=rgb_to_jpeg(rgb_buf,&jpeg_buf);
		write_read_len = write(clientfd,(char *)&size,sizeof(long));
		if(write_read_len == -1) {
			perror("write camera data");
			camera_stop(&camerafd);
    		free(video_buf);
    		close(camerafd);
    		close(clientfd);
    		break;
		}
        write_read_len = write(clientfd,jpeg_buf,size);
        if(write_read_len == -1) {
			perror("write camera data");
			camera_stop(&camerafd);
    		free(video_buf);
    		close(camerafd);
    		close(clientfd);
    		break;
		}
        push_queue(&camerafd);
    }
	/************************************/
	pthread_detach(pthread_self());
	sem_v(g_sem_id);
}
