#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

typedef struct Node {
    int val;
    struct Node* next;
    struct Node* prev;
} Node;

typedef Node* List;

typedef struct Number {
    List integer;
    List fraction;
    char sign;
} Number;

void init_list(List *list_pointer);
void init_number(Number *number_pointer);
void append(List *list_pointer, char d);
void insertbig(List *list_pointer, char d);
List convert_to_list(char *number);
Number convert_to_number(char *digits);
void reverse(List *list_pointer);
List copy_list(List list);
void traverse(List list);
void print_list(List list);
void print_number(Number number);
int compare(Number number1, Number number2);
void trim_zeroes(Number *number_pointer);

#endif