#include <iostream>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc == 1)
    {
        system("start Assembler.exe");
        system("start CPU.exe");
        return 0;
    }
    else if (argc == 2)
    {
        char command1[] = "start Assembler.exe ";
        strcat(command1, argv[argc - 1]);
        system(command1);
        system("start CPU.exe");
        return 0;
    }

    return 1;
}
