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
        //printf("[%lg]\n", temp_val);
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

    while (isdigit(**buffer) || (**buffer == '.'))
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

    struct Rix rix_struct = {};  // сделать просто массив и поменять кодировку регистров

    int skip_first  = -1;
    int skip_second = -1;

    using namespace my_commands;

    char* OP = (char*) calloc(OP_SIZE, sizeof(char));
    assert(OP);

    for (int i = 0; i < byte_struct->bytecode_capacity; i++)
    {
        if(EXIT_CONDITION == 1)
            break;

        if((skip_first == i) || (skip_second == i))
            continue;

        if(static_cast<int>(byte_struct->data[i]) == static_cast<int>(Commands::CMD_PUSH))  //PUSH
        {
            if (static_cast<int>(byte_struct->data[i + 1]) == S_NUMBER_SPEC)
                push_stack(Stack, byte_struct->data[i + 2]);
            else if (static_cast<int>(byte_struct->data[i + 1]) == S_REGIST_SPEC)
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
            else if (static_cast<int>(byte_struct->data[i + 1]) == OP_NUMBER)
            {
                int adress = static_cast<int>(byte_struct->data[i + 2]);
                double *temp_pointer = (double*) (OP + adress);
                double x = *temp_pointer;
                printf("Push in stack from OP(number)[%d] number [%lg]\n", adress, x);
                push_stack(Stack, x);
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == OP_REGIST)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RAX))
                {
                    int adress = static_cast<int>(rix_struct.rax);
                    double* temp_pointer = (double*) (OP + adress);
                    double x = *temp_pointer;
                    printf("Pushing in stack from OP(rix)[%d] number [%lg]\n", adress, x);
                    push_stack(Stack, x);
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RBX))
                {
                    int adress = static_cast<int>(rix_struct.rbx);
                    double* temp_pointer = (double*) (OP + adress);
                    double x = *temp_pointer;
                    printf("Pushing in stack from OP(rix)[%d] number [%lg]\n", adress, x);
                    push_stack(Stack, x);
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RCX))
                {
                    int adress = static_cast<int>(rix_struct.rcx);
                    double* temp_pointer = (double*) (OP + adress);
                    double x = *temp_pointer;
                    printf("Pushing in stack from OP(rix)[%d number [%lg]\n", adress, x);
                    push_stack(Stack, x);
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RDX))
                {
                    int adress = static_cast<int>(rix_struct.rdx);
                    double* temp_pointer = (double*) (OP + adress);
                    double x = *temp_pointer;
                    printf("Pushing in stack from OP(rix)[%d number [%lg]\n", adress, x);
                    push_stack(Stack, x);
                }
            }
            else
            {
                printf("Error in push\n");
                EXIT_CONDITION = 1;
            }

            skip_first  = i + 1;
            skip_second = i + 2;
        }
        else if(static_cast<int>(byte_struct->data[i]) == static_cast<int>(Commands::CMD_POP))//POP
        {
            if(Stack->cur_size < 1)
            {
                printf("not enough numbers to pop\n");
                EXIT_CONDITION = 1;
                break;
            }
            if (static_cast<int>(byte_struct->data[i + 1]) == S_NUMBER_SPEC)
            {
                pop_stack(Stack);
                skip_first  = i + 1;
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == S_REGIST_SPEC)       // Сделать через switch
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
            else if (static_cast<int>(byte_struct->data[i + 1]) == OP_NUMBER) // следующее число -- это адрес оперативки
            {
                double x = pop_stack(Stack);

                double* temp_pointer = (double*) (OP + static_cast<int>(byte_struct->data[i + 2]));
                *temp_pointer        = x;
                printf("Pushing from Stack to OP[%d] number [%lg]\n", static_cast<int>(byte_struct->data[i + 2]), x);

                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == OP_REGIST)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RAX))
                {
                    double x = pop_stack(Stack);

                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct.rax));
                    *temp_pointer        = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RBX))
                {
                    double x = pop_stack(Stack);

                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct.rbx));
                    *temp_pointer        = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RCX))
                {
                    double x = pop_stack(Stack);

                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct.rcx));
                    *temp_pointer        = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RDX))
                {
                    double x = pop_stack(Stack);

                    double* temp_pointer = (double*) (OP + static_cast<int>(rix_struct.rdx));
                    *temp_pointer        = x;
                }

                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else  if (static_cast<int>(byte_struct->data[i + 1]) == OP_CHAR_REG)
            {
                if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RAX))
                {
                    char x = static_cast<char>(pop_stack(Stack));

                    char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct.rax));
                    *temp_pointer      = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RBX))
                {
                    char x = static_cast<char>(pop_stack(Stack));

                    char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct.rbx));
                    *temp_pointer      = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RCX))
                {
                    char x = static_cast<char>(pop_stack(Stack));

                    char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct.rcx));
                    *temp_pointer      = x;
                }
                else if (static_cast<int>(byte_struct->data[i + 2]) == static_cast<int>(Commands::CMD_RDX))
                {
                    char x = static_cast<char>(pop_stack(Stack));

                    char* temp_pointer = (char*) (OP + static_cast<int>(rix_struct.rdx));
                    *temp_pointer      = x;
                }

                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else if (static_cast<int>(byte_struct->data[i + 1]) == OP_CHAR_NUM)
            {
                char x = static_cast<char>(pop_stack(Stack));

                char* temp_pointer = (char*) (OP + static_cast<int>(byte_struct->data[i + 2]));
                *temp_pointer        = x;
                //printf("Pushing from Stack to OP[%d] number [%lg]\n", static_cast<int>(byte_struct->data[i + 2]), x);

                skip_first  = i + 1;
                skip_second = i + 2;
            }
            else
            {
                printf("Error in pop\n");
                EXIT_CONDITION = 1;
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
                        assert(result);
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
                case static_cast<int>(Commands::CMD_ABS):
                {
                    if(Stack->cur_size < 1)
                    {
                        printf("not enough numbers to abs\n");
                        EXIT_CONDITION = TRUE;
                        break;
                    }

                    double x = pop_stack(Stack);
                    push_stack(Stack, abs(x));
                    break;
                }
                case static_cast<int>(Commands::CMD_DRAW):
                {
                    system("clear");
                    txCreateWindow(SIZEX, SIZEY);
                    txSetDefaults();

                    for (int y = 0; y < SIZEY; y++)
                        for (int x = 0; x < SIZEX; x++)
                        {
                            //printf("x = %d, y = %d\n", x, y);
                            int cur_pixel = 3 * (SIZEX * y + x);
                            txSetPixel(x, y, RGB(OP[cur_pixel], OP[cur_pixel + 1], OP[cur_pixel + 2]));
                        }

                    //txDestroyWindow();
                    break;
                }
                case static_cast<int>(Commands::CMD_FILL):
                {
                    system("cls");
                    txCreateWindow(SIZEX, SIZEY);
                    txSetDefaults();
                    for (int y = 0; y < SIZEY; y++)
                        for (int x = 0; x < SIZEX; x++)
                            txSetPixel(x, y, RGB(OP[0], OP[1], OP[2]));
                    break;
                }
                case static_cast<int>(Commands::CMD_CIRC):
                {
                    system("cls");
                    //txCreateWindow(SIZEX, SIZEY);
                    //txSetDefaults();

                    if(Stack->cur_size < 3)
                    {
                        printf("not enough numbers to circ. Size = %d\n", Stack->cur_size);
                        EXIT_CONDITION = TRUE;
                        break;
                    }

                    int Radius = static_cast<int>(pop_stack(Stack));
                    int y_center = static_cast<int>(pop_stack(Stack));
                    int x_center = static_cast<int>(pop_stack(Stack));

                    for (int y = 0; y < SIZEY; y++)
                        for (int x = 0; x < SIZEX; x++)
                            if (( pow((x - x_center), 2) + pow((y - y_center), 2) ) < pow(Radius, 2))
                                txSetPixel(x, y, RGB(OP[3], OP[4], OP[5]));
                    break;
                }
                case static_cast<int>(Commands::CMD_KOPM):
                {
                    system("cls");
                    txCreateWindow(SIZEX, SIZEY);
                    txSetDefaults();

                    txSetFillColor(RGB(100, 100, 230));
                    txFloodFill(5, 5);


                    txTextOut(100, 145, "KOPM ONE LOVE");

                    txSetColor(RGB(0, 0, 0), 1);
                    txLine(175, 160, 165, 145);
                    txLine(175, 160, 185, 145);
                    txArc(165, 140, 175, 150, 0, 182);
                    txArc(175, 140, 185, 150, 0, 182);


                    txSetColor(RGB(237, 19, 19), 1);
                    txLine(215, 140, 205, 125);
                    txLine(215, 140, 225, 125);
                    txArc(205, 120, 215, 130, 0, 182);
                    txArc(215, 120, 225, 130, 0, 182);

                    txSetColor(RGB(48, 217, 65), 1);
                    txLine(115, 100, 105, 85);
                    txLine(115, 100, 125, 85);
                    txArc(105, 80, 115, 90, 0, 182);
                    txArc(115, 80, 125, 90, 0, 182);

                    //txSetFillColor(RGB(237, 19, 19));
                    //txFloodFill(215, 135);

                    break;
                }
                case static_cast<int>(Commands::CMD_CAT):
                {
                    system("cls");
                    txCreateWindow(SIZEX, SIZEY);
                    txSetDefaults();

                    txSetFillColor(RGB(0, 100, 167));
                    txFloodFill(5 , 5);

                    txSetFillColor(RGB(125, 125, 125)); // цвет тела
                    txEllipse(100, 70, 180, 180);       // рисуем тело

                    txSetFillColor(RGB(0, 0, 0));       // черный цвет
                    txEllipse(130, 90, 160, 150);       // рисуев внутри туловища

                    txSetFillColor(RGB(154, 148, 148)); // цвет головы
                    txCircle(140, 45, 25);              // голова

                    txSetColor(RGB(0, 0, 0), 1);        // цвет и толщина усиков
                    txLine(140, 55, 170, 45);           // сами усики
                    txLine(140, 55, 175, 53);
                    txLine(140, 55, 181, 61);
                    txLine(140, 55, 120, 45);
                    txLine(140, 55, 115, 53);
                    txLine(140, 55, 109, 61);

                    txSetFillColor(RGB(255, 255, 255)); // Цвет глаз (белый)
                    txEllipse(130, 35, 141, 50);        // глаз Л
                    txEllipse(145, 35, 156, 50);        // глаз П

                    txLine(120, 30, 125, 10);           // ухо Л
                    txLine(125, 10, 133, 23);

                    txSetFillColor(RGB(154, 148, 148)); // цвет заливки уха
                    txFloodFill(125, 12);               // заливаем ухо

                    txLine(150, 22, 158, 10);           // ухо П
                    txLine(158, 10, 160, 32);

                    txSetFillColor(RGB(154, 148, 148)); // цвет заливки уха
                    txFloodFill(157, 17);               // заливаем ухо


                    txSetFillColor(RGB(0, 0, 0));       // цвет заливки зрачка
                    txEllipse(132, 39, 137, 48);        // зрачок Л
                    txEllipse(147, 39, 152, 48);        // зрачок П

                    txTextOut(180, 45, "Meow-meow!");



                    break;
                }
                case static_cast<int>(Commands::CMD_MEM):
                {
                    system("cls");
                    txCreateWindow(SIZEX, SIZEY);
                    txSetDefaults();

                    HDC mem = txLoadImage("C:/Users/Danik/Documents/Задачи_СИ/Projects/ASSEM_CPU/bin/Debug/Mem.bmp");
                    txBitBlt(txDC(), 0, 0, SIZEX, SIZEY, mem, 0, 0);

                    srand(time(nullptr));

                    int number =  rand() % 6;

                    switch(number)
                    {
                        case 0:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 123, "Этот код не написал");

                            txTextOut(160, 110, "Тут вообще всё");
                            txTextOut(160, 123, "в одном файле");

                            txTextOut(8, 262, "У этого вообще");
                            txTextOut(8, 275, "нет функций!");


                            txTextOut(160, 262, "Ну нахер, дропа-");
                            txTextOut(160, 275, "ем проект");

                            break;
                        }
                        case 1:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 110, "Никто не ответил");
                            txTextOut(6, 123, "\"да\" или \"понятно\"");

                            txTextOut(160, 110, "Не задали ни");
                            txTextOut(160, 123, "одного вопроса");

                            txTextOut(8, 262, "Воскресенье очень");
                            txTextOut(8, 275, "напряженный день");


                            txTextOut(160, 262, "Скорее всего при-");
                            txTextOut(160, 275, "дется дропать ILAB");

                            break;
                        }
                        case 2:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 110, "Полторашка убежала,");
                            txTextOut(6, 123, "когда я ее гладил");

                            txTextOut(160, 110, "По физ-ре 0 посе-");
                            txTextOut(160, 123, "щений за семак");

                            txTextOut(8, 262, "БРС не сильно боль-");
                            txTextOut(8, 275, "ше, чем посещений");


                            txTextOut(160, 262, "Ухожу в академ, бу-");
                            txTextOut(160, 275, "ду гладить котиков");

                            break;
                        }
                        case 3:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 110, "Поступил на физ-");
                            txTextOut(6, 123, "тех чтоб бухать");

                            txTextOut(160, 110, "Эти на нк не поз-");
                            txTextOut(160, 123, "вали в пятницу");

                            txTextOut(8, 262, "Эти курить пошли без");
                            txTextOut(8, 275, "тебя уже который раз");


                            txTextOut(160, 262, "Ну нахер, лучше");
                            txTextOut(160, 275, "пойду в бомонку");

                            break;
                        }
                        case 4:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 110, "Пошел к деду");
                            txTextOut(6, 123, "чтобы прогать");

                            txTextOut(160, 110, "На матан времени");
                            txTextOut(160, 123, "не хватает");

                            txTextOut(8, 262, "На общесос почти ни");
                            txTextOut(8, 275, "разу не успел сходить");


                            txTextOut(160, 262, "Ну нахер, переве-");
                            txTextOut(160, 275, "дусь к Хохлову");

                            break;
                        }
                        case 5:
                        {
                            txSetColor(RGB(0, 0, 0));
                            txTextOut(6, 110, "Пишешь проект");
                            txTextOut(6, 123, "на ILAB'e");

                            txTextOut(160, 110, "Там пару сотен");
                            txTextOut(160, 123, "багов");

                            txTextOut(8, 262, "Там еще почему-то");
                            txTextOut(8, 275, "ничего не работает");


                            txTextOut(160, 262, "Ну нахер, ли-");
                            txTextOut(160, 275, "ваю с ILAB'a");

                            break;
                        }
                    }

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

    free(OP);
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





