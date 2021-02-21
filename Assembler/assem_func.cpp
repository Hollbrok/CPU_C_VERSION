#include "assem.h"

static int NEW_COMMAND_ERROR       = 0;   // Если была обнаружена новая команда, то завершаем шарманку
static int IS_LAST_COMMAND_PUSH    = 0;   // Для проверки на неопознанную команду
static int IS_LAST_COMMAND_JMP     = 0;   // если был jmp или условные переход или выхов функции
static int END_STATE               = 0;



void text_construct(text_t* text_s, FILE* text)
{

    long file_length = size_of_file(text);

    text_s->data = (char*) calloc(file_length, sizeof(char));
    assert(text_s->data);

    int file_lines = 0;
    file_length    = 0;

    useful_sizes(text, text_s, &file_lines, &file_length);

    text_s->lines = (line_t*) calloc(file_lines + 1, sizeof(line_t));
    assert(text_s->lines);

    line_t* lines = (line_t*) calloc(file_lines + 1, sizeof(line_t));
    assert(lines);


    long length  = 0;
    long n_structs = 0;

    while(TRUE)
    {
        int iter_length = 0;

        while(isspace(text_s->data[length])) // !!! это не равно следующему:
            length++;                        // while(isspace(text_s->data[length++]));
                                             // с точностью до единицы !!!

        if(length >= file_length)
            break;

        lines[n_structs].line   = &(text_s->data)[length];  // length = кол-ву символом разделителей
        //lines[n_structs].length = length;                 // -//-

        while ((text_s->data[length] != '\n') && (length < file_length))
        {
            //printf("length = %d\n", length);
            //printf("string = %d, length = %d\n", n_structs, length);
            if ((text_s->data[length] == ';'))
            {
                while (text_s->data[length] != '\n')
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
        text_s->lines[j].line   = lines[j].line;
        text_s->lines[j].length = lines[j].length;
    }

    text_s->n_struct    = n_structs;
    text_s->length_file = file_length;

    free(lines);

    FILE* test = fopen("[!]info.txt", "w");
    assert(test);

    fprintf(test, "Number of lines      = %d\n", text_s->n_struct);
    fprintf(test, "Length of the file   = %d\n", text_s->length_file);

    for(int x = 0; x < text_s->n_struct; x++)
    {
        fprintf(test, "lines[%d]. [", x + 1);

        for(int y = 0; y < text_s->lines[x].length; y++)
            fprintf(test, "%c", text_s->lines[x].line[y]);

        fprintf(test, "]\n");
        fprintf(test, "\tlength = %ld\n\n", text_s->lines[x].length);
    }

    //printf_text_s(text_s);        // NEED FOR DEBUG
    fclose(test);
}

void text_destruct(text_t* text_s)  // FOR DEBUG
{
    free(text_s->data);
    text_s->data = nullptr;

    free(text_s->lines);
    text_s->lines = nullptr;
}

void code_construct(text_t* text_s, code_t* code_s)
{
    assert(text_s);
    assert(code_s);

    // Тут обрабатывать комментарии
    code_s->length = text_s->length_file + 3; // не надо + 3

    code_s->data = (char*) calloc(code_s->length, sizeof(char));
    assert(code_s->data);

    int cur_size = 0;
    //printf("n_struct = %d\n", text_s->n_struct);

    for(int x = 0; x < text_s->n_struct; x++)
    {
        for(int y = 0; y < text_s->lines[x].length; y++)
        {
            code_s->data[cur_size++] = text_s->lines[x].line[y];
            if (isspace(text_s->lines[x].line[y]))
                while(isspace(text_s->lines[x].line[y + 1]))
                    y++;
            /*if(x == text_s->n_struct - 1)
            {
                for(int temp = 0; temp < 3; temp++)
                    code_s->data[cur_size++] = text_s->lines[x].line[temp];
                break;
            }
            else
            {
                //if(text_s->lines[x].line[y])
                code_s->data[cur_size++] = text_s->lines[x].line[y];
            }   */

        }

        if((cur_size < code_s->length + 1))
            code_s->data[cur_size++] = ' ';
    }

    code_s->data[cur_size] = '\0';
    int terms = 0;
    code_s->terms = terms;

    for(int i = 0; i < code_s->length; i++)
        if(code_s->data[i] == ' ')
        {
            code_s->terms++;
            while(isspace(code_s->data[i]))
                i++;
        }
    //printf("terms = %d\n", code_s->terms);
    print_code_buffer(code_s);
    return;
}

void print_code_buffer(code_t* code_s)
{
    assert(code_s);
    FILE* code_text = fopen("[!]code_struct.txt", "w");

    fwrite(code_s->data, sizeof(char), strlen(code_s->data), code_text);

    return;
}

void get_ass_code(code_t* code_s, ass_code* ass_s)
{
    // На этом этапе я уже гарантирую отсутствие комментариев и лишних разделительных
    // символов, поэтому дополнительная обработка не нужна
    assert(code_s);
    assert(ass_s);

    ass_s->data      = (double*) calloc(code_s->terms, sizeof(double));
    assert(ass_s->data);

    double* rix_call = (double*) calloc(code_s->terms, sizeof(double));
    assert(rix_call);

    int rix_cur_size   = 0;
    int cur_code_size  = 0;



    //printf("code_s->terms = %d\n", code_s->terms);
    for (int i = 0; i < code_s->terms; i++)
    {
        if(NEW_COMMAND_ERROR)
            break;
        char* temp = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char));

        /*while(isspace(code_s->data[cur_code_size]))
        {
            if (code_s->data[cur_code_size] == '\0')
            {
                break;
                END_STATE = 1;
            }
            cur_code_size++;
        }
        if(END_STATE)
        {
            printf("END_STATE, break\n");
            break;
        }*/
        /*if(code_s->data[cur_code_size] == ';')
        {                                            // Если комментрий,
            while((code_s->data[cur_code_size] != '\n') && (code_s->data[cur_code_size])) // до конца строки игнорим
                cur_code_size++;                                                          // текст
            continue;
        } */

        //if(code_s->data[cur_code_size] == '\0')
            //break;
        int j = 0;

        for (j = 0; j < MAX_SIZE_COMMAND; j++)
        {

            if((code_s->data[j + cur_code_size] != ' '))
                temp[j] = code_s->data[j + cur_code_size];
            else
            {
                temp[j] = '\0';
                break;
            }
        }
        //printf("[");
        //for (int index = 0; temp[index]; index++)
            //printf("%c", temp[index]);
        //printf("]\n");

        cur_code_size += j + 1;

        if (!strcmp(temp, "push"))
        {
            ass_s->data[i]           = 1;
            IS_LAST_COMMAND_PUSH     = 1;
        }
        else if (!strcmp(temp, "add"))
            ass_s->data[i] = 2;
        else if (!strcmp(temp, "mul"))
            ass_s->data[i] = 3;
        else if (!strcmp(temp, "div"))
            ass_s->data[i] = 4;
        else if (!strcmp(temp, "sub"))
            ass_s->data[i] = 5;
        else if (!strcmp(temp, "sin"))
            ass_s->data[i] = 6;
        else if (!strcmp(temp, "cos"))
            ass_s->data[i] = 7;
        else if (!strcmp(temp, "pow"))
            ass_s->data[i] = 8;
        else if (!strcmp(temp, "sqr"))
            ass_s->data[i] = 9;
        else if (!strcmp(temp, "in"))
            ass_s->data[i] = 10;
        else if (!strcmp(temp, "out"))
            ass_s->data[i] = 11;
        else if (!strcmp(temp, "del"))
            ass_s->data[i] = 12;
        else if (!strcmp(temp, "ln"))
            ass_s->data[i] = 13;
        else if (!strcmp(temp, "log10"))
            ass_s->data[i] = 14;
        else if (!strcmp(temp, "quadratic"))
            ass_s->data[i] = 15;
        else if (!strcmp(temp, "log2"))
            ass_s->data[i] = 16;
        else if (!strcmp(temp, "rax"))
        {
            ass_s->data[i]           = 17;
            rix_call[rix_cur_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "rbx"))
        {
            ass_s->data[i]           = 18;
            rix_call[rix_cur_size++] = 2;
            IS_LAST_COMMAND_PUSH = 0;

        }
        else if (!strcmp(temp, "rcx"))
        {
            ass_s->data[i]           = 19;
            rix_call[rix_cur_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "rdx"))
        {
            ass_s->data[i]           = 20;
            rix_call[rix_cur_size++] = 2;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (!strcmp(temp, "pop"))
            ass_s->data[i] = 21;
        else if (!strcmp(temp, "je"))
        {
            ass_s->data[i]      = 24;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jab"))
        {
            ass_s->data[i]      = 25;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jae"))
        {
            ass_s->data[i]      = 26;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jbe"))
        {
            ass_s->data[i]      = 27;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "ja"))
        {
            ass_s->data[i]      = 28;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jb"))
        {
            ass_s->data[i]      = 29;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "hlt"))
        {
            ass_s->data[i] = 0;
            //END_STATE = 1;
        }

        else if (IS_LAST_COMMAND_PUSH)
        {
            ass_s->data[i]           = (double) std::atof(temp);
            rix_call[rix_cur_size++] = 1;
            IS_LAST_COMMAND_PUSH     = 0;
        }
        else if (temp[strlen(temp) - 1] == ':')
            ass_s->data[i] = 22;
        else if (!strcmp(temp, "ret"))
            ass_s->data[i] = 30;
        else if (!strcmp(temp, "call"))
        {
            ass_s->data[i] = 31;
            IS_LAST_COMMAND_JMP = 1;
        }
        else if (!strcmp(temp, "jmp"))
        {
            ass_s->data[i]      = 23;
            IS_LAST_COMMAND_JMP = 1;

        }
        else if (IS_LAST_COMMAND_JMP)
        {
            int temp_cur_code_size = 0;
            int correction         = 0;

            for (int index = 0; index < code_s->terms; index++)
            {
                char* temp_str = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char));
                assert(temp_str);

                int iter = 0;

                /*while(isspace(code_s->data[temp_cur_code_size]))
                {
                    if (code_s->data[temp_cur_code_size] == '\0')
                    {
                        break;
                        END_STATE = 1;
                    }
                    temp_cur_code_size++;
                }
                if(END_STATE)
                {
                    END_STATE = 0;
                    break;
                }*/

                for (iter = 0; iter < MAX_SIZE_COMMAND; iter++)
                {
                    if(code_s->data[iter + temp_cur_code_size] != ' ')
                        temp_str[iter] = code_s->data[iter + temp_cur_code_size];
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
                        ass_s->data[i] = index + correction;     // index теперь смотрит на ... 22 ..., В CPU будет после итерации на след. действие -- GOOD.
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
            //printf("ERRORERRORERRORERRORERRORERROR\n");
            //printf("i = %d\n[", i);
            //for(int index = 0; temp[index]; index++)
               // printf("%c", temp[index]);
            //printf("]\n");

            FILE* error = fopen("[!]ERRORS.txt", "ab");
            fprintf(error, "\n\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
            fprintf(error, "Unknown command..\n");
            fprintf(error, "assembler_code[%d] = [%s]\n", i, temp);
            fclose(error);
            NEW_COMMAND_ERROR = 1;
            free(temp);
            break;
        }
        ass_s->ass_size++;
        free(temp);
    }



    if(NEW_COMMAND_ERROR)
        return;


    FILE* assembler_txt = fopen("[!]assembler_code.txt", "w");
    rix_cur_size = 0;

    //printf("HERE size = %d", ass_s->ass_size);
    for(int i = 0; i < ass_s->ass_size; i++)
    {
        if(((int)ass_s->data[i] == 1) || ((int)ass_s->data[i] == 21))
        {
            fprintf(assembler_txt, "%lg ", ass_s->data[i]);
            fprintf(assembler_txt, "%lg ", rix_call[rix_cur_size++]);
            i++;
            fprintf(assembler_txt, "%lg ", ass_s->data[i]);
        }
        else
            fprintf(assembler_txt, "%lg ", ass_s->data[i]);
    }

    free(rix_call);
    fclose(assembler_txt);
    return;
}

long size_of_file(FILE* text)
{
    assert(text);

    fseek(text, 0, SEEK_END);
    long file_length = ftell(text);
    fseek(text, 0, SEEK_SET);
    file_length++;

    return file_length;
}

void useful_sizes(FILE* text, text_t* text_s, int* file_lines, long* file_length)
{
    assert(text);
    assert(text_s);
    assert(file_lines);
    assert(file_length);

    while((text_s->data[*file_length] = fgetc(text)) && (!feof(text)))
    {
        if(text_s->data[*file_length] == '\n')
            (*file_lines)++;
        (*file_length)++;
    }
}

void printf_text_s(text_t* text_s) // FOR DEBUG
{
    assert(text_s);

    FILE* res = fopen("[!]string_text.txt", "w");

    for(int i = 0; i < text_s->n_struct; i++)
    {
        for(int k = 0; k < text_s->lines[i].length; k++)
            fprintf(res, "%c", text_s->lines[i].line[k]);
        fprintf(res, " ");
    }

    fclose(res);
}

void code_destruct(code_t* code_s)
{
    assert(code_s);

    free(code_s->data);
    code_s->data = nullptr;

}

void ass_code_destruct(ass_code* ass_s)
{
    assert(ass_s);

    free(ass_s->data);
    ass_s->data = nullptr;

    ass_s->ass_size = 0;
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
