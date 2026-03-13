#ifndef BANK_H
#define BANK_H
#include "customer.h"
#include "inventory.h"
#include <time.h>

typedef struct{
    Customer *ptrCustomer;
    BankInventory stock_inventory;
    BankInventory fund_inventory;
    size_t count;
    size_t capacity;
}Bank;
ErrorCode bank_init(Bank *bankPtr);
int bank_find_customer(const Bank *bankPtr, const char *name, const char *phone);
ErrorCode bank_add_customer(Bank *bankPtr,const char *name, const char *phone);
ErrorCode bank_remove_customer(Bank *bankPtr, const char *name,const char *phone);
static ErrorCode expand_Customers(Bank *bankPtr);

#endif