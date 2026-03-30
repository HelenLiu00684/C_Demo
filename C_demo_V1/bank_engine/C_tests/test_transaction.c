/*gcc tests/test_transaction.c src/transaction.c -Iinclude -o tests/test_transaction*/
#include "transaction.h"
#include <assert.h>
#include <stdio.h>
#define RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin....\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finished....\n",#functionName);\
        printf("************************************************\n");\
    }while(0);


static void test_savings_credit_valid();
static void test_asset_valid();
static void test_savings_credit_invalid();
static void test_asset_invalid();

int main() {
    RUN_TEST(test_savings_credit_valid);
    RUN_TEST(test_asset_valid);
    RUN_TEST(test_savings_credit_invalid);
    RUN_TEST(test_asset_invalid);
    printf("\nAll transaction unit tests passed.\n");
}
/* Test valid savings and credit transactions */
static void test_savings_credit_valid(){
    Transaction txObj;
    ErrorCode err;

    err = transaction_saving_credit_create(&txObj,DEPOSIT,1000);

    assert(err == ERROR_OK);
    assert(txObj.type == DEPOSIT);
    assert(txObj.data.savings_credit.amount == 1000);
    /* verify transaction ID */
    assert(txObj.id >= 202603060001);
    printf("[INFO]id = %lu\n",txObj.id);
    /* verify timestamp */
    assert(txObj.timestamp >0);
    printf("[INFO]timestamp = %s",ctime(&txObj.timestamp));
    printf("[INFO]test_savings_credit_valid_DEPOSIT passed.\n");

    err = transaction_saving_credit_create(&txObj,WITHDRAW,1500);
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_WITHDRAW passed.\n");
    err = transaction_saving_credit_create(&txObj,CREDIT_SPEND,2000);
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_CREDIT_SPEND passed.\n");
    err = transaction_saving_credit_create(&txObj,CREDIT_REPAY,2500);
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_CREDIT_REPAY passed.\n");
}
/* Test valid asset transactions */
static void test_asset_valid(){
    Transaction txObj;
    ErrorCode err;

    err = transaction_stock_fund_create(&txObj,STOCK_BUY,101.99,100,"ABCD-1234");
    assert(err == ERROR_OK);
    assert(txObj.type == STOCK_BUY);
    assert(txObj.data.asset.price == 101.99);
    assert(txObj.data.asset.quantity == 100);
    /* verify transaction ID */
    assert(txObj.id >= 202603060001);
    printf("[INFO]id = %lu\n",txObj.id);
    /* verify timestamp */
    assert(txObj.timestamp >0);
    printf("[INFO]timestamp = %s",ctime(&txObj.timestamp));
    printf("[INFO]test_savings_credit_valid_STOCK_BUY passed.\n");

    err = transaction_stock_fund_create(&txObj,STOCK_SELL,202.99,200,"ABCD-1234");
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_STOCK_SELL passed.\n");
    err = transaction_stock_fund_create(&txObj,FUND_BUY,303.99,300,"ABCD-1234");
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_FUND_BUY passed.\n");
    err = transaction_stock_fund_create(&txObj,FUND_SELL,404.99,400,"ABCD-1234");
    assert(err == ERROR_OK);
    printf("[INFO]test_savings_credit_valid_FUND_SELL passed.\n");
}
/* Test invalid savings and credit transactions */
static void test_savings_credit_invalid(){
    Transaction txObj;
    ErrorCode err;

    err = transaction_saving_credit_create(&txObj,DEPOSIT,-1000);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_savings_credit_invalid_NEGATIVE passed.\n");

    err = transaction_saving_credit_create(&txObj,FUND_BUY,1000);
    assert(err == ERROR_INVALID_TRANSCATION_TYPE);
    printf("[INFO]test_savings_credit_invalid_MISTYPE passed.\n");

}

/* Test invalid asset transactions */
static void test_asset_invalid(){
    Transaction txObj;
    ErrorCode err;

    err = transaction_stock_fund_create(&txObj,STOCK_BUY,-101.99,100,"ABCD-1234");
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_transaction_stock_fund_create_invalid_NEGATIVE_PRICE passed.\n");

    err = transaction_stock_fund_create(&txObj,STOCK_BUY,101.99,-100,"ABCD-1234");
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_transaction_stock_fund_create_invalid_NEGATIVE_QUANTITY passed.\n");

    err = transaction_stock_fund_create(&txObj,STOCK_BUY,101.99,100,NULL);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_transaction_stock_fund_create_invalid_EMPTY_SYMBOL passed.\n");

    err = transaction_stock_fund_create(&txObj,STOCK_BUY,101.99,100,"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_transaction_stock_fund_create_invalid_TOOLONG_SYMBOL passed.\n");

    err = transaction_stock_fund_create(&txObj,DEPOSIT,101.99,100,"ABCD-1234");
    assert(err == ERROR_INVALID_TRANSCATION_TYPE);
    printf("[INFO]test_transaction_stock_fund_create_invalid_MISTYPE_SYMBOL passed.\n");
}