#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <pthread.h>

#include <camera_if.h>

#define CAMERA_DEV "dev/video4"

extern int g_judge_camera;
extern int g_sem_id;
extern int sem_v(int sem_id);

void camera(void *parameter);
extern pthread_mutex_t g_mutex ; 



#endif 
