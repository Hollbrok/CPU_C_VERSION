#ifndef CPU_INFO_H_INCLUDED
#define CPU_INFO_H_INCLUDED

constexpr int START_SIZE = 10; // Начальный размер стека.
constexpr int size_time  = 40;

struct Bytecode
{
    double* data              = nullptr;
    int     bytecode_capacity = 0;
};

struct Rix
{
    double rax = NAN;
    double rbx = NAN;
    double rcx = NAN;
    double rdx = NAN;
};

#endif // CPU_INFO_H_INCLUDED
