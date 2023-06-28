#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include <stdlib.h>

typedef struct{
    size_t size;
    size_t filled;
    void** data;
}ListHead;

void freeList(ListHead** head, void (freeData)(void* data));

ListHead* createList();

int append(ListHead* head, void* data);

void memoryFailExit();

#endif //DYNAMIC_LIST_H
