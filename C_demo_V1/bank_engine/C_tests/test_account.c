/*gcc C_tests/test_account.c src/account.c src/transaction.c -Iinclude -o C_tests/test_account
./C_tests/test_account*/
#include <assert.h>
#include "account.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_UNIT 40
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
static void test_account_find_position();
static void test_account_append_transaction();
static void test_account_update_position();
static void test_account_set_avg_cost();
static void test_setup_print_account(Account *accountObj_SavingsPtr, Account *accountObj_StockPtr,Transaction **txObj_sf_ptr,Transaction **txObj_sc_ptr);
static void test_account_update_saving_credit();

int main(){
    RUN_TEST(test_account_init);
    RUN_TEST(test_account_add_position);
    RUN_TEST(test_account_update_position);
    RUN_TEST(test_account_find_position);
    RUN_TEST(test_account_set_avg_cost);
    RUN_TEST(test_account_update_saving_credit);
    RUN_TEST(test_account_append_transaction);
    printf("\nAll account unit tests passed.\n");    
}

static void test_account_init(){
    Account accountObj;
    ErrorCode err;
    err = account_init(&accountObj, Account_Savings);
    assert(err == ERROR_OK && accountObj.type == Account_Savings && accountObj.data.savings.balance == 0);
    printf("[INFO]test_account_init_Saving passed.\n");
    err = account_init(&accountObj, Account_Credit);
    assert(err == ERROR_OK && accountObj.type == Account_Credit && accountObj.data.credit.credit_limit == CREDIT_INIT_LIMIT && accountObj.data.credit.credit_used == 0);
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
    int index = account_find_position(&accountObj,"AAAB");
    printf("[DEBUG] New First Item: New count = %zu\n",accountObj.data.stock.count);
    printf("[DEBUG] New First Item: New symbol = %s\n",accountObj.data.stock.ptrPos[index].symbol);
    assert(accountObj.data.stock.ptrPos[index].quantity == 100);
    assert(strcmp(accountObj.data.stock.ptrPos[index].symbol,"AAAB")==0);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update buy a new item passed.\n");

    /*        1.normal update buy        */        
    err = account_update_position(&accountObj,"AAAB",200);
    index = account_find_position(&accountObj,"AAAB");
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
    index = account_find_position(&accountObj,"AAAB");
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
    index = account_find_position(&accountObj,"AAAB");
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
    index = account_find_position(&accountObj,"BBBC");
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
    index = account_find_position(&accountObj,"CCCD");
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
    account_free(&accountObj);
};


static void test_account_find_position(){
        /*  
        precondition: 
            - add "AAAB"/"BBBC"/"CCCD" to stock list
            - add "ABBB"/"BCCC"/"CDDD" to fund list
        0.checking a normal one from stock list
        1.checking a normal one from fund list
        2.print a List of stock
        3.print a List of fund
        4.parameter checking
        5.invalid item checking
        */
    /*
            precondition: 
            - add "AAAB"/"BBBC"/"CCCD"/"DDDE"/"EEEF" to stock list
            - add "ABBB"/"BCCC"/"CDDD" to fund list
    */    
    Account accountObj_Stock;
    Account accountObj_Fund;
    ErrorCode err;
    account_init(&accountObj_Stock, Account_Stock);
    account_init(&accountObj_Fund, Account_Fund);
    printf("[DEBUG] Initial: initial count = %zu\n",accountObj_Stock.data.stock.count);
    printf("[DEBUG] Initial: initial count = %zu\n",accountObj_Fund.data.fund.count);
    account_update_position(&accountObj_Stock,"AAAB",100);    
    account_update_position(&accountObj_Stock,"BBBC",200);  
    account_update_position(&accountObj_Stock,"CCCD",300); 
    account_update_position(&accountObj_Stock,"DDDE",400); 
    account_update_position(&accountObj_Stock,"EEEF",500);  

    account_update_position(&accountObj_Fund,"ABBB",1000);    
    account_update_position(&accountObj_Fund,"BCCC",2000);  
    account_update_position(&accountObj_Fund,"CDDD",3000);  

    printf("[DEBUG] After Initial: initial count = %zu\n",accountObj_Stock.data.stock.count);
    printf("[DEBUG] After Initial: initial count = %zu\n",accountObj_Fund.data.fund.count);
    /*         0.checking a normal one from stock list      */
    int index = -1;
    index = account_find_position(&accountObj_Stock,"BBBC");
    assert(index == 1);
    printf("[INFO]test_account_find_position from Stock Listpassed.\n");
    /*         1.checking a normal one from fund list      */
    index = account_find_position(&accountObj_Fund,"CDDD");
    assert(index == 2);
    printf("[INFO]test_account_find_position from Fund Listpassed.\n");
    /*        2.print a List of stock   */
    for(int i=0;i<accountObj_Stock.data.stock.count;i++){
        printf("[INFO] The current id is :%d, and the name is :%s\n",i,accountObj_Stock.data.stock.ptrPos[i].symbol);
    }
    index = account_find_position(&accountObj_Stock,"EEEF");
    assert(index == 4);
    printf("[INFO]test_account_find_position from Stock Listpassed.\n");
        /*        3.print a List of fund   */
    for(int i=0;i<accountObj_Fund.data.fund.count;i++){
        printf("[INFO] The current id is :%d, and the name is :%s\n",i,accountObj_Fund.data.fund.ptrPos[i].symbol);
    }
    index = account_find_position(&accountObj_Fund,"ABBB");
    assert(index == 0);
    printf("[INFO]test_account_find_position from Fund Listpassed.\n");
    /*              4.parameter checking        */
    index = account_find_position(&accountObj_Stock,NULL);
    assert(index == -1);
    index = account_find_position(NULL,"AAAB");
    assert(index == -1);
    printf("[INFO]test_account_find_position_parameter_ passed.\n");
    /*              5.invalid item checking        */    
    index = account_find_position(&accountObj_Stock,"ZZZZ");
    assert(index == -1);
    printf("[INFO]test_account_find_position_invalid_ passed.\n");
    account_free(&accountObj_Fund);
    account_free(&accountObj_Stock);
};

