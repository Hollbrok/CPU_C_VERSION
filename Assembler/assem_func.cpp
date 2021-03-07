#include "assem.h"

static int NEW_COMMAND_ERROR       = 0;   // Если была обнаружена новая команда, то завершаем шарманку
static int IS_LAST_COMMAND_PUSH    = 0;   // Для проверки на неопознанную команду
static int IS_LAST_COMMAND_JMP     = 0;   // если был jmp или условные переход или выхов функции
//static int END_STATE             = 0;



auto text_construct(Text* text_struct, FILE* user_code) -> void
{

    long file_length = size_of_file(user_code);

    text_struct->data = (char*) calloc(file_length, sizeof(char));
    assert(text_struct->data);

    int file_lines = 0;
    file_length    = 0;

    useful_sizes(user_code, text_struct, &file_lines, &file_length);

    text_struct->lines = (Line*) calloc(file_lines + 1, sizeof(Line));
    assert(text_struct->lines);

    Line* lines = (Line*) calloc(file_lines + 1, sizeof(Line));
    assert(lines);


    long length  = 0;
    long n_structs = 0;

    while (TRUE)
    {
        int iter_length = 0;

        while (isspace(text_struct->data[length]))   // !!! это не равно следующему:
            length++;                               // while(isspace(text_struct->data[length++]));
                                                    // с точностью до единицы !!!

        if(length >= file_length)
            break;

        lines[n_structs].line   = &(text_struct->data)[length];  // length = кол-ву символом разделителей
        //lines[n_structs].length = length;                 // -//-

        while ((text_struct->data[length] != '\n') && (length < file_length))
        {
            //printf("length = %d\n", length);
            //printf("string = %d, length = %d\n", n_structs, length);
            if ((text_struct->data[length] == ';'))
            {
                while (text_struct->data[length] != '\n')
                    length++;
                //printf("BREAK\n");
                break;
            }
            length++;
            iter_length++;
        }
        //if()
        //printf("AFTER WHILE\n");

        //length++; // мб убрать
        //iter_length++;

        lines[n_structs].length = iter_length;
        while (isspace(lines[n_structs].line[lines[n_structs].length - 1]))
                (lines[n_structs].length)--;

        n_structs++;
    }

    for (int j = 0; j < n_structs; j++)
    {
        text_struct->lines[j].line   = lines[j].line;
        text_struct->lines[j].length = lines[j].length;
    }

    text_struct->n_struct    = n_structs;
    text_struct->length_file = file_length;

    free(lines);

    FILE* info = fopen("[!]info.txt", "w");
    assert(info);

    fprintf(info, "Number of lines      = %d\n", text_struct->n_struct);
    fprintf(info, "Length of the file   = %d\n", text_struct->length_file);

    for (int x = 0; x < text_struct->n_struct; x++)
    {
        fprintf(info, "lines[%d]. [", x + 1);

        for (int y = 0; y < text_struct->lines[x].length; y++)
            fprintf(info, "%c", text_struct->lines[x].line[y]);

        fprintf(info, "]\n");
        fprintf(info, "\tlength = %ld\n\n", text_struct->lines[x].length);
    }

    //print_text_struct(text_struct);        // NEED FOR DEBUG
    fclose(info);
}

auto text_destruct(Text* text_struct) -> void  // FOR DEBUG
{
    free(text_struct->data);
    text_struct->data = nullptr;

    free(text_struct->lines);
    text_struct->lines = nullptr;
}

auto print_text_struct(Text* text_struct) -> void // FOR DEBUG
{
    assert(text_struct);

    FILE* res = fopen("[!]string_text.txt", "w");

    for (int i = 0; i < text_struct->n_struct; i++)
    {
        for (int k = 0; k < text_struct->lines[i].length; k++)
            fprintf(res, "%c", text_struct->lines[i].line[k]);
        fprintf(res, " ");
    }

    fclose(res);
}

