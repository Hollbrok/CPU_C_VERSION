#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "cpu_info.h"

/*#define DEBUG*/       // if you need to find a bug or fix something
#define DOUBLE_T        // that's the type of data
#include <stack.h>

#include "Commands.h"

void make_ass_s(FILE* text, ass_code* ass_s);

void ignore_spaces(char** buffer);

void CPU(ass_code* ass_s, stack_t* Stack);

void print_for_user(stack_t* Stack);

void ass_code_destruct(ass_code* ass_s);

char* set_time(struct tm *u);

char* define_date();

double get_number(char** buffer);

#endif // CPU_H_INCLUDED
