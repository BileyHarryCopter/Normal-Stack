#include <stdio.h>
#include <stdlib.h>

enum
{
    FALSE = 0,
    TRUE,
    ERROR
};

#define MyAss(expresion, file, line, function)                  \
{                                                               \
    if (!(expresion))                                           \
    {                                                           \
        Assert (FALSE, file, __LINE__, __PRETTY_FUNCTION__);    \
        return ERROR;                                           \
    }                                                           \
}                                                               \


int Assert (int expresion, FILE * file, const unsigned int line,
            const char * function)
{
    if (expresion == FALSE)
    {
        fprintf (file, "Problem on %u line\n", line);
        fprintf (file, "In the function %s\n", function);
        return ERROR;
    }
    return 0;
}

int main () {

    int a = 0;

    FILE * logfile = fopen ("Logfile.txt", "w");

    MyAss(a != 0, logfile, __LINE__, __PRETTY_FUNCTION__);

    printf ("xsefdwef\n");

    return 0;
}
