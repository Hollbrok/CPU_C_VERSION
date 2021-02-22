#include "assem.h"

int main()//int main(int argc, char *argv[])
{
    printf("Assembling in progress..\n");

    FILE* text = fopen("user_code[for user].txt", "r");
    assert(text);

    text_t text_s = {};                     // структура текста
    text_construct(&text_s, text);          // конструктор текста

    struct code_t code_s = {};              // структура кода пользователя (нужна по-большей части для удаления комментариев, лишних пробелов и для удобного дебага)
    code_construct(&text_s, &code_s);       // вычисляет кол-во команд + удобнее обрабатывать код

    struct ass_code ass_s = {};             // структура ассемблерного кода
    get_ass_code(&code_s, &ass_s);          // получает ассемблерный код из текста

    text_destruct(&text_s);                 // деконструктор текста
    code_destruct(&code_s);                 // деконструктор структуры кода (преобразованного текста)
    ass_code_destruct(&ass_s);              // деконструктор структуры ассемблерного кода

    printf("DONE!!\n");
    fclose(text);

    return 0;
}
