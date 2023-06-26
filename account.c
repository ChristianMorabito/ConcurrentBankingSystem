#include "account.h"
#include "dynamic_list.h"
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
ListHead* accountList = NULL;
ListHead* threadList = NULL;

int* mallocInt(){
    int* amount = malloc(sizeof(int)); // create space for account
    if (!amount) memoryFailExit();  // force exit if malloc fails
    *amount = 0; // initialise account balance
    return amount;
}

int initialiseAccount(){
    if (!accountList){
        accountList = createList(); // create account list.
        threadList = createList(); // create thread list
    }
    if (!accountList || !threadList) memoryFailExit(); // force exit if malloc fails
    int* amount = mallocInt();
    int identifier = (int)accountList->filled; // this is the account ID to be returned
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

    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1) return -1;
//    pthread_mutex_lock(&lock); // lock
    AccountAmount* argPtr = malloc(sizeof(AccountAmount));
    AccountAmount arg = {account, amount};
    argPtr = &arg;
    pthread_create(threadList->data[account], NULL, threadDeposit, &argPtr);
//    pthread_join((pthread_t)threadList->data[account],NULL); //TODO: do safety return
//    pthread_mutex_unlock(&lock); // unlock
    return 0; //TODO: do safety return
}

void* threadDeposit(void* voidArg){
    AccountAmount* arg = voidArg;
    int account = arg->account; int amount = arg->amount;
    *(int* )accountList->data[account] += amount;
    return NULL;
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