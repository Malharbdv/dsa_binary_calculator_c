#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "number.c"


List integer_addition(List list1, List list2, int flag) {
    List p = list1; List q = list2;
    while (p->next) p = p->next;
    while (q->next) q = q->next;

    List sum;
    init_list(&sum);

    int carry = flag, add = 0;

    while (p && q) {
        add = (int)(p->val + q->val - '0' - '0') + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));
        p = p->prev;
        q = q->prev;
    }

    while (p) {
        add = (int)(p->val - '0') + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));
        p = p->prev;
    }

    while (q) {
        add = (int)(q->val - '0') + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));
        q = q->prev;
    }

    if (carry != 0) {
        insertbeg(&sum, (char) (carry + '0'));
    }

    return sum;
}


List fraction_addition(List list1, List list2, int *flag) {
    List sum;
    init_list(&sum);

    List p = list1, q = list2;

    while (p->next && q->next) {
        p = p->next;
        q = q->next;
    }

    if (p->next) sum = copy_list(p->next);
    else if (q->next) sum = copy_list(q->next);
    int carry = 0, add = 0;

    while (p && q) {
        add = (int)(p->val + q->val - '0' - '0') + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));

        p = p->prev;
        q = q->prev;
    }

    if (carry != 0) {
        *flag = 1;
    }

    return sum;
}


Number addition(Number number1, Number number2) {
    Number sum;
    init_number(&sum);

    int carry = 0;
    sum.fraction = fraction_addition(number1.fraction, number2.fraction, &carry);
    sum.integer = integer_addition(number1.integer, number2.integer, carry);
    return sum;
}


List integer_subtraction(List list1, List list2, int flag) {
    List p = list1; List q = list2;
    while (p->next) p = p->next;
    while (q->next) q = q->next;

    List difference;
    init_list(&difference);

    int carry = flag, subtract = 0;

    while (p && q) {
        subtract = (int)(p->val - q->val - '0' - '0') - carry;
        carry = subtract / 10;
        subtract = subtract % 10;
    
        insertbeg(&difference, (char) (subtract + '0'));
        p = p->prev;
        q = q->prev;
    }

    while (p) {
        subtract = (int)(p->val - '0') + carry;
        carry = subtract / 10;
        subtract = subtract % 10;
    
        insertbeg(&difference, (char) (subtract + '0'));
        p = p->prev;
    }

    while (q) {
        subtract = (int)(q->val - '0') + carry;
        carry = subtract / 10;
        subtract = subtract % 10;
    
        insertbeg(&difference, (char) (subtract + '0'));
        q = q->prev;
    }

    if (carry != 0) {
        insertbeg(&difference, (char) (carry + '0'));
    }

    return difference;
}


int main() {
    Number n1, n2;
    char *num1 = "1234568350.654186451321", *num2 = "9235421548384.1";
    n1 = convert_to_number(num1); print_number(n1); printf("\n");
    n2 = convert_to_number(num2); print_number(n2); printf("\n");

    int flag = 0;
    Number sum = addition(n1, n2);
    print_number(sum);
    return 0;
}

