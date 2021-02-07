#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

const int MAX_SIZE_COMMAND  = 20;
const int DEBUG_STATE       = 0;


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
    QUADRATIC   = 15,
    LOG2        = 16,
    RAX         = 17,
    RBX         = 18,
    RCX         = 19,
    RDX         = 20,
    POP         = 21,

};

#endif // COMMANDS_H_INCLUDED
