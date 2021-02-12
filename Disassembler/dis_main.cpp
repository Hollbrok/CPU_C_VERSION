#include "disassembler.h"

int main()
{
    printf("Disassembling in progress..\n");

    setlocale(LC_ALL, "russian");

    FILE* text   = fopen("[!]assembler_code.txt", "r");
    FILE* result = fopen("disassem_result.txt",   "wb");

    struct ass_code ass_s = {};
    make_ass_s(text, &ass_s);

    disassembler(&ass_s, result);

    printf("DONE!!\n");

    ass_code_destruct(&ass_s);

    return 0;
}
