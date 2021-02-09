#include "cpu.h"

int END_STATE               = 0;   // Статус выхода из ассемблирования кода
int EXIT_CONDITION          = 0;   // Статус выхода из перевода ассеблерного кода в действия
int PRINT_STATE             = 0;   // Отвечает за то, дополнять в print_for_user или записывать сначала в зависимости от того,
                                   // была ли вызвана дополнительная функция для печати

int NEW_COMMAND_ERROR       = 0;   // Если была обнаружена новая команда, то gg
int IS_LAST_COMMAND_PUSH    = 0;   // Для проверки на неопознанную команду

void make_ass_s(FILE* text, ass_code* ass_s)
{
    fseek(text, 0, SEEK_END);
    int file_length = ftell(text);
    fseek(text, 0, SEEK_SET);
    file_length++;

    char* buffer_char = (char*) calloc(file_length, sizeof(char));
    assert(buffer_char);
    //printf("для buffer_char выделено [%d]\n", file_length);

    fread(buffer_char, sizeof(char), file_length, text);

    int space_counter = 0;
    for(int index = 0; index < file_length; index++)
        if(buffer_char[index] == ' ')
            space_counter++;

    ass_s->max_ass_size = space_counter;//int size_buffer = space_counter;
    //printf("Размер buffer_double = %d\n", size_buffer);

    ass_s->data     = (double*) calloc(ass_s->max_ass_size, sizeof(double));
    assert(ass_s->data);
    int ass_cur_size = 0;

    while(*buffer_char)//for(int index = 0; index < size_buffer; index++)
    {
        //printf("IN WHILE buffer_char = [%c]\n", *buffer_char);
        double temp_val = get_number(&buffer_char);
        //printf("temp_val = %lg\n", temp_val);

        ass_s->data[ass_cur_size++] = temp_val;
        ignore_spaces(&buffer_char);
    }

    //printf("max = %d, size = %d\n", ass_s->max_ass_size, ass_s->ass_size);
    ass_s->max_ass_size = ass_cur_size;
    //printf("7777777\n7777777777\n777777777\n");
    //printf("ass_size = [%d]\n", ass_s->ass_size);

    /*for(int index = 0; index < ass_s->max_ass_size; index++)
    {
        //printf("$%d$", index);
        printf("[%lg]\n", ass_s->data[index]);
    } */

}

double get_number(char** buffer)
{
    ignore_spaces(buffer);
    double number = atof(*buffer);
    //printf("number = %lg\n", number);
    //getch();

    while (isdigit(**buffer) || (**buffer == ','))
        (*buffer)++;

    ignore_spaces(buffer);

    return number;
}


void ignore_spaces(char** buffer)
{
    while (isspace(**buffer))// || (**buffer == ','))
        (*buffer)++;
}

