#include "cpu.h"

int main()
{
    printf("CPU in progress..\n");

    setlocale(LC_ALL, "russian");

    FILE* text = fopen("[!]assembler_code.txt", "r");
    FILE* res  = fopen("[!]log_stack.txt", "ab");

    fprintf(res, "\tDate of the log_stack : %s (dd/mm/yy)\n\n", define_date());
    fclose(res);

    char* name = nullptr;
    START_ACTIONS(Stack);
    Construct(&Stack, START_SIZE);

    struct ass_code ass_s = {};
    make_ass_s(text, &ass_s);

    CPU(&ass_s, &Stack);
    stack_dump(&Stack);
    ass_code_destruct(&ass_s);
    printf("DONE!!\n");

    return 0;
}
