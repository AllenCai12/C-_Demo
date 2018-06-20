//
//  main.c
//  Queue_LinkedList
//
//  Created by chenyufeng on 16/3/7.
//  Copyright © 2016年 chenyufengweb. All rights reserved.
//

/**
 *  使用带头结点尾结点的的非循环双向链表链表来实现队列，因为当使用单链表的时候，不会存在像数组那样的节点浪费，所以当使用链表实现队列的时候，可以不用循环队列，简化操作。

 在双链表的头部进行删除，尾部进行插入；
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int elemType;
typedef struct MyQueue{

    int element;
    struct MyQueue *prior;
    struct MyQueue *next;
}Queue;

void InitialQueue(Queue **pHead,Queue **pTail);
void EnQueue(Queue *head,Queue *tail,int x);
void DeQueue(Queue *head,Queue *tail);
void PrintQueue(Queue *head,Queue *tail);
int IsEmpty(Queue *head,Queue *tail);


//初始化带头结点和尾结点的非循环双向链表
void InitialQueue(Queue **pHead,Queue **pTail){

    *pHead = (Queue *)malloc(sizeof(Queue));
    *pTail = (Queue *)malloc(sizeof(Queue));

    if (*pHead == NULL || *pTail == NULL) {
        printf("%s函数执行，内存分配失败，初始化双链表失败\n",__FUNCTION__);
    }else{
        //这个里面是关键，也是判空的重要条件
        (*pHead)->next = NULL;
        (*pTail)->prior = NULL;

        //链表为空的时候把头结点和尾结点连起来
        (*pHead)->prior = *pTail;
        (*pTail)->next = *pHead;

        printf("%s函数执行，带头结点和尾节点的双向非循环链表初始化成功\n",__FUNCTION__);
    }
}

//入队,也就是在链表的尾部插入节点
void EnQueue(Queue *head,Queue *tail,int x){

    Queue *pInsert;
    pInsert = (Queue *)malloc(sizeof(Queue));
    memset(pInsert, 0, sizeof(Queue));
    pInsert->next = NULL;
    pInsert->prior = NULL;
    pInsert->element = x;

    tail->next->prior = pInsert;
    pInsert->next = tail->next;
    tail->next = pInsert;
    pInsert->prior = tail;
}

//出队,在队列头部删除元素
void DeQueue(Queue *head,Queue *tail){

    if (IsEmpty(head,tail)) {
        printf("队列为空，出队列失败\n");
    }else{

        Queue *pFreeNode;
        pFreeNode = head->prior;
        head->prior->prior->next = head;
        head->prior = head->prior->prior;

        free(pFreeNode);
        pFreeNode = NULL;
    }
}

//打印出从队列头部到尾部的所有元素
void PrintQueue(Queue *head,Queue *tail){

    Queue *pMove;
    pMove = head->prior;
    printf("当前队列中的元素为(从头部开始):");
    while (pMove != tail) {
        printf("%d ",pMove->element);
        pMove = pMove->prior;
    }
    printf("\n");
}



//判断队列是否为空,为空返回1，否则返回0
int IsEmpty(Queue *head,Queue *tail){

    if (head->prior == tail) {
        return 1;
    }

    return 0;
}

int main(int argc, const char * argv[]) {

    Queue *pHead;//头结点
    Queue *pTail;//尾结点

    InitialQueue(&pHead, &pTail);

    EnQueue(pHead, pTail, 2);EnQueue(pHead, pTail, 1);
    EnQueue(pHead, pTail, 9);EnQueue(pHead, pTail, 3);EnQueue(pHead, pTail, 4);
    PrintQueue(pHead, pTail);

    DeQueue(pHead,pTail);DeQueue(pHead,pTail);DeQueue(pHead,pTail);
    PrintQueue(pHead, pTail);
    free(pHead);
    free(pTail);

    return 0;
}


