#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "number.c"


List integer_addition(List list1, List list2, int carry_forward) {
    List p = list1; List q = list2;

    if (!p) return q;
    if (!q) return p;

    while (p->next) p = p->next;
    while (q->next) q = q->next;

    List sum;
    init_list(&sum);

    int carry = carry_forward, add = 0;

    while (p && q) {
        int p_digit = p->val - '0';
        int q_digit = q->val - '0';

        add = p_digit + q_digit + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));
        p = p->prev;
        q = q->prev;
    }

    while (p) {
        int p_digit = p->val - '0';

        add = p_digit + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));
        p = p->prev;
    }

    while (q) {
        int q_digit = q->val - '0';

        add = q_digit + carry;
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


List fraction_addition(List list1, List list2, int *carry_forward) {
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
        int p_digit = p->val - '0';
        int q_digit = q->val - '0';
        add = p_digit + q_digit + carry;
        carry = add / 10;
        add = add % 10;
    
        insertbeg(&sum, (char) (add + '0'));

        p = p->prev;
        q = q->prev;
    }

    if (carry != 0) {
        *carry_forward = 1;
    }

    return sum;
}


Number addition(Number number1, Number number2) {
    if (number1.sign == '+' && number2.sign == '-') return subtraction(number1, negation(number2));
    else if (number1.sign == '-' && number2.sign == '+') return negation(subtraction(negation(number1), number2));

    Number sum;
    init_number(&sum);

    int carry = 0;
    sum.fraction = fraction_addition(number1.fraction, number2.fraction, &carry);
    sum.integer = integer_addition(number1.integer, number2.integer, carry);
    return sum;
}


List integer_subtraction(List list1, List list2, int carry_forward) {
    List p = list1; List q = list2;

    if (q == NULL) return p;

    while (p->next) p = p->next;
    while (q->next) q = q->next;

    List difference;
    init_list(&difference);

    int carry = carry_forward, subtract = 0;

    while (p && q) {
        int p_digit = p->val - '0';
        int q_digit = q->val - '0';

        subtract = p_digit - q_digit - carry;
        if (subtract < 0) {
            carry = 1;
            subtract += 10;
        }

        else {
            carry = 0;
        }
        
        insertbeg(&difference, (char) (subtract + '0'));
        p = p->prev;
        q = q->prev;
    }

    while (p) {
        int p_digit = p->val - '0';
        subtract = p_digit - carry;

        if (p_digit < (char)carry) {
            subtract += 10;
            carry = 1;
        }
        else {
            carry = 0;
        }
        insertbeg(&difference, (char) (subtract + '0'));
        p = p->prev;
    }
    trim_zeroes_integer(&difference);
    return difference;
}


List fraction_subtraction(List list1, List list2, int *carry_forward) {
    List p = copy_list(list1); List q = copy_list(list2);
    List difference; init_list(&difference);    

    while (p->next && q->next) {
        p = p->next;
        q = q->next;
    }

    while (q->next) {
        append(&p, '0');
        q = q->next;
        p = p->next;
    }

    if (p->next) {
        difference = copy_list(p->next);
    }

    int carry = 0, subtract = 0;

    while (p && q) {
        int p_digit = p->val - '0';
        int q_digit = q->val - '0';

        subtract = p_digit - q_digit - carry;
        if (subtract < 0) {
            carry = 1;
            subtract += 10;
        }

        else {
            carry = 0;
        }
        
        insertbeg(&difference, (char) (subtract + '0'));
        p = p->prev;
        q = q->prev;
    }

    if (carry != 0) {
        *carry_forward = 1;
    }

    trim_zeroes_integer(&difference);
    return difference;
}


