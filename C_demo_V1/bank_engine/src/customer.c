
#include "customer.h"
#include "account.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_COUNT_TYPE 4
static unsigned long customer_id_default = 100001;



ErrorCode customer_init(Customer *customerPtr,const char *name,const char *phone){
    if(name == NULL||customerPtr == NULL||phone == NULL) return  ERROR_INVALID_ARGUMENT;
    strncpy(customerPtr->name,name,sizeof(customerPtr->name)-1);
    customerPtr->name[sizeof(customerPtr->name)-1]='\0';
    strncpy(customerPtr->phone,phone,sizeof(customerPtr->phone)-1);
    customerPtr->phone[sizeof(customerPtr->phone)-1]='\0';
    Account *accountptr= malloc(4*sizeof(Account));
    customerPtr->accounts_size = 0;
    if(accountptr != NULL){
        customerPtr->accounts = accountptr;
        customerPtr->customer_id = customer_id_default++;
    }else {
        return  ERROR_MEMORY_REALLOCATE;
    }
    for(int i =0;i<MAX_COUNT_TYPE;i++){
        customerPtr->accounts[i].type = -1;
    }
    customerPtr->timestamp = time(NULL);
    return ERROR_OK;
};
ErrorCode customer_destroy(Customer *customerPtr){
     if(customerPtr == NULL) return  ERROR_INVALID_ARGUMENT;
     if(customerPtr->accounts != NULL) free(customerPtr->accounts);
     return ERROR_OK;
};
int customer_find_account(const Customer *customerPtr,AccountType type){
    if(customerPtr == NULL || customerPtr->accounts == NULL) return -1;
    for(int i=0;i<MAX_COUNT_TYPE;i++){
        if(customerPtr->accounts[i].type == type)
         return i;
    }
    return -1;
};
ErrorCode customer_add_account(Customer *customerPtr,AccountType type){
    if (customerPtr==NULL) return ERROR_INVALID_ARGUMENT;
    int customer_find_index = customer_find_account(customerPtr,type);
    if (customer_find_index !=-1) 
        return  ERROR_ALREADY_EXISTS;
    else if (customerPtr->accounts_size == MAX_COUNT_TYPE)
        return  ERROR_ACCOUNT_REACHED_MAX;
    ErrorCode account_init_result = account_init(&customerPtr->accounts[type], type);
    if(account_init_result != ERROR_OK){
        return  ERROR_ACCOUNT_ADDED_FAILED;
    }
    customerPtr->accounts_size ++;
    return ERROR_OK;
};
ErrorCode customer_remove_account(Customer *customerPtr,AccountType type){
    if(customerPtr==NULL) return ERROR_INVALID_ARGUMENT;
    int customer_find_index = customer_find_account(customerPtr,type);
    if (customer_find_index ==-1) 
        return  ERROR_ALREADY_EXISTS;
    else{
        customerPtr->accounts[customer_find_index].type = -1;
        return ERROR_OK;
    }

};

void print_customer_detail(Customer *customerPtr){
    printf(
            "Customer\n"
            "Customer_id: %ld\n"
            "Name: %s\n"
            "Phone: %s\n"
            "Create time: %s",
            customerPtr->customer_id,
            customerPtr->name,
            customerPtr->phone,
            ctime(&customerPtr->timestamp));
    if(customerPtr->accounts_size == 0){
        printf("No active accounts....\n");
    }else{
        for(int i = 0;i<customerPtr->accounts_size;i++){
            switch(customerPtr->accounts[i].type){
                case Account_Savings:
                    printf("Saving account has been created.\n");
                    break;
                case Account_Credit:
                    printf("Credit account has been created.\n");
                    break;
                case Account_Stock:
                    printf("Stock account has been created.\n");
                    break;
                case Account_Fund:
                    printf("Fund account has been created.\n");
                    break;
            }              
        }
    }
};