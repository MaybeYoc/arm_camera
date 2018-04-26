#ifndef __COM_H_
#define __COM_H_

#include <adc_if.h>
#include <buzzer_if.h>
#include <pthread.h>

extern int g_sem_id;
extern int sem_v(int sem_id);
extern pthread_mutex_t g_mutex ; 

void com(void *parameter);


#endif 
