#ifndef CPU_INFO_H_INCLUDED
#define CPU_INFO_H_INCLUDED

constexpr int START_SIZE = 10; // Начальный размер стека.
constexpr int size_time  = 40;

struct ass_code
{
    double* data            = nullptr;
    int     max_ass_size    = 0;
};

struct rix
{
    double rax = NAN;
    double rbx = NAN;
    double rcx = NAN;
    double rdx = NAN;
};

#endif // CPU_INFO_H_INCLUDED
