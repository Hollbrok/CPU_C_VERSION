#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

constexpr int MAX_SIZE_COMMAND  = 20;
constexpr int MAX_SIZE_LABLE    = 15;
constexpr int DEBUG_STATE       = 0;

constexpr int S_NUMBER_SPEC     = 10;   // push 2       S -- working with a Stack
constexpr int S_REGIST_SPEC     = 20;   // push rbx     S -- working with a Stack
constexpr int OP_NUMBER         = 11;   // push [2]     OP -- working with Operative memory
constexpr int OP_REGIST         = 21;   // push [rbx]   OP -- working with Operative memory
constexpr int OP_CHAR_NUM       = 22;   // push (2)
constexpr int OP_CHAR_REG       = 23;   // push (rbx)   -- побайтовой пуш, а не double


namespace my_commands
{
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
        CMD_QUADRATIC   = 15, // лишн€€
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
        CMD_CALL        = 31,
        CMD_DRAW        = 32,
        CMD_FILL        = 33,
        CMD_ABS         = 34,
        CMD_CIRC        = 35,
        CMD_CAT         = 36,
        CMD_KOPM        = 37
    };
}


#endif // COMMANDS_H_INCLUDED
