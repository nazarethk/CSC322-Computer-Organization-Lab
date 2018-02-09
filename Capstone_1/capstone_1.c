/*******************************************
* Pinky Machine Architectural simulator *
*******************************************/
/*******************************************
* Author: Nazareth H. Keshishian *
* ID: 201701143
* CSC 322: Computer Organization Lab *
* Spring 2018 *
* *
* Final Capstone Code *
*******************************************/
#include <stdio.h>
#include <inttypes.h>
/********************************************
* Pinky Machine Operation code definitions *
********************************************/
#define OP_HALT 0x0
#define OP_LDW 0x1
#define OP_STW 0x2
#define OP_ADD 0x3
#define OP_SUB 0x4
#define OP_AND 0x5
#define OP_OR 0x6
#define OP_XOR 0x7
#define OP_ADDS 0x8
#define OP_SUBS 0x9
#define OP_ANDS 0xA
#define OP_ADDI 0xB
#define OP_SUBI 0xC
#define OP_CBZ 0xD
#define OP_CBNZ 0xE
#define OP_BZ 0xF
#define OP_BN 0x10
#define OP_BR 0x11
#define OP_BL 0x12
#define OP_RET 0x13
#define OP_SL 0x14
#define OP_SR 0x15

#define W0 0x0
#define W1 0x1
#define W2 0x2
#define W3 0x3
#define W4 0x4
#define W5 0x5
#define W6 0x6
#define W7 0x7
#define W8 0x8
#define W9 0x9
#define W10 0xA
#define W11 0xB
#define W12 0xC
#define W13 0xD
#define W14 0xE
#define W15 0xF
#define W16 0x10
#define W17 0x11
#define W18 0x12
#define W19 0x13
#define W20 0x14
#define W21 0x15
#define W22 0x16
#define W23 0x17
#define W24 0x18
#define W25 0x19
#define W26 0x1A
#define W27 0x1B
#define W28 0x1C
#define W29 0x1D
#define W30 0x1E
#define W31 0x1F
/*************************************
* Picky Machine Assembly code macros *
*************************************/

#define HALT OP_HALT << 20

#define LDW(DestReg, BaseReg, Address) (OP_LDW << 20) | (Address << 10) | (BaseReg << 5) | DestReg

#define STW(SourceReg, BaseReg, Address) (OP_STW << 20) | (Address << 10) | (BaseReg << 5) | SourceReg

#define ADD(DestReg, SourceReg2, SourceReg1) (OP_ADD << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define SUB(DestReg, SourceReg2, SourceReg1) (OP_SUB << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define AND(DestReg, SourceReg2, SourceReg1) (OP_AND << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define OR(DestReg, SourceReg2, SourceReg1) (OP_OR << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define XOR(DestReg, SourceReg2, SourceReg1) (OP_XOR << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define ADDI(DestReg, SourceReg2, SourceReg1) (OP_ADDI << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define ADDS(DestReg, SourceReg2, SourceReg1) (OP_ADDS << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define SUBI(DestReg, SourceReg2, SourceReg1) (OP_SUBI << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

#define SUBS(DestReg, SourceReg2, SourceReg1) (OP_SUBS << 20) | (SourceReg2 << 15) | (SourceReg1 << 5) | DestReg

char *Instr[] =
{
"Halt","Load","Store", "Add", "Sub","Logical And","Logical Or", "Logical Xor",
"AddS", "SubS", "Logical ANDS", "Shift Left", "Shift Right",
"AddI","SubI","CBZ","CBNZ","BZ","BN","BR","BL","RET"
};
/**********************************
* Pinky MachineInstruction memory *
**********************************/
uint32_t IM[256] =
{
LDW(W0,0, 0), // 0
LDW(W1,1, 0), // 1
LDW(W2,2, 0), // 2
ADD(W3,W0,W1), // 3
SUB(W4,W2,W0),
AND(W5,W4,W3),
STW(W5,0,0), // 3
HALT // 4
};
uint16_t DM[256] =
{
5, // 0
10, // 1
20 // 2
};
/*****************************************************
* Pinky Machine Register File - 32x32-bit registers *
*****************************************************/
uint16_t R[32];
/****************************
* Condition Flag registers *
****************************/
uint8_t Z, N;
/****************************
* Instruction Pointer *
****************************/
int8_t Opcode;
int16_t PC = 0;
/****************************
* Simulator variables *
****************************/
unsigned short sRun = 1;
void load(uint16_t iIndex, uint16_t iAddress, uint16_t iDisplacement);
void store(uint16_t iIndex, uint16_t iAddress, uint16_t iDisplacement);

