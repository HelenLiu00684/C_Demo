#include "error.h"
#include "account.h"
#include "transaction.h"
#include "symbolmap.h"
#include "inventory.h"
#include "bank.h"
#include <stdlib.h>

double calculate_new_avg(Bank *bank,const char *customerName,const char *customerPhone,const char *symbol,int quantityNew,double costNew){
    if(account == NULL || symbol == NULL || quan_num == 0 || price == 0)
        return  ERROR_INVALID_ARGUMENT;
    int exit_quantity = 0;
    double exit_avg_cost = 0;
    int index_customer=bank_find_customer(bankPtr, customerName, customerPhone);
    SymbolType symboltype = symbol_lookup(symbol);
    AccountType accounttype = NULL;
    AssetAccount assetaccount;
    if (symboltype == Symbol_Stock)
        assetaccount = stock;
        accounttype = Account_Stock;
    else if (symboltype == Symbol_Fund)
        assetaccount = fund;
        accounttype = Account_Fund;
    else 
        return -1;
    int index_account = customer_find_account(bank->customerPtr,accounttype);
    int index_position = account_find_position(&bank->ptrCustomer[index_customer]->accounts[index_account],symbol);
    exit_quantity = bank->ptrCustomer[index_customer]->accounts[index_account]->data.assetaccount->ptrPos[index_position].quantity;
    exit_avg_cost = bank->ptrCustomer[index_customer]->accounts[index_account]->data.assetaccount->ptrPos[index_position].avg_cost;
    double total_cost = exit_quantity * exit_avg_cost + quantityNew * costNew;
    return (total_cost/(exit_quantity + quantityNew));
}

ErrorCode apply_stockfund_operation(Bank *bank,Account * account,const char *symbol,int quan_num,double price){
        //1.confirm the parameter is valid

    if(account == NULL || symbol == NULL || quan_num == 0 || price == 0)
        return  ERROR_INVALID_ARGUMENT;
    ErrorCode acc_result=ERROR_WRONG;
    ErrorCode inventory_result=ERROR_WRONG;
    ErrorCode transaction_result=ERROR_WRONG;
    int trade_qty = abs(quan_num);
    int inventory_index = -1;
    int inventory_quantity = -1;
    int account_index = -1;
    int account_ass_quantity = -1;
    Transaction tx_entity;

    //2.identify the business content (saving/credit or stock/fund)

    //stock or fund:BankInventory
    AssetAccount *ass = NULL;
    TransactionType type = NO_DEFINE;
    SymbolType stype = symbol_lookup(symbol);

    if(stype == Symbol_Stock && account->type !=Account_Stock)
    return  ERROR_ACCOUNT_INVENTORY_MISMATCH;
    if(stype == Symbol_Fund && account->type !=Account_Fund)
    return  ERROR_ACCOUNT_INVENTORY_MISMATCH;

    BankInventory *invObjPtr=NULL;
    switch (stype){
        case Symbol_Stock: 
            invObjPtr= &bank->stock_inventory; 
            ass = &account->data.stock;
            if (quan_num > 0)
                type = STOCK_BUY;
            else
                type = STOCK_SELL;
            break;
        case Symbol_Fund: 
            invObjPtr= &bank->fund_inventory; 
            ass = &account->data.fund;
            if (quan_num > 0)
                type = FUND_BUY;
            else
                type = FUND_SELL;
            break;
        default:
            return ERROR_INVALID_SN;
    }

    //3.check the quantity form bank inventory and compare with quan_num to makesure the number is reasonble or not
    inventory_index = inventory_find_position(invObjPtr,symbol);
    if(inventory_index != -1){
        inventory_quantity = invObjPtr->ptrPos[inventory_index].quantity;
    }else{
        return  ERROR_INVALID_SN;
    }

    account_index = account_find_position(account, symbol);
     if(type == STOCK_SELL || type == FUND_SELL ){
        if(account_index!= -1){
            account_ass_quantity = ass->ptrPos[account_index].quantity;
            if(account_ass_quantity < trade_qty){
                return  ERROR_INSUFFENT_ACCOUNT_QUANTITY;
            }
        }else{
            return ERROR_INVALID_SN;
            }
    }   

    if(type == STOCK_BUY || type == FUND_BUY ){
        if(inventory_quantity < trade_qty){
            return  ERROR_INSUFFENT_INVENTORY; // inventory_quantity is not enough.
        }      
    }


    //4.if the number is reasonable then change the bank inventory and the account     
    //4.1 first step: check this is buy or sell
    acc_result = account_update_position(account,symbol,quan_num);    
    if(acc_result==ERROR_OK){
        int inven_quan_num = -quan_num;
        inventory_result = inventory_update_quantity(invObjPtr,symbol,inven_quan_num);
        if(inventory_result==ERROR_OK){
            transaction_result = transaction_stock_fund_create(&tx_entity,type,price,quan_num,symbol);
            if(transaction_result==ERROR_OK){
                account_append_transaction(account,&tx_entity);
                return ERROR_OK;
            }else{
                inventory_result = inventory_update_quantity(invObjPtr,symbol,quan_num);
                acc_result = account_update_position(account,symbol,-quan_num);
                if(acc_result == ERROR_OK && inventory_result == ERROR_OK) 
                    return ERROR_ROLLBACK_OK;
                else return ERROR_ROLLBACK_WRONG;

            }
        }else{
            acc_result = account_update_position(account,symbol,-quan_num);
            if(acc_result == ERROR_OK) return ERROR_ROLLBACK_OK;
            else return ERROR_ROLLBACK_WRONG;
        }
    }else{
        return ERROR_INVLID_ACCOUNT_UPDATE;
        }

}    

