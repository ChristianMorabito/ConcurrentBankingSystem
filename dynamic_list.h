#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include <stdlib.h>

typedef struct{
    size_t size;
    size_t filled;
    void** data;
}ListHead;

ListHead* createList();
int append(ListHead* head, void* data);

#endif //DYNAMIC_LIST_H
