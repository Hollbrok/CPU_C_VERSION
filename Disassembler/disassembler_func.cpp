#include "disassembler.h"

auto make_bytecode(FILE* text, Bytecode* byte_struct) -> void
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

    byte_struct->bytecode_capacity = space_counter;

    byte_struct->data     = (double*) calloc(byte_struct->bytecode_capacity, sizeof(double));
    assert(byte_struct->data);
    int ass_cur_size = 0;

    while(*buffer_char)
    {
        double temp_val             = get_number(&buffer_char);
        byte_struct->data[ass_cur_size++] = temp_val;
        ignore_spaces(&buffer_char);
    }
    byte_struct->bytecode_capacity = ass_cur_size;
    return;
}

auto ignore_spaces(char** buffer) -> void
{
    while (isspace(**buffer))
        (*buffer)++;
}

auto bytecode_destruct(Bytecode* byte_struct) -> void //перед free нужно еще пройтись по всем элементам занулить их
{
    assert(byte_struct);

    free(byte_struct->data);
    byte_struct->data = nullptr;

    byte_struct->bytecode_capacity = 0;
}

auto disassembler(Bytecode* byte_struct, FILE* result) -> void
{
    assert(byte_struct);

    int skip_first  = -1;
    int skip_second = -1;
    for (int i = 0; i < byte_struct->bytecode_capacity; i++)
    {
        if ((skip_first == i) || (skip_second == i))
            continue;
        if (static_cast<int>(byte_struct->data[i]) == 1) // PUSH
        {
            if(static_cast<int>(byte_struct->data[i + 1]) == 1)
            {
                fprintf(result, "push %lg\n", byte_struct->data[i + 2]);
                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == 2)
            {
                switch(static_cast<int>(byte_struct->data[i + 2]))
                {
                    case static_cast<int>(Commands::CMD_RAX):
                    {
                        fprintf(result, "push rax\n");
                        break;
                    }
                    case static_cast<int>(Commands::CMD_RBX):
                    {
                        fprintf(result, "push rbx\n");
                        break;
                    }
                    case static_cast<int>(Commands::CMD_RCX):
                    {
                        fprintf(result, "push rcx\n");
                        break;
                    }
                    case static_cast<int>(Commands::CMD_RDX):
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
                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else
                printf("BIG NT\n");
        }
        else if (static_cast<int>(byte_struct->data[i]) == 21) // PUSH
        {
            if (static_cast<int>(byte_struct->data[i + 1]) == 1)
                fprintf(result, "pop\n");
            else if (static_cast<int>(byte_struct->data[i + 1]) == 2)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == 17)
                    fprintf(result, "pop rax\n");
                else if (static_cast<int>(byte_struct->data[i + 2]) == 18)
                    fprintf(result, "pop rbx\n");
                else if (static_cast<int>(byte_struct->data[i + 2]) == 19)
                    fprintf(result, "pop rcx\n");
                else if (static_cast<int>(byte_struct->data[i + 2]) == 20)
                    fprintf(result, "pop rdx\n");
            }

            skip_first  = i + 1;
            skip_second = i + 2;

        }
        else
        {
            switch(static_cast<int>(byte_struct->data[i]))
            {
                case static_cast<int>(Commands::CMD_HLT):/*hlt*/
                {
                    fprintf(result, "hlt\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_PUSH):/*push(то есть error, так как если push, то сюда не должно дойти)*/
                {
                    FILE* error = fopen("ERROR_PRINT.txt", "ab");
                    fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                    fprintf(error, "ERROR in LINE %d", __LINE__);
                    fclose(error);
                    break;
                }
                case static_cast<int>(Commands::CMD_ADD):/*add*/
                {
                    fprintf(result, "add\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_MUL):/*mul*/
                {
                    fprintf(result, "mul\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_DIV):/*div*/
                {
                    fprintf(result, "div\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_SUB):/*sub*/
                {
                    fprintf(result, "sub\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_SIN):/*sin*/
                {
                    fprintf(result, "sin\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_COS):/*cos*/
                {
                    fprintf(result, "cos\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_POW):/*pow*/
                {
                    fprintf(result, "pow\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_SQRT):/*sqrt*/
                {
                    fprintf(result, "sqrt\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_IN):/*in*/
                {
                    fprintf(result, "in\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_OUT):/*out*/
                {
                    fprintf(result, "out\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_DEL):/*del*/
                {
                    fprintf(result, "del\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_LN):/*ln*/
                {
                    fprintf(result, "ln\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_LOG10):/*log10*/
                {
                    fprintf(result, "log10\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_LOG2):/*log2*/
                {
                    fprintf(result, "log2\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_JMP):/*23*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "jmp :LAB%d\n", number_command);
                    break;
                }
                case static_cast<int>(Commands::CMD_LABEL):/*22*/
                {
                    fprintf(result, "LAB%d:\n", i);
                    break;
                }
                case static_cast<int>(Commands::CMD_JE):/*24 ==*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "je :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_JAB):/*25 !=*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "jab :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_JAE):/*26 >=*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "jae :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_JBE):/*27 <=*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "jbe :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_JA):/*28 >*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "ja :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_JB):/*29*/
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "jb :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                case static_cast<int>(Commands::CMD_RET):
                {
                    fprintf(result, "ret\n");
                    break;
                }
                case static_cast<int>(Commands::CMD_CALL):
                {
                    int number_command = static_cast<int>(byte_struct->data[i + 1]);
                    fprintf(result, "call :LAB%d\n", number_command);
                    skip_first = i + 1;
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "ab");
                    fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                    fprintf(error, "Disassemblers doesn't know this command..\n");
                    fprintf(error, "bytecode[%d] = [%d]", i, static_cast<int>(byte_struct->data[i]));
                    fclose(error);
                }
            }
        }
    }

    return;
}

auto get_number(char** buffer) -> double
{
    ignore_spaces(buffer);
    double number = atof(*buffer);

    while (isdigit(**buffer) || (**buffer == ','))
        (*buffer)++;

    ignore_spaces(buffer);
    return number;
}

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;


    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

auto define_date(void) -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}
