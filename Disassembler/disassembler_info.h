#ifndef DISASSEMBLER_INFO_H_INCLUDED
#define DISASSEMBLER_INFO_H_INCLUDED

constexpr int size_time     = 40;

struct ass_code
{
    double* data            = nullptr;
    int     max_ass_size    = 0;
};

#endif // DISASSEMBLER_INFO_H_INCLUDED
