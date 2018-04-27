#include <stdio.h>
#include <string.h>

#include <com.h>  /**/


void com(void *parameter)
{	
	int clientfd = *(int *)parameter;
	pthread_mutex_unlock(&g_mutex);  //mutex解锁  
	g_judge_com = 1;
	printf("com.c\n");
	/***********test*code****************/
	char buf[18] = "my is COM";
	int len;
	while(1) {
		len = write(clientfd, buf, sizeof(buf));
		if(len == -1) {
			perror("write socket");
			break;
		}
		len = read(clientfd, buf, 10);
		if(len == -1) {
			perror("read socket");
			break;
		}
		printf("%s\n", buf);
		if(strncmp(buf, "QUIT", 4) == 0)
			break;
		
	}
	close(clientfd);
	g_judge_com = 0;
	pthread_detach(pthread_self());
	/************************************/
	sem_v(g_sem_id);

}

