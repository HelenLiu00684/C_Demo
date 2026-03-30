/*gcc tests/test_symbolmap.c src/symbolmap.c -Iinclude -o tests/test_symbolmap
.tests/test_symbolmap*/
#include "symbolmap.h"
#include <assert.h>
#include <stdio.h>
#define  RUN_TEST(functionName)\
    do{\
        printf("************************************************\n");\
        printf("Testing Unit %s has begin....\n",#functionName);\
        functionName();\
        printf("Testing Unit %s has finished....\n",#functionName);\
        printf("************************************************\n");\
    }while(0);

static void test_symbol_lookup_valid();
static void test_symbol_lookup_invalid();

int main() {
    RUN_TEST(test_symbol_lookup_valid);
    RUN_TEST(test_symbol_lookup_invalid);

    printf("\nAll symbolmap unit tests passed.\n");
}
/* Test valid symbols */
static void test_symbol_lookup_valid(){
    SymbolType st;
    st = symbol_lookup("AAAB");
    assert(st == Symbol_Stock);
    printf("[INFO]test_test_symbol_lookup_valid_Symbol_Stock passed.\n");

    st = symbol_lookup("CCCD");
    assert(st == Symbol_Fund);
    printf("[INFO]test_test_symbol_lookup_valid_Symbol_Fund passed.\n");
};
/* Test invalid input */
static void test_symbol_lookup_invalid(){
    SymbolType st;
    st = symbol_lookup("ZZZZ");
    assert(st == Symbol_Invalid);
    printf("[INFO]test_test_symbol_lookup_invalid_Symbol_Value passed.\n");

    st = symbol_lookup("");
    assert(st == Symbol_Invalid);
    printf("[INFO]test_test_symbol_lookup_invalid_Symbol_Null passed.\n");

    st = symbol_lookup("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    assert(st == Symbol_Invalid);
    printf("[INFO]test_test_symbol_lookup_invalid_Symbol_TooLong passed.\n");
};
