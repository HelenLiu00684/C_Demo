/*gcc tests/test_inventory.c src/inventory.c -Iinclude -o tests/test_inventory
.tests/test_inventory*/
#include "inventory.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#define RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin......\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finish...... \n",#functionName);\
        printf("************************************************\n");\
    }while(0);

static void test_inventory_init();
static void test_inventory_add_position();
static void test_inventory_buy_position();
static void test_inventory_sell_position();

int main() {
    RUN_TEST(test_inventory_init);
    RUN_TEST(test_inventory_add_position);
    RUN_TEST(test_inventory_buy_position);
    RUN_TEST(test_inventory_sell_position);
    printf("\nAll inventory unit tests passed.\n");
}
static void test_inventory_init(){
    BankInventory invObj;
    ErrorCode err;
    err = inventory_init(&invObj,Symbol_Stock);
    assert(err == ERROR_OK);
    printf("[INFO]test_inventory_init passed.\n");
    err = inventory_init(&invObj,Symbol_Invalid);
    assert(err == ERROR_INVALID_SYMBOL);
    printf("[INFO]test_inventory_init_missing_parameter passed.\n");
    inventory_free(&invObj);
}
static void test_inventory_add_position(){
    /*  0.normal add
        1.add duplicate
        2.parameter check
        3.find invalid
        4.extend invalid
    */
    BankInventory invObj;
    ErrorCode err;
    inventory_init(&invObj,Symbol_Stock); //Initializes the Inventory
    //0.normal add
    err = inventory_add_position(&invObj,"AAAB",100,25.25);
    assert(err == ERROR_OK||invObj.ptrPos[0].avg_cost == 25.25);
    printf("[INFO]test_inventory_add_normal passed.\n");
    //1.add duplicate
    err = inventory_add_position(&invObj,"AAAB",200,25.25);
    assert(err == ERROR_ALREADY_EXISTS);
    printf("[INFO]test_inventory_add_duplicate passed.\n");
    //2.parameter check
    /*inv==NULL || symbol==NULL || quantity <0 || symbol[0] == '\0'|| price <0*/
    err = inventory_add_position(NULL,"AAAB",200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_inv passed.\n");
    err = inventory_add_position(&invObj,"",200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_symbol passed.\n");
    err = inventory_add_position(&invObj,"AAAB",-200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_quanity passed.\n");
    err = inventory_add_position(&invObj,"AAAB",200,-25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_price passed.\n");
    //3.find invalid
    err = inventory_add_position(&invObj,"AAAB",200,25.25);
    assert(err == ERROR_ALREADY_EXISTS);
    printf("[INFO]test_inventory_invalid_index passed.\n");
    //4.extend invalid   
    for(int i=0;i<20;i++)
    {
        char symbol[8];
        sprintf(symbol,"AAA%03d",i);

        inventory_add_position(&invObj,symbol,100,10);
    }
    assert(invObj.capacity >= 20);
    printf("[DEBUG] expand: new capacity = %zu\n", invObj.capacity);
    printf("[INFO]test_inventory_invalid_index passed.\n");
    inventory_free(&invObj);
}
static void test_inventory_buy_position(){
    /*  
        OK:
        0.normal buy
        1.normal buy new_item
        2.argment error:ERROR_INVALID_ARGUMENT
 */
    BankInventory invObj;
    ErrorCode err;
    int index;
    inventory_init(&invObj,Symbol_Stock); //Initializes the Inventory
    inventory_add_position(&invObj,"AAAA",100,100);//Add the standard amount the Inventory
    inventory_add_position(&invObj,"AAAB",200,200);//Add the standard amount the Inventory
    index = inventory_find_position(&invObj,"AAAB");
    printf("DEBUG after add avg=%f\n", invObj.ptrPos[index].avg_cost);
    inventory_add_position(&invObj,"AAAC",300,300);//Add the standard amount the Inventory

    /* 0.normal buy */

    err = inventory_buy_position(&invObj,"AAAB",200,400);
    index = inventory_find_position(&invObj,"AAAB");
    printf("[DEBUG] symbol=%s index=%d\n", invObj.ptrPos[1].symbol, index);
    printf("[DEBUG] The amount of the symbol is: %d\n", invObj.ptrPos[index].quantity);
    printf("[DEBUG] The avg_cost of the symbol is: %.2f\n", invObj.ptrPos[index].avg_cost);
    assert(invObj.ptrPos[index].quantity==400 && fabs(invObj.ptrPos[1].avg_cost-300) < 0.01 );
    printf("[INFO]test_inventory_normal_buy passed.\n");

    /*1.normal buy new_item*/
    inventory_buy_position(&invObj,"AAAB",400,100);
    inventory_buy_position(&invObj,"AAAB",200,200);
    index = inventory_find_position(&invObj,"AAAB");
    printf("[DEBUG] The symbol is: %s\n", invObj.ptrPos[index].symbol);
    printf("[DEBUG] The amount of the symbol is: %d\n", invObj.ptrPos[index].quantity);
    printf("[DEBUG] The avg_cost of the symbol is: %.2f\n", invObj.ptrPos[index].avg_cost);
    assert(invObj.ptrPos[index].quantity==1000 && fabs(invObj.ptrPos[index].avg_cost-200) < 0.01 );
    printf("[INFO]test_inventory_normal buy new_item passed.\n");
    /*        2.argment error:ERROR_INVALID_ARGUMENT*/
    err = inventory_buy_position(NULL,"AAAB",200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_inv passed.\n");
    err = inventory_buy_position(&invObj,NULL,200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_symbol passed.\n");
    err = inventory_buy_position(&invObj,"AAAB",-200,25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_quanity passed.\n");
    err = inventory_buy_position(&invObj,"AAAB",200,-25.25);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_buy_parameter_price passed.\n");
    inventory_free(&invObj);
}



static void test_inventory_sell_position(){
    /*  
        OK:
        0.normal sell        
        1.sell twice
        2.reduce to zero, find_result = -1 ,count --
        3.reduce over amount:ERROR_INSUFFICIENT_INVENTORY       
        4.argment error:ERROR_INVALID_ARGUMENT

 */
    BankInventory invObj;
    ErrorCode err;
    int index=-2;
    inventory_init(&invObj,Symbol_Stock); //Initializes the Inventory
    inventory_add_position(&invObj,"AAAA",200,25.25);//Add the standard amount the Inventory
    /*0.normal sell */
    err = inventory_sell_position(&invObj,"AAAA",25);
    index = inventory_find_position(&invObj,"AAAA");
    printf("[DEBUG] The symbol is: %s\n", invObj.ptrPos[index].symbol);
    printf("[DEBUG] The amount of the symbol is: %d\n", invObj.ptrPos[index].quantity);
    assert(invObj.ptrPos[index].quantity==175 && invObj.ptrPos[index].avg_cost == 25.25);
    printf("[INFO]test_inventory_normal_sell passed.\n");

    /*1.sell twice*/
    inventory_buy_position(&invObj,"AAAB",20,100);
    inventory_buy_position(&invObj,"AAAB",20,50);
    inventory_buy_position(&invObj,"AAAC",30,250);
    inventory_sell_position(&invObj,"AAAB",15);
    index = inventory_find_position(&invObj,"AAAB");
    printf("[DEBUG] The symbol is: %s\n", invObj.ptrPos[index].symbol);
    printf("[DEBUG] The amount of the symbol is: %d\n", invObj.ptrPos[index].quantity);
    printf("[DEBUG] The avg_cost of the symbol is: %.2f\n", invObj.ptrPos[index].avg_cost);
    assert(invObj.ptrPos[index].quantity==25 && invObj.ptrPos[index].avg_cost == 75 );
    printf("[INFO]test_inventory_normal sell twice passed.\n");
    /* 2.reduce to zero, find_result = -1 ,count --:*/
    inventory_sell_position(&invObj,"AAAB",25);
    inventory_buy_position(&invObj,"AAAD",80,50);
    index = inventory_find_position(&invObj,"AAAC");
    printf("[DEBUG] The symbol is: %s\n", invObj.ptrPos[index].symbol);
    printf("[DEBUG] The quantity of the symbol is: %d\n", invObj.ptrPos[index].quantity);
    printf("[DEBUG] The avg_cost of the symbol is: %.2f\n", invObj.ptrPos[index].avg_cost);
    assert(strcmp(invObj.ptrPos[index].symbol,"AAAC")==0 &&  fabs(invObj.ptrPos[index].avg_cost-250) < 0.01);
    printf("[INFO]test_inventory_sell to zero passed.\n");
    /* 3.reduce over amount:ERROR_INSUFFICIENT_INVENTORY */
    err = inventory_sell_position(&invObj,"AAAC",-100);
    index = inventory_find_position(&invObj,"AAAC");
    printf("[DEBUG] The symbol is: %s\n", invObj.ptrPos[1].symbol);
    printf("[DEBUG] The quantity of the symbol is: %d\n", invObj.ptrPos[1].quantity);
    printf("[DEBUG] The avg_cost of the symbol is: %.2f\n", invObj.ptrPos[1].avg_cost);
    assert(strcmp(invObj.ptrPos[index].symbol,"AAAC" )==0  && invObj.ptrPos[index].quantity==30 && invObj.ptrPos[index].avg_cost == 250);
    printf("[INFO]test_inventory_normal sell over amount passed.\n");
    /*4.argment error:ERROR_INVALID_ARGUMENT*/
    err = inventory_sell_position(NULL,"AAAB",-10);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_inv passed.\n");
    err = inventory_sell_position(&invObj,NULL,-10);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_parameter_symbol passed.\n");
    err = inventory_sell_position(&invObj,"AAAB",-10);
    assert(err == ERROR_INVALID_ARGUMENT);
    printf("[INFO]test_inventory_sell_parameter_quanity passed.\n");
    inventory_free(&invObj); 
}        
   