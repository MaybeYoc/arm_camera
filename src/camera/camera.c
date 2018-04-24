#include <stdio.h>

#include <camera.h>  /**/

void camera(void *parameter)
{
	pthread_mutex_unlock(&mutex);  //mutex解锁  
	printf("My is camera.c!\n");
	while(1);
}
