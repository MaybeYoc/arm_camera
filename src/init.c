#include <stdio.h>

#include <init.h>  /**/


pthread_cbk callBack[] = {
	camera,
	com,
};

int init_sem(int sem_id, int init_value) 

{

    union semun sem_union;

    sem_union.val = init_value;     /* init_value为初始值 */

    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)

    {

        perror("Initialize semaphore");        

        return -1;

    }

    return 0;

}

/* 从系统中删除信号量的函数 */

int del_sem(int sem_id) 

{

    union semun sem_union;

    if (semctl(sem_id, 0, IPC_RMID, sem_union) == -1)

    {

        perror("Delete semaphore");

        return -1; 

    }

}

/* P操作函数 */

int sem_p(int sem_id)

{

    struct sembuf sem_b;

    sem_b.sem_num = 0;  /* 单个信号量的编号应该为0 */

    sem_b.sem_op = -1;  /* 表示P操作 */

    sem_b.sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/

    if (semop(sem_id, &sem_b, 1) == -1) 

    {

        perror("P operation");

        return -1;

    }

    return 0;

}

/* V操作函数*/

int sem_v(int sem_id)

{

    struct sembuf sem_b;

    sem_b.sem_num = 0; /* 单个信号量的编号应该为0 */

    sem_b.sem_op = 1; /* 表示V操作 */

    sem_b.sem_flg = SEM_UNDO; /* 系统自动释放将会在系统中残留的信号量*/

    if (semop(sem_id, &sem_b, 1) == -1)

    {

        perror("V operation");

        return -1;

    }

    return 0;

}



