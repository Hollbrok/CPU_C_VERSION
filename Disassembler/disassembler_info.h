#ifndef DISASSEMBLER_INFO_H_INCLUDED
#define DISASSEMBLER_INFO_H_INCLUDED

constexpr int size_time     = 40;

struct Bytecode
{
    double* data              = nullptr;
    int     bytecode_capacity = 0;
};

#endif // DISASSEMBLER_INFO_H_INCLUDED
