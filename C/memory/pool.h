#ifndef _POOL_H
#define _POOL_H

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define LEN sizeof(memblock)//内存块节点大小
#define OPERA_OK  0//操作失败
#define OPERA_ERR 1//操作成功
//内存块节点
typedef struct MEMBLOCK
{
    char* pmem;             //内存指针 
    MEMBLOCK *next;     //指向下一节点的指针
}memblock;
//内存池节点
typedef struct MEMPOOL
{
    int  cnt;                               //数量
    int  usedcnt;                       //使用个数
    int blocksize;                      //内存块大小
    char* firstaddr;                    //起始地址
    char* lastaddr;                     //结束地址
    MEMBLOCK *firstblock;       //指向下一节点的指针
}mempool;

/********************************************
 *@brief 创建n个节点的内存池链表
 *@param
 *    num           数量
 *    blocksize   内存块大小
 *@return OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
mempool *CreatePool(int num, int blocksize);

/********************************************
 *@brief 销毁内存池
 *@param
 *    poolhead         内存池指针
 *@return OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
int  DestroyPool(mempool  *poolhead);

/********************************************
 *@brief  得到一个内存块
 *@param
 *    poolhead            内存池指针
 *@return 内存块地址
 *@see
 *********************************************/
char *GetMemblock(mempool *poolhead);

/********************************************
 *@brief  释放一个内存块
 *@param
 *    pmem               内存块地址
 *    poolhead       内存池指针
 *@return  OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
int ReleaseMemblock(char* pmem, mempool *poolhead);


#endif

