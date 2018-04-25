#ifndef __INIT_H_
#define __INIT_H_

#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/sem.h>  
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>

#include <camera.h>
#include <com.h>


#define IP "192.168.206.132"
#define PORT 3214

#define CAMERA 0
#define COM 1

typedef void (*pthread_cbk)(void *parameter);
extern pthread_cbk callBack[];

pthread_mutex_t mutex;


int sem_id;
int init_sem(int sem_id, int init_value);
int del_sem(int sem_id);
int sem_p(int sem_id);
int sem_v(int sem_id);
union semun   
{  
    int val;  
        struct semid_ds *buf;  
        unsigned short  *array;  
        struct seminfo  *__buf;   
};  
  

#endif 
