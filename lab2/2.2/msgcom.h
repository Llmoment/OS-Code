#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MSGKEY 5678  //消息队列关键字
#define SND_TYPE 1   //client发送消息类型
#define RCV_TYPE 4   //server发送消息类型

struct msgtype{
    long mtype;
    char text[50];   //传送消息结构
};