void sr(uint16_t destination, uint16_t shiftamt, uint16_t source);
void sl(uint16_t destination, uint16_t shiftamt, uint16_t source);

void sub(uint16_t destination, uint16_t source1, uint16_t source2);
void subi(uint16_t destination, uint16_t source1, uint16_t source2);
void subs(uint16_t destination, uint16_t source1, uint16_t source2);

void add(uint16_t uDest, uint16_t uReg1, uint16_t uReg2);
void addi(uint16_t destination, uint16_t source1, uint16_t source2);
void adds(uint16_t destination, uint16_t source1, uint16_t source2);

void or(uint16_t destination, uint16_t source1, uint16_t source2);
void Xor(uint16_t destination, uint16_t source1, uint16_t source2);
void and(uint16_t destination, uint16_t source1, uint16_t source2);

void CBZ(uint16_t destination, uint16_t address);
void CBNZ(uint16_t destination, uint16_t address);
void BZ(uint16_t address);
void BN(uint16_t address);
void RET();
void BL(uint16_t address);
void BR(uint16_t address);

void halt(void);
void Decode(uint32_t IR, uint16_t *iOpcode, uint16_t *iDestReg, uint16_t *iSourceReg2, uint16_t *iSourceReg1, uint16_t *iShiftAmt, uint16_t *iAddress, uint16_t *iBaseReg);
uint32_t IR;

/***************************
* Decode Unit *
****************************/

void Decode(uint32_t IR, uint16_t *iOpcode, uint16_t *iDestReg, uint16_t *iSourceReg2,
uint16_t *iSourceReg1, uint16_t *iShiftAmt, uint16_t *iAddress, uint16_t *iBaseReg)
{
*iOpcode = (IR >> 20);
*iDestReg = -1;
*iSourceReg2 = -1;
*iSourceReg1 = -1;
*iShiftAmt = -1;
*iAddress = -1;
*iBaseReg = -1;
switch (*iOpcode)
{
case OP_HALT:
break;

case OP_LDW:
*iBaseReg = (IR >> 5) & 0x001F;
*iAddress = (IR >> 10) & 0x03FF;
*iDestReg = (IR & 0x001F);
break;

case OP_STW:
*iBaseReg = (IR >> 5) & 0x001F;
*iAddress = (IR >> 10) & 0x03FF;
*iSourceReg1 = (IR & 0x001F);
break;
case OP_ADD:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_SUB:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_AND:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_OR:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_XOR:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_ADDS:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_SUBS:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_ANDS:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_SL:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_SR:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_ADDI:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_SUBI:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_CBZ:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_CBNZ:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_BZ:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_BN:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_BR:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_BL:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;

case OP_RET:
*iSourceReg1 = (IR >> 5) & 0x001F;
*iSourceReg2 = (IR >> 15) & 0x001F;
*iDestReg = (IR & 0x001F);
break;


}
}
int main(void);
/***************************
* Main fetch/execute loop *
***************************/
int main()
{
uint16_t iOpcode, iDestReg, iSourceReg2, iSourceReg1, iShiftAmt, iAddress, iBaseReg;
uint16_t iRegIndex;
R[W31] = 0;
R[W5] = 0;
R[W6] = 0;
printf("Pinky Machine simulator (version 1.0)\n");
printf("----------------------------\n");
while(sRun) {
printf("\nCurrent Memory address: %02X\n", (unsigned short) PC);
IR = IM[PC];
Decode(IR, &iOpcode, &iDestReg, &iSourceReg2, &iSourceReg1, &iShiftAmt, &iAddress, &iBaseReg);
printf("Current Instruction:%s\n", Instr[iOpcode] );
switch (iOpcode) {
case OP_LDW:
load(iDestReg, iAddress, iBaseReg);

PC = PC + 1;
break;
case OP_STW:
store(iSourceReg1, iAddress, iBaseReg);
PC = PC + 1;
break;
case OP_ADD:
add(iDestReg, iSourceReg1, iSourceReg2);
PC = PC + 1;
break;
case OP_SUB:
SUB(iDestReg, iSourceReg1, iSourceReg2);
PC = PC + 1;
break;
case OP_AND:
AND(iDestReg, iSourceReg1, iSourceReg2);
PC = PC + 1;
break;
case OP_HALT:
halt();
break;
}
printf("\nPC: %02x\n", (unsigned) PC);
for (iRegIndex =1; iRegIndex < 33; iRegIndex++)
{
printf("R[%d] = %d ", iRegIndex-1, R[iRegIndex-1]);
if ((iRegIndex % 6) == 0)
printf("\n");
}
}
printf("\n\nLeaving the simulator.\n\n");
return (0);
}
/*******************************************
* Load a value from memory *
*******************************************/
void load(uint16_t iDestReg, uint16_t iAddress, uint16_t iBaseReg)
{
R[iDestReg] = DM[iAddress+iBaseReg];
}
/*******************************************
* Store a value in memory *
*******************************************/
void store(uint16_t iSourceReg, uint16_t iAddress, uint16_t iBaseReg)
{
DM[iAddress + R[iBaseReg]] = R[iSourceReg];
//printf("%d %d\n", iSourceReg, R[iSourceReg]);
}

