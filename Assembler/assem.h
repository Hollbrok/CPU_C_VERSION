#ifndef ASSEM_H_INCLUDED
#define ASSEM_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "assem_info.h"

#define DOUBLE_T        // that's the type of data
/*#define DEBUG*/       // if you need to find a bug or fix something
#include <stack.h>

#include "Commands.h"

void text_construct(text_t* text, FILE* onegin);

void useful_sizes(FILE* text, text_t* text_s, int* file_lines, long* file_lenght);

void code_construct(text_t* text_s, code_t* code_s);

void print_code_buffer(code_t* code_s); // печатает в файл code_struct буфер структуры code (для просмотра как робят комментарии)

void get_ass_code(code_t* code_s, ass_code* ass_s);

void text_destruct(text_t* text);

void code_destruct(code_t* code_s);

void ass_code_destruct(ass_code* ass_s);

void printf_text_s(text_t* text_s); // печатает в файл string_text текст без комментариев в виде строки

char* set_time(struct tm *time);

char* define_date(void);

long size_of_file(FILE* text);
#endif // ASSEM_H_INCLUDED
