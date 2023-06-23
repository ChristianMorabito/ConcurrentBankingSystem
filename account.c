#include "account.h"
#include "dynamic_list.h"
#include <pthread.h>
#include <stdio.h>


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
ListHead* accountList = NULL;
ListHead* threadList = NULL;

int initialiseAccount(){
    if (!accountList) accountList = createList(); // create account list.
    if (!accountList) memoryFailExit(); // force exit if malloc fails
    int* amount = malloc(sizeof(int)); // create space for account
    if (!amount) memoryFailExit();  // force exit if malloc fails
    *amount = 0; // initialise account balance
    int identifier = (int)accountList->filled; // this is the account ID to be returned
    int accountStatus = append(accountList, amount); // append account to dynamic list
    if (accountStatus == -1) memoryFailExit();
    return identifier;
}

int initialiseThread(){
    pthread_t* newThread = malloc(sizeof(pthread_t));
    threadList = createList();
    int identifier = (int)threadList->filled;
    append(threadList, newThread);
    return identifier;
}

int deposit(int account, int amount){
    pthread_mutex_lock(&lock); // lock
    int* resultPtr;
    AccountAmount arg = {account, amount};
    pthread_create(threadList->data[account], NULL, threadDeposit, &arg);
    pthread_join((pthread_t)threadList->data[account], (void** )&resultPtr);
    int result = *resultPtr;
    free(resultPtr);
    pthread_mutex_unlock(&lock); // unlock
    return result; // returns 0 or -1 based on success of deposit
}

void* threadDeposit(void* voidArg){
    int* result = malloc(sizeof(int));
    AccountAmount arg = *(AccountAmount*)voidArg;
    int account = arg.account; int amount = arg.amount;
    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1){
        *result = -1;
        return result; // if account invalid, return -1 (as ptr)
    }
    // otherwise, add amount to account & return 0 (as ptr)
    *(int* )accountList->data[account] += amount;
    *result = 0;
    return result;
}

void withdraw(int account, int amount) {
    if (account < 0 || account > accountList->filled - 1){
        return;
    }
    if (*(int* )accountList->data[account] - amount < 0){
        return;
    }

    *(int* )accountList->data[account] -= amount;

}

void printAccount(int account){
    printf("Account %d: %d\n", account, *(int* )accountList->data[account]);
}

int getBalance(int account){
    return *(int*)accountList->data[account];
}

pthread_t* mallocThread(){
    pthread_t* threadPtr = malloc(sizeof(pthread_t));
    if (!threadPtr) memoryFailExit(); // force exit if malloc fails
    return threadPtr;
}