Number subtraction(Number number1, Number number2) {
    Number difference; init_number(&difference);

    if (number1.sign == '+' && number2.sign == '-') return addition(number1, negation(number2));
    else if (number1.sign == '-' && number2.sign == '+') return negation(addition(negation(number1), number2));
    
    int carry_forward = 0;
    switch (compare_numbers(number1, number2)) {
        case 0:
            difference.fraction = fraction_subtraction(number1.fraction, number2.fraction, &carry_forward);
            difference.integer = integer_subtraction(number1.integer, number2.integer, carry_forward);
            difference.sign = '+';
            break;

        case 1:
            difference.fraction = fraction_subtraction(number2.fraction, number1.fraction, &carry_forward);
            difference.integer = integer_subtraction(number2.integer, number1.integer, carry_forward);
            difference.sign = '-';
            break;

        case 2:
            difference = zero_number();
            break;

        default:
            break;
    }

    trim_zeroes(&difference);
    return difference;
}


List list_multiplication(List list1, List list2) {
    List product; init_list(&product);
    List *possible_products = (List*)malloc(10*sizeof(List));

    possible_products[0] = zero_list();
    possible_products[1] = copy_list(list1);

    for (int i = 2; i < 10; i++) {
        List p = list1;
        List temp; init_list(&temp);

        while (p->next) p = p->next; 
        int carry = 0, mul = 0;

        while (p) {
            int p_digit = p->val - '0';
            mul = p_digit * i + carry;
            carry = mul / 10;
            mul = mul % 10;

            insertbeg(&temp, (char)(mul + '0'));
            p = p->prev;
        }

        if (carry != 0) {
            insertbeg(&temp, (char)(carry + '0'));
        }
        possible_products[i] = temp;
    }

    List q = list2;
    while (q->next) q = q->next;

    int carry = 0;
    List mul; init_list(&mul);
    int pos = 0;

    while (q) {
        int q_digit = q->val - '0';
        mul = copy_list(possible_products[q_digit]);
        
        for (int i = 0; i < pos; i++) 
            append(&mul, '0');
        
        product = integer_addition(product, mul, 0);
        // traverse(product);
        pos += 1;
        q = q->prev;
    }
    // printf(" = product in list_multiplication\n");
    return product;
}


Number multiplication(Number number1, Number number2) {
    List complete_list1; init_list(&complete_list1);
    append_list(&complete_list1, number1.integer);
    append_list(&complete_list1, number1.fraction);
    int first_decimal = list_length(number1.fraction);

    List complete_list2; init_list(&complete_list2);
    append_list(&complete_list2, number2.integer);
    append_list(&complete_list2, number2.fraction);
    int second_decimal = list_length(number2.fraction);

    int total_decimal = first_decimal + second_decimal;

    List output = list_multiplication(complete_list1, complete_list2);
    Number product; init_number(&product);

    List temp = output;
    if (temp == NULL) return zero_number();
    while (temp->next) temp = temp->next;

    int temp_flag = 0;
    for (int i = 0; i < total_decimal; i++) {
        if (temp->prev == NULL) {
            insertbeg(&temp, '0');
            temp_flag = 1;
        }
        else
            temp = temp->prev;
    }

    product.fraction = temp->next;
    product.fraction->prev = NULL;

    temp->next = NULL;
    if (temp_flag) product.integer = temp;
    else product.integer = output;

    trim_zeroes(&product);

    if (number1.sign == '+' && number2.sign == '+') product.sign = '+';
    else if (number1.sign == '-' && number2.sign == '-') product.sign = '+';
    else if (number1.sign == '-' && number2.sign == '+') product.sign = '-';
    else if (number1.sign == '+' && number2.sign == '-') product.sign = '-';

    return product;
}