ErrorCode apply_savingcreditoperation(Account * account,double price){
    if(account == NULL || price == 0)
        return   ERROR_INVALID_ARGUMENT;
    ErrorCode acc_result=ERROR_WRONG;
    ErrorCode transaction_result=ERROR_WRONG;
    double withDrawPrice = 0;
    double creditExpense = 0;
    Transaction tx_entity;

    //1.confirm the parameter is valid.
    if(account == NULL || price == 0)
        return     ERROR_INVALID_ARGUMENT;
    //2.identify the business content (saving/credit)
    TransactionType type = NO_DEFINE;
    switch (account->type){
        case Account_Savings: 
            if (price > 0)
                type = DEPOSIT;
            else
                type = WITHDRAW;
                withDrawPrice = -price;
            break;
        case Account_Credit: 
            if (price > 0){
                type = CREDIT_SPEND;
                creditExpense = price;
            }
            else
                type = CREDIT_REPAY;
            break;
    }


    //3.check the withdraw from bank or credit expense is be allowed
    
    if(account->data.savings.balance < withDrawPrice && type == WITHDRAW){
        return  ERROR_INSUFFENT_SAVING; // inventory_quantity is not enough.
    } 
    if(account->data.credit.credit_limit + account->data.credit.credit_used < creditExpense && type == CREDIT_SPEND ){
        return  ERROR_EXCEED_CREDIT_LIMITATION; // inventory_quantity is not enough.
    }
    acc_result = account_update_saving_credit(account,price);
    if(acc_result==ERROR_OK){
        transaction_result = transaction_saving_credit_create(&tx_entity,type,price);
        if(transaction_result==ERROR_OK){
            account_append_transaction(account,&tx_entity);
            return ERROR_OK;
        }else{
            acc_result = account_update_saving_credit(account,-price);
            if(acc_result == ERROR_OK) 
                return ERROR_ROLLBACK_OK;
            else return ERROR_ROLLBACK_WRONG;
            }
        }
        return ERROR_INVLID_ACCOUNT_UPDATE;
        
}