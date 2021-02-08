#include "assem.h"

int END_STATE               = 0;   // Статус выхода из ассемблирования кода
int EXIT_CONDITION          = 0;   // Статус выхода из перевода ассеблерного кода в действия
int PRINT_STATE             = 0;   // Отвечает за то, дополнять в print_for_user или записывать сначала в зависимости от того,
                                   // была ли вызвана дополнительная функция для печати

int NEW_COMMAND_ERROR       = 0;   // Если была обнаружена новая команда, то gg
int IS_LAST_COMMAND_PUSH    = 0;   // Для проверки на неопознанную команду


void text_construct(text_t* text_s, FILE* text)
{

    long file_length = size_of_file(text);

    text_s->data = (char*) calloc(file_length, sizeof(char));
    assert(text_s->data);

    //text_s->lines = (line_t*) calloc(file_length, sizeof(line_t)); //??
    //assert(text_s->lines);                                         // |
                                                                     // |
    int file_lines = 0;                                              // |
    file_length    = 0;                                              // |
                                                                     // |
    useful_sizes(text, text_s, &file_lines, &file_length);           // |
                                                                     // |
    text_s->lines = (line_t*) calloc(file_lines + 1, sizeof(line_t));   //<"
    assert(text_s->lines);

    line_t* lines = (line_t*) calloc(file_lines + 1, sizeof(line_t));
    assert(lines);


    long length = 0;
    long n_structs = 0;

    other_sizes(length, n_structs, file_length, text_s, lines);
    free(lines);

    FILE* test = fopen("[!]info.txt", "w");
    assert(test);

    fprintf(test, "Number of lines      = %d\n", text_s->n_struct);
    fprintf(test, "Length of the file   = %d\n", text_s->length_file);

    for(int x = 0; x < text_s->n_struct; x++)
    {
        fprintf(test, "lines[%d]. $", x + 1);

        for(int y = 0; y < text_s->lines[x].length - 1; y++)
            fprintf(test, "%c", text_s->lines[x].line[y]);

        fprintf(test, "$\n");
        fprintf(test, "\tlength = %ld\n\n", text_s->lines[x].length - 1);
    }

    printf_text_s(text_s);
    fclose(test);
}

void get_code(text_t* text_s, code_t* code_s)
{
    assert(text_s);
    assert(code_s);

    // Разбить функцию на две, то есть добавить еще assembler_code func
    // Через структуру ass_code + добавить (constructor and destructor)
    // Сделать через структуру code_t: добавить конструктор и деструктор в конце функции


    int size_of_code = text_s->length_file + 3;
    code_s->length = size_of_code;

    code_s->data = (char*) calloc(code_s->length, sizeof(char));
    assert(code_s->data);

    #ifdef DEBUG
        printf("\n\nAlloc size is (%d)\n\n", size_of_code);
    #endif

    int cur_size = 0;

    #ifdef DEBUG
        printf("\Strings = %d\n\n",text_s->n_struct);
    #endif


    for(int x = 0; x < text_s->n_struct; x++)
    {
        for(int y = 0; y < text_s->lines[x].length - 1; y++)
        {
            if(x == text_s->n_struct - 1)                       //lines[text_s->n_struct - 1].length)
            {
                for(int temp = 0; temp < 3; temp++)
                    code_s->data[cur_size++] = text_s->lines[x].line[temp];
                break;
            }

            else
                code_s->data[cur_size++] = text_s->lines[x].line[y];

        }

        if((cur_size < size_of_code + 1))
            code_s->data[cur_size++] = ' ';
    }

    code_s->data[cur_size] = '\0';
    int terms = 0;
    code_s->terms = terms;

    #ifdef DEBUG
        printf("size of code is = %d\n", size_of_code);
        printf("\"%s\"\n", code_s->data);
    #endif

    for(int i = 0; i < size_of_code; i++)
        if(code_s->data[i] == ' ')
            code_s->terms++;
    code_s->terms++;

    #ifdef DEBUG
        printf("terms = %d\n", code_s->terms);
    #endif

    //free(code_s->data) в деструктор запихнуть.
    return;
}

