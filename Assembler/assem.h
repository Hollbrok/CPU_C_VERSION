#ifndef ASSEM_H_INCLUDED
#define ASSEM_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "assem_info.h"

//#define DOUBLE_T  // that's the type of data
/*#define DEBUG*/ // if you need to find a bug or fix something
//#include "stack.h"

#include "Commands.h"

using number_of_labels = int;

#define cmd_check(name, command)   \
	else if (!strcmp(temp, #name)) \
		byte_struct->data[i] = command;

#define rix_check(name, command)                  \
	else if (!strcmp(temp, #name))                \
	{                                             \
		byte_struct->data[i] = command;           \
		specifiers[flags_size++] = S_REGIST_SPEC; \
		IS_LAST_COMMAND_PUSH = false;             \
	}

#define transition_check(name, command) \
	else if (!strcmp(temp, #name))      \
	{                                   \
		byte_struct->data[i] = command; \
		IS_LAST_COMMAND_JMP = true;     \
	}

#define bracket_check(bracket)   \
	else if (temp[0] == bracket) \
		bracket_exe(bracket, temp, byte_struct, &flags_size, specifiers, i);

auto text_construct(Text *text_struct, FILE *user_code) -> void;

auto text_destruct(Text *text_struct) -> void;

auto print_text_struct(Text *text_struct) -> void;

auto code_construct(Text *text_struct, Code *code_struct) -> void;

auto code_destruct(Code *code_struct) -> void;

auto print_code_buffer(Code *code_struct) -> void;

auto get_bytecode(Code *code_struct, Bytecode *byte_struct) -> void;

auto bytecode_destruct(Bytecode *byte_struct) -> void;

auto useful_sizes(FILE *user_code, Text *text_struct, int *file_lines, long *file_length) -> void;

auto set_time(struct tm *time) -> char *;

auto define_date(void) -> char *;

auto size_of_file(FILE *user_code) -> long;

// auto com_to_int(my_commands::Commands command) -> int;

auto get_labels(Label *labels, Code *code_struct) -> number_of_labels;

inline get_lexeme(int *j, Code *code_struct, char *temp);

inline bracket_exe(char spec, char *temp, Bytecode *byte_struct, int *flags_size, double *specifiers, int i);

auto define_specs(int *SPEC_NUMBER, int *SPEC_REGIST, char spec) -> int;

auto error_process(int i, char *temp) -> int;

auto print_error(FILE *error, int error_bit, Bytecode *byte_struct) -> void;

auto print_good(FILE *error) -> void;

auto determine_status(Bytecode *byte_struct) -> void;

auto get_byte(int digit, int number_of_bit) -> bool;

auto is_jmp(double x) -> bool;

#endif // ASSEM_H_INCLUDED
