#include "stack.h"
FILE * Logfile;
FILE * Asserts;
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
                     //    Turn on the asserts    //
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
int Assert (int expresion, FILE * file, const unsigned int line,
            const char * function)
{
    if (expresion == FALSE)
    {
        fprintf (file, "Assertion failed\n");
        fprintf (file, "Problem on %u line\n", line);
        fprintf (file, "In the function %s\n", function);
        return ERROR;
    }
    return 0;
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackCtor (Stack * stk)
{
    Logfile = fopen ("Logfile.txt", "w");
    //TODO: google.com --> fopen (return value)
    // if (Logfile == NULL)
    // perror ("Problem with file:");
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    stk -> canary1 = BEZLAB;
    stk -> canary2 = AZEZBEK;
    stk -> capacity = CAP_SAMPLE;
    stk -> data = (double *) ((char *) calloc ((stk -> capacity) * sizeof (double) + 2 * sizeof (long), sizeof (char)) + sizeof (long));
    ////MyAss(stk -> data != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    * (stk -> data - 1) = ICECUBE;
    * (stk -> data + stk -> capacity) = FACE;
    stk -> size = 0;
    StackCheck (stk);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackDtor (Stack * stk)
{
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    //finish program (call the simple assert)
    StackCheck (stk);
    //segfault fix fix;
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
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    stk -> capacity += CAP_SAMPLE;
    //??????? For what it?
    stk -> data = (double *) ((char *) realloc ((char *) stk -> data - sizeof (long), (stk -> capacity) * sizeof (double) + 2 * sizeof (long)) + sizeof (long));
    * (stk -> data + stk -> capacity) = FACE;
    StackCheck (stk);
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
// Red = reduce - уменьшить
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackResRed (Stack * stk) // think about
{
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    stk -> capacity -= CAP_SAMPLE;
    stk -> data = (double *) ((char *) realloc ((char *) stk -> data - sizeof (long), (stk -> capacity) * sizeof (double) + 2 * sizeof (long)) + sizeof (long));
    * (stk -> data + stk -> size) = FACE;
    StackCheck (stk);
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
}
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
//===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*===*//
void StackPush (Stack * stk, double value)
{
    StackCheck (stk);
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
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
    //MyAss(stk != NULL, Logfile, __LINE__, __PRETTY_FUNCTION__);
    //MyAss(stk -> size == 0, Logfile, __LINE__, __PRETTY_FUNCTION__);
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
            fprintf (Logfile, "CANARY1 of the Data:     %0X\n",
                               (unsigned int) * (stk -> data - 1));
            fprintf (Logfile, "DATA:                    \n\n");
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
            fprintf (Logfile, "\nEND OF DATA\n");
            fprintf (Logfile, "CANARY2 of the Data:     %0X\n",
                               (unsigned int) * (stk -> data + stk -> capacity));
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
            if (stk -> size == 0)
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
    if ( (stk != NULL) || (stk -> data != NULL) || (stk -> size == 0) ||
         (stk -> canary1) != BEZLAB || (stk -> canary2) != AZEZBEK ||
         (* (stk -> data - 1) != ICECUBE) || (* (stk -> data + stk -> capacity) != FACE))
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
    if ( (stk != NULL) || (stk -> data != NULL) || (stk -> size == 0) ||
         (stk -> canary1) != BEZLAB || (stk -> canary2) != AZEZBEK ||
         (* (stk -> data - 1) != ICECUBE) || (* (stk -> data + stk -> capacity) != FACE))
    {
        StackOk (stk);
    }
}
