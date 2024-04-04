#include <iostream>
#include <algorithm>
#include"Cpu.h"
#include "Instr.h"
#include "Reg.h"
using namespace std;

CPU::CPU() 
{
    for (int i = 0; i < 10000; i++)
    {
        M[i] = "invalid";
        Mout[i] = "00";
    }
    memset(R, 0, sizeof(R));
    ZF = 1; SF = OF = 0; //初始化状态码
    Stat = AOK; //初始化状态为正常操作
    o.open("output.json");
}

vector<string>CPU::_split(string temp) 
{
    vector<string> str;
    string word;
    stringstream input;
    input << temp;
    while (input >> word)
    {
        str.push_back(word);
    }
    return str;

}

int CPU::read_file(string path)
{
    ifstream read(path, ios::in);
    if (!read)
    {
        cout << "the file can not open" << endl;
        return -1;
    }
    string str;
    while (getline(read, str) && str != "")
    {
        vector<string> vec = _split(str);
        if (vec[0][0] == '0' && vec[0][1] == 'x' && ((vec[1][0] >= '0' && vec[1][0] <= '9') || vec[1][0] >= 'a' && vec[1][0] <= 'f'))//只读取有用的代表指令的行数
        {
            int k = hextoDec(vec[0].substr(0, vec[0].find_last_of(':')));
            M[k] = vec[1];
        }

    }
    return 0;
}

string CPU::GetBinaryStringFromHexString(string strHex)//将16进制字符串转换为2进制
{
    string sReturn = "";
    unsigned int len = strHex.length();
    for (unsigned int i = 0; i < len; i++)
    {
        switch (strHex[i])
        {
        case '0': sReturn.append("0000"); break;
        case '1': sReturn.append("0001"); break;
        case '2': sReturn.append("0010"); break;
        case '3': sReturn.append("0011"); break;
        case '4': sReturn.append("0100"); break;
        case '5': sReturn.append("0101"); break;
        case '6': sReturn.append("0110"); break;
        case '7': sReturn.append("0111"); break;
        case '8': sReturn.append("1000"); break;
        case '9': sReturn.append("1001"); break;
        case 'a': sReturn.append("1010"); break;
        case 'b': sReturn.append("1011"); break;
        case 'c': sReturn.append("1100"); break;
        case 'd': sReturn.append("1101"); break;
        case 'e': sReturn.append("1110"); break;
        case 'f': sReturn.append("1111"); break;
        }
    }
    return sReturn;
}

string CPU::Dectohex(int64_t num) //10进制转16进制字符串
{
    if (num < 0)
    {
        uint64_t x = 0;
        x = uint64_t(num);
        int y = 0;//循环中的余数
        string s = "";//输出的结果
        if (x == 0)//0比较特殊，单独处理
        {
            s = '0' + s;
        }

        while (x > 0)//大于0的数
        {
            y = x % 16;  //求余
            if (y < 10)   //小于10的余数
                s = char('0' + y) + s;  //利用字符的ascll码在字符串前面拼接
            else
                s = char('a' - 10 + y) + s;  //大于9的余数用abcde表示
            x = x / 16;
        }
        if (s.length() % 2)
            s = '0' + s;
        for (int i = s.length(); i <= 15; i++)
            s = '0' + s;
        string str = "";
        int index = 0;
        for (int i = s.length() - 2; i >= 0; i = i - 2) //内存统一用小端法储存
        {
            str += s[i];
            str += s[i + 1];
            index = index + 2;
        }
        return str;
    }
    int y = 0;
    string s = "";
    if (num == 0)
    {
        s = '0' + s;
    }

    while (num > 0)
    {
        y = num % 16;
        if (y < 10)
            s = char('0' + y) + s;
        else
            s = char('a' - 10 + y) + s;
        num = num / 16;
    }
    if (s.length() % 2)
        s = '0' + s;
    for (int i = s.length(); i <= 15; i++)
        s = '0' + s;
    string str = "";
    int index = 0;
    for (int i = s.length() - 2; i >= 0; i = i - 2)
    {
        str += s[i];
        str += s[i + 1];
        index = index + 2;
    }
    return str;
}

