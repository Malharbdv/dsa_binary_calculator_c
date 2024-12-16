#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "functions.h"
#include "number.h"

typedef struct NNode {
    Number number;
    struct NNode *next;
} NNode;

typedef struct CNode {
    char operator;
    struct CNode *next;
} CNode;

typedef NNode* NStack;
typedef CNode* CStack;

void init_cstack(CStack *);
void init_nstack(NStack *);
void push_cstack(CStack *, char);
void push_nstack(NStack *, Number);
char pop_cstack(CStack *);
Number pop_nstack(NStack *);
int empty_cstack(CStack);
int empty_nstack(NStack);

Number evaluate(Number, Number, char);
Number evaluate_expression(char *);

#endif