static void test_account_set_avg_cost(){
    /*
            precondition: 
            - add "AAAB"/"BBBC"/"CCCD" to stock list
            - add "ABBB"/"BCCC"/"CDDD" to fund list
        0.Update Saving a normal one from stock list
        1.Update Saving a normal one from fund list
        2.invalid update find a invalid one from fund list
        3.parameter checking
    */
    Account accountObj_Stock;
    Account accountObj_Fund;
    ErrorCode err;
    account_init(&accountObj_Stock, Account_Stock);
    account_init(&accountObj_Fund, Account_Fund);
    printf("[DEBUG] Initial: initial count = %zu\n",accountObj_Stock.data.stock.count);
    printf("[DEBUG] Initial: initial count = %zu\n",accountObj_Fund.data.fund.count);
    account_update_position(&accountObj_Stock,"AAAB",100);    
    account_update_position(&accountObj_Stock,"BBBC",200);  
    account_update_position(&accountObj_Stock,"CCCD",300); 
    account_update_position(&accountObj_Stock,"DDDE",400); 
    account_update_position(&accountObj_Stock,"EEEF",500);  

    account_update_position(&accountObj_Fund,"ABBB",1000);    
    account_update_position(&accountObj_Fund,"BCCC",2000);  
    account_update_position(&accountObj_Fund,"CDDD",3000);  

    printf("[DEBUG] After Initial: initial count = %zu\n",accountObj_Stock.data.stock.count);
    printf("[DEBUG] After Initial: initial count = %zu\n",accountObj_Fund.data.fund.count);
    /*          0.Update Saving a normal one from stock list    */
    err = account_set_avg_cost(&accountObj_Stock, "BBBC", 200.2002);
    int index = -1;
    index = account_find_position(&accountObj_Stock,"BBBC");
    printf("set avg return = %d\n", err);
    printf("[DEBUG] After saving stock item update: name: %s, avg_cost: %f\n",accountObj_Stock.data.stock.ptrPos[index].symbol,accountObj_Stock.data.stock.ptrPos[index].avg_cost);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_saving stock item update_ passed.\n");
    /*          1.Update Saving a normal one from fund list    */
    err = account_set_avg_cost(&accountObj_Fund, "CDDD", 2000.2002);
    index = account_find_position(&accountObj_Fund,"CDDD");
    printf("[DEBUG] After saving  fund item update: name: %s, avg_cost: %f\n",accountObj_Fund.data.fund.ptrPos[index].symbol,accountObj_Fund.data.fund.ptrPos[index].avg_cost);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_saving fund item update_ passed.\n");
    /*              2.invalid update find a invalid one from fund list      */
    err = account_set_avg_cost(&accountObj_Stock, "CDDD", 2000.2002);
    assert(err == ERROR_INVALID_ACCOUNT_UPDATE);
    /*              3.parameter checking      */
    err = account_set_avg_cost(NULL, "CDDD", 2000.2002);;
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_set_avg_cost(&accountObj_Stock, NULL, 2000.2002);;
    assert(err == ERROR_INVALID_ARGUMENT);
    err = account_set_avg_cost(&accountObj_Stock, "CDDD", -2000.2002);;
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_account_set_avg_cost_parameter_ passed.\n");
};