double CPU::fhextoDec(string hex)  //16进制字符串转十进制
{
    double sum = 0, p = 0;
    for (size_t now = hex.size() - 1; now > 1; now--) {
        switch (hex[now]) {
        case'1':
            sum += 14 * (long long)pow(16, p);
            break;
        case'2':
            sum += 13 * (long long)pow(16, p);
            break;
        case'3':
            sum += 12 * (long long)pow(16, p);
            break;
        case'4':
            sum += 11 * (long long)pow(16, p);
            break;
        case'5':
            sum += 10 * (long long)pow(16, p);
            break;
        case'6':
            sum += 9 * (long long)pow(16, p);
            break;
        case'7':
            sum += 8 * (long long)pow(16, p);
            break;
        case'8':
            sum += 7 * (long long)pow(16, p);
            break;
        case'9':
            sum += 6 * (long long)pow(16, p);
            break;
        case'a':
            sum += 5 * (long long)pow(16, p);
            break;
        case'b':
            sum += 4 * (long long)pow(16, p);
            break;
        case'c':
            sum += 3 * (long long)pow(16, p);
            break;
        case'd':
            sum += 2 * (long long)pow(16, p);
            break;
        case'e':
            sum += 1 * (long long)pow(16, p);
            break;
        case'0':
            sum += 15 * (long long)pow(16, p);
            break;
        default:
            break;
        }
        p++;
    }
    return -sum - 1;
}

long long CPU::hextoDec(string hex) 
{
    long long sum = 0, p = 0, flag = 1;
    if (hex.size() == 18 && (hex[2] == 'f' || hex[2] == 'e' || hex[2] == 'd' || hex[2] == 'c' || hex[2] == 'b' || hex[2] == 'a' || hex[2] == '9' || hex[2] == '8'))
    {
        return fhextoDec(hex);
    }
    int count = 1;
    if (hex[0] != '0' && hex[1] != 'x')
        count = -1;
    for (size_t now = hex.size() - 1; now > count; now--) {
        switch (hex[now]) {
        case'1':
            sum += 1 * (long long)pow(16, p);
            break;
        case'2':
            sum += 2 * (long long)pow(16, p);
            break;
        case'3':
            sum += 3 * (long long)pow(16, p);
            break;
        case'4':
            sum += 4 * (long long)pow(16, p);
            break;
        case'5':
            sum += 5 * (long long)pow(16, p);
            break;
        case'6':
            sum += 6 * (long long)pow(16, p);
            break;
        case'7':
            sum += 7 * (long long)(long long)pow(16, p);
            break;
        case'8':
            sum += 8 * (long long)pow(16, p);
            break;
        case'9':
            sum += 9 * (long long)pow(16, p);
            break;
        case'a':
            sum += 10 * (long long)pow(16, p);
            break;
        case'b':
            sum += 11 * (long long)pow(16, p);
            break;
        case'c':
            sum += 12 * (long long)pow(16, p);
            break;
        case'd':
            sum += 13 * (long long)pow(16, p);
            break;
        case'e':
            sum += 14 * (long long)pow(16, p);
            break;
        case'f':
            sum += 15 * (long long)pow(16, p);
            break;
        default:
            break;
        }
        p++;
    }
    return sum;
}

int CPU::charToint(char x)
{
    if (x >= '0' && x <= '9')
    {
        return int(x - 48);
    }
    switch (x)
    {
    case 'a':
        return 0xa;
    case 'b':
        return 0xb;
    case 'c':
        return 0xc;
    case 'd':
        return 0xd;
    case 'e':
        return 0xe;
    case 'f':
        return 0xf;
    }
}

