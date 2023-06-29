#include "account.h"
#include "dynamic_list.h"
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

ListHead* accountList = NULL;
ListHead* threadList = NULL;

int* mallocInt(){
    int* amount = malloc(sizeof(int)); // create space for account
    if (!amount) memoryFailExit();  // force exit if malloc fails
    *amount = 0; // initialise account balance
    return amount;
}

void initLists(){
    if (!accountList){
        accountList = createList(); // create account list.
        threadList = createList(); // create thread list
    }
    if (!accountList) memoryFailExit(); // force exit if malloc fails

}

int initialiseAccount(){
    initLists(); // create x2 lists. one to hold accounts & another to hold threads.
    int identifier = (int)accountList->filled; // this is the account ID to be returned
    int* amount = mallocInt(); // create memory to hold account balance. This will be attached to the accountList
    int accountStatus = append(accountList, amount); // append account to dynamic list
    if (accountStatus == -1) memoryFailExit();
    initialiseThread();
    return identifier;
}

int initialiseThread(){
    pthread_t* newThread = malloc(sizeof(pthread_t));
    if (!newThread) memoryFailExit();
    int identifier = (int)threadList->filled;
    append(threadList, newThread);
    return identifier;
}

int deposit(int account, int amount){
    pthread_t* thread = threadList->data[account];
    if (account < 0 || account > accountList->filled - 1) return -1; // check if account number is in a valid range
    AccountAmount arg = {account, amount}; // create struct to pass as function argument ( in pthread_create() )
    if (pthread_create(thread, NULL, threadDeposit, &arg)) threadCreateFail();
    if (pthread_join(*thread, NULL)) threadJoinFail();
    return 0;
}

void* threadDeposit(void* voidArg){
    AccountAmount* arg = voidArg; // typecast voidArg back to struct
    int account = arg->account; int amount = arg->amount;
    *(int* )accountList->data[account] += amount; // increase account balance
    return NULL;
}

int withdraw(int account, int amount) {
    pthread_t* thread = threadList->data[account];
    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1) return -1;
    // check if withdrawal doesn't leave balance below 0.
    if (*(int* )accountList->data[account] - amount < 0) return -1;
    AccountAmount arg = {account, amount}; // create struct to pass as function argument ( in pthread_create() )
    if (pthread_create(thread, NULL, threadWithdraw, &arg)) threadCreateFail();
    if (pthread_join(*thread, NULL)) threadJoinFail();
    return 0;
}

void* threadWithdraw(void* voidArg){
    AccountAmount* arg = voidArg; // typecast voidArg back to struct
    int account = arg->account; int amount = arg->amount;
    *(int* )accountList->data[account] -= amount; // increase account balance
    return NULL;
}

int getBalance(int account){
    int* balance;
    pthread_t* thread = threadList->data[account];
    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1) return -1;
    if (pthread_create(thread, NULL, threadGetBalance, &account)) threadCreateFail();
    if (pthread_join(*thread, (void** )&balance)) threadJoinFail();
    return *balance;
}

void* threadGetBalance(void* voidArg){
    int account = *(int* )voidArg;
    return accountList->data[account];
}

void threadCreateFail(){
    printf("Thread create fail!\n");
    exit(-1);
}

void threadJoinFail(){
    printf("Thread create fail!\n");
    exit(-1);
}

void programExit(){
    freeList(&accountList, (void* )freeAccounts);
    freeList(&threadList, (void* )freeThreads);
}

void freeAccounts(int* account){
    free(account);
}

void freeThreads(pthread_t* thread){
    free(thread);

}