#include <sys/shm.h>
#include <string.h>
#include "com.h"

#define SHMKEY 18001
#define SIZE 1024
#define SEMKEY 5

int main(){
    char *segaddr;
    char str[20];
    int segid, sid1, pid;
    //创建共享内存段
    if((segid = shmget(SHMKEY, SIZE, IPC_CREAT|0666)) == -1){
        perror("shmget");
    }
    segaddr = shmat(segid, NULL, 0);
    //设置信号量，初始化为1
    sid1 = creatsem(SEMKEY, 1);
    pid = fork();
    /*
    利用单个信号量实现进程之间的互斥;
    一个进程进行了senWait后访问共享内存，
    访问完毕后进行semSignal操作，允许另一进程访问共享内存
    */
    if(!pid){
        //子进程读取共享内存中数据      
        while(1){         
            semWait(sid1);
            printf("Received from Parent: %s\n", segaddr);
            semSignal(sid1);
        }
    }
    else{
        //父进程写入共享内存
        while(1){
            semWait(sid1);
            scanf("%s", segaddr);
            semSignal(sid1);
        }
    }
    return 0;
}

