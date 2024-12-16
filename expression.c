#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression.h"
#include "functions.c"


Number evaluate(Number number1, Number number2, char oper) {
    Number answer; init_number(&answer);

    switch (oper) {
        case '+':
            answer = addition(number1, number2);
            break;

        case '-':
            answer = subtraction(number1, number2);
            break;

        case '*':
            answer = multiplication(number1, number2);
            break;

        case '/':
            answer = division(number1, number2);
            break;

        default:
            printf("The operator specified is not recognized: %c\n", oper);
            break;
    }

    return answer;
}


void init_cstack(CStack *cstack) {
    *cstack = NULL;
    return;
}


void init_nstack(NStack *nstack) {
    *nstack = NULL;
    return;
}


void push_cstack(CStack *cstack, char operat) {
    CNode *cnode = (CNode*)malloc(sizeof(CNode));

    if (!cnode) {
        printf("Memory was not allocated properly. Please retry!\n");
        exit(1);
    }

    cnode->next = NULL;
    cnode->operator = operat;

    if(*cstack == NULL) {
        *cstack = cnode;
    }
    else {
        cnode->next = *cstack;
        *cstack = cnode;
    }
    return;
}


void push_nstack(NStack *nstack, Number number) {
    NNode *nnode = (NNode*)malloc(sizeof(NNode));

    if (!nnode) {
        printf("Memory was not allocated properly. Please retry!\n");
        exit(1);
    }

    nnode->number = copy_number(number);
    nnode->next = NULL;

    if (*nstack == NULL) {
        *nstack = nnode;
    }
    else {
        nnode->next = *nstack;
        *nstack = nnode;
    }
    return;
}


char pop_cstack(CStack *cstack) {
    char operat;

    if (*cstack == NULL) {
        printf("The nstack is empty!");
        return operat;
    }

    operat = (*cstack)->operator;
    CNode *temp = *cstack;
    *cstack = temp->next;
    temp->next = NULL;
    free(temp);
    return operat;
}


Number pop_nstack(NStack *nstack) {
    if (nstack == NULL || *nstack == NULL) {
        printf("The nstack is empty!");
        return zero_number();
    }

    Number number = copy_number((*nstack)->number);
    NNode *temp = *nstack;
    *nstack = temp->next;

    temp->next = NULL;
    free(temp);
    return number;
}


int is_operator(char c) {
    if (c == '+' || c == '-' || c == '/' || c == '*') return 1;
    else return 0;
}


int is_number(char c) {
    if (c - '0' >= 0 && c - '0' <= 9) return 1;
    else return 0;
} 


int empty_cstack(CStack cstack) {
    if (cstack == NULL) return 1;
    else return 0; 
}


int empty_nstack(NStack nstack) {
    if (nstack == NULL) return 1;
    else return 0; 
}


char top_cstack(CStack cstack) {
    if (cstack == NULL) {
        printf("Error: Attempting to access top of an empty stack!\n");
        exit(1);
    }
    return cstack->operator;
}


Number top_nstack(NStack nstack) {
    if (nstack == NULL) {
        printf("Error: Attempting to access top of an empty stack!\n");
        exit(1);
    }
    return nstack->number;
}


int precedence(char c) {
    if (!is_operator(c)) {
        printf("Error: the character passed to the precedence function is not an operator.");
        exit(1);
    }

    switch (c) {
        case '*':
            return 20;

        case '/':
            return 20;
        
        case '-':
            return 10;

        case '+':
            return 10;

        default:
            return 0;
    }
    return 0;
}


void print_cstack(CStack cstack) {
    if (cstack == NULL) {
        printf("The cstack is empty.\n");
        return;
    }
    CStack temp = cstack;
    printf("CStack -> ");
    while (temp) {
        printf("%c -> ", temp->operator);
        temp = temp->next;
    }
    printf("NULL\n");
    return;
}


void print_nstack(NStack nstack) {
    if (nstack == NULL) {
        printf("The nstack is empty.\n");
        return;
    }

    NStack temp = nstack;
    printf("\nNStack -> \n");
    while (temp != NULL) {
        print_number(temp->number);
        printf(" -> \n");
        temp = temp->next;
    }
    printf("NULL\n");
}


