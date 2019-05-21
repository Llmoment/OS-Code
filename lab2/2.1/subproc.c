#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

//声明信号处理程序
void signal_process();

int main(){
    pid_t pid;
    int status;
    signal(SIGUSR1, signal_process);

    if (pid = fork()){
        //父进程
        printf("Parent: This is parent process.\n");
        printf("Parent: will send signal.\n");
        //父进程发送信号并等待子进程终止
        kill(pid, SIGUSR1);
        wait(&status);
        printf("status = %d: Parent finished.\n", status);
    }
    else{
        //子进程
        sleep(3);
        printf("Child: signal received.\n");
        printf("Child: listing files......\n");
        //子进程进行映像改换
        execl("/bin/ls", "ls", "-l", (char *) 0);
        //映像改换出错处理
        printf("Child: execl error.\n");
        exit(2);
    }
    return 0;
}

void signal_process(){
    printf("This is signal processing function.\n");
}