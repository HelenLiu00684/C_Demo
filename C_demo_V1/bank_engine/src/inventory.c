#include "inventory.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>
#define QUANTITY_DEFAULT 0
#define PRICE_DEFAULT 0

static ErrorCode expand_inventory(BankInventory *inv);

ErrorCode inventory_init(BankInventory *inv,SymbolType type){
    if (inv==NULL)
        return ERROR_INVALID_ARGUMENT;
    inv->type = type;
    inv->capacity = 0;
    inv->count = 0;
    inv->ptrPos = NULL;
    return ERROR_OK;
}

static int find_bank_position(BankInventory *inv,const char *symbol){
    if(inv==NULL || symbol==NULL)
        return -1;

    for(int i=0;i<inv->count;i++){
        if(strcmp(inv->ptrPos[i].symbol,symbol)==0)
            return i;
    }
    return -1;  
};
int inventory_find_position(BankInventory *inv,const char *symbol){
    if(inv==NULL || symbol==NULL)
        return -1;
    return find_bank_position(inv,symbol);    
};
ErrorCode inventory_add_position(BankInventory *inv,const char *symbol,int quantity,double price){
    if(inv==NULL || symbol==NULL)
        return ERROR_INVALID_ARGUMENT;
    if(inv->count*10>=inv->capacity*8||inv->capacity == 0){
        ErrorCode err = expand_inventory(inv);
        if(err!=ERROR_OK)
            return  ERROR_EXPAND_FAILED;
    }
    int pos = inventory_find_position(inv,symbol);
    if (pos!=-1){
            return ERROR_ALREADY_EXISTS;
        }
    strncpy(inv->ptrPos[inv->count].symbol,symbol,sizeof(inv->ptrPos[inv->count].symbol)-1);
    inv->ptrPos[inv->count].symbol[sizeof(inv->ptrPos[inv->count].symbol)-1] ='\0';
    inv->ptrPos[inv->count].quantity = quantity;
    inv->ptrPos[inv->count].price = price;
    inv->count++;
    return ERROR_OK;

};
ErrorCode inventory_update_quantity(BankInventory *inv,const char *symbol,int quantity){
    if(inv==NULL || symbol==NULL)
        return ERROR_INVALID_ARGUMENT;
    int index = inventory_find_position(inv,symbol);
    if(index==-1){
        return ERROR_INVALID_ARGUMENT;
    }
    if(inv->ptrPos[index].quantity + quantity > 0){
        inv->ptrPos[index].quantity = inv->ptrPos[index].quantity + quantity;
        return ERROR_OK;
    }else if (inv->ptrPos[index].quantity + quantity < 0){
        return ERROR_INVALID_ARGUMENT;
    }else{
        for(int i=index;i<inv->count-1;i++){
            inv->ptrPos[i]=inv->ptrPos[i+1];
        }
        inv->count--;
        
    }
    return ERROR_OK;
};
ErrorCode inventory_update_price(BankInventory *inv,const char *symbol,double price){
    if(inv==NULL || symbol==NULL)
        return ERROR_INVALID_ARGUMENT;
    int index = inventory_find_position(inv,symbol);
    if(index==-1){
        return inventory_add_position(inv,symbol,QUANTITY_DEFAULT,price);
    }
    inv->ptrPos[index].price=price;
    return ERROR_OK;
};

static ErrorCode expand_inventory(BankInventory *inv){
    if(inv->capacity == 0 ){
        size_t initial_size = 16;
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
};

