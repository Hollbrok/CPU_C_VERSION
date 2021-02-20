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

void get_code(text_t* text_s, code_t* code_s);

void get_ass_code(code_t* code_s, ass_code* ass_s);

void text_destruct(text_t* text);

void code_destruct(code_t* code_s);

void ass_code_destruct(ass_code* ass_s);

void useful_sizes(FILE* text, text_t* text_s, int* file_lines, long* file_lenght);

void printf_text_s(text_t* text_s);

char* set_time(struct tm *time);

char* define_date(void);

inline make_full_text_struct(long length, long n_structs, int file_lines, text_t* text_s, line_t *lines);

long size_of_file(FILE* text);
#endif // ASSEM_H_INCLUDED
