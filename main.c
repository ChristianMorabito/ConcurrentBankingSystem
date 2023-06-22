#include <stdio.h>
#include "dynamic_list.h"
#include "account.h"



int main() {
    int account1 = initialiseAccount();
    int account2 = initialiseAccount();
    int account3 = initialiseAccount();

    deposit(account1, 5);
    deposit(account2, 10);
    deposit(account3, 15);
    deposit(account3, 20);
    printAccount(account1);
    printAccount(account2);
    printAccount(account3);

    return 0;
}
