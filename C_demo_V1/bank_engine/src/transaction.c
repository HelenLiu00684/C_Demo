/*
Transaction Implementation

Implements functions to create two types of transaction:
Savings/Credit or Stock/Fund.

*/
#include "transaction.h"
#include "error.h"
#include <time.h>
#include <string.h>
/*Generate a global transaction ID*/
static unsigned long next_tx_id = 202603060001;
/* Create a savings or credit transaction */
ErrorCode transaction_saving_credit_create(Transaction *tx,TransactionType type,double amount){
        if (tx == NULL)
            return ERROR_INVALID_POINTER;
        if (amount <= 0)
            return ERROR_INVALID_ARGUMENT;
        if (type != DEPOSIT &&
            type != WITHDRAW &&
            type != CREDIT_SPEND &&
            type != CREDIT_REPAY )
            return ERROR_INVALID_TRANSCATION_TYPE;
        tx->timestamp = time(NULL);
        tx->id = next_tx_id++;
        tx->type = type;
        tx->data.savings_credit.amount = amount;
  
        return ERROR_OK;
    }
/* Create a stock or fund transaction */
ErrorCode transaction_stock_fund_create(Transaction *tx,TransactionType type,double price,int quantity,const char *symbol){
    if (tx == NULL)
        return ERROR_INVALID_POINTER;
    if (price <= 0 || quantity <=0)
        return ERROR_INVALID_ARGUMENT;
    if (symbol == NULL)
        return ERROR_INVALID_ARGUMENT;
    if(strlen(symbol)>=sizeof(tx->data.asset.symbol))
        return ERROR_INVALID_ARGUMENT;
    if (type != STOCK_BUY &&
        type != STOCK_SELL &&
        type != FUND_BUY &&
        type != FUND_SELL )
        return ERROR_INVALID_TRANSCATION_TYPE;
    tx->timestamp = time(NULL);
    tx->id = next_tx_id++;
    tx->data.asset.quantity = quantity;
    tx->data.asset.price = price;
    tx->type = type;
    strncpy(tx->data.asset.symbol,symbol,sizeof(tx->data.asset.symbol)-1); //copy string array to an array and set the last char = '\0'
    tx->data.asset.symbol[sizeof(tx->data.asset.symbol)-1]='\0';        
    return ERROR_OK;

};
