#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int creatsem(key_t key, int ini);
void semWait(int sid);
void semSignal(int sid);
int getval(int sid);
static void semcall(int sid, int op);


int creatsem(key_t key, int ini){
    /*
    以关键字key创建信号量组，返回标识数
    args:
        key: 信号量关键字
        ini: 信号量初始值
    return:
        sid: 信号量组标识数
    */
    int sid;
    union semun{
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    }arg;
    if(sid = semget(key, 1, 0666|IPC_CREAT) == -1){
        perror("semget\n");
    }
    arg.val = ini;
    if(semctl(sid, 0, SETVAL, arg) == -1){
        perror("semctl\n");
    }
    return (sid);
}

int getval(int sid){
    /*获取指定信号量的值*/
    int ret;
    ret = semctl(sid, 0, GETVAL);
    return ret;
}

void semWait(int sid){
    /*指定信号量值减1*/
    semcall(sid, -1);
}

void semSignal(int sid){
    /*指定信号量值加1*/
    semcall(sid, 1);
}

static void semcall(int sid, int op){
    /*
    对给定信号量进行指定操作
    args:
        sid: 信号量标识数
        op: 对应信号量val属性的变化值
    */
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = op;
    sb.sem_flg = SEM_UNDO;
    if (semop(sid, &sb, 1) == -1){
        perror("semop\n");
    }
}