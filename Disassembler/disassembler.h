#ifndef DISASSEMBLER_H_INCLUDED
#define DISASSEMBLER_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "disassembler_info.h"
#include "Commands.h"

void make_ass_s(FILE* text, ass_code* ass_s);

void ignore_spaces(char** buffer);

void ass_code_destruct(ass_code* ass_s);

double get_number(char** buffer);

char* set_time(struct tm *u);

char* define_date(void);

void disassembler(ass_code* ass_s, FILE* result);

#endif // DISASSEMBLER_H_INCLUDED
