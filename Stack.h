#include "Service.h"
typedef struct Stack Stack;

FILE * Logfile;

#define BEZLAB 0xBE31AB
#define AZEZBEK 0xA3E3BEC
#define STACKBAD "STATUS of the Stack:     wrong\n"

enum
{
    OKEY,
    BAD
};

void StackDump (Stack * stk, int status);
void StackOk (Stack * stk);
void StackCheck (Stack * stk);

size_t CAP_SAMPLE = sizeof (double);
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
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
void StackCtor (Stack * stk)
{
    Logfile = fopen ("Logfile.txt", "w"); //TODO: google.com --> fopen (return value)
    // if (Logfile == NULL)
      // perror ("Problem with file:");
    meowsert (stk != NULL);
    stk -> canary1 = BEZLAB;
    stk -> canary2 = AZEZBEK;
    stk -> capacity = CAP_SAMPLE;
    stk -> data = (double *) ((char *) calloc ((stk -> capacity) * sizeof (double) + 2 * sizeof (long), sizeof (char)) + sizeof (long));
    meowsert (stk -> data != NULL);
    * (stk -> data - 1) = stk -> canary1;
    * (stk -> data + stk -> capacity) = stk -> canary2;
    stk -> size = 0;
    StackCheck (stk);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDtor (Stack * stk)
{
    meowsert (stk != NULL); //finish program (call the simple assert)
    StackCheck (stk); //segfault fix, Logfile fix;

    stk -> canary1 = 0;
    stk -> canary2 = 0;
    stk -> size = 0;
    stk -> capacity = 0;
    fclose (Logfile);
    free (stk -> data - 1);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
// Enc = encrease - увеличить
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackResEnc (Stack * stk)
{
    // увеличивать стэк во время неполного заполнения
    meowsert (stk != NULL);
    stk -> capacity += CAP_SAMPLE;
    * (stk -> data + stk -> size + 1) = 1; //??????? For what it?
    stk -> data = (double *) ((char *) realloc ((char *) stk -> data - sizeof (long), (stk -> capacity) * sizeof (double) + 2 * sizeof (long)) + sizeof (long));
    StackCheck (stk);
    meowsert (stk != NULL);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
// Red = reduce - уменьшить
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackResRed (Stack * stk) // think about
{
    meowsert (stk != NULL);
    stk -> capacity -= CAP_SAMPLE;
    * (stk -> data + stk -> size) = 0;
    stk -> data = (double *) ((char *) realloc ((char *) stk -> data - sizeof (long), (stk -> capacity) * sizeof (double) + 2 * sizeof (long)) + sizeof (long));
    StackCheck (stk);
    meowsert (stk != NULL);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackPush (Stack * stk, double value)
{
    StackCheck (stk);
    meowsert (stk != NULL);
    if (stk -> size == stk -> capacity - 1) //when should you do it?
    {
        StackResEnc (stk);
    }
    * (stk -> data + stk -> size) = value;
    (stk -> size)++;
    StackCheck (stk);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
double StackPop (Stack * stk)
{
    meowsert (stk != NULL);
    meowsert (stk -> size >= 0); // think
    StackCheck (stk);
    double value = * (stk -> data + stk -> size - 1);
    * (stk -> data + stk -> size - 1) = 0.0;
    if (stk -> size == stk -> capacity - CAP_SAMPLE)
    {
        StackResRed (stk);
    }
    (stk -> size)--;
    return value;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDump (Stack * stk, int status)
{
    unsigned int nprob = 0;
    switch (status)
    {
        case OKEY:
        {
            fprintf (Logfile, "STATUS of the Stack:     good\n");
            fprintf (Logfile, "CANARY1 of the Stack:    %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "SIZE of the Stack:       %zu\n", stk -> size);
            fprintf (Logfile, "CAPACITY of the Stack:   %zu\n", stk -> capacity);
            fprintf (Logfile, "CANARY1 of the Data:     %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "DATA:                       \n\n");
            for (size_t i = 0; i < stk -> capacity; i++)
            {
                if (i < stk -> size)
                {
                    fprintf (Logfile, ">>> data[%zu] = %.1lf\n", i, * (stk -> data + i));
                }
                else
                {
                    fprintf (Logfile, "    data[%zu] = %.1lf\n", i, * (stk -> data + i));
                }
            }
            fprintf (Logfile, "\n                 END OF DATA\n");
            fprintf (Logfile, "CANARY2 of the Data:     %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "CANARY2 of the Stack:    %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "////////////////////////////////\n\n\n");
            break;
        }
        case BAD:
        {
            fprintf (Logfile, "STATUS of the Stack:        bad\n");
            if (stk -> capacity < 0)
            {
                fprintf (Logfile, "Value of CAPACITY is wrong: %zu\n", stk -> capacity);
            }
            if (stk -> size < 0)
            {
                fprintf (Logfile, "Value of SIZE is wrong:     %zu\n", stk -> size);
            }
            if (stk -> canary1 != BEZLAB)
            {
                fprintf (Logfile, "Initial value of CANARY1:   %0X\n", (unsigned int) 0xBE31AB);
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            if (stk -> canary2 != AZEZBEK)
            {
                fprintf (Logfile, "Initial value of CANARY2:   %0X\n", (unsigned int) 0xA3E3BEC);
                fprintf (Logfile, "I'm very sad\nCause bird is dead\n");
                fprintf (Logfile, "https://www.youtube.com/watch?v=krokDpyAWlM\n\n");
            }
            fprintf (Logfile, "////////////////////////////////\n\n\n");
            break;
        }
        default:  // придумать вариант для просмотра ошибки
        {
            fprintf (Logfile, "     ___UNKNOWN PROBLEM___     \n");
            fprintf (Logfile, "STATUS of the Stack:    unknown\n");
            fprintf (Logfile, "Whole checking:                \n");
            fprintf (Logfile, "CANARY1 of the Stack:       %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "SIZE of the Stack:          %zu\n", stk -> size);
            fprintf (Logfile, "CAPACITY of the Stack:      %zu\n", stk -> capacity);
            fprintf (Logfile, "CANARY1 of the Data:        %0X\n", (unsigned int) stk -> canary1);
            fprintf (Logfile, "CANARY2 of the Data:        %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "CANARY2 of the Stack:       %0X\n", (unsigned int) stk -> canary2);
            fprintf (Logfile, "Thanks for working with Stack  \n");
        }
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackOk (Stack * stk)
{
    if ((stk -> canary1) != BEZLAB || (stk -> canary2) != AZEZBEK)
    {
        StackDump (stk, BAD);
    }
    else
    {
        StackDump (stk, OKEY);
    }
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackCheck (Stack * stk)
{
    if ((stk -> canary1) != BEZLAB || (stk -> canary2) != AZEZBEK)
    {
        StackOk (stk);
    }
}
