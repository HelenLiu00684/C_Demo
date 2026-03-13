
#include "bank.h"
#include <stdio.h>

int main(void){
  Bank bank;
  bank_init(&bank);

  bank_add_customer(&bank,"Helen", "01-6043966050");
  bank_add_customer(&bank,"Emily", "01-6179526664");

  int customer_index = -1;
  customer_index = bank_find_customer(&bank, "Helen","");
  if(customer_index!=-1) printf("find out ! %s\n",bank.ptrCustomer[customer_index].name);

  ErrorCode customer_account_result = ERROR_WRONG;
  customer_account_result = customer_add_account(&bank.ptrCustomer[customer_index],Account_Savings);
  if(customer_account_result == ERROR_OK){
    printf("Account created.\n");
  }

  customer_account_result = account_update_saving_credit(&bank.ptrCustomer[customer_index].accounts[Account_Savings], 100);
    if(customer_account_result == ERROR_OK){
    printf("Record created.\n");
    printf("The Savings is %f.\n",bank.ptrCustomer[customer_index].accounts[Account_Savings].data.savings.balance);
  }
}