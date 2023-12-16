#ifndef genericH
#define genericH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_SIZE 81

enum constantsValues {
    NA = -1,
    MCR = 7,
    ENDMCR = 8,
    NOMCR = 9,
    IMM = 10,
    REG = 11,
    LABEL = 12,
    DATA = 13,
    CODE = 14,
    ENTRY = 15,
    EXT = 16
};


#endif