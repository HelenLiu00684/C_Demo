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
// static void test_customer_add_account();
// static void test_customer_find_account();
// static void test_customer_remove_account();
// static void test_customer_destroy();

int main(){
    RUN_TEST(test_customer_init);
    // RUN_TEST(test_customer_add_account);
    // RUN_TEST(test_customer_find_account);
    // RUN_TEST(test_customer_remove_account);
    // RUN_TEST(test_customer_destroy);
    printf("\nAll account unit tests passed.\n");    
}
//ErrorCode customer_init(Customer *customerPtr,const char *name,const char *phone);

static void test_customer_init(){
    Customer customerObj;
    ErrorCode err;
    err = customer_init(&customerObj,"Helen Liu","6043966050");
    print_customer_detail(&customerObj);
    assert(err == ERROR_OK && strcmp(customerObj.name,"Helen Liu") == 0);
    printf("[INFO]test_account_init_Saving passed.\n");
};