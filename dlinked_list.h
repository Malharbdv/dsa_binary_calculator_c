#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

typedef struct Node {
    int val;
    Node* next;
    Node* prev;
} Node;

typedef Node* List;

List init_list();
List convert_to_list(char *number);
void append(List list, char d);

#endif