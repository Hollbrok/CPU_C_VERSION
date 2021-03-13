#include "assem.h"

int main()//int main(int argc, char *argv[])
{
    printf("Assembling in progress..\n");

    FILE* user_code = fopen("user_code[for user].txt", "r");
    assert(user_code);

    Text text_struct = {};                          // структура текста
    text_construct(&text_struct, user_code);        // конструктор текста

    Code code_struct = {};                          // структура кода пользователя (нужна по-большей части для удаления комментариев, лишних пробелов и для удобного дебага)
    code_construct(&text_struct, &code_struct);     // вычисляет кол-во команд + удобнее обрабатывать код

    Bytecode byte_struct = {};                      // структура ассемблерного кода
    get_bytecode(&code_struct, &byte_struct);       // получает ассемблерный код из текста

    text_destruct(&text_struct);                    // деконструктор текста
    code_destruct(&code_struct);                    // деконструктор структуры кода (преобразованного текста)
    bytecode_destruct(&byte_struct);                // деконструктор структуры ассемблерного кода

    printf("DONE!!\n");
    fclose(user_code);

    return 0;
}
