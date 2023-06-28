#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

typedef struct{
    int account;
    int amount;
}AccountAmount;

int initialiseAccount();

int deposit(int account, int amount);

int withdraw(int account, int amount);

void* threadWithdraw(void* voidArg);

int getBalance(int account);

void* threadGetBalance(void* voidArg);

void* threadDeposit(void* voidArg);

int initialiseThread();

void programExit();

void freeAccounts(int* account);

void freeThreads(pthread_t* thread);

void threadCreateFail();

void threadJoinFail();

int* mallocInt();

void initLists();

#endif //ACCOUNT_H