void get_ass_code(code_t* code_s, ass_code* ass_s)
{
    assert(code_s);
    assert(ass_s);

    ass_s->data      = (double*) calloc(code_s->terms + 2, sizeof(double));
    assert(ass_s->data);

    double* rix_call = (double*) calloc(code_s->terms + 2, sizeof(double));
    assert(rix_call);

    int rix_cur_size = 0;

    int assembler_size = 0;


    #ifdef DEBUG
        printf("TERMS = %d\n", code_s->terms);
        printf("\n\n\n");
    #endif

    int cur_code_size  = 0;



    for(int i = 0; i < code_s->terms; i++)
    {
        if(END_STATE)
            break;

        char* temp = (char*) calloc(MAX_SIZE_COMMAND + 1, sizeof(char)); //или сделать через calloc + free ???

        #ifdef DEBUG
            printf("Already in tmp ");
        #endif

        int j = 0;

        for(j; j < MAX_SIZE_COMMAND; j++)
        {
            if(code_s->data[j + cur_code_size] != ' ')
            {
                temp[j] = code_s->data[j + cur_code_size];
                //printf("_%c[%d]_", temp[j], j);
                #ifdef DEBUG
                    printf("_%c[%d]_", temp[j], j);
                #endif
            }

            else
            {
                temp[j] = '\0';
                break;
            }
        }

        cur_code_size += j + 1;

        #ifdef DEBUG
            printf("\nj = %d\n", j);
        #endif

        #ifdef DEBUG
            printf("temp[%d] = (%s)\n", i, temp);
            printf("\n\n");
        #endif

        if(!strcmp(temp, "push"))
        {
            ass_s->data[i]           = 1;
            //rix_call[rix_cur_size++] = 1;
            IS_LAST_COMMAND_PUSH     = 1;
        }
        else if(!strcmp(temp, "add"))
            ass_s->data[i] = 2;
        else if(!strcmp(temp, "mul"))
            ass_s->data[i] = 3;
        else if(!strcmp(temp, "div"))
            ass_s->data[i] = 4;
        else if(!strcmp(temp, "sub"))
            ass_s->data[i] = 5;
        else if(!strcmp(temp, "sin"))
            ass_s->data[i] = 6;
        else if(!strcmp(temp, "cos"))
            ass_s->data[i] = 7;
        else if(!strcmp(temp, "pow"))
            ass_s->data[i] = 8;
        else if(!strcmp(temp, "sqr"))
            ass_s->data[i] = 9;
        else if(!strcmp(temp, "in"))
            ass_s->data[i] = 10;
        else if(!strcmp(temp, "out"))
            ass_s->data[i] = 11;
        else if(!strcmp(temp, "del"))
            ass_s->data[i] = 12;
        else if(!strcmp(temp, "ln"))
            ass_s->data[i] = 13;
        else if(!strcmp(temp, "log10"))
            ass_s->data[i] = 14;
        else if(!strcmp(temp, "quadratic"))
            ass_s->data[i] = 15;
        else if(!strcmp(temp, "log2"))
            ass_s->data[i] = 16;
        else if(!strcmp(temp, "rax"))
        {
            ass_s->data[i]           = 17;
            rix_call[rix_cur_size++] = 2;
        }
        else if(!strcmp(temp, "rbx"))
        {
            //printf("ass_s->cur_rix = %d", ass_s->cur_rix);
            ass_s->data[i]           = 18;
            rix_call[rix_cur_size++] = 2;
            //printf("i = %d\n", i);
            //printf("ass_s->cur_rix = %d\n", ass_s->cur_rix);
            //printf("ass_s->rix_call[cur_rix = %d] = %d\n\n\n", ass_s->cur_rix, ass_s->rix_call[ass_s->cur_rix]);
            //ass_s->cur_rix++;

        }
        else if(!strcmp(temp, "rcx"))
        {
            ass_s->data[i]           = 19;
            rix_call[rix_cur_size++] = 2;
        }
        else if(!strcmp(temp, "rdx"))
        {
            ass_s->data[i]           = 20;
            rix_call[rix_cur_size++] = 2;
        }
        else if(!strcmp(temp, "pop"))
            ass_s->data[i] = 21;
        else if(!strcmp(temp, "hlt"))
        {
            ass_s->data[i] = 0;
            #ifdef DEBUG
                printf("Команда номер %d распозналась как выход, то есть = 0\n", i + 1);
            #endif
            END_STATE = 1;
        }

        else
        {
            #ifdef DEBUG
                printf("i = %d. _%s_\n", i, temp);
            #endif

            if(IS_LAST_COMMAND_PUSH)
            {
                ass_s->data[i]    = (double) std::atof(temp);
                rix_call[rix_cur_size++] = 1;
                IS_LAST_COMMAND_PUSH = 0;
            }
            else
            {
                FILE* error = fopen("[!]ERRORS.txt", "ab");
                fprintf(error, "\n\tДата error'a : %s (чч/мм/гг)\n\n", define_date());
                fprintf(error, "Unknown command..\n");
                fprintf(error, "assembler_code[%d] = %s\n", i, temp);
                fclose(error);
                NEW_COMMAND_ERROR = 1;
                break;//хз, мб убрать break?? по сути ни на что не влияет, так как дальше стоит
                      //проверка на NEW_COMMAND_ERROR, но убирает дальнейшую обработку команда.
                      //можно оставить число для проверки как работает после новой команды.
            }



            #ifdef DEBUG
                printf("assembler_code = %lf\n\n\n", ass_s->data[i]);
            #endif
        }

        ass_s->ass_size++;
        free(temp);
    }



    if(NEW_COMMAND_ERROR)
    {
        //free(ass_s->data);
        //ass_s->data = nullptr;
        return;
    }

    FILE* assembler_txt = fopen("[!]assembler_code.txt", "w");

    //int cur_rix = 0;
    //for(int i = 0; i < rix_cur_size; i++)
       // printf("[%lg]", rix_call[i]);
    //printf("\n");

    rix_cur_size = 0;


    for(int i = 0; i < ass_s->ass_size; i++)
    {
        if((ass_s->data[i] == 1) || (ass_s->data[i] == 21))
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

    #ifdef DEBUG
        printf("\nassembler_size = %d\n\n\n", assembler_size);
    #endif

    return;
}

void text_destruct(text_t* text_s)
{
    free(text_s->data);
    text_s->data = nullptr;

    free(text_s->lines);
    text_s->lines = nullptr;

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

    while((text_s->data[*file_length] = fgetc(text)) && !feof(text))
    {
        if(text_s->data[*file_length] == '\n')
            (*file_lines)++;
        (*file_length)++;
    }
}

void printf_text_s(text_t* text_s)
{
    assert(text_s);
    //assert(lines);

    FILE* res = fopen("[!]complied_code.txt", "w");

    for(int i = 0; i < text_s->n_struct; i++)
    {
        for(int k = 0; k < text_s->lines[i].length - 1; k++)
            fprintf(res, "%c", text_s->lines[i].line[k]);
        fprintf(res, " ");
    }

    fclose(res);
}

inline other_sizes(long length, long n_structs, int file_length, text_t* text_s, line_t *lines)
{
    while(TRUE)
    {
        while(TRUE)
        {
            if(length >= file_length)
                break;
            if((text_s->data[length] == ' ') || (text_s->data[length] == '\t') || (text_s->data[length] == '\n'))
                length++;
            else
                break;
        }

        if(length >= file_length)
            break;

        lines[n_structs].line = &(text_s->data)[length];
        lines[n_structs].length = length;


        while(text_s->data[length] != '\n')
        {
            if(length >= file_length)
                break;

            length++;
        }

        length++;

        lines[n_structs].length = length - lines[n_structs].length;
        //text_s->lines[n_structs].lenght = lines[n_structs].length;
        n_structs++;
    }

    for(int j = 0; j < n_structs; j++)
    {
        text_s->lines[j].line   = lines[j].line;
        text_s->lines[j].length = lines[j].length;
    }

    text_s->n_struct    = n_structs;
    text_s->length_file = file_length;
}

void code_destruct(code_t* code_s)
{
    assert(code_s);

    free(code_s->data);
    code_s->data = nullptr;

}

void ass_code_destruct(ass_code* ass_s)//перед free нужно еще пройтись по всем элементам занулить их
{
    assert(ass_s);

    free(ass_s->data);
    ass_s->data = nullptr;

    ass_s->ass_size = 0;
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
