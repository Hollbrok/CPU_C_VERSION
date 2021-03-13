#ifndef ASSEM_H_INCLUDED
#define ASSEM_H_INCLUDED

#include <Txlib.h>
#include <time.h>
#include "assem_info.h"

#define DOUBLE_T        // that's the type of data
/*#define DEBUG*/       // if you need to find a bug or fix something
#include <stack.h>

#include "Commands.h"

auto text_construct(Text* text_struct, FILE* user_code) -> void;

auto text_destruct(Text* text_struct) -> void;

auto print_text_struct(Text* text_struct) -> void;                      // �������� � ���� string_text ����� ��� ������������ � ���� ������

auto code_construct(Text* text_struct, Code* code_struct) -> void;

auto code_destruct(Code* code_struct) -> void;

auto print_code_buffer(Code* code_struct) -> void;                  // �������� � ���� code_struct ����� ��������� code (��� ��������� ��� ����� �����������)

auto get_bytecode(Code* code_struct, Bytecode* byte_struct) -> void;

auto bytecode_destruct(Bytecode* byte_struct) -> void;

auto useful_sizes(FILE* user_code, Text* text_struct, int* file_lines, long* file_length) -> void;

auto set_time(struct tm *time) -> char*;

auto define_date(void) -> char*;

auto size_of_file(FILE* user_code) -> long;

auto com_to_int(my_commands::Commands command) -> int;

#endif // ASSEM_H_INCLUDED
