#include "transaction.h"
#include "account.h"
#include "error.h"
#include <stdio.h>
#define MAX_TX 1000000
#define MIN_TX 10000
#define MAX_ACCOUNT 100
/*
gcc python3_test/test_transaction_append_stress.c \
    src/transaction.c \
    src/account.c \
    -Iinclude \
    -o python3_test/test_transaction_append_stress
*/

#define RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin......\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finish...... \n",#functionName);\
        printf("************************************************\n");\
}while(0);

static void test_single_account_saving_tx();
static void test_single_account_stock_tx();
static void test_100_single_account_tx();
static void test_100_multi_account_tx();
int main(){
    RUN_TEST(test_single_account_saving_tx);
    printf("****************test_single_account_saving_tx finished.....****************\n");
    RUN_TEST(test_single_account_stock_tx);
    printf("****************test_single_account_stock_tx finished.....****************\n");
    RUN_TEST(test_100_single_account_tx);
    printf("****************test_100*single_account_tx finished.....****************\n");
    RUN_TEST(test_100_multi_account_tx);
    printf("****************test_100*multi_account_tx finished.....****************\n");
    printf("\nAll account append tx tests passed.\n");    
}


static void test_single_account_saving_tx(){
    printf("****************1. Single account append 1000000 tx stress****************\n");
    Account accountObj;
    account_init(&accountObj, Account_Savings);
    for(int i=0;i<MIN_TX;i++){
        Transaction txOBJ;
        transaction_saving_credit_create(&txOBJ,DEPOSIT,11*i+1.01);
        account_append_transaction(&accountObj, &txOBJ);
    if(i % 1000 == 0)
        printf("The %d has passed......\n",i);
    }
    printf(
        "The total count is: %ld\n"
        "The total capacity is: %ld\n",
        accountObj.tx_count,
        accountObj.tx_capacity);
    account_free(&accountObj);

}

static void test_single_account_stock_tx(){
    printf("****************2. Single stock account append 1000000 tx stress****************\n");
    Account accountObj;
    account_init(&accountObj, Account_Stock);
    for(int i=0;i<MIN_TX;i++){
        Transaction txOBJ;
        char symbol[16];
        snprintf(symbol,sizeof(symbol),"AAA%03d",i);
        transaction_stock_fund_create(&txOBJ,STOCK_BUY,11*i+1.01,11*i,symbol);
        account_append_transaction(&accountObj, &txOBJ);
    if(i % 1000 == 0)
        printf("The %d has passed......\n",i);
    }
    printf(
        "The total count is: %ld\n"
        "The total capacity is: %ld\n",
        accountObj.tx_count,
        accountObj.tx_capacity);
    account_free(&accountObj);
}

static void test_100_single_account_tx(){
    printf("****************3. Single saving account append 1000000 tx stress****************\n");
    for(int i = 0;i<MAX_ACCOUNT;i++){
        test_single_account_saving_tx();
    }
};
static void test_100_multi_account_tx(){
    printf("****************4. Multi account append 1000000 tx stress****************\n");
    for(int i = 0;i<MAX_ACCOUNT;i++){
        test_single_account_saving_tx();
    }
    for(int i = 0;i<MAX_ACCOUNT;i++){
        test_single_account_stock_tx();
    }
};
