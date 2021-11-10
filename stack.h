#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

enum
{
    FALSE = 0,
    TRUE,
    ERROR,
    OKEY,
    BAD
};

#define BEZLAB   0xBE31AB
#define ICECUBE  0x1CECBE
#define FACE     0xFACE
#define AZEZBEK  0xA3E3BEC
// define для assert
#define MyAss(expresion, file, line, func)                    \
{                                                             \
    if (!(expresion))                                         \
    {                                                         \
        Assert (FALSE, file, __LINE__, __PRETTY_FUNCTION__);  \
        return ERROR;                                         \
    }                                                         \
}                                                             \

// оставить с const
static size_t CAP_SAMPLE = sizeof (double);
struct Stack;
typedef struct Stack Stack;
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                         // definitions of functions
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Assert (int expresion, FILE * file,
            const unsigned int line,
            const char * function);
void StackDump (Stack * stk, int status);
void StackOk (Stack * stk);
void StackCheck (Stack * stk);
void StackCtor (Stack * stk);
void StackDtor (Stack * stk);
void StackResEnc (Stack * stk);
void StackResRed (Stack * stk);
void StackPush (Stack * stk, double value);
double StackPop (Stack * stk);
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                         // definition of the struct
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
struct Stack {
    long canary1;
    double * data;
    size_t size;
    size_t capacity;
    // hash
    long canary2;
};
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
