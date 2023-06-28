#include "dynamic_list.h"
#include <stdio.h>

void freeList(ListHead** head, void (freeData)(void* data)){
    int filled = *(int* )(*head)->filled;
    for (int i = 0; i < filled; i++){
        freeData((*head)->data[0]);
    }
    free((*head)->data);
    free(*head);
}
ListHead* createList(){
    ListHead* head = malloc(sizeof(ListHead));
    if (!head){
        return NULL;
    }
    void* list = malloc(sizeof(void*) * 10);
    if (!list){
        return NULL;
    }
    head->data = list;
    head->size = 10;
    head->filled = 0;
    return head;
}

int append(ListHead* head, void* data){
    size_t size, filled;
    size = head->size;
    filled = head->filled;

    if ((size - filled) < 1){
        void* temp = realloc(head->data, sizeof(void*) * (size * 2));
        // if reallocation fails, nullify 'filled' point & return -1.
        if (!temp){
            head->data[filled] = NULL;
            return -1;
        }
        head->data = temp; // if reallocation succeeds, then 'temp' can now be the new head->data
        head->size *= 2;
    }
    head->data[filled] = data;
    head->filled++;

    return 0;
}

void memoryFailExit(){
    puts("Memory failed! Exiting");
    exit(-1);
}