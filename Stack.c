#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Stack.h"

int main () {
    struct Stack stk = {};
    StackCtor (&stk);
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    for (int i = 0; i < 1000; i++)
    {
        StackPush (&stk, 10.2 + i);
        //printf ("data[%d] = %lf\n", i, * (stk.data + i));
    }

    for (int i = 0; i < 500; i ++)
    {
        StackPop (&stk);
    }

    StackDump (&stk, OKEY);

//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
    StackDtor (&stk);
    return 0;
}
