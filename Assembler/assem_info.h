#ifndef ASSEM_INFO_H_INCLUDED
#define ASSEM_INFO_H_INCLUDED

constexpr int size_time      = 40;
constexpr int MAX_LABELS     = 50;
constexpr int MAX_LABEL_SIZE = 20;

struct Line
{
    char *line  = nullptr;              // ��������� �� ����� ������
    long length = 0;                    // ����� ������
};

struct Text
{
    char* data              = nullptr;  // ��������� �� �����
    Line* lines             = nullptr;  // ��������� �� ��������� ������ (�� ���� ������ �������� �����)
    int n_struct            = 0;        // ���-�� �������� �����
    int length_file         = 0;        // ����� �����, �� ���� ������ ������
};

struct Code
{
    char*   data   = nullptr;  // ��������� �� ����� ����
    int     length = 0;        // ����� ������
    int     terms  = 0;        // ���-�� ������
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