void CPU(ass_code* ass_s, stack_t* Stack)
{
    assert(Stack);
    assert(ass_s);
    //assert(reg_s);

    if(NEW_COMMAND_ERROR)
        return;

    struct rix rix_s = {};

    int skip_first  = -1;
    int skip_second = -1;

    //ass_s->cur_rix = 0;

    for(int i = 0; i < ass_s->max_ass_size; i++)
    {
        if(EXIT_CONDITION == 1)
            break;

        #ifdef DEBUG
            printf("i before skip = %d\n", i);
            printf("assembler[%d] = %lf\n", i, ass_s->data[i]);
        #endif


        if((skip_first == i) || (skip_second == i))
        {
            #ifdef DEBUG
                printf("\n i == skip = %d\n\n", skip);
            #endif
            continue;
        }
        else if(ass_s->data[i] == 1)//PUSH
        {
            #ifdef DEBUG
                printf("i after skip = %d\n", i);
                printf("Pushing this number %lf\n",ass_s->data[i]);
                printf("(double)push_num = %lf\n", ass_s->data[i]);
            #endif
            if (ass_s->data[i + 1] == 1)
                push_stack(Stack, ass_s->data[i + 2]);
            else if (ass_s->data[i + 1] == 2)
            {
                if (ass_s->data[i + 2] == 17)
                    push_stack(Stack, rix_s.rax);
                else if (ass_s->data[i + 2] == 18)
                    push_stack(Stack, rix_s.rbx);
                else if (ass_s->data[i + 2] == 19)
                    push_stack(Stack, rix_s.rcx);
                else if (ass_s->data[i + 2] == 20)
                    push_stack(Stack, rix_s.rdx);
            }

            skip_first  = i + 1;
            skip_second = i + 2;

        }

        else if(ass_s->data[i] == 21)//POP rix
        {
            if (ass_s->data[i + 1] == 1)
                pop_stack(Stack);
            else if (ass_s->data[i + 1] == 2)
            {
                if (ass_s->data[i + 2] == 17)
                    rix_s.rax = pop_stack(Stack);
                else if (ass_s->data[i + 2] == 18)
                    rix_s.rbx = pop_stack(Stack);
                else if (ass_s->data[i + 2] == 19)
                    rix_s.rcx = pop_stack(Stack);
                else if (ass_s->data[i + 2] == 20)
                    rix_s.rdx = pop_stack(Stack);
            }

            skip_first  = i + 1;
            skip_second = i + 2;

            /*FILE* error = fopen("[!]ERRORS.txt", "ab");
            fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
            fprintf(error, "ERROR in recognizing registers\n");
            fprintf(error, "assembler_code[%d] = %d", i, (int)ass_s->data[i]);
            fclose(error);*/
        }
        else
        {
            switch((int) (ass_s->data[i]))
            {
                case HLT:/*hlt*/
                {
                    //
                    //275//if()
                    //
                    EXIT_CONDITION = 1;
                    #ifdef DEBUG
                        printf("exit on command number %d, = %lf\n", i + 1, ass_s->data[i]);
                    #endif
                    break;
                }
                case PUSH:/*push(то есть error, так как если push, то сюда не должно дойти)*/
                {
                    FILE* error = fopen("ERROR_PRINT.txt", "ab");
                    fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                    fprintf(error, "ERROR in LINE %d", __LINE__);
                    fclose(error);
                    break;
                }
                case ADD:/*add*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x1 + x2);
                    break;
                }
                case MUL:/*mul*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x1 * x2);
                    break;
                }
                case DIV:/*div*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x2 / x1);
                    break;
                }
                case SUB:/*sub*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x2 - x1);
                    break;
                }
                case SIN:/*sin*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, sin(x1));
                    break;
                }
                case COS:/*cos*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, cos(x1));
                    break;
                }
                case POW:/*pow*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if((x1 < 0) || (x1 == 0 && x2 == 0))
                    {
                        FILE* result = fopen("results[for user].txt", "ab");
                        if(!PRINT_STATE)
                        {
                            fclose(result);
                            FILE* result = fopen("results[for user].txt", "wb");
                        }

                        fprintf(result, "sorry, but you should read more"
                                        "about the rules for exponentiation");
                        fclose(result);
                        PRINT_STATE = 1;
                    }

                    push_stack(Stack, pow(x2, x1));
                    break;
                }
                case SQRT:/*sqrt*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, sqrt(x1));
                    break;
                }
                case IN:/*in*/
                {
                    double x1 = 0;
                    scanf("%lf", &x1);
                    push_stack(Stack, x1);
                    break;
                }
                case PRINT:/*out*/
                {
                    double x1 = pop_stack(Stack);

                    FILE* result = fopen("results[for user].txt", "ab");

                    if(!PRINT_STATE)
                    {
                        fclose(result);
                        FILE* result = fopen("results[for user].txt", "wb");
                    }

                    PRINT_STATE = 1;

                    fprintf(result, "At your request we show the number at the top of\n"
                                    "the stack at the time of the call : [%lg]\n\n", x1);
                    push_stack(Stack, x1);
                    fclose(result);
                    break;
                }
                case DEL:/*del*/
                {
                    double trash = pop_stack(Stack);
                    break;
                }
                case LN:/*ln*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log(x1));
                    break;
                }
                case LOG10:/*log10*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log10(x1));
                    break;
                }
                case LOG2:/*log2*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log2(x1));
                    break;
                }
                case JMP:/*23*/
                {
                    for(int index = 0; index < ass_s->max_ass_size; index++)
                        if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                        {
                            //printf("HEY\n");
                            i = index; // в некст цикле i = index + 1. Тем самым мы
                                       // начинаем работу со следующей команды после метки
                            break;
                        }

                    break;
                }
                case LABEL:/*22*/
                {
                    skip_first = i + 1;
                    break;
                }
                case JE:/*24*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 == x1) // JE -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                case JAB:/*25*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 != x1) // JAB -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                case JAE:/*26*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 >= x1) // JAE -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                case JBE:/*27*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 <= x1) // JBE -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                case JA:/*28*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 > x1) // JA -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                case JB:/*29*/
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if(x2 < x1) // JB -- TRUE, то нужно выполнить переход на метку
                    {
                        for(int index = 0; index < ass_s->max_ass_size; index++)
                            if((ass_s->data[i + 1] == ass_s->data[index]) && (ass_s->data[index - 1] == 22))
                            {
                                //printf("IT WORKS\n");
                                i = index; // в некст цикле i = index + 1. Тем самым мы
                                           // начинаем работу со следующей команды после метки
                                break;     // Оно брейкается только с ифа, или с for??
                            }              // Ответ: из всего.

                    }
                    else
                        i++;
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "ab");
                    fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                    fprintf(error, "Unknown command..\n");
                    fprintf(error, "assembler_code[%d] = %d", i, (int)ass_s->data[i]);
                    fclose(error);
                }
            }
        }
    }


}

