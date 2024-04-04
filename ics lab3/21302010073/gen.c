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
    int temp = __ctx_record(&(__this_gen->ctx));//���浱ǰ������
    if(temp==0)
    {
        __this_gen = __this_gen->caller; //����ǰ�������л�Ϊ��caller
        __ctx_recover(&(__this_gen->ctx), -1);//�ָ���caller��������¼�õ�������
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
    int t = __ctx_record(&(__this_gen->ctx));//���浱ǰ������
    if(t == 0)
    {
      gen->caller = __this_gen;
      __this_gen = gen;  //��Ŀ��generator��caller����Ϊ��ǰgenerator���л�gen
      __ctx_recover(&(gen->ctx), -1);//�ָ���Ŀ��generator������
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
    *((u64*)temp) = g->stack+7992; //%rsp ��Ӧ������Ƶ�recover����� mov %rsp (%rdi)
    temp = (char*)temp + 8;
   *((u64*)temp) = f;//(%rsp)
    temp = (char*)temp + 8;
   *((u64*)temp) = g->stack+8000;//%rbp
    temp = (char*)temp + 8;
    *((u64*)temp) = skip_throw; //����ʱ�����շ��ص�ַ(%rbp)
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
    temp->last = __this_gen->eh_list; //��ǰ������ָ��ǰһ��������
    __this_gen->eh_list = ctx;//����ͷָ�����µ�������

    

    // TODO

}

void* __eh_pop()
{
    assert(__this_gen->eh_list != NULL);
    __ctx_type* temp = __this_gen->eh_list;//ȡ������ͷָ������µ�������
    __this_gen->eh_list = temp->last;//��������ͷλ��
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
    if(__this_gen->eh_list==NULL) //�ж��쳣����ջ�Ƿ�Ϊ��
    {
        __this_gen = __this_gen->caller;
        throw(ERR_GENEND);  
      }  
    
    __ctx_recover(__eh_pop(), error);

    assert(0); // shouldn't run here
}
void skip_throw()
{
  throw(ERR_GENEND); //���庯������throw
}