bool CPU::instr_valid(int icode, int ifun) //判断指令是否合法
{

    switch (icode)
    {
    case 0: //halt
    case 1: //nop
    case 3: //irmovq
    case 4: // rmmovq
    case 5: // mrmovq
    case 8: //call
    case 9: //ret
    case 0xb: //pop
    case 0xa: //pushq
    case 0xc: //iaddq
        if (ifun == 0) // halt
            return 1;
        break;
    case 2: // rrmovq cmovxx
    case 7: // jxx
        if (ifun >= 0 && ifun <= 6)
            return 1;
        break;
    case 6:
        if (ifun >= 0 && ifun <= 4) // opq
            return 1;
        break;
    default:
        break;
    }
    return 0;
}

bool CPU::need_regids(int icode)
{
    if (icode == IRRMOVQ || icode == IOPQ ||
        icode == IPUSHQ || icode == IPOPQ ||
        icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ || icode == IIADDQ)
        return 1;
    else
        return 0;
}

bool CPU::need_valC(int icode)
{
    if (icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ || icode == IJXX || icode == ICALL || icode == IIADDQ)
        return 1;
    else
        return 0;
}

int CPU::get_icode(string instr)
{
    return charToint(instr[0]);
}

int CPU::get_ifun(string instr)
{
    return charToint(instr[1]);
}

int CPU::get_rA(string instr)
{
    if (instr[0] == '3')
    {
        return 0xf;
    }
    return charToint(instr[2]);
}

int CPU::get_rB(string instr)
{
    if (instr[0] == 'a' || instr[0] == 'b')
    {
        return 0xf;
    }
    return charToint(instr[3]);
}

uint64_t CPU::get_valP(int icode)
{
    uint64_t valP;
    if (icode == IHALT || icode == INOP || icode == IRET)
        valP = PC + 1;
    else if (icode == IRRMOVQ || icode == IOPQ || icode == ICMOVXX || icode == IPUSHQ || icode == IPOPQ)
        valP = PC + 2;
    else if (icode == IJXX || icode == ICALL)
        valP = PC + 9;
    else
        valP = PC + 10;
    return valP;
}

int64_t CPU::get_valC(int icode, string instr)
{
    int64_t valC = 0;
    if (icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ || icode == IIADDQ)
        instr = instr.substr(4);
    else if (icode == IJXX || icode == ICALL)
        instr = instr.substr(2);
    for (int i = 0; i < instr.length(); i = i + 2) //由于内存是按两位的小端法储存
    {
        char x = instr[i + 1];
        instr[i + 1] = instr[i];
        instr[i] = x;
    }
    string s(instr.rbegin(), instr.rend());
    string _instr = GetBinaryStringFromHexString(s);
    string bi_instr(_instr.rbegin(), _instr.rend());
    for (int i = 0; i < bi_instr.length(); i++)
    {
        int sign = 1;
        if (i == bi_instr.length() - 1 && bi_instr[i] == '1')
            sign = -1;
        int64_t num = sign * charToint(bi_instr[i]);
        for (int j = 0; j < i; j++)
        {
            num *= 2;
        }
        valC += num;
    }

    return valC;
}

void CPU::fetch(int64_t pc, string instr) //取指
{
    icode = get_icode(instr);
    ifun = get_ifun(instr);
    if (need_regids(icode))
    { 
        rA = get_rA(instr);
        rB = get_rB(instr);
    }
    PC = pc;
    if (need_valC(icode))
    {
        valC = get_valC(icode, instr);
    }
    valP = get_valP(icode);
}

void CPU::PC_update() //更新PC
{
    PC = valP;
    return;
}

void CPU::write_back_IADDq()
{
    R[rB] = valE;
    PC_update();
    return;
}

void CPU::execute_IADDq()
{
    valE = valC + valB;
    //设置条件码
    ZF = (valE == 0);
    SF = (valE < 0);

    if (valC > 0 && valB > 0 && valE < 0)
        OF = 1;
    if (valC < 0 && valB < 0 && valE > 0)
        OF = 1;

    write_back_IADDq();
    return;
}

