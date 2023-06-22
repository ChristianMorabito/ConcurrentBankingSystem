#include "account.h"
#include "dynamic_list.h"
#include <stdio.h>

ListHead* listHead = NULL;

int initialiseAccount(){
    if (!listHead){
        listHead = createList();
        if (!listHead){
            puts("Memory failed! Exiting");
            exit(-1);
        }
    }
    int identifier = (int)listHead->filled;
    int* amount = malloc(sizeof(int));
    if (!amount){
        puts("Memory failed! Exiting");
        exit(-1);
    }
    *amount = 0;
    int status = append(listHead, amount);
    if (status == -1){
        puts("Memory failed! Exiting");
        exit(-1);
    }

    return identifier;
}

void deposit(int account, int amount){
    if (account < 0 || account > listHead->filled - 1){
        return;
    }
    *(int* )listHead->data[account] += amount;
}

void printAccount(int account){
    printf("Account %d: %d\n", account, *(int* )listHead->data[account]);

}