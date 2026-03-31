/*gcc C_tests/test_customer.c src/customer.c src/account.c src/transaction.c -Iinclude -o C_tests/test_customer
./C_tests/test_customer*/

#include <assert.h>
#include "customer.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin......\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finish...... \n",#functionName);\
        printf("************************************************\n");\
}while(0);
static void test_customer_init();
static void test_customer_add_account();
// static void test_customer_find_account();
// static void test_customer_remove_account();
// static void test_customer_destroy();

int main(){
    RUN_TEST(test_customer_init);
    RUN_TEST(test_customer_add_account);
    // RUN_TEST(test_customer_find_account);
    // RUN_TEST(test_customer_remove_account);
    // RUN_TEST(test_customer_destroy);
    printf("\nAll account unit tests passed.\n");    
}
//ErrorCode customer_init(Customer *customerPtr,const char *name,const char *phone);

static void test_customer_init(){
    //1.normal customer create
    Customer customerObj;
    ErrorCode err;
    err = customer_init(&customerObj,"Helen Liu","6043966050");
    print_customer_detail(&customerObj);
    assert(err == ERROR_OK && strcmp(customerObj.name,"Helen Liu") == 0);
    printf("[INFO]test_customer_init_create passed.\n");
     //2.parameter check
    /*&customerObj==NULL || name==NULL ||  number==NULL */
    err = customer_init(NULL,"Helen Liu","6043966050");
    assert(err == ERROR_INVALID_ARGUMENT);
    err = customer_init(&customerObj,NULL,"6043966050");
    assert(err == ERROR_INVALID_ARGUMENT);
    err = customer_init(&customerObj,"Helen Liu",NULL);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_customer_init_parameter passed.\n");
};

static void test_customer_add_account(){
    Customer customerObj;
    ErrorCode err;
    customer_init(&customerObj,"Helen Liu","6043966050");

    //1.normal account add
    err = customer_add_account(&customerObj,Account_Savings);
    assert(err == ERROR_OK && customerObj.accounts_size ==1);
    err = customer_add_account(&customerObj,Account_Credit);
    assert(err == ERROR_OK && customerObj.accounts_size ==2);
    err = customer_add_account(&customerObj,Account_Stock);
    assert(err == ERROR_OK && customerObj.accounts_size ==3);
    err = customer_add_account(&customerObj,Account_Fund);
    assert(err == ERROR_OK && customerObj.accounts_size ==4);
    print_customer_detail(&customerObj);
    printf("[INFO]test_customer add normal accounts passed.\n");
    //3.add the fifth account 
    err = customer_add_account(&customerObj,Account_Others);
    assert(err == ERROR_ACCOUNT_REACHED_MAX && customerObj.accounts_size == 4);
    printf("[INFO]test_customer_ add fifth accounts failed passed.\n");
    //2.add the existing account 
    customer_init(&customerObj,"Emily Yuan","6043966011");
    print_customer_detail(&customerObj);
    err = customer_add_account(&customerObj,Account_Savings);
    assert(err == ERROR_OK && customerObj.accounts_size ==1);
    err = customer_add_account(&customerObj,Account_Credit);
    assert(err == ERROR_OK && customerObj.accounts_size ==2);
    err = customer_add_account(&customerObj,Account_Savings);
    assert(err ==ERROR_ALREADY_EXISTS);
    print_customer_detail(&customerObj);
    printf("[INFO]test_customer_ add same accounts failed passed.\n");
}