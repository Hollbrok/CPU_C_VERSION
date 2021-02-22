#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

constexpr int MAX_SIZE_COMMAND  = 20;
constexpr int MAX_SIZE_LABLE    = 15;
constexpr int DEBUG_STATE       = 0;

enum class Commands
{
    CMD_HLT         = 0,
    CMD_PUSH        = 1,
    CMD_ADD         = 2,
    CMD_MUL         = 3,
    CMD_DIV         = 4,
    CMD_SUB         = 5,
    CMD_SIN         = 6,
    CMD_COS         = 7,
    CMD_POW         = 8,
    CMD_SQRT        = 9,
    CMD_IN          = 10,
    CMD_OUT         = 11,
    CMD_DEL         = 12,
    CMD_LN          = 13,
    CMD_LOG10       = 14,
    CMD_QUADRATIC   = 15, // лишняя
    CMD_LOG2        = 16,
    CMD_RAX         = 17,
    CMD_RBX         = 18,
    CMD_RCX         = 19,
    CMD_RDX         = 20,
    CMD_POP         = 21,
    CMD_LABEL       = 22,
    CMD_JMP         = 23,
    CMD_JE          = 24,
    CMD_JAB         = 25,
    CMD_JAE         = 26,
    CMD_JBE         = 27,
    CMD_JA          = 28,
    CMD_JB          = 29,
    CMD_RET         = 30,
    CMD_CALL        = 31
};


#endif // COMMANDS_H_INCLUDED
