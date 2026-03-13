#ifndef CUSTOMER_H
#define CUSTOMER_H
#include <time.h>
#include "account.h"
typedef struct{
    unsigned long customer_id;
    char name[64];
    Account *accounts;
    char phone[20];
    time_t timestamp;
}Customer;
ErrorCode customer_init(Customer *customerPtr,const char *name,const char *phone);
ErrorCode customer_destroy(Customer *customerPtr);
ErrorCode customer_add_account(Customer *customerPtr,AccountType type);
ErrorCode customer_remove_account(Customer *customerPtr,AccountType type);
void print_account_type(void);
#endif 