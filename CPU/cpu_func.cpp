#include "cpu.h"

int EXIT_CONDITION          = 0;   // Статус выхода из перевода ассеблерного кода в действия

void make_ass_s(FILE* text, ass_code* ass_s)
{
    fseek(text, 0, SEEK_END);
    int file_length = ftell(text);
    fseek(text, 0, SEEK_SET);
    file_length++;

    char* buffer_char = (char*) calloc(file_length, sizeof(char));
    assert(buffer_char);

    fread(buffer_char, sizeof(char), file_length, text);

    int space_counter = 0;
    for(int index = 0; index < file_length; index++)
        if(buffer_char[index] == ' ')
            space_counter++;

    ass_s->max_ass_size = space_counter;

    ass_s->data     = (double*) calloc(ass_s->max_ass_size, sizeof(double));
    assert(ass_s->data);
    int ass_cur_size = 0;

    while (*buffer_char)
    {
        double temp_val             = get_number(&buffer_char);
        ass_s->data[ass_cur_size++] = temp_val;
        ignore_spaces(&buffer_char);
    }
    ass_s->max_ass_size = ass_cur_size;
    return;
}

double get_number(char** buffer)
{
    ignore_spaces(buffer);
    double number = atof(*buffer);

    while (isdigit(**buffer) || (**buffer == ','))
        (*buffer)++;

    ignore_spaces(buffer);
    return number;
}

void ignore_spaces(char** buffer)
{
    while (isspace(**buffer))
        (*buffer)++;
}

void CPU(ass_code* ass_s, stack_t* Stack)
{
    assert(Stack);
    assert(ass_s);

    struct rix rix_s = {};

    int skip_first  = -1;
    int skip_second = -1;

    for (int i = 0; i < ass_s->max_ass_size; i++)
    {
        if(EXIT_CONDITION == 1)
            break;

        if((skip_first == i) || (skip_second == i))
            continue;
        else if((int)ass_s->data[i] == 1)//PUSH
        {
            if ((int)ass_s->data[i + 1] == 1)
                push_stack(Stack, ass_s->data[i + 2]);
            else if ((int)ass_s->data[i + 1] == 2)
            {
                if ((int)ass_s->data[i + 2] == 17)
                    push_stack(Stack, rix_s.rax);
                else if ((int)ass_s->data[i + 2] == 18)
                    push_stack(Stack, rix_s.rbx);
                else if ((int)ass_s->data[i + 2] == 19)
                    push_stack(Stack, rix_s.rcx);
                else if ((int)ass_s->data[i + 2] == 20)
                    push_stack(Stack, rix_s.rdx);
            }

            skip_first  = i + 1;
            skip_second = i + 2;
        }
        else if((int)ass_s->data[i] == 21)//POP / POPrix
        {
            if ((int)ass_s->data[i + 1] == 1)
                pop_stack(Stack);
            else if ((int)ass_s->data[i + 1] == 2)
            {
                if ((int)ass_s->data[i + 2] == 17)
                    rix_s.rax = pop_stack(Stack);
                else if ((int)ass_s->data[i + 2] == 18)
                    rix_s.rbx = pop_stack(Stack);
                else if ((int)ass_s->data[i + 2] == 19)
                    rix_s.rcx = pop_stack(Stack);
                else if ((int)ass_s->data[i + 2] == 20)
                    rix_s.rdx = pop_stack(Stack);
            }

            skip_first  = i + 1;
            skip_second = i + 2;

        }
        else
        {
            switch((int)(ass_s->data[i]))
            {
                case HLT:/*hlt*/
                {
                    EXIT_CONDITION = 1;
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
                    if (Stack->cur_size < 2)
                    {
                        printf("not enough numbers in the stack. Exit..\n");
                        EXIT_CONDITION = 1;
                        break;
                    }

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

                    if ((x1 < 0))
                    {
                        FILE* result = fopen("results[for user].txt", "wb");

                        fprintf(result, "sorry, but you should read more"
                                        "about the rules for exponentiation");
                        fclose(result);
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
                    if(Stack->cur_size == 0)
                    {
                        printf("not enough space in Stack to out. Exit..\n");
                        EXIT_CONDITION = 1;
                        break;
                    }
                    double x1 = pop_stack(Stack);

                    FILE* result = fopen("results[for user].txt", "wb");

                    fprintf(result, "At your request we show the number at the top of\n"
                                    "the stack at the time of the call : [%lg]\n\n", x1);
                    push_stack(Stack, x1);
                    fclose(result);
                    break;
                }
                case DEL:/*del*/
                {
                    pop_stack(Stack);
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
                    i = (int)ass_s->data[i + 1]; // и так целое число, (int) чтобы убрать варнинг
                    break;
                }
                case LABEL:/*22*/
                {
                    break;
                }
                case JE:/*24 ==   */
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 == x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
                    else
                        i++;
                    break;
                }
                case JAB:/*25 !=  */
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 != x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
                    else
                        i++;
                    break;
                }
                case JAE:/*26 >=  */
                {
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 >= x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
                    else
                        i++;
                    break;
                }
                case JBE:/*27 <=  */
                {
                     double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 <= x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
                    else
                        i++;
                    break;
                }
                case JA:/*28 >    */
                {
                     double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 > x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
                    else
                        i++;
                    break;
                }
                case JB:/*29 <    */
                {
                     double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 < x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = (int)ass_s->data[i + 1];
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
    FILE* result = fopen("results[for user].txt", "wb");

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
    char string_time[size_time] = {};
    char *tmp                   = nullptr;

    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

char* define_date()
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