Number evaluate_expression(char *expression) {
    if (expression == NULL) {
        printf("The input expression is empty!\n");
        exit(1);
    }

    CStack cstack; init_cstack(&cstack);
    NStack nstack; init_nstack(&nstack);

    char *digits = (char *)malloc(100 * sizeof(char));
    int index = 0, fraction_flag = 0, bracket_flag = 0, unary_flag = 1;

    for (int i = 0; i < strlen(expression); i++) {

        if (is_number(expression[i])) {
            if (index < 100) {
                digits[index++] = expression[i];
                unary_flag = 0;
            }
        } 
        
        else if (expression[i] == '.') {
        
            if (fraction_flag) {
                printf("Syntax Error: Multiple '.' in a number!\n");
                free(digits);
                exit(1);
            }
            if (index < 100) {
                digits[index++] = expression[i];
                fraction_flag = 1;
            }
        } 
        
        else if (expression[i] == '+' || expression[i] == '-') {

            fraction_flag = 0;
            if (unary_flag) {
                if (index < 100) {
                    digits[index++] = expression[i]; // Handle unary operator
                }
            } else {
                digits[index] = '\0';
                Number number1 = convert_to_number(digits);
                memset(digits, '\0', index);
                index = 0;

                push_nstack(&nstack, number1);

                while (!empty_cstack(cstack) && precedence(top_cstack(cstack)) >= precedence(expression[i])) {
                    char operator = pop_cstack(&cstack);
                    Number number2 = pop_nstack(&nstack);
                    number1 = pop_nstack(&nstack);
                    push_nstack(&nstack, evaluate(number1, number2, operator));
                    print_nstack(nstack);
                }
                push_cstack(&cstack, expression[i]);
                unary_flag = 1;
            }
        } 
        
        else if (expression[i] == '(') {

            bracket_flag++;
            push_cstack(&cstack, expression[i]);
            unary_flag = 1;
        } 
        
        else if (expression[i] == ')') {
            bracket_flag--;
            if (bracket_flag < 0) {
                printf("Syntax Error: Mismatched parentheses!\n");
                free(digits);
                exit(1);
            }

            if (index > 0) {
                digits[index] = '\0';
                Number number = convert_to_number(digits);
                memset(digits, '\0', index);
                index = 0;

                push_nstack(&nstack, number);
            }

            char operator;
            while ((operator = pop_cstack(&cstack)) != '(') {
                Number number2 = pop_nstack(&nstack);
                Number number1 = pop_nstack(&nstack);
                push_nstack(&nstack, evaluate(number1, number2, operator));
            }
        } 
        
        else if (is_operator(expression[i])) {

            if (index > 0) {
                digits[index] = '\0';
                Number number = convert_to_number(digits);
                memset(digits, '\0', index);
                index = 0;

                push_nstack(&nstack, number);
            }
            while (!empty_cstack(cstack) && precedence(top_cstack(cstack)) >= precedence(expression[i])) {
                char operator = pop_cstack(&cstack);
                Number number2 = pop_nstack(&nstack);
                Number number1 = pop_nstack(&nstack);
                push_nstack(&nstack, evaluate(number1, number2, operator));
            }
            push_cstack(&cstack, expression[i]);
            unary_flag = 1;
            fraction_flag = 0;
        }
    }

    if (bracket_flag != 0) {
        printf("Syntax Error: Unmatched parentheses!\n");
        free(digits);
        exit(1);
    }

    if (index > 0) {
        digits[index] = '\0';
        Number number = convert_to_number(digits);
        push_nstack(&nstack, number);
    }

    while (!empty_cstack(cstack)) {
        print_cstack(cstack); print_nstack(nstack);
        char operator = pop_cstack(&cstack);
        Number number2 = pop_nstack(&nstack); print_number(number2); printf("\n");
        Number number1 = pop_nstack(&nstack); print_number(number1); printf("\n");
        push_nstack(&nstack, evaluate(number1, number2, operator));
    }

    free(digits);

    Number result = pop_nstack(&nstack);
    return result;
}


int main() {
    Number n1, n2;
    char *num1 = "68432123543845.131", *num2 = "813556315.13521";
    char *digits = "-33.1653485842*(-56344566843.254384)";

    Number result = evaluate_expression(digits);
    print_number(result);
    printf("\n");
    return 0;
}