#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

constexpr int MAX_SIZE_COMMAND  = 20;
constexpr int MAX_SIZE_LABLE    = 15;
constexpr int DEBUG_STATE       = 0;


enum commands
{
    HLT         = 0,
    PUSH        = 1,
    ADD         = 2,
    MUL         = 3,
    DIV         = 4,
    SUB         = 5,
    SIN         = 6,
    COS         = 7,
    POW         = 8,
    SQRT        = 9,
    IN          = 10,
    PRINT       = 11,
    DEL         = 12,
    LN          = 13,
    LOG10       = 14,
    QUADRATIC   = 15, // лишняя
    LOG2        = 16,
    RAX         = 17,
    RBX         = 18,
    RCX         = 19,
    RDX         = 20,
    POP         = 21,
    LABEL       = 22,
    JMP         = 23,
    JE          = 24,
    JAB         = 25,
    JAE         = 26,
    JBE         = 27,
    JA          = 28,
    JB          = 29,
    RET         = 30,
    CALL        = 31,
};

#endif // COMMANDS_H_INCLUDED
