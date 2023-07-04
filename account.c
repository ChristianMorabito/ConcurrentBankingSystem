#include "account.h"
#include "dynamic_list.h"
#include <pthread.h>
#include <stdio.h>

ListHead* accountList = NULL;
ListHead* threadList = NULL;
ListHead* mutexList = NULL;

int* mallocInt(){
    int* amount = malloc(sizeof(int)); // create space for account
    if (!amount) memoryFailExit();  // force exit if malloc fails
    *amount = 0; // initialise account balance
    return amount;
}

void initLists(){
    if (!accountList){
        accountList = createList(); // create account list.
        threadList = createList(); // create thread list.
        mutexList = createList(); // create mutex list.
    }
    if (!accountList) memoryFailExit(); // force exit if malloc fails
}

int initialiseAccount(){
    initLists(); // create x3 lists, for holding accounts, threads & mutexes
    int identifier = (int)accountList->filled; // this is the account ID to be returned
    int* amount = mallocInt(); // create memory to hold account balance. This will be attached to the accountList
    int accountStatus = append(accountList, amount); // append account to dynamic list
    if (accountStatus == -1) memoryFailExit();
    initialiseThread(identifier); // initialise thread & mutex
    return identifier;
}

void initialiseMutex(int index){
    pthread_mutex_t* newMutex = malloc(sizeof(pthread_mutex_t));
    if (!newMutex) memoryFailExit();
    append(mutexList, newMutex);
    pthread_mutex_init(mutexList->data[index], NULL);
}

void initialiseThread(int index){
    pthread_t* newThread = malloc(sizeof(pthread_t));
    if (!newThread) memoryFailExit();
    append(threadList, newThread);
    initialiseMutex(index);
}

int deposit(int account, int amount){
    pthread_t* thread = threadList->data[account];
    if (account < 0 || account > accountList->filled - 1) return -1; // check if account number is in a valid range
    AccountAmount* arg = malloc(sizeof(AccountAmount)); // create struct to pass data to thread
    arg->account = account; arg->amount = amount;
    // create struct to pass as function argument ( in pthread_create() )
    if (pthread_create(thread, NULL, threadDeposit, arg)) threadCreateFail();
    return 0;
}

void* threadDeposit(void* voidArg){
    AccountAmount* arg = voidArg; // typecast voidArg back to struct
    int account = arg->account; int amount = arg->amount;
    pthread_mutex_lock((pthread_mutex_t* )&mutexList->data[account]);
    *(int* )accountList->data[account] += amount; // increase account balance
    pthread_mutex_unlock((pthread_mutex_t* )&mutexList->data[account]);
    free(voidArg);
    return NULL;
}

int withdraw(int account, int amount) {
    pthread_t* thread = threadList->data[account];
    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1) return -1;
    AccountAmount* arg = malloc(sizeof(AccountAmount)); // create struct to pass data to thread
    arg->account = account; arg->amount = amount;
    if (pthread_create(thread, NULL, threadWithdraw, arg)) threadCreateFail();
    return 0;
}

void* threadWithdraw(void* voidArg){
    AccountAmount* arg = voidArg; // typecast voidArg back to struct
    int account = arg->account; int amount = arg->amount;
    pthread_mutex_lock((pthread_mutex_t* )&mutexList->data[account]);
    *(int* )accountList->data[account] -= amount; // increase account balance
    pthread_mutex_unlock((pthread_mutex_t* )&mutexList->data[account]);
    free(voidArg);
    return NULL;
}

int getBalance(int account){
    // check if account number is in a valid range
    if (account < 0 || account > accountList->filled - 1) return -1;
    return *(int*)accountList->data[account];
}

void threadCreateFail(){
    printf("Thread create fail!\n");
    exit(-1);
}


void programExit(){
    freeList(&accountList, (void* )freeAccounts);
    freeList(&threadList, (void* )freeThreads);
    freeList(&mutexList, (void* )freeMutexes);
}

void freeAccounts(int* account){
    free(account);
}

void freeThreads(pthread_t* thread){
    free(thread);

}

void freeMutexes(pthread_mutex_t* mutex){
    free(mutex);
}