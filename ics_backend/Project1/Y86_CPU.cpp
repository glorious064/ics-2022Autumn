/*
   项目名称：Y86-CPU
   设计者：匡子奕 邹懿
   介绍：本项目基于Y86的顺序实现，以Y86的汇编代码文件作为输入，完成了不同指令（包括增加的iaddq)的实现，模拟以Y86-64指令集为架构的CPU处理器
*/

#include <iostream>
#include"Cpu.h"

using namespace std;


int main() {
    CPU simulator;
    simulator.write("j-cc.yo");
    return 0;
}
