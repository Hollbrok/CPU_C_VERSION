#include "cpu.h"

int main()
{
    printf("CPU in progress..\n");

    FILE* text = fopen("[!]assembler_code.txt", "r");
    FILE* res  = fopen("[!]log_stack.txt", "ab");

    fprintf(res, "\tDate of the log_stack : %s (dd/mm/yy)\n\n", define_date());
    fclose(res);

    struct Bytecode byte_struct = {};
    get_bytecode(text, &byte_struct);


    char* name = nullptr;                   // this is need for stack name
    START_ACTIONS(Stack);                   // make stack struct
    Construct(&Stack, START_SIZE);          // constructor of stack

    START_ACTIONS(Stack_call);              // similarly, but this is
    Construct(&Stack_call, START_SIZE);     // the stack of calls
    CPU(&byte_struct, &Stack, &Stack_call);

    //stack_dump(&Stack);

    bytecode_destruct(&byte_struct);
    stack_destruct(&Stack);
    stack_destruct(&Stack_call);
    printf("DONE!!\n");

    return 0;
}
