#include "disassembler.h"

int main()
{
    printf("Disassembling in progress..\n");

    setlocale(LC_ALL, "russian");

    FILE* text   = fopen("[!]assembler_code.txt", "r");
    FILE* result = fopen("disassem_result[for user].txt",   "wb");
    assert(text);
    assert(result);

    struct Bytecode bytecode = {};
    make_bytecode(text, &bytecode);

    disassembler(&bytecode, result);

    printf("DONE!!\n");

    bytecode_destruct(&bytecode);

    return 0;
}
