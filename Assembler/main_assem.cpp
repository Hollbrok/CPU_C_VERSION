#include "assem.h"

int main()	//int main(int argc, char *argv[])
{
    printf("Assembling in progress..\n");

    FILE* user_code = fopen("user_code[for user].txt", "rb");	// fopen(argv[1], "rb");
    assert(user_code);

    Text text_struct = {};                          // text struct
    text_construct(&text_struct, user_code);        // text construct

    Code code_struct = {};                          // code struct (delete comments and etc.)
    code_construct(&text_struct, &code_struct);     // code construct

    Bytecode byte_struct = {};                      // bytecode struct
    get_bytecode(&code_struct, &byte_struct);       // bytecode construct

    text_destruct(&text_struct);                    // deconstruct of text structure
    code_destruct(&code_struct);                    // deconstruct of code structure
    bytecode_destruct(&byte_struct);                // deconstruct of bytecode structure

    printf("DONE!!\n");
    fclose(user_code);

    return 0;
}
