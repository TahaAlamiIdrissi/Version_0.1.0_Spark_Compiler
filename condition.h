#ifndef CONDITION_H
#define CONDITION_H
#include "ast.h"

typedef enum {EQ, GT, LT, GEQT, LEQT, DIFF} Type_Comp;

typedef enum { _NONE, _OR, _AND, _XOR} Logical_Link;

typedef struct {
    int rangvar;
    AST right;
    Type_Comp typecomp;
}condvaluetype;


typedef struct LIST_COND {
    condvaluetype first;
    Logical_Link typelink;
    struct LIST_COND * next;
} listcondvalueType;

union COND_ELT
{
    condvaluetype condition;
    listcondvalueType listcondition;
}condelt;


#endif