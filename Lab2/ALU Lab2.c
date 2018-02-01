#include<stdio.h>
#include<stdlib.h>

// Input and Output Registers
int A = 0;
int B = 0;
int Result = 0;
int OpCode = 0;
int s0 = 0;
int s1 = 0;

// Status registers
int Z = 0;
int Ovf = 0;

// define OpCodes
#define OP_ADD 0x00
#define OP_SUB 0x01
#define OP_LEFT 0x02
#define OP_RIGHT 0x03
#define LEFT 0
#define RIGHT 1





int AddSub(int A, int B, int OpCode);
int mux(short In1, short In2, short Sel);
int AND(int A, int B);
int OR(int A, int B);
int main(void);


int main(void){
   A = 0b0000000001111011;
   B = 0b0000000001100100;
   Result = 0;
    int operation=0;
     while (operation>4&operation<1){
     printf("Which operation would you like? ADD(1)/SUB(2)/OR(3)/AND(4) > ");
     scanf("%d", &operation); // read user input

           if (operation == 1){
               OpCode = OP_ADD;
               s0 = LEFT; // Doesn't matter
               s1 = LEFT;
               }
             else if(operation == 2){
                 OpCode = OP_SUB;
                 s0 = LEFT; // Doesn't matter
                 s1 = LEFT;
                 
             }
             else if (operation == 3){
                 OpCode = 0; // Doesn't matter
                 s0 = LEFT;
                 s1 = RIGHT;
               }
             else if (operation == 4){
                 OpCode = 0; // Doesn't matter
                 s0 = RIGHT;
                 s1 = RIGHT;
               }
             else
                 printf("WHAT!");
        }
        int Out = AddSub(A, B, OpCode);
        int AndOut = AND(A, B);
        int OrOut = OR(A, B);
        int MuxOut = mux(AndOut, OrOut, s0);
        int MuxOut2 = mux(Out, MuxOut, s1);

        Result = MuxOut2;


        printf("Register A: %d\tRegister B: %d\tRegister Result: %d\n", A, B,Result);
   }



   int AddSub(int A, int B, int OpCode)
   {
   	int Sum=0;

       if (OpCode == OP_ADD){
           Sum = A + B;
       }
       else{
           Sum = A - B;
       }
       if (Sum == 0)
           Z = 1;
         else
         {
             Z = 0;
         }

       return (Sum,0,Z);
   }



   // 2-to-1 Multiplexer
       int mux(short In1, short In2, short Sel)
   {

   	int Out;

   	if (Sel == LEFT)
   		Out = In1;
   	else
   		Out = In2;
   	return(Out);
   }

   // AND Gate
   int AND(int A, int B){
       return A&B;
   }

   // OR Gate
   int OR(int A, int B){
      return A|B;
   }
