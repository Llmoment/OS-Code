#include <stdio.h>
#include <stdlib.h>

struct map {
    unsigned int m_size;
    char *m_addr;
    struct map *next, *pre;
} ;

char* lmalloc(unsigned int size, struct map *&coremap,struct map* tail);
void lfree(unsigned int size, char* addr, struct map* head,struct map* tail);
void PrintMap(struct map* head,char* base_addr,struct map* tail);


int main(){
    struct map *coremap, *head, *tail; //空闲存储区表
    char *p; //内存空间
    p = (char *)malloc(1024);
    //initializing the head node and tail node
    coremap = (struct map *)malloc(sizeof(struct map));
    tail = (struct map *)malloc(sizeof(struct map));
    head = (struct map *)malloc(sizeof(struct map));
    coremap->m_size = 1024;
    coremap->m_addr = p;
    coremap->next = tail;
    coremap->pre = head;
    tail->m_size = 0;
    tail->m_addr = p + 1024;
    tail->next = head;
    tail->pre = coremap;
    head->m_size = 0;
    head->m_addr = p;
    head->next = coremap;
    head->pre = tail;

    char* addr;
    unsigned int size, relative_addr; //relative_addr为相对地址
    char command;

    //等待命令
    while (true){
        scanf("%c",&command);
        switch(command){
            case 'm':
                scanf("%d",&size);
                addr = lmalloc(size,coremap,tail);
                printf("\nmemory state:\n");
                PrintMap(head, p, tail);
                printf("\n");
                break;
            case 'f':
                scanf("%d%d", &size, &relative_addr);
                lfree(size, p+relative_addr, head, tail);
                printf("\nmemory state:\n");
                PrintMap(head, p, tail);
                printf("\n");
                break;
            case 's':
                return 0;
            default:
                break;
        }
    }

    return 0;
}

char* lmalloc(unsigned int size, struct map *&coremap,struct map* tail){
    char *a;
    struct map *bp;
    bp = coremap;
    do{
        if(bp->m_size >= size){
            a = bp->m_addr;
            bp->m_addr += size;
            if((bp->m_size -= size) == 0){ //该空闲区大小为零时，从链表中删除该节点 (未考虑空闲区在结尾的情况)
                bp->next->pre = bp->pre;
                bp->pre->next = bp->next;          
            }
            coremap = bp->next;
            while(coremap->m_size == 0){ //coremap不能是头尾节点
                coremap = coremap->next;
            }
            return a;
        }
        bp = bp->next;
    }while(bp != coremap);
    printf("Not enough memory !\n");
    return NULL;
}

void lfree(unsigned int size, char* addr, struct map* head,struct map* tail){
    struct map *bp;
    if(size == 0){
        printf("The memory size can not be zero !");
        return;
    }

    for(bp = head->next;((bp->m_addr <= addr) && (bp->next != tail)); bp = bp->next);
    if(bp->pre->m_addr + bp->pre->m_size == addr && bp->pre->m_size != 0){ //与前空闲区相连
        bp->pre->m_size += size;
        if(addr + size == bp->m_addr){ //与前后空闲区相连
            bp->pre->m_size += bp->m_size;
            //删除bp自身
            bp->next->pre = bp->pre;
            bp->pre->next = bp->next;
        }
    }
    else{
        if(addr + size ==bp->m_addr){ //只与后空闲区相连
            bp->m_addr -= size;
            bp->m_size += size;
        }
        else{ //不与其他空闲区相连，或位于链尾
            struct map * new_space;
            //创建新节点
            new_space = (struct map *)malloc(sizeof(struct map));
            new_space->m_size = size;
            new_space->m_addr = addr;
            new_space->pre = bp->pre;
            new_space->next = bp;
            //插入新节点
            bp->pre->next = new_space;
            bp->pre = new_space;
        }

    }
    return;
}

void PrintMap(struct map* head,char* base_addr,struct map* tail){
    //以相对地址的方式输出内存使用及空闲情况
    struct map* temp;
    if(head->next->m_addr > base_addr){
        printf("|-------%-4d-------|\n",base_addr - base_addr);
        printf("     used block      \n");
    }
    for(temp = head->next; temp != tail; temp = temp->next){
        printf("|-------%-4d-------|\n",temp->m_addr - base_addr);
        printf("     free block       \n");
        if(temp->next == tail){
            if(temp->m_addr + temp->m_size == tail->m_addr){
                printf("|-------%-4d-------|\n",tail->m_addr - base_addr);
            }
            else{
                printf("|-------%-4d-------|\n",temp->m_addr + temp->m_size - base_addr);
                printf("     used block       \n");
                printf("|-------%-4d-------|\n",tail->m_addr - base_addr);
            }
        }
        else{
                printf("|-------%-4d-------|\n",temp->m_addr + temp->m_size - base_addr);
                printf("     used block       \n");
        }
    }
    return;
}

