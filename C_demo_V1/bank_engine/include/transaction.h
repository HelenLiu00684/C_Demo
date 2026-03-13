/*
Transaction Module

The module defines the transaction data structure and declare the functions used by create transaction records.

TransactionType indicates the 8 different transaction types based on the services.
Savings: deposit/withdraw
Credit: credit_spend/credit_repay
Stock: stock_buy/stock_sell
Fund: fund_buy/fund_sell
*/
#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "error.h"
#include <time.h>
#include <stdbool.h>
typedef enum{
    DEPOSIT=0,
    WITHDRAW,

    CREDIT_SPEND,
    CREDIT_REPAY,

    STOCK_BUY,
    STOCK_SELL,

    FUND_BUY,
    FUND_SELL,
    NO_DEFINE
}TransactionType;
/* Defines each transaction recording*/
typedef struct{
    unsigned long id;
    TransactionType type;
    union{
        struct{
            double amount;
        } savings_credit;
        struct{
            double price;
            int quantity;
            char symbol[16];
        }asset;
    }data;
    time_t timestamp;
}Transaction;
/*Declares transaction of savings and credit functions*/
ErrorCode transaction_saving_credit_create(Transaction *tx,TransactionType type,double price);
/*Declares transaction of stock and fund functions*/
ErrorCode transaction_stock_fund_create(Transaction *tx,TransactionType type,double price,int quantity,const char *symbol);

#endif