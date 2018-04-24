#ifndef __INIT_H_
#define __INIT_H_

#include <sys/types.h>  
#include <sys/ipc.h>  
#include <sys/sem.h>  

#include <camera.h>
#include <com.h>

#define _PATH_NAME_ "/tmp"  
#define _PROJ_ID_ 0x666

union semun   
{  
    int val;  
        struct semid_ds *buf;  
        unsigned short  *array;  
        struct seminfo  *__buf;   
};  
  
int create_sem_set(int nums);  
int get_sem_set(int nums);  
int init_sem_set(int sem_id, int which, int val);  
int P(int sem_id);  
int V(int sem_id);  
int destroy(int sem_id); 


#define IP "192.168.206.132"
#define PORT 3214

#define CAMERA 0
#define COM 1

typedef void (*pthread_cbk)(void *parameter);
extern pthread_cbk callBack[];

#endif 
