#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "transaction.h"
#include <stddef.h> 
typedef struct{
    char symbol[16];
    double quantity;//User holding

}Position;

typedef struct{
    Position *ptrPos;
    size_t capacity; //size_t is unsign type Max position of each account
    size_t count;//how many kind of positions
}AssetAccount;

typedef enum{
    Account_Savings = 0,
    Account_Credit,
    Account_Stock,
    Account_Fund,
}AccountType;

typedef struct{
    AccountType type;

    Transaction *tx;
    size_t tx_capacity;
    size_t tx_count;

    union{
        struct{
            double balance;
        } savings;
        struct{
            double credit_limit;
            double credit_used;
        } credit;
        AssetAccount stock;
        AssetAccount fund;
    } data;
}Account;
ErrorCode account_init(Account *acc, AccountType type);
int account_find_position(Account *account, const char *symbol);
ErrorCode account_destroy(Account* acc);
ErrorCode account_append_transaction(Account* acc, const Transaction* tx);
ErrorCode account_add_position(Account *account, const char *symbol,double quantity);
ErrorCode account_update_position(Account *account,const char *symbol,double quantity);
ErrorCode account_update_saving_credit(Account *account,double price);
#endif 