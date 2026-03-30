#ifndef ACCOUNT_H
#define ACCOUNT_H
#include "transaction.h"
#include <stddef.h> 
#define capacity_size 20
#define CREDIT_INIT_LIMIT 5000
typedef struct{
    char symbol[16];
    int quantity;   //User holding quantity
    double avg_cost;   //The average price of each Position
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
/* Initialize the account based on accoutType only.*/
ErrorCode account_init(Account *acc, AccountType type);
/* Search for a symbol inside an AssetAccount and return its index, or -1 if not found. BUY
SELL/UPDATE/DELETE/QUER*/
int account_find_position(Account *account, const char *symbol);
/* Destory the account */
ErrorCode account_free(Account* acc);
/* ADD the transaction under the account when capacity is insufficient */
ErrorCode account_append_transaction(Account* acc, const Transaction* tx);
/* Add one item to the stock or fund account, don't need input price, just created.*/
ErrorCode account_add_position(Account *account, const char *symbol,int quantity);
/*Sell or Buy a asset and update the quantity in fund or stock.*/
ErrorCode account_update_position(Account *account,const char *symbol,int quantity);
/* Update the money business money and credit card */
ErrorCode account_update_saving_credit(Account *account,double price);
/* Set the avg_cost for the assets */
ErrorCode account_set_avg_cost(Account *account, const char *symbol, double avg_cost);
void print_account(Account *account);
void asset_SF_print(AssetAccount *assetObjPtr,int max_count);
#endif 