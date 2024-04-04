#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <fstream>
#include <sstream>
#include<string.h>
#include <vector>
using namespace std;
typedef enum
{
	AOK = 1,  //正常操作
	HLT = 2, //遇到halt指令
	ADR = 3, //遇到非法地址
	INS = 4  //遇到非法指令
} status;

class CPU
{
    /*
    私有成员变量声明：
    R[15] : 为64位寄存器数组，存储不同寄存器的值
    PC : 无符号64位数，代表下一条指令地址
    valP : 记录下一条指令地址
    M[10000] / Mout[10000] : 前者为内存数组，后者为内存输出数组
    icode : 第一个字节高四位代码部分
    ifun : 第一个字节的第四位功能部分
    rA, rB : 需要寄存器指令中对应的寄存器
    ZF SF OF: 三个条件码，分别初始化为1，0，0
    Cnd : 判断条件（为1则满足）
    valA, valB : 分别为寄存器rA, rB存储的值
    valC : 在需要valC的指令中对应位最后8个字节储存的数
    valE, valM : 分别为执行和访存中计算的数
    ofstream o("output.json) : 一个全局输出流，将结果输出到目标文件
*/
private:
    int64_t R[15], frsp;
    uint64_t PC, valP;
    string M[10000], Mout[10000];
    int icode, ifun, rA, rB;
    int Cnd, ZF, SF, OF;
    int64_t valA, valC, valB, valE, valM;
    status Stat;
    ofstream o;
public:
    CPU();
    vector<string> //分割字符串函数
        _split(string temp);
    int read_file(string path);
    string GetBinaryStringFromHexString(string strHex);
    string Dectohex(int64_t num);
    double fhextoDec(string hex);
    long long hextoDec(string hex);
    int charToint(char x);
    bool instr_valid(int icode, int ifun);
    bool need_regids(int icode);
    bool need_valC(int icode);
    int get_icode(string instr);
    int get_ifun(string instr);
    int get_rA(string instr);
    int get_rB(string instr);
    uint64_t get_valP(int icode);
    int64_t get_valC(int icode, string instr);
    void fetch(int64_t pc, string instr);
    void PC_update();
    void write_back_IADDq();
    void execute_IADDq();
    void decode_IADDq();
    void write_back_OPq();
    void execute_OPq();
    void decode_OPq();
    void write_back_rrmovq();
    void execute_rrmovq();
    void decode_rrmovq();
    void write_back_cmovxx();
    void execute_cmovxx();
    void decode_cmovxx();
    void write_back_irmovq();
    void execute_irmovq();
    void memory_rmmovq();
    void execute_rmmovq();
    void decode_rmmovq();
    void write_back_mrmovq();
    void memory_mrmovq();
    void execute_mrmovq();
    void decode_mrmovq();
    void write_back_pushq();
    void memory_pushq();
    void execute_pushq();
    void decode_pushq();
    void write_back_popq();
    void memory_popq();
    void execute_popq();
    void decode_popq();
    void PC_update_jxx_Dest();
    void execute_jmp_Dest();
    void execute_jle_Dest();
    void execute_jl_Dest();
    void execute_je_Dest();
    void execute_jne_Dest();
    void execute_jge_Dest();
    void execute_jg_Dest();
    void PC_update_call_Dest();
    void write_back_call_Dest();
    void memory_call_Dest();
    void execute_call_Dest();
    void decode_call_Dest();
    void PC_update_ret();
    void write_back_ret();
    void memory_ret();
    void execute_ret();
    void decode_ret();
    void output();
    void out_mem2(ofstream& o);
    void out_mem();
    void M_to_Mout();
    void run(string path);
    void write(string path);
};
#endif