/*******************************************
* CBZ *
*******************************************/
void CBZ(uint16_t destination, uint16_t address){
if(R[destination]==0){
PC=address;

}
}

/*******************************************
* CBNZ *
*******************************************/
void CBNZ(uint16_t destination, uint16_t address){
if(R[destination]!=0){
PC=address;

}
}

/*******************************************
* BZ *
*******************************************/
void BZ(uint16_t address){
if(Z==0){
PC=address;

}
}

/*******************************************
* BN *
*******************************************/
void BN(uint16_t address){
if(N<0){
PC=address;

}
}

/*******************************************
* BR *
*******************************************/
void BR(uint16_t address){

PC=address;

}

/*******************************************
* BL *
*******************************************/
void BL(uint16_t address){
if(N<0){
W30<PC;
PC=address;

}
}

/*******************************************
* RET *
*******************************************/
void RET(){

PC=W30;

}

/****************************************************************************************
* Shift right the content of a register in the register file and store the results *
****************************************************************************************/
void sr(uint16_t destination, uint16_t shiftamt, uint16_t source)
{
R[destination]=R[source];
R[destination]<<shiftamt;
}
/****************************************************************************************
* Shift left the content of a register in the register file and store the results *
****************************************************************************************/
void sl(uint16_t destination, uint16_t shiftamt, uint16_t source)
{
R[destination]=R[source];
R[destination]<<shiftamt;

}
/**********************************************
* Add The contents of the two registers *
* in the register file and store the results *
**********************************************/
void add(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination]= R[source1] + R[source2];
}
/**********************************************
* Sub The contents of the two registers *
* in the register file and store the results *
**********************************************/
void sub(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] - R[source2];
}
/***************************************************
* Logical 'and' the contents of the two registers *
* in the register file and store the results *
***************************************************/
void and(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] & R[source2];
}

void ands(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] & R[source2];
if(R[destination]==0){
Z=1;
}
else{
Z=0;
}

if(R[destination]<0){
N=1;
}
else{
N=0;
}
}
/***************************************************
* Logical 'or' the contents of the two registers *
* in the register file and store the results *
***************************************************/
void or(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] | R[source2];
}

/***************************************************
* Logical 'Xor' the contents of the two registers *
* in the register file and store the results *
***************************************************/
void Xor(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] ^ R[source2];
}
/*************************************************************************************
* Sub The contents of a register and an immediate and stores the results *
************************************************************************************/
void subi(uint16_t destination, uint16_t source1, uint16_t source2)
{

R[destination] = R[source1] - R[source2];
}
/*************************************************************************************
* Add The contents of a register and an immediate and stores the results *
************************************************************************************/
void addi(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] + R[source2];

}
/************************************************************************************
* Add The contents of a register and an immediate and stores the results *
* Set the condition flags *
************************************************************************************/
void adds(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] + R[source2];
if(R[destination]==0){
Z=1;
}
else{
Z=0;
}

if(R[destination]<0){
N=1;
}
else{
N=0;
}
}
/*************************************************************************************
* Sub The contents of two registers and an immediate and stores the results *
* Set the condition flags *
************************************************************************************/
void subs(uint16_t destination, uint16_t source1, uint16_t source2)
{
R[destination] = R[source1] - R[source2];
if(R[destination]==0){
Z=1;
}
else{
Z=0;
}

if(R[destination]<0){
N=1;
}
else{
N=0;
}

}
/*******************************************
* Halt the execution of the system *
*******************************************/
void halt()
{
printf("Halt executed, program normally terminated. ");
sRun = 0;
}
