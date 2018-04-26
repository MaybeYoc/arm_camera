#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <init.h>    /**/

int code_exit = 0;

int main()
{
/*	camera();
 */
 	/**********test quit tcp sig*******************/
 	struct sigaction act;  
	memset(&act,0,sizeof(act));  
	act.sa_handler = SIG_IGN;//设定接受到指定信号后的动作为忽略  
	//act.sa_handler = signal_handler;//设定接受到指定信号后的动作为 相关函数  
	act.sa_flags = 0;  
	sigemptyset(&act.sa_mask);  
	sigaction(SIGPIPE,&act,NULL);//屏蔽SIGPIPE信号  
	sigaction(SIGCHLD,&act,NULL);//屏蔽子进程终结信号，让init进程去处
 	/**********************************************/
 	
	pthread_mutex_init(&g_mutex,NULL);
 	g_sem_id = semget(ftok(".", 'a'), 1, 0666 | IPC_CREAT); /* 创建一个信号量*/
    init_sem(g_sem_id, 1);
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
	if((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &err, sizeof(err)))) {
		perror("setsockopt failed");
		return -1;
	}
	if(-1 == bind(fd, (struct sockaddr *)&serveraddr, len)) {
		perror("bind");
		return -1;
	}
	listen(fd,10);
	while(1) {
		int clientfd = accept(fd, (struct sockaddr *)&clientaddr, &len);
		if(clientfd < 0) {
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
		pthread_mutex_lock(&g_mutex);
		void *parameter = (void *)&clientfd;
		pthread_t pid1, pid2;
		if (strncmp(buf,"CAMERA", 6) == CAMERA) {
			/*CAMERA.C*/
			err = pthread_create(&pid1, NULL, (void *)callBack[CAMERA], parameter);
			if(err != 0) {
				perror("caeate pthread");
				close(clientfd);
				pthread_mutex_unlock(&g_mutex);  //mutex解锁 
				continue;
			}
			printf("create pthread CAMERA success!\n");
			sem_p(g_sem_id);
		} else if(strncmp(buf,"COM", 3) == 0) {
			/*COM.C*/
			err = pthread_create(&pid2, NULL, (void *)callBack[COM], parameter);
			if(err != 0) {
				perror("create pthread");
				close(clientfd);
				pthread_mutex_unlock(&g_mutex);  //mutex解锁 
				continue;
			}
			printf("create pthread COM success!\n");
			sem_p(g_sem_id);
		} else {
			/**/
			close(clientfd);
			pthread_mutex_unlock(&g_mutex);  //mutex解锁  
			printf("not cmd!\n");
		}
		/*pthread_t pid;
		pthread_create(pid, NULL, callBack_fun[1], parameter);
		pthread_join(pid, NULL);*/
		//pthread_mutex_destroy(&mutex);
		//del_sem(int sem_id)
	}
	pthread_mutex_destroy(&g_mutex);
	del_sem(g_sem_id);
	for(;;) {
		if(code_exit != 0)
			break;
	}
	return 0;
}
