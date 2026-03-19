/*
Inventory Implementation

This module implements the inventory management functions.

It provides functionality to:

- initialize the inventory structure
- update asset quantities when accounts buy or sell assets
- add asset quantities held by the bank
*/
#include "inventory.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define QUANTITY_DEFAULT 0
#define PRICE_DEFAULT 0
static int find_bank_position(BankInventory *inv,const char *symbol);
static ErrorCode expand_inventory(BankInventory *inv);
static double inventory_average_cost(BankInventory *inv,const char *symbol,int quantity,double cost);
/* Initializes the inventory structure */
ErrorCode inventory_init(BankInventory *inv,SymbolType type){
    if (inv==NULL)
        return ERROR_INVALID_ARGUMENT;
    if (type!=Symbol_Stock && type!=Symbol_Fund)
        return ERROR_INVALID_SYMBOL;
    inv->type = type;
    inv->capacity = 0;
    inv->count = 0;
    inv->ptrPos = NULL;
    return ERROR_OK;
}
/* Internal helper function that finds the index of a symbol in the inventory */
static int find_bank_position(BankInventory *inv,const char *symbol){
    if(inv==NULL || symbol==NULL)
        return -1;

    for(int i=0;i<inv->count;i++){
        if(strcmp(inv->ptrPos[i].symbol,symbol)==0)
            return i;
    }
    return -1;  
}
/* Finds the index of a specific asset in the inventory */
int inventory_find_position(BankInventory *inv,const char *symbol){
    if(inv==NULL || symbol==NULL)
        return -1;
    return find_bank_position(inv,symbol);    
};
/* Adds a new asset position into the inventory */
ErrorCode inventory_add_position(BankInventory *inv,const char *symbol,int quantity,double cost){
    double avg_cost = 0;
    if(inv==NULL || symbol==NULL || quantity <0 || symbol[0] == '\0'|| cost <0)
        return ERROR_INVALID_ARGUMENT;

    int pos = inventory_find_position(inv,symbol);
    if (pos!=-1){
            return ERROR_ALREADY_EXISTS;
        }
    if(inv->count*10>=inv->capacity*8||inv->capacity == 0){
        ErrorCode err = expand_inventory(inv);
        if(err!=ERROR_OK)
            return  ERROR_EXPAND_FAILED;
    }
    strncpy(inv->ptrPos[inv->count].symbol,symbol,sizeof(inv->ptrPos[inv->count].symbol)-1);
    inv->ptrPos[inv->count].symbol[sizeof(inv->ptrPos[inv->count].symbol)-1] ='\0';
    inv->ptrPos[inv->count].quantity = quantity;
    inv->ptrPos[inv->count].avg_cost = cost;
    inv->ptrPos[inv->count].total_cost =  inv->ptrPos[inv->count].avg_cost * quantity;
    inv->count++;
    return ERROR_OK;

}
/* Updates the quantity of an asset position due to buy  */
ErrorCode inventory_buy_position(BankInventory *inv,const char *symbol,int quantity,double cost){

    ErrorCode err=ERROR_UNKNOWN;
    if(inv==NULL || symbol==NULL ||quantity <= 0||cost<=0)
        return ERROR_INVALID_ARGUMENT;
    int index = inventory_find_position(inv,symbol);
    if(index==-1){
        err = inventory_add_position(inv,symbol,quantity,cost);
        return ERROR_OK;
    }
  
    inv->ptrPos[index].avg_cost = inventory_average_cost(inv,symbol,quantity,cost);
    // inv->ptrPos[index].quantity = inv->ptrPos[index].quantity + quantity;
    // inv->ptrPos[index].total_cost = inv->ptrPos[index].avg_cost * inv->ptrPos[index].quantity ;


    return ERROR_OK;
}

/* Updates the quantity of an asset position due to sell operations */
ErrorCode inventory_sell_position(BankInventory *inv,const char *symbol,int quantity){

    if(inv==NULL || symbol==NULL || quantity<=0)
        return ERROR_INVALID_ARGUMENT;
    int index = inventory_find_position(inv,symbol);
    if(index==-1){
        return ERROR_NOT_FOUND;
    }
    if(inv->ptrPos[index].quantity - quantity > 0){
        inv->ptrPos[index].quantity = inv->ptrPos[index].quantity - quantity;
        inv->ptrPos[index].total_cost = inv->ptrPos[index].avg_cost * inv->ptrPos[index].quantity ;
        return ERROR_OK;
    }else if (inv->ptrPos[index].quantity - quantity < 0){
        return ERROR_INSUFFICIENT_INVENTORY;
    }else{
        for(int i=index;i<inv->count-1;i++){
            inv->ptrPos[i]=inv->ptrPos[i+1];
        }
        inv->count--;
        inv->ptrPos[index].total_cost = 0;
        
    }
    return ERROR_OK;
}

/* Calculate the avg_cost of the asset */
static double inventory_average_cost(BankInventory *inv,const char *symbol,int quantity,double cost){
    // int total_quantity = 0 ;
    // double total_cost = 0 ;
    int index = inventory_find_position(inv,symbol);
    // printf("The old quantity is %d\n",inv->ptrPos[index].quantity);
    // printf("The old avg_price is %.2f\n",inv->ptrPos[index].avg_cost);
    // printf("The old total_cost is %.2f\n",inv->ptrPos[index].avg_cost*inv->ptrPos[index].quantity);
    inv->ptrPos[index].total_cost = inv->ptrPos[index].quantity * inv->ptrPos[index].avg_cost + quantity * cost;
    inv->ptrPos[index].quantity = quantity + inv->ptrPos[index].quantity;
    // printf("The new total_cost is %.2f\n",inv->ptrPos[index].total_cost);
    // printf("The new quantity is %d\n",inv->ptrPos[index].quantity);
    // printf("The new avg_price is %.2f\n",(inv->ptrPos[index].total_cost/inv->ptrPos[index].quantity));
    return (inv->ptrPos[index].total_cost/inv->ptrPos[index].quantity);
}

/* Internal function used to expand the inventory capacity */
static ErrorCode expand_inventory(BankInventory *inv){
    if(inv->capacity == 0 ){
        size_t initial_size = 10;
        BankPosition *tmpptr = malloc(initial_size * sizeof(BankPosition));
        if(tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        inv->ptrPos = tmpptr;
        inv->capacity = initial_size;
    }else{
        size_t new_BankPosition_capacity = 2 * inv->capacity;
        BankPosition *tmpptr = realloc(inv->ptrPos,new_BankPosition_capacity * sizeof(BankPosition));
        if (tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        inv->capacity = new_BankPosition_capacity;
        inv->ptrPos = tmpptr;
    }
    return ERROR_OK;
}
/* Free all asset in the inventory */
void inventory_free(BankInventory *inv)
{
    if(inv==NULL) return;

    free(inv->ptrPos);
    inv->ptrPos = NULL;
    inv->capacity = 0;
    inv->count = 0;
}

