/*gcc tests/test_account.c src/account.c -Iinclude -o tests/test_account 
./tests/test_account*/
#include <assert.h>
#include "account.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#define RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin......\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finish...... \n",#functionName);\
        printf("************************************************\n");\
}while(0);

static void test_account_init();
static void test_account_add_position();
static void test_account_update_position();
// static void test_account_find_position();
// static void test_account_append_transaction();


// static void test_account_update_saving_credit();

int main(){
    RUN_TEST(test_account_init);
    RUN_TEST(test_account_add_position);
    RUN_TEST(test_account_update_position);
    // RUN_TEST(test_account_find_position);
    // RUN_TEST(test_account_append_transaction);


    // RUN_TEST(test_account_update_saving_credit);
    printf("\nAll account unit tests passed.\n");    
}

static void test_account_init(){
    Account accountObj;
    ErrorCode err;
    err = account_init(&accountObj, Account_Savings);
    assert(err == ERROR_OK && accountObj.type == Account_Savings && accountObj.data.savings.balance == 0);
    printf("[INFO]test_account_init_Saving passed.\n");
    err = account_init(&accountObj, Account_Credit);
    assert(err == ERROR_OK && accountObj.type == Account_Credit && accountObj.data.credit.credit_limit == 0 && accountObj.data.credit.credit_used == 0);
    printf("[INFO]test_account_init_Credit passed.\n");
    err = account_init(&accountObj, Account_Stock);
    assert(err == ERROR_OK && accountObj.type == Account_Stock && accountObj.data.stock.ptrPos == NULL && accountObj.data.stock.capacity == 0 && accountObj.data.stock.count == 0);
    printf("[INFO]test_account_init_Stcok passed.\n");
    err = account_init(&accountObj, Account_Fund);
    assert(err == ERROR_OK && accountObj.type == Account_Fund && accountObj.data.fund.ptrPos == NULL && accountObj.data.fund.capacity == 0 && accountObj.data.fund.count == 0);
    printf("[INFO]test_account_init_Fund passed.\n");
    err = account_init(NULL, Account_Fund);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_account_init_Missing_Argument passed.\n");
};
static void test_account_add_position(){
        /*  
        0.normal add
        1.add another one
        2.add duplicate
        3.parameter check
        5.extend invalid
    */
    /*        0.normal add      */
    Account accountObj;
    ErrorCode err;
    account_init(&accountObj, Account_Stock);
    err = account_add_position(&accountObj, "AAAB", 100);
    printf("DEBUG after add account type %d\n", accountObj.type);
    printf("DEBUG after add account symbol %s\n", accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].symbol);
    printf("DEBUG after add account asset quantity %d\n", accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].quantity);
    printf("DEBUG after add account asset count %ld\n", accountObj.data.stock.count);
    assert(accountObj.type == Account_Stock );
    assert(strcmp(accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].symbol,"AAAB")==0);
    assert(accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].quantity == 100);
    assert(accountObj.data.stock.count == 1);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_add_one item passed.\n");

    /*        1.add another one      */
    err = account_add_position(&accountObj, "BBBC", 200);
    printf("DEBUG after add account count %ld\n", accountObj.data.stock.count);
    for(int i=0;i<=(accountObj.data.stock.count-1);i++){
        printf("DEBUG after add another account symbol  %s\n", accountObj.data.stock.ptrPos[i].symbol);
        printf("DEBUG after add account asset quantity %d\n", accountObj.data.stock.ptrPos[i].quantity);
    }
    assert(accountObj.data.stock.count == 2);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_add_other item passed.\n");
    /*        2.add duplicate      */
    err = account_add_position(&accountObj, "AAAB", 300);
    printf("DEBUG after add account count %ld\n", accountObj.data.stock.count);
    assert(accountObj.data.stock.count == 2);
    assert(err == ERROR_ALREADY_EXISTS);
    printf("[INFO]test_account_add_duplicated item passed.\n");
    /*        3.parameter check      */
    err = account_add_position(NULL,"AAAB", 100);
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_add_position(&accountObj,NULL, 100);
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_add_position(&accountObj,"AAAB", -100);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_account_add_parameter_quanity passed.\n");

    /*        5.extend invalid      */
    for(int i=0;i<=37;i++){
        char symbol[16];
        sprintf(symbol,"AAA%03d",i);
        account_add_position(&accountObj,symbol,400+i);
    }
    printf("[DEBUG] expand: new capacity = %zu\n",accountObj.data.stock.capacity);
    assert(accountObj.data.stock.count == 40);
    assert(accountObj.data.stock.capacity == 80);
    printf("[INFO]test_account_extend invalid passed.\n");
    account_free(&accountObj);

};
static void test_account_update_position(){

        /*  

        0.buy a new item
        1.normal update buy
        2.normal update sell
        3.normal buy duplicate
        4.normal sell duplicate > 0
        5.normal sell duplicate = 0
        6.normal sell duplicate < 0
        7.extend normal sell an intermediate item item = 0,delete the item normally
        8.sell a no-existing item
        9.parameter checking
        */

    /*        0.buy a new item        */
    Account accountObj;
    ErrorCode err;
    account_init(&accountObj, Account_Stock);
    printf("[DEBUG] Initial: initial count = %zu\n",accountObj.data.stock.count);
    err = account_update_position(&accountObj,"AAAB",100);
    int index = account_find_position(&accountObj,"AAAB")
    printf("[DEBUG] New First Item: New count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] New First Item: New symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    assert(accountObj.data.stock.ptrPos[index].quantity == 100);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"AAAB")==0);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update buy a new item passed.\n");

    /*        1.normal update buy        */        
    err = account_update_position(&accountObj,"AAAB",200);
    int index = account_find_position(&accountObj,"AAAB")
    printf("[DEBUG] Buy an existing Item: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Buy an existing Item: symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    printf("[DEBUG] Buy an existing Item: quantity = %d\n",accountObj.data.stock.ptrPos[index].quantity);
    assert(accountObj.data.stock.count == 1);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"AAAB")==0);
    assert(accountObj.data.stock.ptrPos[index].quantity == 300);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update normal update buy passed.\n");
    /*        2.normal update sell        */        
    err = account_update_position(&accountObj,"AAAB",-150);
    int index = account_find_position(&accountObj,"AAAB")
    printf("[DEBUG] Buy an existing Item: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Buy an existing Item: symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    printf("[DEBUG] Buy an existing Item: quantity = %d\n",accountObj.data.stock.ptrPos[index].quantity);
    assert(accountObj.data.stock.count == 1);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"AAAB")==0);
    assert(accountObj.data.stock.ptrPos[index].quantity == 150);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update normal update sell passed.\n");
    /*        3.normal buy duplicate        */        
    err = account_update_position(&accountObj,"AAAB",250);
    int index = account_find_position(&accountObj,"AAAB")
    printf("[DEBUG] Buy an existing Item: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Buy an existing Item: symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    printf("[DEBUG] Buy an existing Item: quantity = %d\n",accountObj.data.stock.ptrPos[index].quantity);
    assert(accountObj.data.stock.count == 1);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"AAAB")==0);
    assert(accountObj.data.stock.ptrPos[index].quantity == 400);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update normal update  buy duplicate passed.\n");
    /*        4.normal sell duplicate > 0        */        
    err = account_update_position(&accountObj,"BBBC",450);
    err = account_update_position(&accountObj,"CCCD",350);
    err = account_update_position(&accountObj,"BBBC",-50);
    int index = account_find_position(&accountObj,"BBBC")
    printf("[DEBUG] Sell an existing Item >0: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Sell an existing Item >0: symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    printf("[DEBUG] Sell an existing Item >0: quantity = %d\n",accountObj.data.stock.ptrPos[index].quantity);
    assert(accountObj.data.stock.count == 3);
    assert(strcmp(accountObj.data.stock.ptrPos[accountObj.data.stock.count-2].symbol,"BBBC")==0);
    assert(accountObj.data.stock.ptrPos[accountObj.data.stock.count-2].quantity == 400);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update normal update sell duplicate > 0 passed.\n");
    /*        5.normal sell duplicate finial quantity = 0        */        
    err = account_update_position(&accountObj,"BBBC",-400);
    printf("[DEBUG] Sell an existing Item finial quantity == 0,How many item in the List: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Sell an existing Item finial quantity == 0,The first item is : symbol = %s\n",accountObj.data.stock.ptrPos[0].symbol);
    printf("[DEBUG] Sell an existing Item finial quantity == 0,The last item is: symbol = %s\n",accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].symbol);
    printf("[DEBUG] Sell an existing Item finial quantity == 0,The last item is: quantity = %d\n",accountObj.data.stock.ptrPos[accountObj.data.stock.count-1].quantity);
    assert(accountObj.data.stock.count == 2);
    assert(account_find_position(&accountObj, "BBBC")==-1);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update normal update sell duplicate = 0 passed.\n");
    /*        6.normal sell duplicate < 0        */        
    /*        && 7.extend normal sell an intermediate item item = 0,delete the item normally        */        
    account_update_position(&accountObj,"BBBC",400);
    err = account_update_position(&accountObj,"CCCD",-400);
    int index = account_find_position(&accountObj,"CCCD")
    printf("[DEBUG] Sell an existing Item finial quantity < 0,How many item in the List: count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] Sell an existing Item finial quantity < 0,The last item is: symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    printf("[DEBUG] Sell an existing Item finial quantity < 0,The last item is: quantity = %d\n",accountObj.data.stock.ptrPos[index].quantity);
    assert(accountObj.data.stock.count == 3);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"CCCD")==0);
    assert(accountObj.data.stock.ptrPos[index].quantity == 350);
    assert(err == ERROR_INSUFFENT_ACCOUNT_QUANTITY);
    printf("[INFO]test_account_update normal update sell duplicate < 0 passed.\n");
   
    /*        8.sell a no-existing item        */        
    err = account_update_position(&accountObj,"DDDE",-400);
    assert(err == ERROR_INVALID_ACCOUNT_UPDATE);
    printf("[INFO]test_account_update normal update sell a no-existing item passed.\n");
    /*        9.parameter checking        */  
    err = account_update_position(NULL,"AAAB", 100);
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_update_position(&accountObj,NULL, 100);
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_update_position(&accountObj,"AAAB", 0);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_account_parameter_quanity passed.\n");
};

// };
// static void test_account_find_position();
// static void test_account_append_transaction();


// static void test_account_update_saving_credit();
