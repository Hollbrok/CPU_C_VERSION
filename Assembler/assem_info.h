#ifndef ASSEM_INFO_H_INCLUDED
#define ASSEM_INFO_H_INCLUDED

constexpr int size_time      = 40;
constexpr int MAX_LABELS     = 50;
constexpr int MAX_LABEL_SIZE = 20;

struct Line
{
    char *line  = nullptr;              // pointer to line
    long length = 0;                    // length of line
};

struct Text
{
    char* data              = nullptr;  // pointer on buffer
    Line* lines             = nullptr;  // pointer to the line struct
    int n_struct            = 0;        // number of lines
    int length_file         = 0;        // size of buffer
};

struct Code
{
    char*   data   = nullptr;  // pointer to buffer of code
    int     length = 0;        // length of buffer
    int     terms  = 0;        // numbers of lexeme
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
