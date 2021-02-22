#include "assem.h"

static int NEW_COMMAND_ERROR       = 0;   // Если была обнаружена новая команда, то завершаем шарманку
static int IS_LAST_COMMAND_PUSH    = 0;   // Для проверки на неопознанную команду
static int IS_LAST_COMMAND_JMP     = 0;   // если был jmp или условные переход или выхов функции
//static int END_STATE               = 0;



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

    while(TRUE)
    {
        int iter_length = 0;

        while(isspace(text_struct->data[length])) // !!! это не равно следующему:
            length++;                        // while(isspace(text_struct->data[length++]));
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

    for(int j = 0; j < n_structs; j++)
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

    for(int x = 0; x < text_struct->n_struct; x++)
    {
        fprintf(info, "lines[%d]. [", x + 1);

        for(int y = 0; y < text_struct->lines[x].length; y++)
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

    for(int i = 0; i < text_struct->n_struct; i++)
    {
        for(int k = 0; k < text_struct->lines[i].length; k++)
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

    for(int x = 0; x < text_struct->n_struct; x++)
    {
        for(int y = 0; y < text_struct->lines[x].length; y++)
        {
            code_struct->data[cur_size++] = text_struct->lines[x].line[y];
            if (isspace(text_struct->lines[x].line[y]))
                while(isspace(text_struct->lines[x].line[y + 1]))
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
            while(isspace(code_struct->data[i]))
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

    assert(code_struct);
    assert(byte_struct);

    byte_struct->data = (double*) calloc(code_struct->terms, sizeof(double));
    assert(byte_struct->data);

    double* numbers_flag = (double*) calloc(code_struct->terms, sizeof(double));
    assert(numbers_flag);

    int flags_size     = 0;
    int cur_code_size  = 0;

    for (int i = 0; i < code_struct->terms; i++)
    {
        if(NEW_COMMAND_ERROR)
            break;
        char* temp = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char));

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
            byte_struct->data[i]           = 1;
            IS_LAST_COMMAND_PUSH     = 1;
        }
        else if (!strcmp(temp, "add"))
            byte_struct->data[i] = 2;
        else if (!strcmp(temp, "mul"))
            byte_struct->data[i] = 3;
        else if (!strcmp(temp, "div"))
            byte_struct->data[i] = 4;
        else if (!strcmp(temp, "sub"))
            byte_struct->data[i] = 5;
        else if (!strcmp(temp, "sin"))
            byte_struct->data[i] = 6;
        else if (!strcmp(temp, "cos"))
            byte_struct->data[i] = 7;
        else if (!strcmp(temp, "pow"))
            byte_struct->data[i] = 8;
        else if (!strcmp(temp, "sqr"))
            byte_struct->data[i] = 9;
        else if (!strcmp(temp, "in"))
            byte_struct->data[i] = 10;
        else if (!strcmp(temp, "out"))
            byte_struct->data[i] = 11;
        else if (!strcmp(temp, "del"))
            byte_struct->data[i] = 12;
        else if (!strcmp(temp, "ln"))
            byte_struct->data[i] = 13;
        else if (!strcmp(temp, "log10"))
            byte_struct->data[i] = 14;
        else if (!strcmp(temp, "quadratic"))
            byte_struct->data[i] = 15;
        else if (!strcmp(temp, "log2"))
            byte_struct->data[i] = 16;
        else if (!strcmp(temp, "rax"))
        {
            byte_struct->data[i]     = 17;
            numbers_flag[flags_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "rbx"))
        {
            byte_struct->data[i]           = 18;
            numbers_flag[flags_size++] = 2;
            IS_LAST_COMMAND_PUSH = 0;

        }
        else if (!strcmp(temp, "rcx"))
        {
            byte_struct->data[i]           = 19;
            numbers_flag[flags_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "rdx"))
        {
            byte_struct->data[i]           = 20;
            numbers_flag[flags_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "pop"))
            byte_struct->data[i] = 21;
        else if (!strcmp(temp, "je"))
        {
            byte_struct->data[i]      = 24;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jab"))
        {
            byte_struct->data[i]      = 25;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jae"))
        {
            byte_struct->data[i]      = 26;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jbe"))
        {
            byte_struct->data[i]      = 27;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "ja"))
        {
            byte_struct->data[i]      = 28;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jb"))
        {
            byte_struct->data[i]      = 29;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "hlt"))
        {
            byte_struct->data[i] = 0;
            //END_STATE = 1;
        }

        else if (IS_LAST_COMMAND_PUSH)
        {
            byte_struct->data[i]           = (double) std::atof(temp);
            numbers_flag[flags_size++] = 1;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (temp[strlen(temp) - 1] == ':')
            byte_struct->data[i] = 22;
        else if (!strcmp(temp, "ret"))
            byte_struct->data[i] = 30;
        else if (!strcmp(temp, "call"))
        {
            byte_struct->data[i] = 31;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jmp"))
        {
            byte_struct->data[i]      = 23;
            IS_LAST_COMMAND_JMP = 1;

        }
        else if (IS_LAST_COMMAND_JMP)
        {
            int temp_cur_code_size = 0;
            int correction         = 0;

            for (int index = 0; index < code_struct->terms; index++)
            {
                char* temp_str = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char));
                assert(temp_str);

                int iter = 0;
                for (iter = 0; iter < MAX_SIZE_COMMAND; iter++)
                {
                    if(code_struct->data[iter + temp_cur_code_size] != ' ')
                        temp_str[iter] = code_struct->data[iter + temp_cur_code_size];
                    else
                    {
                        temp_str[iter] = '\0';
                        break;
                    }
                }

                if (!strcmp(temp_str, "push"))
                    correction++;
                else if (!strcmp(temp_str, "pop"))
                    correction++;
                else if (temp_str[strlen(temp) - 1] == ':')
                {
                    int ERROR_LABEL = 0;
                    for (int iter_2 = 0; iter_2 < (int)(strlen(temp) - 1); iter_2++)
                        if (temp_str[iter_2] != temp[iter_2 + 1])
                        {
                            //printf("ERROR_LABEL. i = %d\n", i);
                            ERROR_LABEL = 1;
                            break;
                        }

                    if((!ERROR_LABEL) && (temp_str[strlen(temp) - 1] == temp[0]))
                    {
                        //printf("i = %d, GOOD\n", i);
                        //printf("correction is %d, index is %d\n", correction, index);
                        //printf("LABEL = %d\n", index + correction);
                        byte_struct->data[i] = index + correction;     // index теперь смотрит на ... 22 ..., В CPU будет после итерации на след. действие -- GOOD.
                        free(temp_str);
                        break;
                    }
                }

                free(temp_str);
                temp_cur_code_size += iter + 1;
            }

            IS_LAST_COMMAND_JMP = 0;
        }
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
        free(temp);
    }



    if(NEW_COMMAND_ERROR)
        return;


    FILE* assembler_txt = fopen("[!]assembler_code.txt", "w");
    assert(assembler_txt);
    flags_size = 0;

    for(int i = 0; i < byte_struct->bytecode_capacity; i++)
    {
        if(((int)byte_struct->data[i] == 1) || ((int)byte_struct->data[i] == 21))
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

    while((text_struct->data[*file_length] = fgetc(user_code)) && (!feof(user_code)))
    {
        if(text_struct->data[*file_length] == '\n')
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
