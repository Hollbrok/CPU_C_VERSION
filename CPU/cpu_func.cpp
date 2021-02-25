#include "cpu.h"

static int EXIT_CONDITION          = 0;   // Статус выхода из перевода ассеблерного кода в действия
static int SECOND_PRINT            = 0;   // Для принта for_user

auto get_bytecode(FILE* text, Bytecode* byte_struct) -> void
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

    while (*buffer_char)
    {
        double temp_val             = get_number(&buffer_char);
        byte_struct->data[ass_cur_size++] = temp_val;
        ignore_spaces(&buffer_char);
    }
    byte_struct->bytecode_capacity = ass_cur_size;
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

auto ignore_spaces(char** buffer) -> void
{
    while (isspace(**buffer))
        (*buffer)++;
}

auto CPU(Bytecode* byte_struct, stack_t* Stack, stack_t* Stack_call) -> void
{
    assert(Stack);
    assert(Stack_call);
    assert(byte_struct);

    struct Rix rix_struct = {}; // сделать просто массив и поменять кодировку регистров

    int skip_first  = -1;
    int skip_second = -1;

    using namespace my_commands;

    for (int i = 0; i < byte_struct->bytecode_capacity; i++)
    {
        //printf("i = %d\n", i);
        if(EXIT_CONDITION == 1)
            break;

        if((skip_first == i) || (skip_second == i))
        {
            //printf("skip_f_s = [%d],[%d]\n", skip_first, skip_second);
            continue;
        }
        //printf("in start i = %d, ass[i] = %.0lf\n", i, byte_struct->data[i]);
        if(static_cast<int>(byte_struct->data[i]) == static_cast<int>(Commands::CMD_PUSH))//PUSH
        {
            if (static_cast<int>(byte_struct->data[i + 1]) == 1)
                push_stack(Stack, byte_struct->data[i + 2]);
            else if (static_cast<int>(byte_struct->data[i + 1]) == 2)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RAX))
                    push_stack(Stack, rix_struct.rax);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RBX))
                    push_stack(Stack, rix_struct.rbx);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RCX))
                    push_stack(Stack, rix_struct.rcx);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RDX))
                    push_stack(Stack, rix_struct.rdx);
            }

            skip_first  = i + 1;
            skip_second = i + 2;
        }
        else if(static_cast<int>(byte_struct->data[i]) == static_cast<int>(Commands::CMD_POP))//POP / POPrix
        {
            if(Stack->cur_size < 1)
            {
                printf("not enough numbers to pop\n");
                EXIT_CONDITION = 1;
                break;
            }
            if (static_cast<int>(byte_struct->data[i + 1]) == 1)
            {
                pop_stack(Stack);
                skip_first  = i + 1;
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == 2)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RAX))
                    rix_struct.rax = pop_stack(Stack);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RBX))
                    rix_struct.rbx = pop_stack(Stack);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RCX))
                    rix_struct.rcx = pop_stack(Stack);
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RDX))
                    rix_struct.rdx = pop_stack(Stack);

                skip_first  = i + 1;
                skip_second = i + 2;
            }
        }
        else
        {
            switch((int)(byte_struct->data[i]))          // В switch'e нельзя использовать non-constexpr constants, поэтому тут без команды com_to_int
            {
                case static_cast<int>(Commands::CMD_HLT):/*hlt*/
                {
                    EXIT_CONDITION = TRUE;
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
                case static_cast<int>(Commands::CMD_MUL):/*mul*/
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to mul\n");
                        EXIT_CONDITION = 1;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x1 * x2);
                    break;
                }
                case static_cast<int>(Commands::CMD_DIV):/*div*/
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to div\n");
                        EXIT_CONDITION = 1;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x2 / x1);
                    break;
                }
                case static_cast<int>(Commands::CMD_SUB):/*sub*/
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to sub\n");
                        EXIT_CONDITION = 1;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);
                    push_stack(Stack, x2 - x1);
                    break;
                }
                case static_cast<int>(Commands::CMD_SIN):/*sin*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, sin(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_COS):/*cos*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, cos(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_POW):/*pow*/
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("no numbers to pow\n");
                        EXIT_CONDITION = 1;
                        break;
                    }
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
                case static_cast<int>(Commands::CMD_SQRT):/*sqrt*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, sqrt(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_IN):/*in*/
                {
                    double x1 = 0;
                    scanf("%lf", &x1);
                    push_stack(Stack, x1);
                    break;
                }
                case static_cast<int>(Commands::CMD_OUT):/*out*/
                {
                    //printf("in out\n");
                    if(Stack->cur_size == 0)
                    {
                        printf("not enough space in Stack to out. Exit..\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);

                    if(!SECOND_PRINT)
                    {
                        FILE* result = fopen("results[for user].txt", "wb");
                        SECOND_PRINT = TRUE;
                        fprintf(result, "[%lg]\n", x1);
                        fclose(result);
                    }
                    else
                    {
                        FILE* result = fopen("results[for user].txt", "a");
                        fprintf(result, "[%lg]\n", x1);
                        fclose(result);
                    }


                    push_stack(Stack, x1);
                    break;
                }
                case static_cast<int>(Commands::CMD_DEL):/*del*/
                {
                    pop_stack(Stack);
                    break;
                }
                case static_cast<int>(Commands::CMD_LN):/*ln*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_LOG10):/*log10*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log10(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_LOG2):/*log2*/
                {
                    double x1 = pop_stack(Stack);
                    push_stack(Stack, log2(x1));
                    break;
                }
                case static_cast<int>(Commands::CMD_CALL):
                {
                    //printf("CALL in i = [%d]\n", i);
                    push_stack(Stack_call, i);
                    i = (int)byte_struct->data[i + 1]; // и так целое число, (int) чтобы убрать варнинг
                    break;
                }
                case static_cast<int>(Commands::CMD_RET):
                {
                    //printf("RET is command number i = [%d]\n", i);
                    if(Stack_call->cur_size < 1)
                    {
                        printf("no number in stack_call to return\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    i = static_cast<int>(pop_stack(Stack_call)) + 1;
                    //printf("Next command will be i = %d\n", i + 1);
                    break;
                }
                case static_cast<int>(Commands::CMD_JMP):/*23*/
                {
                    i = static_cast<int>(byte_struct->data[i + 1]); // и так целое число, (int) чтобы убрать варнинг
                    break;
                }
                case static_cast<int>(Commands::CMD_LABEL):/*22*/
                {
                    break;
                }
                case static_cast<int>(Commands::CMD_JE):/*24 ==   */
                {
                    //printf("in JE\n");
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JE\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    //printf("x1 = %lg, x2 = %lg\n", x1, x2);
                    if (is_equal(x2, x1)) // JAE -- TRUE, то нужно выполнить переход на метку
                    {
                        //printf("РАВНЫ\n");
                        i = static_cast<int>(byte_struct->data[i + 1]);
                        //printf("Идем на i = %d\n", i);
                    }
                    else
                        i++;
                    break;
                }
                case static_cast<int>(Commands::CMD_JAB):/*25 !=  */
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JAB\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (!is_equal(x2, x1)) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = static_cast<int>(byte_struct->data[i + 1]);
                    else
                        i++;
                    break;
                }
                case static_cast<int>(Commands::CMD_JAE):/*26 >=  */
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JAE\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 >= x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = static_cast<int>(byte_struct->data[i + 1]);
                    else
                        i++;
                    break;
                }
                case static_cast<int>(Commands::CMD_JBE):/*27 <=  */
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JBE\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 <= x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = static_cast<int>(byte_struct->data[i + 1]);
                    else
                        i++;
                    break;
                }
                case static_cast<int>(Commands::CMD_JA):/*28 >    */
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JA\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 > x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = static_cast<int>(byte_struct->data[i + 1]);
                    else
                        i++;
                    break;
                }
                case static_cast<int>(Commands::CMD_JB):/*29 <    */
                {
                    if(Stack->cur_size < 2)
                    {
                        printf("not enough numbers to JB\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }
                    double x1 = pop_stack(Stack);
                    double x2 = pop_stack(Stack);

                    if (x2 < x1) // JAE -- TRUE, то нужно выполнить переход на метку
                        i = static_cast<int>(byte_struct->data[i + 1]);
                    else
                        i++;
                    break;
                }
                default:
                {
                    FILE* error = fopen("[!]ERRORS.txt", "ab");
                    fprintf(error, "\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                    fprintf(error, "Unknown command..\n");
                    fprintf(error, "bytecode[%d] = %d", i, static_cast<int>(byte_struct->data[i]));
                    fclose(error);
                }
            }
        }
    }
}

auto bytecode_destruct(Bytecode* byte_struct) -> void
{
    assert(byte_struct);

    free(byte_struct->data);
    byte_struct->data = nullptr;

    byte_struct->bytecode_capacity = 0;
}

auto print_for_user(stack_t* Stack) -> void
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

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;

    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return(tmp);
}

auto define_date() -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}

auto is_equal(double a, double b) -> bool
{
    // Проверяем числа на их близость - это нужно в случаях, когда сравниваемые числа являются нулевыми или около нуля
	double diff = fabs(a - b);
	if (diff <= absEpsilon)
		return true;

	// В противном случае, возвращаемся к алгоритму Кнута
	return diff <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * epsilon);
}





