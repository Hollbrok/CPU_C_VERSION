#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "cpu_info.h"

/*#define DEBUG*/       // if you need to find a bug or fix something
#define DOUBLE_T        // that's the type of data
#include <stack.h>

#include "Commands.h"

typedef int ERROR_STATE;

auto get_bytecode(FILE* text, Bytecode* byte_struct) -> void;

auto CPU(Bytecode* byte_struct, stack_t* Stack, stack_t* Stack_call) -> void;

auto determine_status(Bytecode* byte_struct, stack_t* Stack) -> void;

auto bytecode_destruct(Bytecode* byte_struct) -> void;

auto print_for_user(stack_t* Stack) -> void;

auto ignore_spaces(char** buffer) -> void;

auto print_good(FILE* error) -> void;

auto print_error(FILE* error, int error_bit) -> void;

auto get_number(char** buffer) -> double;

auto set_time(struct tm *time) -> char*;

auto define_date() -> char*;

auto is_equal(double a, double b) -> bool;

auto fill_screen(char* OP) -> void;

auto draw_kopm() -> void;

auto draw_circ(char* OP, stack_t* Stack, Bytecode* byte_struct) -> void;

auto draw_screen(char* OP) -> void;

auto draw_mem() -> void;

auto draw_cat() -> void;

auto get_byte(int digit, int number_of_bit) -> bool;

auto cmd_push_exe(int command, int i, stack_t* Stack, Rix *rix_struct, Bytecode* byte_struct, char *OP) -> ERROR_STATE;

auto cmd_pop_exe(int command, int i, stack_t* Stack, Rix *rix_struct, Bytecode* byte_struct, char *OP) -> ERROR_STATE;

auto cmd_compair_exe(int command, stack_t* Stack, Bytecode* byte_struct, int *i) -> ERROR_STATE;

#endif // CPU_H_INCLUDED