void print_for_user(stack_t* Stack)
{
    FILE* result = fopen("results[for user].txt", "a");

    if(!PRINT_STATE)
    {
        fclose(result);
        FILE* result = fopen("results[for user].txt", "w");
    }

    if(Stack->cur_size > 0)
    {
        fprintf(result, "числа в стеке:\n");

        int Size = Stack->cur_size;
        int max_order = 0;

        while(Size > 9)
        {
            Size /= 10;
            max_order++;
        }

        for(int i = 0; i < Stack->cur_size; i++)
        {

            int order  = 0;
            int i_copy = i + 1;

            while(i_copy > 9)
            {
                i_copy /= 10;
                order++;
            }

            #ifdef DEBUG
                printf("max_order = %d\n", max_order);
                printf("order = %d\n", order);
            #endif

            switch(order)
            {
                case 0:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }

                case 1:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 1; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }

                case 2:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 2; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                case 3:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 3; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                case 4:
                {
                    fprintf(result, "[");

                    for(int x = 0; x < max_order - 4; x++)
                        fprintf(result, "0");

                    fprintf(result, "%d] = %.2lf\n", i + 1, Stack->data[i]);
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "w");
                    fprintf(error, "Too many numbers in stack\n");
                    fclose(error);
                }
            }
        }
    }
    else fprintf(result, "Чисел в стеке нет.\n"
                         "Currently size of Stack is %d", Stack->cur_size);


    fclose(result);
}

char* set_time(struct tm *time)
{
    int size_time               = 40;

    char string_time[size_time] = {0};
    char *tmp                   = nullptr;


    int length = strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

char* define_date(void)
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}

void ass_code_destruct(ass_code* ass_s)//перед free нужно еще пройтись по всем элементам занулить их
{
    assert(ass_s);

    free(ass_s->data);
    ass_s->data = nullptr;

    ass_s->max_ass_size = 0;
}



