#include "disassembler.h"

int EXIT_CONDITION = 0;

void disassembler(ass_code* ass_s, FILE* result)
{
    assert(ass_s);

    int skip_first  = -1;
    int skip_second = -1;

    for (int i = 0; i < ass_s->max_ass_size; i++)
    {
        if (EXIT_CONDITION == 1)
            break;
        if ((skip_first == i) || (skip_second == i))
            continue;
        //printf("ass_s->data[%d] = %lg\n", i, ass_s->data[i]);
        if ((int)ass_s->data[i] == 1) // PUSH
        {
            //printf("PUSH");
            if((int)ass_s->data[i + 1] == 1)
            {
                //printf(" NUMBER\n");
                fprintf(result, "push %lg\n", ass_s->data[i + 2]);
                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else if ((int)ass_s->data[i + 1] == 2)
            {
                //printf(" rix\n");
                switch((int)ass_s->data[i + 2])
                {
                    case RAX:
                    {
                        fprintf(result, "push rax\n");
                        break;
                    }
                    case RBX:
                    {
                        fprintf(result, "push rbx\n");
                        break;
                    }
                    case RCX:
                    {
                        fprintf(result, "push rcx\n");
                        break;
                    }
                    case RDX:
                    {
                        fprintf(result, "push rdx\n");
                        break;
                    }
                    default:
                    {
                        printf("BIG NT\n");
                        break;
                    }
                }
                //printf("SKIPPPP\n");
                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else
                printf("BIG NT\n");
        }
        else if ((int)ass_s->data[i] == 21) // PUSH
        {
            if ((int)ass_s->data[i + 1] == 1)
                fprintf(result, "pop\n");
            else if ((int)ass_s->data[i + 1] == 2)
            {
                if ((int)ass_s->data[i + 2] == 17)
                    fprintf(result, "pop rax\n");
                else if ((int)ass_s->data[i + 2] == 18)
                    fprintf(result, "pop rbx\n");
                else if ((int)ass_s->data[i + 2] == 19)
                    fprintf(result, "pop rcx\n");
                else if ((int)ass_s->data[i + 2] == 20)
                    fprintf(result, "pop rdx\n");
            }

            skip_first  = i + 1;
            skip_second = i + 2;

        }
        else
        {
            switch((int)ass_s->data[i])
            {
                case HLT:/*hlt*/
                {
                    EXIT_CONDITION = 1;
                    fprintf(result, "hlt\n");
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
                    //printf("%lg\n",ass_s->data[i]);
                    fprintf(result, "add\n");
                    break;
                }
                case MUL:/*mul*/
                {
                    fprintf(result, "mul\n");
                    break;
                }
                case DIV:/*div*/
                {
                    fprintf(result, "div\n");
                    break;
                }
                case SUB:/*sub*/
                {
                    fprintf(result, "sub\n");
                    break;
                }
                case SIN:/*sin*/
                {
                    fprintf(result, "sin\n");
                    break;
                }
                case COS:/*cos*/
                {
                    fprintf(result, "cos\n");
                    break;
                }
                case POW:/*pow*/
                {
                    fprintf(result, "pow\n");
                    break;
                }
                case SQRT:/*sqrt*/
                {
                    fprintf(result, "sqrt\n");
                    break;
                }
                case IN:/*in*/
                {
                    fprintf(result, "in\n");
                    break;
                }
                case PRINT:/*out*/
                {
                    fprintf(result, "out\n");
                    break;
                }
                case DEL:/*del*/
                {
                    fprintf(result, "del\n");
                    break;
                }
                case LN:/*ln*/
                {
                    fprintf(result, "ln\n");
                    break;
                }
                case LOG10:/*log10*/
                {
                    fprintf(result, "log10\n");
                    break;
                }
                case LOG2:/*log2*/
                {
                    fprintf(result, "log2\n");
                    break;
                }
                case JMP:/*23*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "jmp :LAB%d\n", number_command);
                    break;
                }
                case LABEL:/*22*/
                {
                    //int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "LAB%d:\n", i);
                    //skip_first = i + 1;
                    break;
                }
                case JE:/*24 ==*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "je :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case JAB:/*25 !=*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "jab :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case JAE:/*26 >=*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "jae :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case JBE:/*27 <=*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "jbe :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case JA:/*28 >*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "ja :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case JB:/*29*/
                {
                    int number_command = (int)ass_s->data[i + 1];
                    fprintf(result, "jb :LAB%d\n", number_command);
                    skip_first = i + 1;
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

    return;
}

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

    while(*buffer_char)
    {
        double temp_val             = get_number(&buffer_char);
        //printf("get number %lg\n", temp_val);
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

void ass_code_destruct(ass_code* ass_s)//перед free нужно еще пройтись по всем элементам занулить их
{
    assert(ass_s);

    free(ass_s->data);
    ass_s->data = nullptr;

    ass_s->max_ass_size = 0;
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

char* define_date(void)
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}
