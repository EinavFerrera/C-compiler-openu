#ifndef genericH
#define genericH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_SIZE 81
#define MAX_LABEL_SIZE 32

enum constantsValues
{
    NA = -1,
    MCR = 7,
    ENDMCR,
    NOMCR,
    IMM,
    REG,
    LABEL,
    DATA_LABEL,
    DATA_INT,
    DATA_STRING,
    CODE,
    ENTRY,
    EXT
};
#endif