#ifndef genericH
#define genericH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 81
#define MAX_LABEL_SIZE 32
#define MAX_DEFINES 100 /*max number of defines*/

#include "nodes.h"
#include "preAssembler.h"
#include "firstAssembler.h"
#include "secAssembler.h"

enum constantsValues
{
    NA = -1,
    FALSE = 0,
    TRUE = 1,
    MCR = 10,
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
enum nodeAttributes
{
    ORIGINAL_LINE = 100,
    LINE_NUM,
    LINE_TYPE,
    LABEL_TEXT,
    OP_CODE,
    OPERAND_COUNT,
    OPERAND_TYPE_1,
    OPERAND_TYPE_2,
    OPERAND_LABEL_1,
    OPERAND_LABEL_2,
    OPERAND_IMM_1,
    OPERAND_IMM_2,
    OPERAND_REG_1,
    OPERAND_REG_2,
    TARGET_LABEL_ADDRESS,
    LABLE_ADDRESS_LINE,
    ARE,
    LINE_SIZE,
    NUM_OF_WORDS,
    DATA_ARRAY,
    ERROR_COUNT,
    DECLERATION,
    USED_LABEL,
    MAIN_TYPE,
    TYPE
};
#endif
