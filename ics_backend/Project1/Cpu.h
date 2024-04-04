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
	AOK = 1,  //��������
	HLT = 2, //����haltָ��
	ADR = 3, //�����Ƿ���ַ
	INS = 4  //�����Ƿ�ָ��
} status;

class CPU
{
    /*
    ˽�г�Ա����������
    R[15] : Ϊ64λ�Ĵ������飬�洢��ͬ�Ĵ�����ֵ
    PC : �޷���64λ����������һ��ָ���ַ
    valP : ��¼��һ��ָ���ַ
    M[10000] / Mout[10000] : ǰ��Ϊ�ڴ����飬����Ϊ�ڴ��������
    icode : ��һ���ֽڸ���λ���벿��
    ifun : ��һ���ֽڵĵ���λ���ܲ���
    rA, rB : ��Ҫ�Ĵ���ָ���ж�Ӧ�ļĴ���
    ZF SF OF: ���������룬�ֱ��ʼ��Ϊ1��0��0
    Cnd : �ж�������Ϊ1�����㣩
    valA, valB : �ֱ�Ϊ�Ĵ���rA, rB�洢��ֵ
    valC : ����ҪvalC��ָ���ж�Ӧλ���8���ֽڴ������
    valE, valM : �ֱ�Ϊִ�кͷô��м������
    ofstream o("output.json) : һ��ȫ�������������������Ŀ���ļ�
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
    vector<string> //�ָ��ַ�������
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

