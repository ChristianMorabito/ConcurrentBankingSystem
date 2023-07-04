#include <stdio.h>
#include "account.h"
#include <unistd.h>


int main() {
    // Initialize accounts
    int account1 = initialiseAccount();
    int account2 = initialiseAccount();
    int account3 = initialiseAccount();

    // Perform deposits
    deposit(account1, 1000);
    deposit(account1, 1000);
    deposit(account2, 2000);
    deposit(account3, 3000);

    // Perform withdrawals
    withdraw(account1, 500);
    withdraw(account2, 800);
    withdraw(account3, 1200);

    sleep(1);

    // Show balances
    printf("Balance of account 1: %d\n", getBalance(account1));
    printf("Balance of account 2: %d\n", getBalance(account2));
    printf("Balance of account 3: %d\n", getBalance(account3));

// TODO: DESTROY MUTEXes pthread_mutex_destroy(&mutex[i])

    // Free memory
    programExit();

    return 0;
}
