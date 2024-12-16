#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "number.h"


void init_list(List *list_pointer) {
    *list_pointer = NULL;
    return;
}


void init_number(Number *number_pointer) {
    List list1, list2;
    init_list(&list1);
    init_list(&list2);

    number_pointer->integer = list1;
    number_pointer->fraction = list2;
    number_pointer->sign = '+';
    return;
}


void append(List *list_pointer, char d) {
    Node* node = (Node*)malloc(sizeof(Node));

    if (!node) { 
        printf("Memory was not allocated properly. Please retry!\n");
        exit(1);
    }

    else {
        node->val = d;
        node->next = NULL;
        node->prev = NULL;
    }

    if (*list_pointer == NULL) {
        *list_pointer = node;
    }
    
    else {
        Node* temp = *list_pointer;
        while (temp->next != NULL) temp = temp->next;
        
        temp->next = node;
        node->prev = temp;
    }

    return;
}


void insertbeg(List *list_pointer, char d) {
    Node* node = (Node*)malloc(sizeof(Node));

    if (!node) { 
        printf("Memory was not allocated properly. Please retry!\n");
        exit(1);
    }

    else {
        node->val = d;
        node->next = NULL;
        node->prev = NULL;
    }

    if (*list_pointer == NULL) {
        *list_pointer = node;
    }
    
    else {
        node->next = *list_pointer;
        (*list_pointer)->prev = node;
        *list_pointer = node;
    }

    return;
}


List convert_to_list(char *number) {
    List list;
    init_list(&list);
    
    int num_digits = strlen(number);
    int i = 0;
    
    if (number[0] == '-' || number[0] == '+') i++; 

    while (number[i]) {
        append(&list, number[i]);
        i += 1;
    }
    return list;
}


List zero_list() {
    List list;
    init_list(&list);

    append(&list, '0');
    return list;
}


Number zero_number() {
    Number number;
    init_number(&number);

    number.integer = zero_list();
    number.fraction = zero_list();
    number.sign = '+';

    return number;
}


Number convert_to_number(char *digits) {
    Number number;
    init_number(&number);

    int digit_length = strlen(digits);
    int i = 0;

    if (digits[0] == '+' || digits[0] == '-') {
        number.sign = digits[0];
        i += 1;
    }   
    else number.sign = '+';

    char _integer[1024] = "", _fraction[1024] = "";

    while (digits[i] && digits[i] != '.') {
        char temp[] = {digits[i], '\0'};
        strncat(_integer, temp, sizeof(_integer) - strlen(_integer) - 1);
        i += 1;
    }

    number.integer = convert_to_list(_integer);

    if (digits[i] == '.') 
        i += 1;

    if (!digits[i]) {
        number.fraction = zero_list();
    }

    else {
        while (digits[i]) {
            char temp[] = {digits[i], '\0'};
            strncat(_fraction, temp, sizeof(_integer) - strlen(_integer) - 1);
            i += 1;
        }

        number.fraction = convert_to_list(_fraction);
    }    
    
    return number;
}


void traverse(List list) {
    if (list == NULL) {
        printf("0");
        return;
    }

    List temp = list;
    
    while (temp) {
        printf("%c", temp->val);
        temp = temp->next;
    }
    return;
}


List copy_list(List list) {
    List copied_list;
    init_list(&copied_list);

    if (!list) {
        return copied_list;
    }
    while (list) {
        append(&copied_list, list->val);
        list = list->next;
    }
    return copied_list;
}


Number copy_number(Number number) {
    Number copied_number;
    init_number(&copied_number);

    copied_number.integer = copy_list(number.integer);
    copied_number.fraction = copy_list(number.fraction);
    copied_number.sign = number.sign;

    return copied_number;
} 


void print_list(List list) {
    if (!list) {
        printf("0");
        return;
    }

    Node *temp = list;

    while(temp->prev != NULL) temp = temp->prev;

    while (temp != NULL) {
        if (temp->prev != NULL) 
            printf("%c", temp->prev->val);
        printf("<=");
        
        printf("%c", temp->val);
        
        printf("=>");
        if (temp->next != NULL)
            printf("%c", temp->next->val);
        
        printf("\t->\t");
        temp = temp->next;
    }
    printf("\n");
    return;
}


void reverse(List *list_pointer) {
    if (!*list_pointer) return;

    Node* temp = *list_pointer;;

    while (temp->next) {
        List temp1 = temp->next;
        temp->next = temp->prev;
        temp->prev = temp1;

        temp = temp->prev;
    }
    *list_pointer = temp;
    List temp1 = temp->next;
    temp->next = temp->prev;
    temp->prev = temp1;
    return;
}


void print_number(Number number) {
    printf("%c", number.sign);
    traverse(number.integer);
    printf(".");
    traverse(number.fraction);
}


/*  Returns 0 if number1 > number2;
    Returns 1 if number1 < number2;
    Returns 2 if number1 = number2; */