auto code_construct(Text* text_struct, Code* code_struct) -> void
{
    assert(text_struct);
    assert(code_struct);

    // Тут обрабатывать комментарии
    code_struct->length = text_struct->length_file + 3; // не надо + 3

    code_struct->data = (char*) calloc(code_struct->length, sizeof(char));
    assert(code_struct->data);

    int cur_size = 0;
    //printf("n_struct = %d\n", text_struct->n_struct);

    for (int x = 0; x < text_struct->n_struct; x++)
    {
        for (int y = 0; y < text_struct->lines[x].length; y++)
        {
            code_struct->data[cur_size++] = text_struct->lines[x].line[y];
            if (isspace(text_struct->lines[x].line[y]))
                while (isspace(text_struct->lines[x].line[y + 1]))
                    y++;
        }

        if ((cur_size < code_struct->length + 1))
            code_struct->data[cur_size++] = ' ';
    }

    code_struct->data[cur_size] = '\0';
    int terms = 0;
    code_struct->terms = terms;

    for (int i = 0; i < code_struct->length; i++)
        if (code_struct->data[i] == ' ')
        {
            code_struct->terms++;
            while (isspace(code_struct->data[i]))
                i++;
        }
    //printf("terms = %d\n", code_struct->terms);
    print_code_buffer(code_struct);
    return;
}

auto code_destruct(Code* code_struct) -> void
{
    assert(code_struct);

    free(code_struct->data);
    code_struct->data = nullptr;

}

auto print_code_buffer(Code* code_struct) -> void
{
    assert(code_struct);
    FILE* code_text = fopen("[!]code_struct.txt", "w");

    fwrite(code_struct->data, sizeof(char), strlen(code_struct->data), code_text);

    return;
}

