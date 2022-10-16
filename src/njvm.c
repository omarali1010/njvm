#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

typedef struct stack {
    uint32_t top,fp;
    uint32_t entry[10000];
}Stack;
Stack program_mem,mystack,SDA;



#define IMMEDIATE(x) ((x) & 0x00FFFFFF)
#define SIGN_EXTEND(i) ((i) & 0x00800000 ? (i) | 0xFF000000 : (i))
#define  HALT 0
#define  PUSHC 1
#define  ADD 2
#define  SUB 3
#define  MUL 4
#define  DIV 5
#define  MOD 6
#define  RDINT 7
#define  WRINT 8
#define  RDCHAR 9
#define  WRCHAR 10
#define PUSHG 11
#define POPG 12
#define ASF 13
#define RSF 14
#define PUSHl 15
#define POPL 16
#define EQ 17
#define NE 18
#define LT 19
#define LE 20
#define GT 21
#define GE 22
#define JMP 23
#define BRF 24
#define BRT 25
#define CALL 26
#define RET 27
#define DROP 28
#define PUSHR 29
#define POPR 30
#define DUP 31


// ------------------------------------ Functions Definition -----------------------------
void CreateStack(Stack *ps);
void Push(uint32_t i, Stack *pStack);
void StackTop(uint32_t *i,Stack *ps);
void Pop (uint32_t *i,Stack *ps);
void PopAtAdress (uint32_t *i,Stack *ps , uint32_t adress);
void PushAtAdress (uint32_t i,Stack *ps , uint32_t adress);
void PopLokal (uint32_t *i,Stack *ps, uint32_t adress);
void PushLokal (uint32_t i,Stack *ps, uint32_t adress);
void  debugging(uint32_t *state , uint32_t *debuginput);




// ------------------------------------ Main Funktion -----------------------------

