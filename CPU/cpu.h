#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "cpu_info.h"

/*#define DEBUG*/       // if you need to find a bug or fix something
#define DOUBLE_T        // that's the type of data
#include <stack.h>

#include "Commands.h"

auto get_bytecode(FILE* text, Bytecode* byte_struct) -> void;

auto CPU(Bytecode* byte_struct, stack_t* Stack, stack_t* Stack_call) -> void;

auto bytecode_destruct(Bytecode* byte_struct) -> void;

auto print_for_user(stack_t* Stack) -> void;

auto ignore_spaces(char** buffer) -> void;

auto get_number(char** buffer) -> double;

auto set_time(struct tm *time) -> char*;

auto define_date() -> char*;

auto is_equal(double a, double b) -> bool;

#endif // CPU_H_INCLUDED
