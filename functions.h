#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "number.h"
#define MAX_CAP 30

List integer_addition(List, List, int);
List fraction_addition(List, List, int *);
Number addition(Number, Number);

List integer_subtraction(List, List, int);
List fraction_subtraction(List, List, int *);
Number subtraction(Number, Number);

List list_multiplication(List, List);
Number multiplication(Number, Number);

List list_division(List, List, int *);
Number division(Number, Number);

Number negation(Number number);

#endif