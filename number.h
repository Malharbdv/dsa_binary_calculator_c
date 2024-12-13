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

void init_list(List *);
void init_number(Number *);
void append(List *, char);
void insertbeg(List *, char);
List convert_to_list(char *);
Number convert_to_number(char *);
void reverse(List *);
List copy_list(List);
Number copy_number(Number);
void traverse(List);
void print_list(List);
void print_number(Number);
int compare_numbers(Number, Number);
int compare_integers(List, List);
int compare_fractions(List, List);
void trim_zeroes(Number *);
void trim_zeroes_integer(List *);
void append_list(List*, List);
int list_length(List);

#endif