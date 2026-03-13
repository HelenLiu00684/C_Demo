#include "bank.h"
#include "customer.h"
#include "inventory.h"
#include "error.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define initial_size 16
#define capacity_size 20
static ErrorCode expand_Customers(Bank *bankPtr);
ErrorCode bank_init(Bank *bankPtr){
    ErrorCode err;
    if(bankPtr==NULL) return ERROR_INVALID_ARGUMENT;
    bankPtr->capacity = 20;
    Customer *customerptr = malloc(bankPtr->capacity*sizeof(Customer));
    if(customerptr==NULL) return ERROR_MEMORY_REALLOCATE;
    bankPtr->ptrCustomer = customerptr;
    bankPtr->count = 0;
    err = inventory_init(&bankPtr->stock_inventory,Symbol_Stock);
    if(err != ERROR_OK)
        return ERROR_WRONG;
    err = inventory_init(&bankPtr->fund_inventory,Symbol_Fund);
    if(err != ERROR_OK)
        return ERROR_WRONG;
    return ERROR_OK;
};
int bank_find_customer(const Bank *bankPtr, const char *name, const char *phone){
    if(bankPtr==NULL) return -1;
    for(int i=0;i<bankPtr->count;i++){
        if(strcmp(bankPtr->ptrCustomer[i].name,name)==0) return i;
    }
    for(int i=0;i<bankPtr->count;i++){
        if(strcmp(bankPtr->ptrCustomer[i].phone,phone)==0) return i;
    }
    return -1;
};
ErrorCode bank_add_customer(Bank *bankPtr,const char *name, const char *phone){
    if(bankPtr==NULL) return ERROR_INVALID_ARGUMENT;
    int customer_index = bank_find_customer(bankPtr, name, phone);
    if(customer_index!=-1){
        return  ERROR_ALREADY_EXISTS;
    }
    if(bankPtr->count*10 >bankPtr->capacity*8) expand_Customers(bankPtr);
    ErrorCode customer_init_result = customer_init(&bankPtr->ptrCustomer[bankPtr->count],name,phone);
    if(customer_init_result==ERROR_OK) bankPtr->count++;
    return ERROR_OK;
};
ErrorCode bank_remove_customer(Bank *bankPtr, const char *name,const char *phone){
    if(bankPtr==NULL) return ERROR_INVALID_ARGUMENT;
    int customer_index = bank_find_customer(bankPtr, name, phone);
    if(customer_index==-1){
        return  ERROR_ALREADY_NOEXISTS;
    }
    customer_destroy(&bankPtr->ptrCustomer[customer_index]);
    for(int i=customer_index;i<bankPtr->count-1;i++){
        bankPtr->ptrCustomer[i]=bankPtr->ptrCustomer[i+1];
        
    }
    bankPtr->count--;
    free(&bankPtr->ptrCustomer[customer_index]);
    return ERROR_OK;

};

static ErrorCode expand_Customers(Bank *bankPtr){
    if(bankPtr->capacity == 0){
        
        bankPtr->ptrCustomer = malloc(initial_size*sizeof(Customer));
        if( bankPtr->ptrCustomer == NULL)
            return ERROR_MEMORY_REALLOCATE;
         bankPtr->capacity = capacity_size;
    }else if( bankPtr->count*10 >bankPtr->capacity*8){
        size_t new_tx_capacity = bankPtr->capacity *2;
        Customer* tmpptr = realloc(bankPtr->ptrCustomer,new_tx_capacity*sizeof(Customer)); // copy old data to a new area, and return a new pointer
        if (tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        bankPtr->ptrCustomer = tmpptr; // update the new locate of the ptr
        bankPtr->capacity = new_tx_capacity; 
        
    }
    return ERROR_OK;
}