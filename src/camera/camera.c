#include <stdio.h>
#include <string.h>

#include <camera.h>  /**/

void camera(void *parameter)
{
	int clientfd = *(int *)parameter;
	pthread_mutex_unlock(&mutex);  //mutex解锁  
	printf("My is camera.c!\n");
	/***********test*code****************/
	char buf[18] = "my is camera!";
	int len;
	while(1) {
		len = write(clientfd, buf, sizeof(buf));
		if(len == -1) {
			perror("socket");
			break;
		}
		len = read(clientfd, buf, 10);
		if(len == -1) {
			perror("socket");
			break;
		}
		printf("%s\n", buf);
		if(strncmp(buf, "QUIT", 4) == 0)
			break;
		
	}
	close(clientfd);
	pthread_detach(pthread_self());
	/************************************/
	sem_v(sem_id);
}
