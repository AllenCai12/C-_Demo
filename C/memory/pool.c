#include "pool.h"

/********************************************
 *@brief 创建n个节点的内存池链表
 *@param
 *    num           数量
 *    blocksize   内存块大小
 *@return OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
mempool *CreatePool(int num,int blocksize )
{
    if(num<=0||blocksize<=0)
    {
        printf("Create  input err\n");
        return NULL;
    }
    //内存池指针分配内存
    mempool *poolhead=NULL;     //内存池指针
    poolhead=(mempool*)malloc(sizeof(mempool));//池节点申请内存
    if (NULL==poolhead)
    {
        printf("poolhead malloc err!\n");
        return NULL;
    }
    memset(poolhead,0,sizeof(mempool));
    //内存池指针部分初始化赋值，其他默认为0无需初始
    poolhead->cnt=num;
    poolhead->blocksize=blocksize;
    //定义链表操作临时指针
    memblock *p1 = NULL;    //创建的新节点的地址
    memblock *p2 = NULL;    
    char* block;                        //内存块指针
    int n = 0;                          //创建前链表的节点总数为0：空链表
    //分配第一个内存块
    p1 = ( memblock *) malloc (LEN);    //开辟第一个新节点
    if (NULL == p1)
    {
        printf("p1 %d malloc err!\n",n+1);
        return NULL;
    }
    memset(p1,0,LEN);
    block=(char*)malloc(blocksize);//开辟内存块
    if (NULL == block)
    {
        printf("firstblock malloc err!\n");
        return NULL;
    }
    memset(block,0,blocksize);
    //*内存池，内存块初始化赋值
    p1->pmem=block;
    poolhead->firstaddr=block;
    p2 = p1;            //如果节点开辟成功，则p2先把它的指针保存下来以备后用
    if(p1==NULL)        //节点开辟不成功
    {
        printf ("\nCann't create it, try it again in a moment!\n");
        return NULL;
    }
    while(n <num)       
    {
        n += 1;         //节点总数增加1个   
        if(n == 1)      //如果节点总数是1，则head指向刚创建的节点p1
        {
            poolhead->firstblock = p1;
            p2->next = NULL;  //此时的p2就是p1,也就是p1->next指向NULL。
        }
        else
        {
            p2->next = p1;  //指向上次下面刚刚开辟的新节点
        }
        p2 = p1;            //把p1的地址给p2保留，然后p1产生新的节点

        p1 = ( memblock *) malloc (LEN);//开辟出新节点
        if (NULL == p1)
        {
            printf("p1 %d malloc err!\n",n+1);
            return NULL;
        }
        memset(p1,0,LEN);
        block=(char*)malloc(blocksize);//开辟出新内存块
        if (NULL == block)
        {
            printf("block %d malloc err!\n", n + 1);
            return NULL;
        }
        memset(block,0,blocksize);
        p1->pmem=block;
    }
    p2->next = NULL;        //链表的最后一个节点指向NULL
    poolhead->lastaddr=p2->pmem+blocksize;//内存池 末尾地址

    free(p1->pmem);
    free(p1);           //跳出了while循环，释放p1   多余的那个空间
    p1 = NULL;          
    return poolhead;        //返回创建链表的头指针 
}

/********************************************
 *@brief 销毁内存池
 *@param
 *    poolhead          内存池指针
 *@return OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
int  DestroyPool(mempool  *poolhead)  
{  
    if(poolhead==NULL)
    {
        return OPERA_ERR;
    }
    memblock  *p1=poolhead->firstblock;  
    memblock  *p2=p1;
    while(p1!=NULL)  
    {  
        p2=p1;  
        p1=p1->next;  
        free(p2->pmem);
        p2->pmem=NULL;
        free(p2);  
    }  
    poolhead->firstblock=NULL;  
    free(poolhead);
    poolhead = NULL;
    return OPERA_OK;  
} 

/********************************************
 *@brief  得到一个内存块
 *@param
 *    poolhead          内存池指针
 *@return 内存块地址
 *@see
 *********************************************/
char *GetMemblock(mempool *poolhead)
{
    if(poolhead->usedcnt==poolhead->cnt)
    {
        printf("GetMemblock ERR !Pool Full!\n");
        return NULL;
    }
    if (poolhead==NULL||poolhead->firstblock==NULL)
    {
        printf("pool in err!\n");
        return NULL;
    }
    memblock* p=poolhead->firstblock;
    poolhead->firstblock = p->next;
    p->next = NULL;
    poolhead->usedcnt++;
    return p->pmem;
}
/********************************************
 *@brief  释放一个内存块
 *@param
 *    pmem               内存块地址
 *    poolhead           内存池指针
 *@return  OPERA_OK/OPERA_ERR
 *@see
 *********************************************/
int ReleaseMemblock(char* pmem, mempool *poolhead)
{
    if(pmem==NULL)
    {
        printf("Realease Mem input ERR!\n");
        return OPERA_ERR;
    }
    memblock *ptemp = (memblock *)malloc(LEN);
    if (NULL == ptemp)
    {
        printf("ptemp malloc err!\n");
        return OPERA_ERR;
    }
    ptemp->pmem = pmem;
    ptemp->next=poolhead->firstblock;
    poolhead->firstblock = ptemp;
    poolhead->usedcnt--;
    return OPERA_OK;
}