int compare_numbers(Number number1, Number number2) {
    trim_zeroes(&number1);
    trim_zeroes(&number2);
    int negatives = 0;

    if (number1.sign == '-' && number2.sign == '+') return 1;
    if (number1.sign == '+' && number2.sign == '-') return 0;
    if (number1.sign == '-' && number2.sign == '-') negatives = 1;

    
    List integer1 = number1.integer;
    List integer2 = number2.integer;

    while (integer1 && integer2) {
        integer1 = integer1->next;
        integer2 = integer2->next;
    }

    if (integer1) {
        if (negatives) return 1;
        else return 0;
    }
    else if (integer2) {
        if (negatives) return 0;
        else return 1;
        }

    else {
        integer1 = number1.integer;
        integer2 = number2.integer;

        while (integer1 && integer2) {
            if (integer1->val > integer2->val) {
                if (negatives) return 1;
                else return 0;
            }
            else if (integer1->val < integer2->val) {
                if (negatives) return 0;
                else return 1;
            }
            else {
                integer1 = integer1->next;
                integer2 = integer2->next;
            } 
        }

        List fraction1 = number1.fraction;
        List fraction2 = number2.fraction;

        while (fraction1 && fraction2) {
            if (fraction1->val > fraction2->val) {
                if (negatives) return 1;
                else return 0;
            }
            else if (fraction1->val < fraction2->val) {
                if (negatives) return 0;
                else return 1;
            }
            else {
                fraction1 = fraction1->next;
                fraction2 = fraction2->next;
            }
        }
        
        if (fraction1) {
            if (negatives) return 1;
            else return 0;
        }
        else if (fraction2) {
            if (negatives) return 0;
            else return 1;
        }
        return 2;
    }
}


/*  Returns 0 if list1 > list2;
    Returns 1 if list1 < list2;
    Returns 2 if list1 = list2; */
int compare_integers(List list1, List list2) {
    trim_zeroes_integer(&list1); trim_zeroes_integer(&list2);

    List integer1 = list1;
    List integer2 = list2;

    while (integer1 && integer2) {
        integer1 = integer1->next;
        integer2 = integer2->next;
    }

    if (integer2 == NULL && integer1 != NULL) return 0;
    else if (integer1 == NULL && integer2 != NULL) return 1;

    else {
        integer1 = list1;
        integer2 = list2;

        while (integer1 && integer2) {
            if (integer1->val > integer2->val) return 0;
            else if (integer1->val < integer2->val) return 1;
            else {
                integer1 = integer1->next;
                integer2 = integer2->next;
            } 
        }
        return 2;
    }
    return -1;
}


/*  Returns 0 if list1 > list2;
    Returns 1 if list1 < list2;
    Returns 2 if list1 = list2; */
int compare_fractions(List list1, List list2) {
    List fraction1 = list1;
    List fraction2 = list2;

    while (fraction1 && fraction2) {
        if (fraction1->val > fraction2->val) return 0;
        else if (fraction1->val < fraction2->val) return 1;
        else {
            fraction1 = fraction1->next;
            fraction2 = fraction2->next;
        }
    }
    
    if (fraction1) return 0;
    else if (fraction2) return 1;
    return 2;
}


void trim_zeroes(Number *number_pointer) {
    List _integer = number_pointer->integer;
    List _fraction = number_pointer->fraction;

    while (_integer && _integer->val == '0') {
        List temp = _integer;
        _integer = _integer->next;
        free(temp);
    }
    
    if (!_integer) _integer = zero_list();
    else _integer->prev = NULL;

    number_pointer->integer = _integer;

    while (_fraction->next) _fraction = _fraction->next;
    while (_fraction && _fraction->val == '0') {
        List temp = _fraction;
        _fraction = _fraction->prev;
        free(temp);
    }

    if (!_fraction) number_pointer->fraction = zero_list();
    else _fraction->next = NULL;

    return;
}


void trim_zeroes_integer(List *list_pointer) {
    List _integer = *list_pointer;

    while (_integer && _integer->val == '0') {
        List temp = _integer;
        _integer = _integer->next;
        free(temp);
    }
    
    if (!_integer) _integer = zero_list();
    else _integer->prev = NULL;

    *list_pointer = _integer;
}


void append_list(List *destination, List source) {
    if (source == NULL) return;
    
    if (*destination == NULL) {
        *destination = source;
        return;
    }

    List temp = *destination;
    while (temp->next) temp = temp->next;

    temp->next = copy_list(source);
    temp->next->prev = temp;
    return;
}


int list_length(List list){
    int length = 0;
    List temp = list;

    while (temp) {
        length += 1; 
        temp = temp->next;
    }
    return length;
}


void free_list(List *list_pointer) {
    if (list_pointer == NULL || *list_pointer == NULL) return;

    List temp;
    while (*list_pointer != NULL) {
        temp = *list_pointer;
        *list_pointer = (*list_pointer)->next;
        free(temp);
    }

    *list_pointer = NULL;
}


// int main() {
//     char *digits1 = "-56415320.4957"; char *digits2 = "+35939064.398475";
//     Number number1 = convert_to_number(digits1); print_number(number1); printf("\n");
//     Number number2 = convert_to_number(digits2); print_number(number2); printf("\n");
//     List list1 = convert_to_list(digits1); traverse(list1); printf("\n");
//     List list2 = convert_to_list(digits2); traverse(list2); printf("\n");
//     printf("%d\n", compare_integers(list2, list1));
//     return 0;
// }