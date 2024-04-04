.text

.globl __ctx_record
__ctx_record:
// TODO
endbr64
mov     %rsp,(%rdi)
mov     (%rsp),%rax
mov    %rax,0x8(%rdi)
mov     %rbp,0x10(%rdi)
mov    (%rbp),%rax
mov     %rax,0x18(%rdi)
mov    %rdi,0x20(%rdi)
xor     %eax,%eax
retq


.globl __ctx_recover
__ctx_recover:

// TODO
endbr64
mov    (%rdi),%rsp
mov     0x8(%rdi),%rax
mov    %rax,(%rsp)
mov    0x10(%rdi),%rbp
mov    0x18(%rdi),%rax
mov    %rax,(%rbp)
mov   0x20(%rdi),%rdi
mov   %rsi,%rax
retq

