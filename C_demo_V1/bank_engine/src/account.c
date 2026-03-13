#include "transaction.h"
#include "account.h"
#include "error.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define initial_size 16
#define capacity_size 20
static ErrorCode expand_transactions(Account* account);
static int find_position(AssetAccount *asset, const char *symbol);
static ErrorCode expand_positions(AssetAccount *asset);

ErrorCode account_init(Account *account, AccountType type){
    if(account == NULL)
        return ERROR_INVALID_POINTER;
    account->type = type;
    account->tx = NULL;
    account->tx_capacity = 0;
    account->tx_count = 0;
    switch(type){
        case Account_Savings: account->data.savings.balance = 0;break;
        case Account_Credit: 
            account->data.credit.credit_limit = 0;
            account->data.credit.credit_used = 0;
            break;
        case Account_Stock: 
            account->data.stock.ptrPos = NULL;
            account->data.stock.capacity = 0;
            account->data.stock.count = 0;
            break;
        case Account_Fund: 
            account->data.fund.ptrPos = NULL;
            account->data.fund.capacity = 0;
            account->data.fund.count = 0;
            break;
    }
    return ERROR_OK;    
}

ErrorCode account_destroy(Account* account){
    if (account == NULL) return ERROR_INVALID_POINTER;

    free(account->tx);
    
    if(account->type == Account_Stock){
        free (account->data.stock.ptrPos); // Only malloc items need to be free: dynamic Array
    }
    else if(account->type == Account_Fund){
        free (account->data.fund.ptrPos);
    }

    
    return ERROR_OK;
}

ErrorCode account_append_transaction(Account* account, const Transaction* tx){
    if(account==NULL||tx==NULL) return ERROR_INVALID_POINTER;
    ErrorCode returnCode = expand_transactions(account);
    if (returnCode != ERROR_OK){
        return returnCode;
    }
    account->tx[account->tx_count]=*tx; // connect the new node to the ptr eg:memcpy(&acc->tx[acc->tx_count], tx, sizeof(Transaction));
    account->tx_count++;    
    
    return ERROR_OK;
}

static ErrorCode expand_transactions(Account* account){
    if(account->tx_capacity == 0){
        account->tx = malloc(initial_size*sizeof(Transaction));
        if(account->tx == NULL)
            return ERROR_MEMORY_REALLOCATE;
        account->tx_capacity = capacity_size;
    }else if(account->tx_count*10 >account->tx_capacity*8){
        size_t new_tx_capacity = account->tx_capacity *2;
        Transaction* tmpptr = realloc(account->tx,new_tx_capacity*sizeof(Transaction)); // copy old data to a new area, and return a new pointer
        if (tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        account->tx = tmpptr; // update the new locate of the ptr
        account->tx_capacity = new_tx_capacity; 
        
    }
    return ERROR_OK;
}
/* Search for a symbol inside an AssetAccount and return its index, or -1 if not found. BUY
SELL/UPDATE/DELETE/QUER*/

int account_find_position(Account *account, const char *symbol){
    if(account == NULL || symbol ==NULL)
        return -1;
    if (account->type == Account_Stock) 
        return find_position(&account->data.stock,symbol);
    else if (account->type == Account_Fund)
        return find_position(&account->data.fund,symbol);
    return -1;
}

static int find_position(AssetAccount *asset, const char *symbol){
    for(int i=0; i<asset->count;i++){
        if(strcmp(asset->ptrPos[i].symbol,symbol) == 0) //find the same name
            return i; 
    }
    return -1; 
}

ErrorCode account_add_position(Account *account, const char *symbol,double quantity){
    if(account == NULL || symbol ==NULL)
        return ERROR_INVALID_POINTER;

    AssetAccount *asset=NULL;
    if (account->type == Account_Stock) 
        asset = &account->data.stock;
    else if (account->type == Account_Fund)
        asset = &account->data.fund;
    else
        return ERROR_INVALID_TYPE;

    if(asset->count*10>=asset->capacity*8){
        ErrorCode err = expand_positions(asset);
        if (err !=ERROR_OK)
            return ERROR_EXPAND_FAILED;
    }

    int pos = account_find_position(account, symbol);
    if (pos !=-1){
        return ERROR_ALREADY_EXISTS;
    }

    strncpy(asset->ptrPos[asset->count].symbol, symbol, sizeof(asset->ptrPos[asset->count].symbol)-1);
    asset->ptrPos[asset->count].symbol[sizeof(asset->ptrPos[asset->count].symbol)-1]='\0';
    asset->ptrPos[asset->count].quantity = quantity;
    asset->count++;
    return ERROR_OK;    
}        

/* Expand the position array when capacity is insufficient */
static ErrorCode expand_positions(AssetAccount *asset){
    if(asset->capacity == 0){
        Position* tmpptr = malloc(initial_size * sizeof(Position));
        if(tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        asset->capacity = capacity_size;
        asset->ptrPos = tmpptr;
    }else{
        size_t new_position_capacity = asset->capacity *2;
        Position* tmpptr = realloc(asset->ptrPos,new_position_capacity * sizeof(Position)); // copy old data to a new area, and return a new pointer
        if (tmpptr == NULL)
            return ERROR_MEMORY_REALLOCATE;
        asset->ptrPos = tmpptr; // update the new locate of the ptr
        asset->capacity = new_position_capacity; 
        
    }
    return ERROR_OK;
}
/*
find index
        ↓
if -1 → add_position
        ↓
get asset
        ↓
new_quantity = old + delta
        ↓
>0 → update
<0 → error
==0 → remove
*/
ErrorCode account_update_position(Account *account,const char *symbol,double quantity){ //the quantity can be +/-
    if(account == NULL || symbol ==NULL)
        return ERROR_INVALID_POINTER;
    int index = account_find_position(account, symbol);
    AssetAccount *asset = NULL;
    if (account->type == Account_Stock)
        asset = &account->data.stock;
    else if (account->type == Account_Fund)
        asset = &account->data.fund;
    else ERROR_INVALID_TYPE;

    if (index == -1) {
        if (quantity <= 0)
            return ERROR_INVALID_ARGUMENT;
        return account_add_position(account, symbol, quantity);// This symbol hasn't exist in 
    }
        
        //double temp_quantity = fabs(quantity); // ignore the postive or negtive
    if (asset->ptrPos[index].quantity + quantity > 0){// Number is reasonable, the quantity will be changed
        asset->ptrPos[index].quantity = asset->ptrPos[index].quantity + quantity;//change the quantity
        return ERROR_OK;
    }
    else if (asset->ptrPos[index].quantity + quantity < 0){
        return ERROR_INVALID_ARGUMENT;
    }else{
        for(int i=index; i<asset->count-1; i++){
            asset->ptrPos[i]=asset->ptrPos[i+1];//decrease the item and arrange the access_list
        }            
        asset->count--;//the total number reduce 
    }
            
    
    return ERROR_OK;

}

ErrorCode account_update_saving_credit(Account *account,double price){
    if(account == NULL || price ==0)
        return ERROR_INVALID_POINTER;
    if (account->type == Account_Savings) 
        account->data.savings.balance = account->data.savings.balance + price;
    else{
        account->data.credit.credit_used = account->data.credit.credit_used + price;
        account->data.credit.credit_limit = account->data.credit.credit_limit - price;
    }
    return ERROR_OK;
}