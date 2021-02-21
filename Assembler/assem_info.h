#ifndef ASSEM_INFO_H_INCLUDED
#define ASSEM_INFO_H_INCLUDED

constexpr int size_time     = 40;

struct line_t
{
    char *line  = nullptr;              // указатель на буфер строки
    long length = 0;                    // длина строки
};

struct text_t
{
    char* data              = nullptr;  // указатель на буфер
    line_t* lines           = nullptr;  // указатель на структуру строки (то есть массив структур строк)
    int n_struct            = 0;        // кол-во непустых строк
    int length_file         = 0;        // длина файла, то есть размер буфера
};

struct code_t
{
    char*   data            = nullptr;  // указатель на буфер кода
    int     length          = 0;        // длина буфера
    int     terms           = 0;        // кол-во лексем
};

struct ass_code
{
    double* data            = nullptr;
    int     ass_size        = 0;
};


#endif // ASSEM_INFO_H_INCLUDED
