#include "cpu.h"

int main()
{
    printf("CPU in progress..\n");

    //setlocale(LC_ALL, "russian");

    FILE* text = fopen("[!]assembler_code.txt", "rb");
    //FILE* res  = fopen("[!]log_stack.txt", "ab");

    //fprintf(res, "\tDate of the log_stack : %s (dd/mm/yy)\n\n", define_date());
    //fclose(res);

    struct Bytecode byte_struct = {};
    get_bytecode(text, &byte_struct);


    char* name = nullptr;                   // this is need for stack name
    START_ACTIONS(Stack);                   // make stack struct
    Construct(&Stack, START_SIZE);          // constructor of stack

    START_ACTIONS(Stack_call);              // similarly, but this is
    Construct(&Stack_call, START_SIZE);     // the stack of calls

    CPU(&byte_struct, &Stack, &Stack_call); // Сделать структуру CPU.

    //FILE* res = fopen("[!]log_stack.txt", "ab");
    //fprintf(res, "\tDate of the log_stack : %s (dd/mm/yy)\n\n", define_date());
    //stack_dump(&Stack);
    //fclose(res);

    bytecode_destruct(&byte_struct);
    stack_destruct(&Stack);
    stack_destruct(&Stack_call);

    //txMessageBox("Привет!");
    //printf("DONE!!\n");

    return 0;
}
