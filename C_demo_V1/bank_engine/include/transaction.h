#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <time.h>
typedef enum{
    DEPOSIT=0,
    WITHDRAW,

    CREDIT_SPEND,
    CREDIT_REPAY,

    STOCK_BUY,
    STOCK_SELL,

    FUND_BUY,
    FUND_SELL
}TransactionType;

typedef struct{
    unsigned long id;
    TranscationType type;
    double price;
    double quantity;
    double amount;

    char symbol[16];
    time_t timestamp;
}Transaction;
#endif

ErrorCode transaction_create(Transaction *tx,unsigned long id,TransactionType type,double price,int quantity,double amount,const char *symbol);
bool transaction_validation(const Transaction* tx);
ErrorCode transaction_destroy(Transaction* tx);