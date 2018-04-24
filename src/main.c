#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <init.h>    /**/

int code_exit = 0;

int main()
{
/*	camera();
 */
 	int sem_id = create_sem_set(1);  
    init_sem_set(sem_id, 0, 1);  
 	int err=1;
 	int read_len;
 	char buf[8];
 
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	if(-1 == fd) {
		perror("Create Socket");
		return -1;
	}
	printf("Create Socket success!\n");
	struct sockaddr_in serveraddr = {0}, clientaddr = {0};
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	serveraddr.sin_port = htons(PORT);
	socklen_t len = sizeof(serveraddr);
	if((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &err, sizeof(err))))
	{
		perror("setsockopt failed");
		return -1;
	}
	if(-1 == bind(fd, (struct sockaddr *)&serveraddr, len))
	{
		perror("bind");
		return -1;
	}
	listen(fd,10);
	while(1) {
		int clientfd = accept(fd, (struct sockaddr *)&clientaddr, &len);
		if(clientfd < 0)
		{
			perror("accept");
			continue;
		}
		printf("accept a new client!\n");
		read_len = read(clientfd, buf, sizeof(buf));
		if(read_len <= 0) {
			perror("read clientfd");
			continue;
		}
		printf("Recv is %s\n", buf);
		void *parameter = (void *)&clientfd;
		pthread_t pid;
		if (strncmp(buf,"CAMERA", 6) == CAMERA) {
			/*CAMERA.C*/
			err = pthread_create(&pid, NULL, (void *)callBack[CAMERA], parameter);
			if(err != 0) {
				perror("caeate pthread");
				continue;
			}
			printf("create pthread CAMERA success!\n");
			P(sem_id);
		} else if(strncmp(buf,"COM", 3) == 0) {
			/*COM.C*/
			err = pthread_create(&pid, NULL, (void *)callBack[COM], parameter);
			if(err != 0) {
				perror("create pthread");
				continue;
			}
			printf("create pthread COM success!\n");
			P(sem_id);
		} else {
			/**/
			printf("not cmd!\n");
		}
		/*pthread_t pid;
		pthread_create(&pid, NULL, callBack_fun[1], parameter);
		pthread_join(&pid, NULL);*/
	}
	for(;;) {
		if(code_exit != 0)
			break;
	}
	return 0;
}