auto get_bytecode(Code* code_struct, Bytecode* byte_struct) -> void
{
    // На этом этапе я уже гарантирую отсутствие комментариев и лишних разделительных
    // символов, поэтому дополнительная обработка не нужна
    //using namespace Commands;

    assert(code_struct);
    assert(byte_struct);

    Label* labels = (Label*) calloc(MAX_LABELS + 1, sizeof(Label));
    assert(labels);

    for (int y = 0; y < MAX_LABELS; y++)
    {
        labels[y].name = (char*) calloc(MAX_LABEL_SIZE, sizeof(char));
        assert(labels[y].name);
    }

    int cur_labels = 0;
    int correction = 0;
    int temp_cur_code_size = 0;

    char* temp = (char*) calloc(MAX_SIZE_COMMAND, sizeof(char));

    for (int i = 0; i < code_struct->terms; i++)
    {
        int iter = 0;
        for (iter = 0; iter < MAX_SIZE_COMMAND; iter++) // теперь в temp_str хранится лексема
        {
            if(code_struct->data[iter + temp_cur_code_size] != ' ')
                temp[iter] = code_struct->data[iter + temp_cur_code_size];
            else
            {
                temp[iter] = '\0';
                break;
            }
        }

        if (!strcmp(temp, "push"))
                correction++;
        else if (!strcmp(temp, "pop"))
                correction++;
        else if (temp[strlen(temp) - 1] == ':')                         // нашли метку
        {
            //printf("GOOD FIND\n");
            strncpy(labels[cur_labels].name, temp, strlen(temp) - 1);                               // поместили указатель

            labels[cur_labels].length = strlen(temp);                       // запомнили длину (в будущем можно будет отсортировать по длинам, чтобы
                                                                                // ускорить процесс обнаржуения меток при ассемблировании) -- эффект. для большого кода
            labels[cur_labels].adress = i + correction;
            cur_labels++;
        }

        temp_cur_code_size += iter + 1;
    }

    int amount_labels = cur_labels;
    cur_labels = 0;



    //for(int iter = 0; iter < amount_labels; iter++)
    //    printf("[%d][%s]\nadress = %d\n", iter, labels[iter].name, labels[iter].adress);


    byte_struct->data = (double*) calloc(code_struct->terms, sizeof(double));
    assert(byte_struct->data);

    double* numbers_flag = (double*) calloc(code_struct->terms, sizeof(double));
    assert(numbers_flag);

    int flags_size     = 0;
    int cur_code_size  = 0;

    using namespace my_commands;

    for (int i = 0; i < code_struct->terms; i++)
    {
        if(NEW_COMMAND_ERROR)
            break;
        //char* temp = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char));

        int j = 0;
        for (j = 0; j < MAX_SIZE_COMMAND; j++)
        {
            if((code_struct->data[j + cur_code_size] != ' '))
                temp[j] = code_struct->data[j + cur_code_size];
            else
            {
                temp[j] = '\0';
                break;
            }
        }

        cur_code_size += j + 1;

        if (!strcmp(temp, "push"))
        {
            byte_struct->data[i] = 1;
            IS_LAST_COMMAND_PUSH = 1;
        }
        else if (!strcmp(temp, "add"))
            byte_struct->data[i] = com_to_int(Commands::CMD_ADD);
        else if (!strcmp(temp, "mul"))
            byte_struct->data[i] = com_to_int(Commands::CMD_MUL);
        else if (!strcmp(temp, "div"))
            byte_struct->data[i] = com_to_int(Commands::CMD_DIV);
        else if (!strcmp(temp, "sub"))
            byte_struct->data[i] = com_to_int(Commands::CMD_SUB);
        else if (!strcmp(temp, "sin"))
            byte_struct->data[i] = com_to_int(Commands::CMD_SIN);
        else if (!strcmp(temp, "cos"))
            byte_struct->data[i] = com_to_int(Commands::CMD_COS);
        else if (!strcmp(temp, "pow"))
            byte_struct->data[i] = com_to_int(Commands::CMD_POW);
        else if (!strcmp(temp, "sqr"))
            byte_struct->data[i] = com_to_int(Commands::CMD_SQRT);
        else if (!strcmp(temp, "in"))
            byte_struct->data[i] = com_to_int(Commands::CMD_IN);
        else if (!strcmp(temp, "out"))
            byte_struct->data[i] = com_to_int(Commands::CMD_OUT);
        else if (!strcmp(temp, "del"))
            byte_struct->data[i] = com_to_int(Commands::CMD_DEL);
        else if (!strcmp(temp, "abs"))
            byte_struct->data[i] = com_to_int(Commands::CMD_ABS);
        else if (!strcmp(temp, "circ"))
            byte_struct->data[i] = com_to_int(Commands::CMD_CIRC);
        else if (!strcmp(temp, "cat"))
            byte_struct->data[i] = com_to_int(Commands::CMD_CAT);
        else if (!strcmp(temp, "KOPM"))
            byte_struct->data[i] = com_to_int(Commands::CMD_KOPM);
        else if (!strcmp(temp, "ln"))
            byte_struct->data[i] = com_to_int(Commands::CMD_LN);
        else if (!strcmp(temp, "mem"))
            byte_struct->data[i] = com_to_int(Commands::CMD_MEM);
        else if (!strcmp(temp, "log10"))
            byte_struct->data[i] = com_to_int(Commands::CMD_LOG10);
        else if (!strcmp(temp, "quadratic"))
            byte_struct->data[i] = 15;
        else if (!strcmp(temp, "log2"))
            byte_struct->data[i] = com_to_int(Commands::CMD_LOG2);
        else if (!strcmp(temp, "rax"))
        {
            if(temp[0] == '[')
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RAX);
                numbers_flag[flags_size++] = OP_REGIST;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RAX);
                numbers_flag[flags_size++] = S_REGIST_SPEC;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
        }
        else if (!strcmp(temp, "rbx"))
        {
            if(temp[0] == '[')
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RBX);
                numbers_flag[flags_size++] = OP_REGIST;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RBX);
                numbers_flag[flags_size++] = S_REGIST_SPEC;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
        }
        else if (!strcmp(temp, "rcx"))
        {
            if(temp[0] == '[')
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RCX);
                numbers_flag[flags_size++] = OP_REGIST;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RCX);
                numbers_flag[flags_size++] = S_REGIST_SPEC;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
        }
        else if (!strcmp(temp, "rdx"))
        {
            if(temp[0] == '[')
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RDX);
                numbers_flag[flags_size++] = OP_REGIST;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else
            {
                byte_struct->data[i]       = com_to_int(Commands::CMD_RDX);
                numbers_flag[flags_size++] = S_REGIST_SPEC;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
        }
        else if (!strcmp(temp, "pop"))
        {
            byte_struct->data[i]     = com_to_int(Commands::CMD_POP);
            numbers_flag[flags_size] = S_NUMBER_SPEC;
            //IS_LAST_COMMAND_POP      = true;
        }
        else if (!strcmp(temp, "je"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JE);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "jab"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JAB);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "jae"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JAE);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "jbe"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JBE);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "ja"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JA);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "jb"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JB);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "hlt"))
            byte_struct->data[i] = com_to_int(Commands::CMD_HLT);
        else if(temp[0] == '[')
        {
            if(isdigit(*(temp + 1)))
            {
                byte_struct->data[i] = std::atoi(temp + 1);
                printf("adress = %lg\n", byte_struct->data[i]);
                numbers_flag[flags_size++] = OP_NUMBER;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else if(strlen(temp) == 5)
            {
                char new_temp[] = {temp[1], temp[2], temp[3]}; // так как длина 5, то [xxx], то в new_temp теперь просто xxx
                if (!strncmp(new_temp, "rax", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RAX);
                    numbers_flag[flags_size++] = OP_REGIST;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rbx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RBX);
                    numbers_flag[flags_size++] = OP_REGIST;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rcx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RCX);
                    numbers_flag[flags_size++] = OP_REGIST;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rdx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RDX);
                    numbers_flag[flags_size++] = OP_REGIST;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
            }
            else printf("BAD(((\n");
        }
        else if (temp[0] == '(')
        {
            if(isdigit(*(temp + 1)))
            {
                byte_struct->data[i] = std::atoi(temp + 1);
                printf("adress = %lg\n", byte_struct->data[i]);
                numbers_flag[flags_size++] = OP_CHAR_NUM;
                IS_LAST_COMMAND_PUSH       = FALSE;
            }
            else if(strlen(temp) == 5)
            {
                char new_temp[] = {temp[1], temp[2], temp[3]}; // так как длина 5, то [xxx], то в new_temp теперь просто xxx
                if (!strncmp(new_temp, "rax", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RAX);
                    numbers_flag[flags_size++] = OP_CHAR_REG;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rbx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RBX);
                    numbers_flag[flags_size++] = OP_CHAR_REG;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rcx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RCX);
                    numbers_flag[flags_size++] = OP_CHAR_REG;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
                else if (!strncmp(new_temp, "rdx", 3))
                {
                    byte_struct->data[i]       = com_to_int(Commands::CMD_RDX);
                    numbers_flag[flags_size++] = OP_CHAR_REG;
                    IS_LAST_COMMAND_PUSH       = FALSE;
                }
            }
            else printf("BAD(((\n");
        }
        else if (IS_LAST_COMMAND_PUSH)
        {
            byte_struct->data[i]       = (double) std::atof(temp);
            numbers_flag[flags_size++] = S_NUMBER_SPEC;
            IS_LAST_COMMAND_PUSH       = FALSE;
        }
        else if (temp[strlen(temp) - 1] == ':')
            byte_struct->data[i] = com_to_int(Commands::CMD_LABEL);
        else if (!strcmp(temp, "ret"))
            byte_struct->data[i] = com_to_int(Commands::CMD_RET);
        else if (!strcmp(temp, "call"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_CALL);
            IS_LAST_COMMAND_JMP  = TRUE;
        }
        else if (!strcmp(temp, "jmp"))
        {
            byte_struct->data[i] = com_to_int(Commands::CMD_JMP);
            IS_LAST_COMMAND_JMP  = TRUE;

        }
        else if (IS_LAST_COMMAND_JMP)
        {
            temp++;
            for (int index = 0; index < amount_labels; index++)
                if(!strcmp(temp, labels[index].name))
                {
                    byte_struct->data[i] = labels[index].adress;
                    break;
                }
            temp--;
            IS_LAST_COMMAND_JMP = 0;
        }
        else if (!strcmp(temp, "draw"))
            byte_struct->data[i] = com_to_int(Commands::CMD_DRAW);
        else if (!strcmp(temp, "fill"))
            byte_struct->data[i] = com_to_int(Commands::CMD_FILL);
        else
        {
            FILE* error = fopen("[!]ERRORS.txt", "ab");
            fprintf(error, "\n\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
            fprintf(error, "Assembler doesn't know this command..\n");
            fprintf(error, "bytecode[%d] = [%s]\n", i, temp);
            fclose(error);
            NEW_COMMAND_ERROR = 1;
            free(temp);
            break;
        }
        byte_struct->bytecode_capacity++;
    }


    if(NEW_COMMAND_ERROR)
        return;


    FILE* assembler_txt = fopen("[!]assembler_code.txt", "w");
    assert(assembler_txt);
    flags_size = 0;

    for (int i = 0; i < byte_struct->bytecode_capacity; i++)
    {
        if ((static_cast<int>(byte_struct->data[i]) == 1) || (static_cast<int>(byte_struct->data[i]) == 21))
        {
            fprintf(assembler_txt, "%lg ", byte_struct->data[i]);
            fprintf(assembler_txt, "%lg ", numbers_flag[flags_size++]);
            i++;
            fprintf(assembler_txt, "%lg ", byte_struct->data[i]);
        }
        else
            fprintf(assembler_txt, "%lg ", byte_struct->data[i]);
    }

    free(numbers_flag);

    for (int y = 0; y < amount_labels; y++)
        if (labels[y].name)
            free(labels[y].name);

    free(labels);
    free(temp);
    fclose(assembler_txt);
    return;
}

