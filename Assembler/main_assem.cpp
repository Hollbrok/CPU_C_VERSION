#include "assem.h"

int main()//int main(int argc, char *argv[])
{
    printf("Assembling in progress..\n");

    FILE* user_code = fopen("user_code[for user].txt", "r");
    assert(user_code);

    Text text_struct = {};                          // ��������� ������
    text_construct(&text_struct, user_code);        // ����������� ������

    Code code_struct = {};                          // ��������� ���� ������������ (����� ��-������� ����� ��� �������� ������������, ������ �������� � ��� �������� ������)
    code_construct(&text_struct, &code_struct);     // ��������� ���-�� ������ + ������� ������������ ���

    Bytecode byte_struct = {};                      // ��������� ������������� ����
    get_bytecode(&code_struct, &byte_struct);       // �������� ������������ ��� �� ������

    text_destruct(&text_struct);                    // ������������� ������
    code_destruct(&code_struct);                    // ������������� ��������� ���� (���������������� ������)
    bytecode_destruct(&byte_struct);                // ������������� ��������� ������������� ����

    printf("DONE!!\n");
    fclose(user_code);

    return 0;
}