// Divides list1 by list2 
List list_division(List list1, List list2, int *cap_forward) {
    if (list1 == NULL || list2 == NULL) return zero_list();

    trim_zeroes_integer(&list1); trim_zeroes_integer(&list2);
    if (compare_integers(list1, zero_list()) || compare_integers(list2, zero_list())) return zero_list();

    List quotient; init_list(&quotient);
    List *possible_products = (List*)malloc(10*sizeof(List));

    possible_products[0] = zero_list();
    possible_products[1] = copy_list(list2);

    for (int i = 2; i < 10; i++) {
        List p = list2;
        List temp; init_list(&temp);

        while (p->next) p = p->next; 
        int carry = 0, mul = 0;

        while (p) {
            int p_digit = p->val - '0';
            mul = p_digit * i + carry;
            carry = mul / 10;
            mul = mul % 10;

            insertbeg(&temp, (char)(mul + '0'));
            p = p->prev;
        }

        if (carry != 0) {
            insertbeg(&temp, (char)(carry + '0'));
        }
        possible_products[i] = temp;
    }

    int cap = 0;
    int running = 1;
    List p = list1;
    List dividend; init_list(&dividend);
    List divisor = copy_list(list2);

    while (running) {
        if (p != NULL) {
            append(&dividend, p->val);
            p = p->next;
        }
        else {
            append(&dividend, '0');
            cap += 1;
        }
        if (cap >= MAX_CAP) running = 0;

        while (compare_integers(dividend, divisor) == 1 && cap <= 10) {
            append(&quotient, '0');
            if (p != NULL) {
                append(&dividend, p->val);
                p = p->next;
            }
            else {
                append(&dividend, '0');
                cap += 1;
            }
            if (cap >= MAX_CAP) {
                running = 0; 
            }
        }

        int i = 0;
        List temp = possible_products[0];

        while (compare_integers(temp, dividend) == 1 && i < 10) {
            i += 1;
            temp = possible_products[i];
        }

        temp = copy_list(possible_products[i-1]);
        dividend = integer_subtraction(dividend, temp, 0);
        if ((compare_integers(dividend, zero_list()) == 2) && p == NULL) running = 0; 
        append(&quotient, (char)(i - 1 + '0'));

    }

    for (int i = 0; i < 10; i++) {
        free_list(&possible_products[i]);
    }
    free(possible_products);
    
    *cap_forward = cap;
    return quotient;
}


// Divides number1 by number2
Number division(Number number1, Number number2) {
    List complete_list1; init_list(&complete_list1);
    append_list(&complete_list1, number1.integer);
    append_list(&complete_list1, number1.fraction);
    int first_decimal = list_length(number1.fraction);

    List complete_list2; init_list(&complete_list2);
    append_list(&complete_list2, number2.integer);
    append_list(&complete_list2, number2.fraction);
    int second_decimal = list_length(number2.fraction);

    int cap = 0;
    List output = list_division(complete_list1, complete_list2, &cap);

    int total_decimal = first_decimal - second_decimal + cap;
    Number quotient; init_number(&quotient);

    List temp = output;
    if (temp == NULL) return zero_number();
    while (temp->next) temp = temp->next;

    int temp_flag = 0;

    for (int i = 0; i < total_decimal; i++) {
        if (temp->prev == NULL) { 
            insertbeg(&temp, '0');
            temp_flag = 1;
        }
        else
            temp = temp->prev;
    }

    quotient.fraction = temp->next;
    quotient.fraction->prev = NULL;

    temp->next = NULL;
    if (temp_flag) quotient.integer = temp;
    else quotient.integer = output;

    if (number1.sign == '+' && number2.sign == '+') quotient.sign = '+';
    if (number1.sign == '-' && number2.sign == '-') quotient.sign = '+';
    if (number1.sign == '-' && number2.sign == '+') quotient.sign = '-';
    if (number1.sign == '+' && number2.sign == '-') quotient.sign = '-';

    trim_zeroes(&quotient);
    return quotient;
}


Number negation(Number number) {
    Number negative = copy_number(number);
    
    if (negative.sign == '+') negative.sign = '-';
    else if (negative.sign == '-') negative.sign = '+';

    return negative;
}


// int main() {
//     Number n1, n2;
//     char *num1 = "-68432123543845.131", *num2 = "813556315.13521";
//     char *digits = "-33.1653485842-56344566843.254384";
    
//     n1 = convert_to_number(num1); n2 = convert_to_number(num2);

//     Number result = subtraction(n1, n2);
//     print_number(result);
//     printf("\n");
//     return 0;
// }