#include "msgcom.h"

int main(){
    struct msgtype buf;
    int qid;
    char str[50];  //暂存文件内容
    FILE *fp;

    fp = fopen("server.txt", "r");
    fgets(str, 50, fp);
    fclose(fp);
    printf("The content of file: %s", str);
    
    //尝试创建消息队列，直至成功为止
    if(qid = msgget(MSGKEY, IPC_CREAT|0666) == -1){
        return (-1);
    }
    while(1){
        //等待client的数据请求
        msgrcv(qid, &buf, 512, SND_TYPE, MSG_NOERROR);
        printf("Server receives a request from client.\n");
        //将返回消息类型设置为约定值
        buf.mtype = RCV_TYPE;
        strcpy(buf.text, str);
        msgsnd(qid, &buf, sizeof(buf.text), 0);
    }
}