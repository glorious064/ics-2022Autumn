# ICS Lab2-BombLab 实验报告

​				                                                                                                                                                        **——邹懿 21302010073**

## 实验思路

### phase_1

首先可以发现，phase_1的参数是一个字符串，而string_not_equal的参数为两个字符串，**根据函数名可以知道其实是在判断输入的字符串是否和string_not_equal的第二个参数是否相同**，然后就在进入string_not_equal前打印%rsi储存的值查看，如图

![](D:\桌面\微信截图_20221019201722.png)

发现为“each line is important",然后将其作为第一关的password输入发现猜想正确！

### phase_2

同理，我们知道phase_2参数为字符串，同时函数调用了read_six_numbers检查输入字符串是否符合某种要求，通过函数名同样可以知道实际上这道题我们需要**输入的是六个数字**。紧接着探索这六个数字满足的关系,如下图，我发现%rax保存的是我们输入的第一数字num1，%ecx储存的是一个定值k，然后在1b5c这一行将%rax和%ecx表示的值相乘，并和 0x4(%rax)代表的值比较，而联想到int为4个字节可以猜测0x4(%rax)也就是输入的第二个数num2，并且由1b62这一行我们知道必须满足num2=k*num1,继续下去也就是一个循环判断相邻两个数是否满足同样的关系，**因此这道题就是输入一个以k为公比的等比数列**

```c
	1b49:	48 8d 05 d0 24 00 00 	lea    0x24d0(%rip),%rax        # 4020 <phase_2_nums>
    1b50:	8b 0d e6 24 00 00    	mov    0x24e6(%rip),%ecx        # 403c <phase_2_nums+0x1c>
    1b56:	48 8d 50 14          	lea    0x14(%rax),%rdx
    1b5a:	8b 30                	mov    (%rax),%esi
    1b5c:	0f af f1             	imul   %ecx,%esi
    1b5f:	39 70 04             	cmp    %esi,0x4(%rax)
    1b62:	74 05                	je     1b69 <phase_2(char*)+0x31>
    1b64:	e8 dd fe ff ff       	callq  1a46 <explode_bomb()>
    1b69:	48 83 c0 04          	add    $0x4,%rax
    1b6d:	48 39 c2             	cmp    %rax,%rdx
    1b70:	75 e8                	jne    1b5a <phase_2(char*)+0x22>
    1b72:	58                   	pop    %rax
    1b73:	c3                   	retq   
```

打印%ecx代表的值如下图，发现k=-3

![](D:\桌面\微信截图_20221019203346.png)

### phase_3

phase_3我认为最重要去理解下面这几行(如下图)，前三行我最先很困惑，不知道在干嘛，我先是跳过这几行观察1ba2这一行发现实际上在判断输入的字符串是否有三个字符，然后我便猜测前三行应该是在把输入的三个参数传入对应的寄存器，其中1st--0x10(%rsp),2nd--0xf(%rsp),3th--0x14(%rsp),当然也可以实际验证,了解了这个之后就在后面寻找对应的满足条件。

```c

  	1b93:	48 8d 4c 24 0f       	lea    0xf(%rsp),%rcx
    1b98:	48 8d 54 24 10       	lea    0x10(%rsp),%rdx
    1b9d:	4c 8d 44 24 14       	lea    0x14(%rsp),%r8
    1ba2:	e8 c9 f5 ff ff       	callq  1170 <__isoc99_sscanf@plt>
    1ba7:	83 f8 03             	cmp    $0x3,%eax
    1baa:	75 65                	jne    1c11 <phase_3(char*)+0x9d>
```

**1st parameter:**

```c

	1bac:	83 7c 24 10 07       	cmpl   $0x7,0x10(%rsp)
    1bb1:	77 5e                	ja     1c11 <phase_3(char*)+0x9d>
```

我们发现**第一个数字首先应该满足小于等于7**

**3th parameter:**

紧接着继续运行我发现给出了第三个参数需要满足的条件

```c
 	1bef:	83 7c 24 14 10       	cmpl   $0x10,0x14(%rsp)
    1bf4:	b0 74                	mov    $0x74,%al
    1bf6:	74 1e                	je     1c16 <phase_3(char*)+0xa2>
    1bf8:	eb 17                	jmp    1c11 <phase_3(char*)+0x9d>
```

我发现**第三个数字必须等于0x10也就是16**(虽然前期被炸傻后还以为0x10就是10orz)

**2nd parameter:**

```c
  //需要满足：
 	1c16:	38 44 24 0f          	cmp    %al,0xf(%rsp)
    1c1a:	75 f5                	jne    1c11 <phase_3(char*)+0x9d>

```

可以发现第二个参数对应的值应该和%al的值相同，而从前面 mov    $0x74,%al 指令可以知道它等于116

然后！！！我就认为第二个字符就是116可是一直错误！！后来我突然根据第一个参数和第二个参数栈指针偏移量只相差1推断2nd 参数是char类型，116是其ascaii码，**所以应该是t!!**

### phase_4

首先需要明确其**参数是一个64位的整数**，这里我们设为x,然后分析代码

**第一个条件：**

```c
 	1c3a:	48 89 f8             	mov    %rdi,%rax
    1c3d:	48 c1 ff 20          	sar    $0x20,%rdi
    1c41:	52                   	push   %rdx
    1c42:	8d 57 ff             	lea    -0x1(%rdi),%edx
    1c45:	83 fa 0d             	cmp    $0xd,%edx
    1c48:	77 07                	ja     1c51 <phase_4(long)+0x1b>
```

