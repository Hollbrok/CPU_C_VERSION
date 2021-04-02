#include "assem.h"

int main(int argc, char *argv[])
{
    printf("Assembling in progress..\n");

    FILE* user_code = nullptr;

    if(argc == 1)
    {
        user_code = fopen("user_code[for_user].asm", "rb");	// fopen(argv[1], "rb");
        assert(user_code && "can't open user_code");
    }
    else if(argc == 2)
    {
        user_code = fopen(argv[argc - 1], "rb");	// fopen(argv[1], "rb");
        assert(user_code && "can't open argv[argc - 1]");
    }
    else
    {
        printf("Too much arguments.\nExit..");
        for(int i = 0; i < argc; i++)
            printf("[%d].%s\n", i + 1, argv[i]);
        fclose(user_code);
        return 2;
    }

    Text text_c(user_code);

    Code code_c(&text_c);

    Bytecode byte_c(&code_c);

    printf("DONE!!\n");
    fclose(user_code);

    return 0;
}