auto bytecode_destruct(Bytecode* byte_struct) -> void
{
    assert(byte_struct);

    free(byte_struct->data);
    byte_struct->data = nullptr;

    byte_struct->bytecode_capacity = 0;
}

auto useful_sizes(FILE* user_code, Text* text_struct, int* file_lines, long* file_length) -> void
{
    assert(user_code);
    assert(text_struct);
    assert(file_lines);
    assert(file_length);

    while ((text_struct->data[*file_length] = fgetc(user_code)) && (!feof(user_code)))
    {
        if (text_struct->data[*file_length] == '\n')
            (*file_lines)++;
        (*file_length)++;
    }
}

auto set_time(struct tm *time) -> char*
{
    char string_time[size_time] = {};
    char *tmp                   = nullptr;

    strftime(string_time, size_time, "%d.%m.%Y %H:%M:%S, %A", time);

    tmp = (char*) malloc(sizeof(string_time));
    strcpy(tmp, string_time);

    return tmp;
}

auto define_date(void) -> char*
{
    const time_t timer    = time(nullptr);
    struct tm* local_time = localtime(&timer);

    return set_time(local_time);
}

auto size_of_file(FILE* user_code) -> long
{
    assert(user_code);

    fseek(user_code, 0, SEEK_END);
    long file_length = ftell(user_code);
    fseek(user_code, 0, SEEK_SET);
    file_length++;

    return file_length;
}

auto com_to_int(my_commands::Commands command) -> int
{
    return static_cast<int>(command);
}
