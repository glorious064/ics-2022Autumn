/* 
 * CS:APP Data Lab 
 * 
 * <×ÞÜ² 21302010073>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
#include "bits.h"


//P1
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(4, 5) = -6, bitNor(-1,-2) = 0
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
   /* according to the character of the ~ | and &*/
   return (~x)&(~y);
}

//P2
/* 
 * tmax - return maximum two's complement integer (0x7fffffff)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  /* tmax represents 01111.....11 and is obviously equal to ~(1<<31)*/
	return ~(1<<31);
}

//P3
/*
 * isTmin - returns 1 if x is the minimum two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x){
   /* exploit that the opposite of tmin is itself and exclude 0*/
   return !(x^(~x+1))+(~(!x)+1);
}

//P4
/*
 * minusOne - return a value of -1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void){
  /* -1 represents 32 bits of 1 and is equal to (~0)*/
	return ~0;
}

//P5
/*
 * absVal - return the absolute value of x
 *   Examples: absVal(-10) = 10
 *			   absVal(5) = 5
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int absVal(int x){
   /* exploit the difference of signbit, negative numbers need invert and plus 1*/
   int temp;
   temp = x>>31;
   x = x^temp;
   return x+(1&temp);
}

//P6 
/*
 * leastBitPos - return a mask that marks the position of the least significant 1 bit.
 *   Examples: leastBitPos(12) = 4
 *			       leastBitPos(-2) = 2
 *             leastBitPos(0)  = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int leastBitPos(int x) {
  /* just reserve the least significant 1 bit, let other bits change into 0*/
	return (((x+~0)^x)&x);
}

//P7
/*
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 20
 *  Rating: 2
 */
int byteSwap(int x, int n, int m){
  /* clear nth and mth byte of x, then fill nth position with mth byte and mth with nth position*/
  int temp1, temp2, sign1, sign2,value1,value2;
  value1=n<<3;
  value2=m<<3;
  temp1 = (x>>value1)&0xff; //reserve nth byte
  temp2 = (x>>value2)&0xff; //reserve mth byte
  sign1 = temp1<<value1;
  sign2 = temp2<<value2;
  x = (x^sign1)^sign2; //clear nth and mth byte
  temp1=temp1<<value2;
  temp2=temp2<<value1;
  x = (x^temp1)^temp2; //exchange nth and mth byte
  return x;
 
}

//P8
/* 
 * logicalShift - shift x to the right by n, using a logical shift
 *   Can assume that 0 <= n <= 31
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3 
 */
int logicalShift(int x, int n) {
  /*exploit difference of signbit, let n ones of  right shifted negative numbers transfer to 0*/
  int temp;
  temp = x>>31;
  x = x>>n;
  temp = temp<<(31+(~n+1)); 
  temp = temp<<1; 
  return x & (~temp);
}

//P9
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  /*considering three situations:different sign and x is nagative or x == y or same sign and x<y*/
    int sign_x,sign_y,sign,sign_yx,sign_eq;
    sign_x = !!(x>>31);
    sign_y = !!(y>>31);
    sign = sign_x^sign_y; //judge whether signx and signy are same or not
    sign_eq = !(x^y);
    sign_yx = !!((x+(~y+1))>>31);
    return (sign&sign_x)|((!sign)&sign_yx)|sign_eq;
    
}

//P10 
/* 
 * multFiveEighths - return floor(x*5/8), for 0 <= x, x is an integer 
 *   Example: multFiveEighths(10) = 6
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int multFiveEighths(int x) {
  /* last three bits of x should multiple 5 first then /8, other bits of x implement reverse operations*/
  int tail;
  tail = x&0x7; //reserve value of last three bits
  x = x>>3;
  x = x+(x<<2);
  tail = tail+(tail<<2);
  tail = tail>>3;
  return x+tail;
}

//P11
/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
  /* exploit different masks to get the number of 1's in different bit units, finally get the number of 1's in 32 bits, namely x*/
  int mask_1, mask_2, mask_4, mask_8, mask_16;
  mask_1 = 0x55 << 8 | 0x55;
  mask_1 |= mask_1 << 16; //mask_1:010101....0101
  mask_2 = 0x33 << 8 | 0x33;
  mask_2 |= mask_2 << 16;//mask_2: 00110011....0011
  mask_4 = 0x0f << 8 | 0x0f;
  mask_4 |= mask_4 << 16;//mask_4: 00001111....00001111
  mask_8 = 0xff << 16 | 0xff;//mask_8:0000000011111111......
  mask_16 = ~0 + (1 << 16);//mask_16:0000...000111111...111
  x = (x&mask_1) + ((x>>1)&mask_1);
  x = (x&mask_2) + ((x>>2)&mask_2);
  x = (x&mask_4) + ((x>>4)&mask_4);
  x = (x&mask_8) + ((x>>8)&mask_8);
  x = (x&mask_16) + ((x>>16)&mask_16);
  return x;
}

//P12
/*
 * greatestBitPos - return a mask that marks the position of the greatest significant 1 bit.
 *   Example: greatestBitPos(12) = 8
 *            greatestBitPos(-2) = -2147483648
 *            greatestBitPos(0)  = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 60
 *   Rating: 4
 */
