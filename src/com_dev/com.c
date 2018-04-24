#include <stdio.h>

#include <com.h>  /**/


void com(void *parameter)
{	pthread_mutex_unlock(&mutex);  //mutex解锁  
	printf("com.c\n");
	sleep(10);
	sem_v(sem_id);
	while(1);
}

