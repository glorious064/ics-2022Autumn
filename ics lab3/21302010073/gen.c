#include "happy.h"

static struct gen
{
    int error;
    int data;
    void* stack;
    __ctx_type ctx;
    struct gen* caller;
    void* eh_list;
    void (*f)(int);
} __gen_0;
struct gen* __this_gen = &__gen_0;

int yield(int value)
{
    if (__this_gen->caller == NULL)
        throw(ERR_GENNIL);
    __this_gen->data = value;
    
    //TODO
    int temp = __ctx_record(&(__this_gen->ctx));//保存当前上下文
    if(temp==0)
    {
        __this_gen = __this_gen->caller; //将当前生成器切换为其caller
        __ctx_recover(&(__this_gen->ctx), -1);//恢复到caller生成器记录好的上下文
    }
    return __this_gen->data;
}  

int send(struct gen* gen, int value)
{
        
    if (gen == NULL)
        throw(ERR_GENNIL);
    if (gen->error)
        throw(gen->error);
    gen->data = value;
    // TODO
    int t = __ctx_record(&(__this_gen->ctx));//保存当前上下文
    if(t == 0)
    {
      gen->caller = __this_gen;
      __this_gen = gen;  //将目标generator的caller设置为当前generator并切换gen
      __ctx_recover(&(gen->ctx), -1);//恢复到目标generator上下文
    }
    
    if (gen->error)
        throw(gen->error);
    return gen->data;                                           
}

struct gen* generator(void (*f)(int), int arg)
{
    struct gen* g = malloc(sizeof(*g));
    memset(g, 0, sizeof(*g));
    g->f = f;
    g->stack = malloc(8192);
      
   g->eh_list = NULL;
   g->error = 0;
   void *temp = &(g->ctx);
   temp = (char*)temp ;
    *((u64*)temp) = g->stack+7992; //%rsp 对应我们设计的recover里面的 mov %rsp (%rdi)
    temp = (char*)temp + 8;
   *((u64*)temp) = f;//(%rsp)
    temp = (char*)temp + 8;
   *((u64*)temp) = g->stack+8000;//%rbp
    temp = (char*)temp + 8;
    *((u64*)temp) = skip_throw; //结束时的最终返回地址(%rbp)
    temp = (char*)temp + 8; 
   *((u64*)temp) = arg; //%rdi
   
    // TODO

    return g;
}

void genfree(struct gen** g)
{
    if (*g == NULL)
        throw(ERR_GENNIL);
    free((*g)->stack);
    free(*g);
    *g = NULL;
}

void __eh_push(void* ctx)
{
    assert(ctx != NULL);
    __ctx_type* temp = ctx;  
    temp->last = __this_gen->eh_list; //当前上下文指向前一个上下文
    __this_gen->eh_list = ctx;//链表头指向最新的上下文

    

    // TODO

}

void* __eh_pop()
{
    assert(__this_gen->eh_list != NULL);
    __ctx_type* temp = __this_gen->eh_list;//取出链表头指向的最新的上下文
    __this_gen->eh_list = temp->last;//更新链表头位置
    return temp;
    
    
    // TODO

}

void* __eh_head()
{
    return __this_gen->eh_list;
}

void throw(int error)
{
    if (error == 0)
        throw(ERR_THROW0);
    if(__this_gen->eh_list==NULL) //判断异常处理栈是否为空
    {
        __this_gen = __this_gen->caller;
        throw(ERR_GENEND);  
      }  
    
    __ctx_recover(__eh_pop(), error);

    assert(0); // shouldn't run here
}
void skip_throw()
{
  throw(ERR_GENEND); //跳板函数跳到throw
}
