/*
Inventory Module

This module defines the bank inventory data structures and declares
the functions used to maintain inventory records.

BankPosition represents the basic data structure for each asset item.

BankInventory represents the container structure used to store
multiple asset positions.
*/
#ifndef INVENTORY_H
#define INVENTORY_H
#include <stddef.h> 
#include <stdlib.h>
#include "error.h"
#include "symbolmap.h"
typedef struct{
    char symbol[16];
    int quantity;// quantity held by the bank
    double avg_cost;// price associated with the asset position
    double total_cost;//avg_cost * quantity
}BankPosition;


typedef struct{
    SymbolType type;
    BankPosition *ptrPos;
    size_t capacity; //size_t is unsign type Max position of each account
    size_t count;//how many kind of positions
}BankInventory;
/* Initializes the inventory structure */
ErrorCode inventory_init(BankInventory *inv,SymbolType type);
/* Finds the index of an asset position in the inventory */
int inventory_find_position(BankInventory *inv,const char *symbol);
/* Adds a new asset position into the inventory */
ErrorCode inventory_add_position(BankInventory *inv,const char *symbol,int quantity,double price);
/* Updates the quantity of an asset position due to buy  */
ErrorCode inventory_buy_position(BankInventory *inv,const char *symbol,int quantity,double cost);
/* Updates the quantity of an asset position due to sell operations */
ErrorCode inventory_sell_position(BankInventory *inv,const char *symbol,int quantity);
/* Free the inventory */
void inventory_free(BankInventory *inv);

#endif