int main(int argc, char *argv[] )  {
    uint32_t  num1,num2,num3,adress,returnValue,debug=0,codeCounter=0,state=25,step=0,divv=0;
    int nn,mm;
    char cr;
    char *ProgramName = argv[1];
    char *debuginput;
/*Aufgabe 0 : 
*
* print the cmd lines :
* --version
* --help
* --debug
*/
    if (!strcmp(argv[1],"--help") ){
        printf("usage: ./njvm2 [options] <code file>\n"
               "--debug          start virtual machine in debug mode"
               "  --version        show version and exit\n"
               "  --help           show this help and exit\n");
        return (99);
    }
    else if (!strcmp(argv[1],"--version")){
        printf("Ninja Virtual Machine version 4 \n");
        return (99);
    }
    else if ( !strcmp(argv[1],"--debug") && argc<3){
        fprintf(stderr,"No file entered to be debugged\n");
        return (99);
    }
    else if (!strcmp(argv[1],"--debug") ){
        debug = 1;

        debugging(&state , &debuginput);
    }

    else if (argc>2){
        fprintf(stderr,"Error: unknown option '%s', try './njvm --help'\n",argv[1]);
        return (99);
    }
    if (debug!=0){
        ProgramName=argv[2];
    }
    /*unsigned int x=5 , y;

    IR = s.entry[PC];
    PC = PC + 1;
    Push(x,&s);
    StackTop(&y,&s);
    printf("%d",y);
    */
    CreateStack(&program_mem);
    FILE *fh = fopen (ProgramName, "r");
    if (fh != NULL) {
        fread(&program_mem.entry, sizeof (program_mem.entry[0]), 5000, fh);



    }
    if (program_mem.entry[0] !=0x46424a4e){
        fprintf(stderr,"The file entered is not a Ninja file\n");
        return (80);
    }
    else if (program_mem.entry[1] !=4){
        fprintf(stderr,"The Ninja Version is wrong\n");
        return (90);
    }
    else{

    }
    if (fh != NULL) {
        fread(&program_mem.entry, sizeof (program_mem.entry[0]), program_mem.entry[2]+4, fh);
        fclose (fh);


    }
            do{
        if (debug==0 |(debug ==1 &state ==4))
            printf("Ninja Virtual Machine started\n");
        uint8_t opcode = program_mem.entry[4] >> 24;
        uint32_t counter = 4;
        while (opcode != HALT) {
            switch (opcode) {

                case 1:
                    Push(program_mem.entry[counter] & 0x00FFFFFF, &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : PUSHC %d \n", codeCounter, num1);
                    break;
                case 2:
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to ADD");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);

                    Push((num1 + num2), &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : ADD %d \n", codeCounter, num1);
                    break;
                case 3 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to SUB");

                    }

                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);

                    Push((-num1 + num2), &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : SUB %d \n", codeCounter, num1);
                    break;
                case 4 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to MUL");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);

                    Push((num1 * num2), &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : MUL %d \n", codeCounter, num1);
                    break;
                case 5 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to DIV");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);
                    if (num1==0){
                        fprintf(stderr,"Cant divide by Zero");

                    }
                    if(num1>>20 <0xf)

                        num1=num1;
                    else{
                        num1= -num1;
                        divv++;
                    }

                    if(num2>>20 <0xf)

                        num2=num2;
                    else{
                        num2 = -num2;
                        divv++;
                    }
                    num3=num2/num1;
                    if (divv==1)
                    Push((-num3), &mystack);
                    else
                        Push((num3), &mystack);

                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : DIV %d \n", codeCounter, num1);
                    divv=0;
                    break;
                case 6 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to MOD");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);

                    Push((num2%num1), &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : MOD %d \n", codeCounter, num1);
                    break;
                case 7 :

                    scanf("%d", &num1);
                    Push(num1, &mystack);
                    StackTop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : RDINT %d \n", codeCounter, num1);
                    break;
                case 8 :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements to WRINT");

                    }
                    Pop(&num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : WRINT \n", codeCounter);
                    else if (debug==0 |(debug ==1 &state ==4))
                        printf("%d", num1);
                    break;
                case 9 :
                    if (debug != 0 & (state == 1| state ==3))
                        printf("Enter a Char\n");
                    scanf("%c", &cr);
                    Push((uint32_t) cr, &mystack);
                    StackTop(&cr, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : RDCHAR  %c \n", codeCounter, cr);
                    break;
                case 0xa :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements to WRCHAR");

                    }
                    Pop(&cr, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : WRCHAR  \n", codeCounter);
                    else if (debug==0 |(debug ==1 &state ==4))
                        printf("%c", cr);

                    break;
                case 0xb :
                    adress = program_mem.entry[counter] & 0x00FFFFFF;
                    PopAtAdress(&num1, &SDA, adress);
                    Push(num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : PUSHG %d \n", codeCounter, num1);
                    break;
                case 0xc :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements to POPG");

                    }
                    Pop(&num1, &mystack);
                    adress = program_mem.entry[counter] & 0x00FFFFFF;
                    PushAtAdress(num1, &SDA, adress);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : POPG %d \n", codeCounter, num1);
                    break;
                case 0xd :
                    num1 = program_mem.entry[counter] & 0x00FFFFFF;
                    Push(mystack.fp, &mystack);
                    mystack.fp = mystack.top;
                    mystack.top += num1;
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : ASF \n", codeCounter);
                    break;
                case 0xe :
                    mystack.top = mystack.fp;
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements to RSF");


                    }
                    Pop(&mystack.fp, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : RSF \n", codeCounter);
                    break;
                case 0xf :
                    adress = (program_mem.entry[counter] & 0x00FFFFFF);

                        adress= SIGN_EXTEND(adress);


                    PopLokal(&num1, &mystack, adress);
                    Push(num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : PUSHL %d \n", codeCounter, num1);
                    break;
                case 0x10 :
                    adress = (program_mem.entry[counter] & 0x00FFFFFF);

                        adress= SIGN_EXTEND(adress);


                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements to POPL");
                        exit(99);

                    }
                    Pop(&num1, &mystack);
                    PushLokal(num1, &mystack, adress);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : POPL %d \n", codeCounter, num1);
                    break;
                case 0x11 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements to EQ");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);
                    if (num1 == num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : EQ \n", codeCounter);
                    break;
                case 0x12 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num1, &mystack);
                    Pop(&num2, &mystack);
                    if (num1 != num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : NE \n", codeCounter);
                    break;
                case 0x13 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num2, &mystack);
                    Pop(&num1, &mystack);
                    if (num1 < num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : LT \n", codeCounter);
                    break;
                case 0x14 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num2, &mystack);
                    Pop(&num1, &mystack);
                    if (num1 <= num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : LE \n", codeCounter);
                    break;
                case 0x15 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num2, &mystack);
                    Pop(&num1, &mystack);
                    if (num1 > num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : GT \n", codeCounter);
                    break;
                case 0x16 :
                    if (mystack.top<2){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num2, &mystack);
                    Pop(&num1, &mystack);
                    if (num1 >= num2)
                        Push(1, &mystack);
                    else
                        Push(0, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : GE \n", codeCounter);
                    break;
                case 0x17 :
                    counter = (program_mem.entry[counter] & 0x00FFFFFF) + 3;
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : JMP \n", codeCounter);
                    break;
                case 0x18 :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num1, &mystack);
                    if (num1 == 0)
                        counter = (program_mem.entry[counter] & 0x00FFFFFF) + 3;
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : BRF \n", codeCounter);
                    break;
                case 0x19 :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num1, &mystack);
                    if (num1 != 0)
                        counter = (program_mem.entry[counter] & 0x00FFFFFF) + 3;
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : BRT \n", codeCounter);
                    break;
                case 0x1A :
                    Push(counter, &mystack);
                    counter = (program_mem.entry[counter] & 0x00FFFFFF) + 3;
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : CALL %d\n", codeCounter, counter + 1);
                    break;
                case 0x1B :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&counter, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : RT %d\n", codeCounter, counter);
                    break;
                case 0x1c :
                    for (int i = 0; i < (program_mem.entry[counter] & 0x00FFFFFF); ++i) {
                        Pop(&num1, &mystack);
                    }
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : DROP \n", codeCounter);
                    break;
                case 0x1d :
                    Push(returnValue, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : PUSHR %d\n", codeCounter, returnValue);
                    break;
                case 0x1e :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&returnValue, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : POPR %d\n", codeCounter, returnValue);
                    break;
                case 0x1f :
                    if (mystack.top<1){
                        fprintf(stderr,"No Enough Elements in the Stack");

                    }
                    Pop(&num1, &mystack);
                    Push(num1 * num1, &mystack);
                    if (debug != 0 & (state == 1| state ==3))
                        printf("%d : DUP \n", codeCounter);
                    break;

                default:

                    fprintf(stderr," you entered a wrong value \n ");

            }
            counter++;
            codeCounter++;
            opcode = program_mem.entry[counter] >> 24;
            if (state==3)
                debugging(&state,&debuginput);
            if (state == 0){
                printf("stack \nsp : %d   fp : %d   \n",mystack.top ,mystack.fp);
                printf("--------------------\n");
                for (int i = 0; i <mystack.top ; ++i) {
                    printf("%d :    %d \n",i,mystack.entry[i]);
                    printf("--------------------\n");
                }
                debugging(&state,&debuginput);
            }
        }
        if (debug != 0 &  state == 1)
            printf("   ------ end of code ------   \n");
        if (debug==0 |(debug ==1 &state ==4))
            printf("Ninja Virtual Machine stopped\n");
        codeCounter=0;
        if (debug!=0 &(state!=3 & state !=0 &state !=5))
            debugging(&state,&debuginput);

    }while (debug!=0 &state !=5 );
    return 0;
}







void CreateStack(Stack *ps){
    ps->top =0;
    ps->fp=0;
}
void Push( uint32_t instruktion, Stack *ps) {

    if(instruktion>>20 <0xf)

    instruktion= IMMEDIATE(instruktion);
    else
        instruktion= SIGN_EXTEND(instruktion);

    ps->entry[ps->top++]=instruktion;

}
void StackTop(uint32_t *i,Stack *ps){
    *i=ps->entry[ps->top-1];
}
void Pop (uint32_t *i,Stack *ps){
    *i=ps->entry[--ps->top];
}
void PopAtAdress (uint32_t *i,Stack *ps , uint32_t adress){
    *i=ps->entry[adress];
}
void PushAtAdress (uint32_t instruktion,Stack *ps , uint32_t adress) {

    if(instruktion>>20 <0xf)

        instruktion= IMMEDIATE(instruktion);
    else
        instruktion= SIGN_EXTEND(instruktion);

    ps->entry[adress]=instruktion;
}
void PushLokal( uint32_t instruktion, Stack *ps , uint32_t adress) {

    if(instruktion>>20 <0xf)

        instruktion= IMMEDIATE(instruktion);
    else
        instruktion= SIGN_EXTEND(instruktion);

    ps->entry[(ps->fp+(adress))]=instruktion;

}
void PopLokal (uint32_t *i,Stack *ps, uint32_t adress){
    *i=ps->entry[(ps->fp+(adress))];
}
void debugging(uint32_t *state , uint32_t  *debuginput){
    /*       inspect   state :0
             list      state :1
             breakoint state :2
             step      state :3
             run      state :4
             quit      state:5

     */

    printf("DEBUG: inspect, list, step, run, quit?\n");

    scanf("%s" , &debuginput);
    if (!strcmp(&debuginput,"inspect")){
        *state=0;
    }
    else if (!strcmp(&debuginput,"list"))
        *state=1;

    else if (!strcmp(&debuginput,"step"))
        *state=3;
    else if (!strcmp(&debuginput,"run"))
        *state=4;
    else if (!strcmp(&debuginput,"quit"))
        *state=5;
    else{
        printf("Sorry, You entered a Wrong value for the Debugger \n\n");
        *state =25;
    }

}
