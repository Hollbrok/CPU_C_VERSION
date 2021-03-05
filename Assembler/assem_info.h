#ifndef ASSEM_INFO_H_INCLUDED
#define ASSEM_INFO_H_INCLUDED

constexpr int size_time      = 40;
constexpr int MAX_LABELS     = 50;
constexpr int MAX_LABEL_SIZE = 20;

struct Line
{
    char *line  = nullptr;              // указатель на буфер строки
    long length = 0;                    // длина строки
};

struct Text
{
    char* data              = nullptr;  // указатель на буфер
    Line* lines             = nullptr;  // указатель на структуру строки (то есть массив структур строк)
    int n_struct            = 0;        // кол-во непустых строк
    int length_file         = 0;        // длина файла, то есть размер буфера
};

struct Code
{
    char*   data   = nullptr;  // указатель на буфер кода
    int     length = 0;        // длина буфера
    int     terms  = 0;        // кол-во лексем
};

struct Bytecode
{
    double* data              = nullptr;
    int     bytecode_capacity = 0;
};

struct Label
{
    char* name    = nullptr;
    size_t length = 0;
    int adress    = 0;
};


#endif // ASSEM_INFO_H_INCLUDED
