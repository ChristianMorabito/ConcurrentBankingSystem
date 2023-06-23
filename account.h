#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <pthread.h>

typedef struct{
    int account;
    int amount;
}AccountAmount;

int initialiseAccount();

int deposit(int account, int amount);

void withdraw(int account, int amount);

int getBalance(int account);

void* threadDeposit(void* amount);

void printAccount(int account);

int initialiseThread();

void programExit();

#endif //ACCOUNT_H