void CPU::decode_IADDq()
{
    valB = R[rB];
    execute_IADDq();
    return;
}

void CPU::write_back_OPq()
{

    R[rB] = valE;
    PC_update();
    return;
}

void CPU::execute_OPq()
{
    switch (ifun)
    {
    case 0:
        valE = valB + valA;
        break;
    case 1:
        valE = valB - valA;
        break;
    case 2:
        valE = valB & valA;
        break;
    case 3:
        valE = valB ^ valA;
        break;
    default:
        break;
    }
    //设置条件码
    ZF = (valE == 0);
    SF = (valE < 0);
    if (ifun == 0)
    {
        if (valA > 0 && valB > 0 && valE < 0)
            OF = 1;
        if (valA < 0 && valB < 0 && valE > 0)
            OF = 1;
    }
    if (ifun == 1)
    {
        if (valA < 0 && valB > 0 && valE < 0)
            OF = 1;
        if (valA > 0 && valB < 0 && valE > 0)
            OF = 1;
    }
    write_back_OPq();
    return;
}

void CPU::decode_OPq()
{
    valA = R[rA];
    valB = R[rB];
    execute_OPq();
    return;
}

void CPU::write_back_rrmovq()
{

    R[rB] = valE;
    PC_update();
    return;
}

void CPU::execute_rrmovq()
{
    valE = 0 + valA;
    write_back_rrmovq();
    return;
}

void CPU::decode_rrmovq()
{
    valA = R[rA];
    execute_rrmovq();
    return;
}

void CPU::write_back_cmovxx()
{
    if (Cnd)
    {
        R[rB] = valE;
    }
    else rB = 0xf;
    PC_update();
}

void CPU::execute_cmovxx()
{
    valE = 0 + valA;
    switch (ifun)
    {
    case 1:
        Cnd = ZF || (SF ^ OF);
        break;
    case 2:
        Cnd = SF ^ OF;
        break;
    case 3:
        Cnd = ZF;
        break;
    case 4:
        Cnd = !ZF;
        break;
    case 5:
        Cnd = !(SF ^ OF);
        break;
    case 6:
        Cnd = !(SF ^ OF) && !ZF;
        break;
    default:
        break;
    }
    write_back_cmovxx();
}

void CPU::decode_cmovxx()
{
    valA = R[rA];
    execute_cmovxx();
    return;
}

void CPU::write_back_irmovq()
{
    R[rB] = valE;
    PC_update();
    return;
}

void CPU::execute_irmovq()
{
    valE = 0 + valC;
    write_back_irmovq();
    return;
}

void CPU::memory_rmmovq()
{

    M[valE] = Dectohex(valA);
    PC_update();
    return;
}

void CPU::execute_rmmovq()
{
    valE = valB + valC;
    memory_rmmovq();
    return;
}

void CPU::decode_rmmovq()
{
    valA = R[rA];
    valB = R[rB];
    execute_rmmovq();
    return;
}

void CPU::write_back_mrmovq()
{
    R[rA] = valM;
    PC_update();
    return;
}

void CPU::memory_mrmovq()
{
    if (M[valE] == "invalid") //访问到非法内存
    {
        Stat = ADR;
        return;
    }
    string g = "0x" + Mout[valE + 7] + Mout[valE + 6] + Mout[valE + 5] + Mout[valE + 4] + Mout[valE + 3] + Mout[valE + 2] + Mout[valE + 1] + Mout[valE];
    valM = hextoDec(g);
    write_back_mrmovq();
    return;
}

void CPU::execute_mrmovq()
{
    valE = valB + valC;
    memory_mrmovq();
    return;
}

void CPU::decode_mrmovq()
{
    valB = R[rB];
    execute_mrmovq();
    return;
}