static void test_account_update_saving_credit(){
        /*
            precondition: 
            - add saving account
            - add credit
        0.deposit to account
        1.withdraw from account
        3.credit charge
        4.credit payback
        3.parameter checking
    */
    Account accountObj_Savings;
    Account accountObj_Credit;
    account_init(&accountObj_Savings, Account_Savings);
    account_init(&accountObj_Credit, Account_Credit);
    ErrorCode err;
    /*        0.deposit to account      */
    err = account_update_saving_credit(&accountObj_Savings,100);
    printf("[DEBUG] After update saving account, balance: %f\n",accountObj_Savings.data.savings.balance);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update_saving_credit_deposit_ passed.\n");
    /*       2. withdraw from account      */
    err = account_update_saving_credit(&accountObj_Savings,-20);
    printf("[DEBUG] After update saving account, balance: %f\n",accountObj_Savings.data.savings.balance);
    assert(err == ERROR_OK);
    err = account_update_saving_credit(&accountObj_Savings,-85);
    printf("[DEBUG] After update saving account, balance: %f\n",accountObj_Savings.data.savings.balance);
    assert(accountObj_Savings.data.savings.balance == 80);
    assert(err == ERROR_INSUFFICIENT_SAVING);
    printf("[INFO]test_account_update_saving_credit_withdraw_ passed.\n");
    /*       3. credit charge      */
    err = account_update_saving_credit(&accountObj_Credit,100);
    printf("[DEBUG] After update credit account, credit_used: %f\n",accountObj_Credit.data.credit.credit_used);
    printf("[DEBUG] After update credit account, credit_remain: %f\n",accountObj_Credit.data.credit.credit_limit);
    assert(accountObj_Credit.data.credit.credit_used == 100);
    assert(accountObj_Credit.data.credit.credit_limit == 4900);
    assert(err == ERROR_OK);
    err = account_update_saving_credit(&accountObj_Credit,4901);
    printf("[DEBUG] After update credit account, credit_used: %f\n",accountObj_Credit.data.credit.credit_used);
    printf("[DEBUG] After update credit account, credit_remain: %f\n",accountObj_Credit.data.credit.credit_limit);
    assert(accountObj_Credit.data.credit.credit_used == 100);
    assert(accountObj_Credit.data.credit.credit_limit == 4900);
    assert(err == ERROR_EXCEED_CREDIT_LIMITATION);
    printf("[INFO]test_account_update_credit charge_ passed.\n");
    /*       4. credit payback      */
    err = account_update_saving_credit(&accountObj_Credit,-200);
    printf("[DEBUG] After update credit account, credit_used: %f\n",accountObj_Credit.data.credit.credit_used);
    printf("[DEBUG] After update credit account, credit_remain: %f\n",accountObj_Credit.data.credit.credit_limit);
    assert(accountObj_Credit.data.credit.credit_used == -100);
    assert(accountObj_Credit.data.credit.credit_limit == 5100);
    assert(err == ERROR_OK);
    err = account_update_saving_credit(&accountObj_Credit,-300);
    printf("[DEBUG] After update credit account, credit_used: %f\n",accountObj_Credit.data.credit.credit_used);
    printf("[DEBUG] After update credit account, credit_remain: %f\n",accountObj_Credit.data.credit.credit_limit);
    assert(accountObj_Credit.data.credit.credit_used == -400);
    assert(accountObj_Credit.data.credit.credit_limit == 5400);
    assert(err == ERROR_OK);
    printf("[INFO]test_account_update_credit payback passed.\n");
};

static void test_setup_print_account(Account *accountObj_SavingsPtr, Account *accountObj_StockPtr,Transaction **txObj_sf_ptr_ptr,Transaction **txObj_sc_ptr_ptr){
    /*
        precondition: 
            - add transaction stock 40 for each symbol
            - add transaction savings 40 
            - add saving account
            - add credit account
*/
    account_init(accountObj_SavingsPtr, Account_Savings);
    account_init(accountObj_StockPtr, Account_Stock);
    *txObj_sf_ptr_ptr = malloc(MAX_UNIT*sizeof(Transaction)*3);
    assert(*txObj_sf_ptr_ptr != NULL);
    *txObj_sc_ptr_ptr = malloc(MAX_UNIT*sizeof(Transaction));
    assert(*txObj_sc_ptr_ptr != NULL);
    printf("########################### 0.0 print out all the symbol and tx ##########################\n");
    for(int i=0;i<MAX_UNIT;i++){
        char symbol[16];
        sprintf(symbol,"AAA%03d-1111",i);
        account_add_position(accountObj_StockPtr,symbol,100+i);
        printf("##################################################################################\n");
        for(int j=0;j<3;j++){
                transaction_stock_fund_create(&(*txObj_sf_ptr_ptr)[3*i+j],STOCK_BUY,101.11+i+j*10,100+i,symbol);
                transaction_print(&(*txObj_sf_ptr_ptr)[3*i+j]);
                account_append_transaction(accountObj_StockPtr, &(*txObj_sf_ptr_ptr)[3*i+j]);
                }
        print_account(accountObj_StockPtr);

    }
    
    for(int i = 0;i<MAX_UNIT;i++){
        transaction_saving_credit_create(&(*txObj_sc_ptr_ptr)[i],DEPOSIT,1000.11+i);
        printf("##################################################################################\n");
        transaction_print(&(*txObj_sc_ptr_ptr)[i]);
        account_append_transaction(accountObj_SavingsPtr, &(*txObj_sc_ptr_ptr)[i]);
    }

}

static void test_account_append_transaction(){
/*

    */

    Account accountObj_Savings;
    Account accountObj_Stock;
    Transaction *txObj_sf_ptr = NULL;
    Transaction *txObj_sc_ptr = NULL;
    test_setup_print_account(&accountObj_Savings, &accountObj_Stock,&txObj_sf_ptr,&txObj_sc_ptr);

}

    /*
        4️.parameter（NULL)
    */
