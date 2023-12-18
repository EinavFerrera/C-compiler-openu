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
    ENDMCR = 8,
    NOMCR = 9,
    IMM = 10,
    REG = 11,
    LABEL = 12,
    DATA = 13,
    DATA_LABEL = 14,
    CODE = 15,
    ENTRY = 16,
    EXT = 17
};
#endif