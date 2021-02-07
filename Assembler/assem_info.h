#ifndef ASSEM_INFO_H_INCLUDED
#define ASSEM_INFO_H_INCLUDED

struct line_t
{
    char *line  = nullptr;
    long length = 0;
};

struct text_t
{
    char* data              = nullptr;
    line_t* lines           = nullptr;
    int n_struct            = 0;
    int length_file         = 0;
};

struct code_t
{
    char*   data            = nullptr;
    int     length          = 0;
    int     terms           = 0;
};

struct ass_code
{
    double* data            = nullptr;
    double* rix_data        = nullptr;
    int*    rix_call        = nullptr;
    int     cur_rix         = 0;
    int     ass_size        = 0;
};


#endif // ASSEM_INFO_H_INCLUDED