void CPU::write_back_pushq()
{
    R[4] = valE;
    PC_update();
    return;
}

void CPU::memory_pushq()
{
    M[valE] = Dectohex(valA);
    write_back_pushq();
    return;
}

void CPU::execute_pushq()
{
    valE = valB - 8;
    memory_pushq();
    return;
}

void CPU::decode_pushq()
{
    valA = R[rA];
    valB = R[4];
    execute_pushq();
    return;
}

void CPU::write_back_popq()
{

    R[4] = valE;
    R[rA] = valM;
    PC_update();
    return;
}

void CPU::memory_popq()
{
    string g = "0x" + Mout[valA + 7] + Mout[valA + 6] + Mout[valA + 5] + Mout[valA + 4] + Mout[valA + 3] + Mout[valA + 2] + Mout[valA + 1] + Mout[valA];
    valM = hextoDec(g);
    write_back_popq();
    return;
}

void CPU::execute_popq()
{
    valE = valB + 8;
    memory_popq();
    return;
}

void CPU::decode_popq()
{
    valA = R[4];
    valB = R[4];
    execute_popq();
    return;
}

void CPU::PC_update_jxx_Dest()
{
    PC = Cnd ? valC : valP;
    return;
}

void CPU::execute_jmp_Dest()
{
    Cnd = 1;
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_jle_Dest()
{
    Cnd = ZF || (SF ^ OF);
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_jl_Dest()
{
    Cnd = SF ^ OF;
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_je_Dest()
{
    Cnd = ZF;
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_jne_Dest()
{
    Cnd = !ZF;
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_jge_Dest()
{
    Cnd = !(SF ^ OF);
    PC_update_jxx_Dest();
    return;
}

void CPU::execute_jg_Dest()
{
    Cnd = !(SF ^ OF) && !ZF;
    PC_update_jxx_Dest();
    return;
}

void CPU::PC_update_call_Dest()
{
    PC = valC;
    return;
}

void CPU::write_back_call_Dest()
{
    R[4] = valE;
    PC_update_call_Dest();
    return;
}

void CPU::memory_call_Dest()
{
    M[valE] = Dectohex(valP);
    write_back_call_Dest();
    return;
}

void CPU::execute_call_Dest()
{
    valE = valB - 8;
    memory_call_Dest();
    return;
}

void CPU::decode_call_Dest()
{
    valB = R[4];
    execute_call_Dest();
    return;
}

void CPU::PC_update_ret()
{
    PC = valM;
    return;
}

void CPU::write_back_ret()
{
    R[4] = valE;
    PC_update_ret();
    return;
}

void CPU::memory_ret()
{
    string g = "0x" + Mout[valA + 7] + Mout[valA + 6] + Mout[valA + 5] + Mout[valA + 4] + Mout[valA + 3] + Mout[valA + 2] + Mout[valA + 1] + Mout[valA];
    valM = hextoDec(g);
    write_back_ret();
    return;
}

void CPU::execute_ret()
{
    valE = valB + 8;
    memory_ret();
    return;
}

void CPU::decode_ret()
{
    valA = R[4];
    valB = R[4];
    execute_ret();
    return;
}

void CPU::output() {

    cout << string(4, ' ') << "{" << endl;
    cout << string(8, ' ') << "\"PC\":" << PC << ',' << endl;

    cout << string(8, ' ') << "\"REG\": {" << endl;
    cout << string(12, ' ') << "\"rax\": " << R[rax] << ',' << endl;
    cout << string(12, ' ') << "\"rcx\": " << R[rcx] << ',' << endl;
    cout << string(12, ' ') << "\"rdx\": " << R[rdx] << ',' << endl;
    cout << string(12, ' ') << "\"rbx\": " << R[rbx] << ',' << endl;
    cout << string(12, ' ') << "\"rsp\": " << R[rsp] << ',' << endl;
    cout << string(12, ' ') << "\"rbp\": " << R[rbp] << ',' << endl;
    cout << string(12, ' ') << "\"rsi\": " << R[rsi] << ',' << endl;
    cout << string(12, ' ') << "\"rdi\": " << R[rdi] << ',' << endl;
    cout << string(12, ' ') << "\"r8\" : " << R[r8] << ',' << endl;
    cout << string(12, ' ') << "\"r9\" : " << R[r9] << ',' << endl;
    cout << string(12, ' ') << "\"r10\": " << R[r10] << ',' << endl;
    cout << string(12, ' ') << "\"r11\": " << R[r11] << ',' << endl;
    cout << string(12, ' ') << "\"r12\": " << R[r12] << ',' << endl;
    cout << string(12, ' ') << "\"r13\": " << R[r13] << ',' << endl;
    cout << string(12, ' ') << "\"r14\": " << R[r14] << endl;
    cout << string(8, ' ') << "}," << endl;

    cout << string(8, ' ') << "\"CC\":{" << endl;
    cout << string(12, ' ') << "\"ZF\" : " << ZF << ',' << endl;
    cout << string(12, ' ') << "\"SF\" : " << SF << ',' << endl;
    cout << string(12, ' ') << "\"OF\" : " << OF << ',' << endl;
    cout << string(8, ' ') << "}," << endl;

    cout << string(8, ' ') << "\"STAT\":" << Stat << ',' << endl;

    cout << string(8, ' ') << "\"MEM\":{" << endl;
    out_mem();
    cout << string(8, ' ') << "}," << endl;

    cout << string(4, ' ') << "}," << endl;


    o << string(4, ' ') << "{" << endl;
    o << string(8, ' ') << "\"PC\":" << PC << ',' << endl;

    o << string(8, ' ') << "\"REG\": {" << endl;
    o << string(12, ' ') << "\"rax\": " << R[rax] << ',' << endl;
    o << string(12, ' ') << "\"rcx\": " << R[rcx] << ',' << endl;
    o << string(12, ' ') << "\"rdx\": " << R[rdx] << ',' << endl;
    o << string(12, ' ') << "\"rbx\": " << R[rbx] << ',' << endl;
    o << string(12, ' ') << "\"rsp\": " << R[rsp] << "/" << frsp << ',' << endl;
    o << string(12, ' ') << "\"rbp\": " << R[rbp] << ',' << endl;
    o << string(12, ' ') << "\"rsi\": " << R[rsi] << ',' << endl;
    o << string(12, ' ') << "\"rdi\": " << R[rdi] << ',' << endl;
    o << string(12, ' ') << "\"r8\" : " << R[r8] << ',' << endl;
    o << string(12, ' ') << "\"r9\" : " << R[r9] << ',' << endl;
    o << string(12, ' ') << "\"r10\": " << R[r10] << ',' << endl;
    o << string(12, ' ') << "\"r11\": " << R[r11] << ',' << endl;
    o << string(12, ' ') << "\"r12\": " << R[r12] << ',' << endl;
    o << string(12, ' ') << "\"r13\": " << R[r13] << ',' << endl;
    o << string(12, ' ') << "\"r14\": " << R[r14] << endl;
    o << string(8, ' ') << "}," << endl;

    o << string(8, ' ') << "\"CC\":{" << endl;
    o << string(12, ' ') << "\"ZF\" : " << ZF << ',' << endl;
    o << string(12, ' ') << "\"SF\" : " << SF << ',' << endl;
    o << string(12, ' ') << "\"OF\" : " << OF << ',' << endl;
    o << string(8, ' ') << "}," << endl;

    o << string(8, ' ') << "\"STAT\":" << Stat << ',' << endl;

    o << string(8, ' ') << "\"MEM\":{" << endl;
    out_mem2(o);
    o << string(8, ' ') << "}," << endl;

    o << string(4, ' ') << "}," << endl;
}

void CPU::out_mem2(ofstream& o) {
    for (int q = 0; q < 9999; q++) {
        if (Mout[q] != "00") {
            int beg = q - q % 8;
            string out = "0x" + Mout[beg + 7] + Mout[beg + 6] + Mout[beg + 5] + Mout[beg + 4] + Mout[beg + 3] + Mout[beg + 2] + Mout[beg + 1] + Mout[beg];

            cout.precision(100);
            o << string(12, ' ') << "\"" << beg << "\":" << hextoDec(out) << "," << endl;
            q = beg + 7;
        }
    }
}

void CPU::out_mem() {
    for (int q = 0; q < 9999; q++) {
        if (Mout[q] != "00" && Mout[q] != "0") {
            int beg = q - q % 8;
            string out;
            out = "0x" + Mout[beg + 7] + Mout[beg + 6] + Mout[beg + 5] + Mout[beg + 4] + Mout[beg + 3] + Mout[beg + 2] + Mout[beg + 1] + Mout[beg];


            cout.precision(100);
            cout << string(12, ' ') << "\"" << beg << "\":" << hextoDec(out) << "," << endl;
            q = beg + 7;
        }
    }
}

void CPU::M_to_Mout() {
    for (int q = 0; q <= 9999; q++) {
        if (M[q].compare("invalid")) {
            size_t t = M[q].size();

            int  c = 0;
            for (; c * 2 < t - 1; c++) {
                string temp = "  ";
                copy(M[q].begin() + c * 2, M[q].begin() + c * 2 + 2, temp.begin());
                Mout[q + c] = temp;
            }
            if (t % 2) {
                string o = "0";
                copy(M[q].begin() + c * 2, M[q].begin() + c * 2 + 1, o.begin());
                Mout[q + c] = o;
            }

        }
    }
    return;
}

void CPU::run(string path)
{

    int flag = read_file(path);
    if (flag == -1) //未读取到文件
    {
        return;
    }
    else
    {
        int index = 0;
        while (Stat == AOK)
        {
            fetch(index, M[index]);
            if (!instr_valid(icode, ifun))
            {
                Stat = INS;

            }
            if (icode == 0)
            {
                Stat = HLT;
            }

            if (icode == 0xa && R[4] < 8)
                Stat = ADR;

            if (Stat == AOK)
            {
                if (icode == 1)
                    PC_update();
                if (icode == 2)
                {
                    if (ifun == 0)
                    {
                        decode_rrmovq();
                    }
                    else
                        decode_cmovxx();
                }
                if (icode == 3)
                {

                    execute_irmovq();
                }
                if (icode == 4)
                {

                    decode_rmmovq();
                }
                if (icode == 5)
                {
                    decode_mrmovq();
                }
                if (icode == 6)
                {
                    decode_OPq();
                }
                if (icode == 7)
                {
                    if (ifun == 0)
                    {
                        execute_jmp_Dest();
                    }
                    if (ifun == 1)
                    {
                        execute_jle_Dest();
                    }
                    if (ifun == 2)
                    {
                        execute_jl_Dest();
                    }
                    if (ifun == 3)
                    {
                        execute_je_Dest();
                    }
                    if (ifun == 4)
                    {
                        execute_jne_Dest();
                    }
                    if (ifun == 5)
                    {
                        execute_jge_Dest();
                    }
                    if (ifun == 6)
                    {
                        execute_jg_Dest();
                    }
                }
                if (icode == 8)
                {
                    decode_call_Dest();
                }
                if (icode == 9)
                {
                    decode_ret();
                }
                if (icode == 0xa)
                {
                    decode_pushq();
                }
                if (icode == 0xb)
                {
                    decode_popq();
                }
                if (icode == 0xc)
                {
                    decode_IADDq();
                }
            }
            index = PC;
            M_to_Mout();
            output();
        }
    }
}

void CPU::write(string path)
{
    o << "[" << endl;
    cout << "[" << endl;
    run(path);
    cout << "]";
    o << "]";
}