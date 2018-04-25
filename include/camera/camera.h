#ifndef __CAMERA_H_
#define __CAMERA_H_

#include <pthread.h>

#include <camera_if.h>

extern int sem_id;
extern int sem_v(int sem_id);

void camera(void *parameter);
extern pthread_mutex_t mutex ; 



#endif 
