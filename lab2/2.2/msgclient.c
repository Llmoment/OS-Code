#include "msgcom.h"

int main(){
    struct msgtype buf;
    int qid, pid;
    FILE *fp;

    fp = fopen("client.txt", "w");
    //获取消息队列标识数
    qid = msgget(MSGKEY, IPC_CREAT|0666);
    buf.mtype = SND_TYPE;
    strcpy(buf.text, "a");
    //发送消息请求
    msgsnd(qid, &buf, sizeof(buf.text), 0);
    //消息接收
    msgrcv(qid, &buf, sizeof(buf.text), RCV_TYPE, MSG_NOERROR);
    printf("Request received a messages from server\n");
    //写入文件
    fputs(buf.text, fp);
    fclose(fp);
    return 0;
}