int greatestBitPos(int x) {
  /* divide x to check if high bits have 1, if there is 1, divide high bits to check again; if there is not 1, check low bits, recursively check in this way until finding the position of the target*/
  int n;
  n = 0;
  n += ((!!(x&(~0<<16))) << 4);
  n += ((!!(x&(~0<<(n+8)))) << 3);
  n += ((!!(x&(~0<<(n+4)))) << 2);
  n += ((!!(x&(~0<<(n+2)))) << 1);
  n += (!!(x&(~0<<(n+1))));
  return 1<<n & x;
 
}

//P13
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /* expolit the sign bits of 0 and nagative number 0f 0 are same, and the others are different*/
  return ((x | (~x+1))>>31)+1;
}

//P14
/*
 * bitReverse - Reverse bits in an 32-bit integer
 *   Examples: bitReverse(0x80000004) = 0x20000001
 *             bitReverse(0x7FFFFFFF) = 0xFFFFFFFE
 *   Max ops: 40
 *   Rating: 4
 */
int bitReverse(int x){
  /*similar to P11, we let different masks to reverse different bit units, from 1 to 16 and finally reverse x */
  int mask_1, mask_2, mask_4, mask_8, mask_16;
  mask_1 = 0x55 << 8 | 0x55;
  mask_1 |= mask_1 << 16;
  mask_2 = 0x33 << 8 | 0x33;
  mask_2 |= mask_2 << 16;
  mask_4 = 0x0f << 8 | 0x0f;
  mask_4 |= mask_4 << 16;
  mask_8 = 0xff << 16 | 0xff;
  mask_16 = 0xff<<8 | 0xff;
  x = ((x&mask_1)<<1)+((x>>1)&mask_1);
  x = ((x&mask_2)<<2)+((x>>2)&mask_2);
  x = ((x&mask_4)<<4)+((x>>4)&mask_4);
  x = ((x&mask_8)<<8)+((x>>8)&mask_8);
  x = (x<<16)+((x>>16)&mask_16);
  return x;  
}

//P15
/*
 * mod3 - calculate x mod 3 without using %.
 *   Example: mod3(100) = 1
 *            mod3(-100) = -1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 90
 *   Rating: 4
 */
 
int mod3(int x){
 /* recursively devide x to calculate the mod of different bit units, then judge the value of final 2 bits to calculate the mod*/
 int mask, mask_16, mod_16, mod_8, mod_4, mod_2, temp, mod;
 mask = x>>31;
 x = (x^mask)+(1&mask); //exploit the conclusion of P5 to get abs(x)
 mask_16 = (0xff<<8) | 0xff; //mask_16:00..0011..11(32bits)
 mod_16 = ((x>>16) & mask_16) + (x&mask_16);
 mod_16 = (((1<<16) & mod_16) >> 16) + (mod_16 & mask_16);
 mod_8 = ((mod_16>>8) & 0xff) + (mod_16 & 0xff);
 mod_8 = (((1<<8) & mod_8)>>8) + (mod_8 & 0xff);
 mod_4 = ((mod_8>>4) & 0xf) + (mod_8 & 0xf);
 mod_4 = (((1<<4) & mod_4)>>4) + (mod_4 & 0xf);
 mod_2 = ((mod_4>>2) & 0x3) + (mod_4 & 0x3);
 mod_2 = (((1<<2) & mod_2)>>2) + (mod_2 & 0x3);
 temp = mod_2 & (mod_2>>1);
 temp = temp + (temp<<1);
 mod = mod_2+(~temp+1);
 return ((mask&(~mod+1)) | (~mask&mod));
 
}

//P16
/* 
 * float_neg - Return bit-level equivalent of expression -f for`
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
  /*if uf is NAN, return uf, if not, change sign bit of uf, then return*/
  int exp, frac;
  exp = (uf<<1)>>24;
  frac = uf<<9;
  if(frac && (!(exp ^ 0xff))) return uf;//frac is not 0 and exp is 255 indicating uf is NAN, so return uf directly
  else return uf^0x80000000; 
}

//P17
/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
  /*discuss this case into 3 situations:
    first special numbers 0 and 0x80000000 
    second valid bits <=24, just let fill frac with valid bits of x
    third valid bits > 24, after abandoning excessive bits, fill frac and judge whether there is need to carry
  */
  int sign, exp, frac, frac_mask, temp; 
  frac_mask = 0x7fffff;
  temp = 0;
  sign = x&0x80000000;
  if(x==0) return x;
  if(x==0x80000000) return 0xcf000000;
  if(sign) x = -x;
  while(x>>temp){
  temp++;
  }
  exp = temp+126; //considering bias
  if(temp<=24) frac = (x<<(24-temp))&frac_mask;
  else{
    frac = (x>>(temp-24))&frac_mask;
    x = x<<(32-temp);
    x &= 0xff;
    if((x>0x80)||((x==0x80)&& (frac&1)))
      frac++;
    if(frac>>23){
      frac = frac&frac_mask;
       exp++;
       }
    }
  return (sign | (exp<<23) | frac);
}

//P18
/* 
 * float_twice - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_twice(unsigned uf) {
  /*discuss several situations: if exp==0, directly x<<1;if exp==255, return uf itself;else let exp plus 1, if exp==255, return INF*/
  int exp, sign;
  exp = (uf<<1)>>24;
  sign = uf&0x80000000;
  if(exp==0) return sign|(uf<<1);//denormalization number
  if(exp==255) return uf; //NAN or INF
  exp++;
  if(exp==255) return sign^(exp<<23); //out of range so just return INF
  uf = uf^((exp-1)<<23); //clear original exp
  return uf^(exp<<23);
}
