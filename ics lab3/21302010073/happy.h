#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <time.h>
#include <string.h>

typedef unsigned long long u64;
typedef struct Ctx{
char ctx[80]; //这个是用来保存寄存器和栈指针的
struct Ctx *last; //用于test3 4构建链表，指向上一个上下文
}__ctx_type; 
struct gen;

#define ERR_THROW0 -1
#define ERR_GENEND -2
#define ERR_GENNIL -3

int __ctx_record(__ctx_type*);
void __ctx_recover(__ctx_type*, int);

void __eh_push(void*);
void* __eh_pop();
void throw(int);
static inline void __eh_check_cleanup(int* error) {

    // TODO  
    if(*error==0)
    {
      __eh_pop(); //用于try正常执行后删去刚push进的上下文
    }
    
}

#define try int error __attribute__((cleanup(__eh_check_cleanup))) = 0; \
__ctx_type ctx;\
error = __ctx_record(&ctx);\ 
if(error == 0){\
  __eh_push(&ctx);


#define catch} else

// TODO

struct gen* generator(void (*)(int), int);
void genfree(struct gen**);
int yield(int);
int send(struct gen*, int);
void skip_throw(); //跳板函数
#define next(gen) send(gen, 0)