这里首先是将x的值先赋值给%rax储存，然后将x右移32位，之后比较(x<<31-1)与0xd(13)的大小，**必须满足(x<<31-1)<=14**，即：x的前32位所代表的数字必须不大于14

**第二个条件：**

```c
  	1c4a:	ff c8                	dec    %eax
    1c4c:	83 f8 0d             	cmp    $0xd,%eax
    1c4f:	76 05                	jbe    1c56 <phase_4(long)+0x20>
```

这里%eax储存的是x的后32位，这里表明其后32位代表的数字x'必须满足(x'-1)<=13**,也就是x'不大于14**

**第三个条件：**

第三个条件需要在美丽的递归中去体会，我们需要在hope(int)函数去”感悟“，不过在这之前这里需要注意传入的是32位，然后通过观察发现它实际上传入的是**x的前32位x1**，接下来就可以开启“递归体验之旅”了,以下是我反汇编后猜测的源代码

```c
  int hope(int x){
        int result;
        int num1 = 1;
        if(!x) {
            result = num1;
            return result;
        }
        int xx = x;
        x = x>>1;
        int value = hope(x);
        value = value*value;
        if(xx&1) value *= 4; 
        result = value;
        return result;     
    }
```

然后我们返回看phase_4后面是在比较hope函数返回值是否与0x1000000（2的24次方）相等，然后我就想怎么能凑出这个答案推导如下

<img src="D:\桌面\微信图片_20221019220530.jpg" style="zoom:67%;" />

### phase_5

对于phase_5的评价是华而不实，但又却始终重要，这里就不分析另外两种情况，只分析开启secret_phase的第三种情况(**第一个数字是8**

首先需要明确它**有三个参数**，然后很简单的我们确定第一个数字为8之后进入对应的函数的极限套娃acquire(int ), is_holding(int ), release(int , int),is_holding(int),release(int).....这需要十分耐心去发现跳出的条件，否则就会进入死循环，这其中最重要的的句子就是

```c
   0x5555555554bc <_ZN8baselock7releaseEii+18>: cmp    $0xff0,%ebp
   0x5555555554c2 <_ZN8baselock7releaseEii+24>: jne    0x5555555554d9 <_ZN8baselock7releaseEii+47>
```

其中%ebp储存的是%edx也就是第三个参数的值，着这行语句要求**第三个参数值必须等于0xff0(4080)**,而对第二个参数值未作要求

### phase_6

```c
   0x555555555d0f <_Z7phase_6Pc+5>:     callq  0x555555555a7c <_Z10string_lenPc>
   0x555555555d14 <_Z7phase_6Pc+10>:    lea    0x27f0(%rip),%rdx        # 0x55555555850b <w2+11>
   0x555555555d1b <_Z7phase_6Pc+17>:    mov    %eax,%r8d
   0x555555555d1e <_Z7phase_6Pc+20>:    xor    %eax,%eax
   0x555555555d20 <_Z7phase_6Pc+22>:    cmp    $0x6,%r8d
   0x555555555d24 <_Z7phase_6Pc+26>:    je     0x555555555d2b <_Z7phase_6Pc+33>
   0x555555555d26 <_Z7phase_6Pc+28>:    callq  0x555555555a46 <_Z12explode_bombv>
```

首先phase_6参数同样为字符串，从调用string_len函数以及比较返回值与6的大小我们可以知道是在要求输入字符串长度必须为6，但其实我们仍然对这个字符串的内容十分茫然，但是！下面的这两行给了我灵感

```c
	1d40:	48 8d 3d b9 27 00 00 	lea    0x27b9(%rip),%rdi        # 4500 <w2>
    1d47:	e8 19 fa ff ff       	callq  1765 <build_candidate_expression_tree(char*, int)>
```

通过bulid_candidate_expression_tree我大概能知道这个字符串应该与计算表达式有关，然后打印0x27b9(%rip)验证（其中/(3-4)是我们自己输入的）

![](D:\桌面\微信截图_20221019230338.png)

这说明我们的猜想正确，然后通过build_candidate_expression_tree函数名可以知道，这是将中缀表达式搭建为二叉树，然后在phase_6后半部分中可以知道，实际上在判断我们的candidata_tree和answer_tree时候相同，这就考验我们的对比能力了

![](D:\桌面\微信图片_20221019232326.jpg)

红色圈的是对应的地址位置

其中answer_tree为

<img src="D:\桌面\微信图片_20221019232351.png" style="zoom:67%;" />

candidate_tree为

<img src="D:\桌面\微信图片_20221019232355.png" style="zoom:67%;" />

然后我们对比这两棵树发现，答案树中和我们搭建的树相同位置的值有几点不同：51‘3’——57‘9’，52‘4’——48‘0’，47‘/’——42‘*’，而我们输入的‘-’号能够相对应说明输入正确，对应调整后可以得出正确答案为/(3-4)

### secret_phase

主要是打印查看答案，secret_phase主要就是两处比较大小满足即可，打印如下（此时输入为1000）

![](D:\桌面\微信图片_20221019233403.jpg)

实际实验中知道，%xmm0代表的值首先被赋值为2*x(x为输入值的后32位所代表的数），然后加10，我们发现需要满足2*x+10>3819.9999...,可以让它为3820然后得出x为1905，然后试一下！！正确了！！拆弹结束！

## 实验结果

![](D:\桌面\微信截图_20221019210434.png)



