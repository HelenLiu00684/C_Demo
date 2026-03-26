#include "transaction.h"
#include "account.h"
#include "error.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define capacity_size 20
static ErrorCode expand_transactions(Account* account);
static int find_position(AssetAccount *asset, const char *symbol);
static ErrorCode expand_positions(AssetAccount *asset);
/* Initialize the account based on accoutType only.*/
ErrorCode account_init(Account *account, AccountType type){
    ErrorCode err;
    if(account == NULL)
        return  ERROR_INVALID_ARGUMENT;
    account->type = type;
    account->tx = NULL;
    account->tx_capacity = 0;
    account->tx_count = 0;
    switch(type){
        case Account_Savings: 
            account->data.savings.balance = 0;

            break;
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
/* Destory the account */
ErrorCode account_free(Account* account){
    if (account == NULL) 
        return ERROR_INVALID_POINTER;

    free(account->tx);
    
    if(account->type == Account_Stock){
        free (account->data.stock.ptrPos); // Only malloc items need to be free: dynamic Array
    }
    else if(account->type == Account_Fund){
        free (account->data.fund.ptrPos);
    }

    return ERROR_OK;
}
/* ADD the transaction under the account when capacity is insufficient */
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
/* Expand the transaction array when capacity is insufficient */
static ErrorCode expand_transactions(Account* account){
    if(account->tx_capacity == 0){
        account->tx = malloc(capacity_size*sizeof(Transaction));
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
/* Add one item to the stock or fund account, don't need input price, just created.*/
ErrorCode account_add_position(Account *account, const char *symbol,int quantity){
    if(account == NULL || symbol ==NULL || quantity <= 0)
        return ERROR_INVALID_ARGUMENT;

    AssetAccount *asset = NULL;
 
    if (account->type == Account_Stock) 
        asset = &account->data.stock;
    else if (account->type == Account_Fund)
        asset = &account->data.fund;
    else
        return ERROR_INVALID_TYPE;

    if(asset->count*10>=asset->capacity*8){
        ErrorCode err = expand_positions(asset);
        if (err !=ERROR_OK)
            return err;
    }

    int pos = -1;
    pos = account_find_position(account, symbol);
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
        Position* tmpptr = malloc(capacity_size * sizeof(Position));
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
Sell or Buy a asset and update the quantity in fund or stock.
1.find the index of symbol
2.if index = -1 invoke add_position to create a position in asset structures.
3.if index != -1 new_quantity = old + delta
new_quantity:
    >0 → update
    <0 → error
    ==0 → remove
*/
ErrorCode account_update_position(Account *account,const char *symbol,int quantity){ //the quantity can be +/-
    double new_quantity= 0 ;
    if(account == NULL || symbol ==NULL||quantity == 0 )
        return ERROR_INVALID_ARGUMENT;
    int index = account_find_position(account, symbol);
    AssetAccount *asset = NULL;
    if (account->type == Account_Stock)
        asset = &account->data.stock;
    else if (account->type == Account_Fund)
        asset = &account->data.fund;
    else return ERROR_INVALID_TYPE;

    if (index == -1 && quantity > 0) {
        return account_add_position(account, symbol, quantity);// This symbol hasn't exist in 
    }else if (index == -1 && quantity < 0){
        return ERROR_INVLID_ACCOUNT_UPDATE;
    }
    new_quantity = asset->ptrPos[index].quantity + quantity;
    if (new_quantity >0 ){// Number is reasonable, the quantity will be changed
        asset->ptrPos[index].quantity = new_quantity;//change the quantity
        return ERROR_OK;
    }
    else if (new_quantity < 0){
        return ERROR_INSUFFENT_ACCOUNT_QUANTITY;
    }else{
        for(int i=index; i<asset->count-1; i++){
            asset->ptrPos[i]=asset->ptrPos[i+1];//decrease the item and arrange the access_list
        }            
        asset->count--;//the total number reduce 
    }            
    return ERROR_OK;
}
/* Update the money business money and credit card */
ErrorCode account_update_saving_credit(Account *account,double amount){
    if(account == NULL || amount ==0)
        return ERROR_INVALID_POINTER;
    if (account->type == Account_Savings) 
        account->data.savings.balance = account->data.savings.balance + amount;
    else{
        account->data.credit.credit_used = account->data.credit.credit_used + amount;
        account->data.credit.credit_limit = account->data.credit.credit_limit - amount;
    }
    return ERROR_OK;
}

/*Update the avg_cost when account buy or sell asset, Engine will calculate the avg_cost.*/
ErrorCode account_set_avg_cost(Account *account, const char *symbol, double avg_cost){
    if(account == NULL || symbol == NULL || avg_cost == 0)
        return ERROR_INVALID_ARGUMENT;
    int index = -1;
    index = account_find_position(account, symbol);
    AssetAccount asset;
    if(account->type == Account_Stock)
        asset = account->data.stock;
    else if(account->type == Account_Fund)
        asset = account->data.fund;
    asset.ptrPos[index].avg_cost = avg_cost;
    return ERROR_OK;
}