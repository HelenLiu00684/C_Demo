#ifndef INVENTORY_H
#define INVENTORY_H
#include <stddef.h> 
#include <stdlib.h>
#include "error.h"
#include "symbolmap.h"
typedef struct{
    char symbol[16];
    int quantity;//Bank holding
    double price;
}BankPosition;


typedef struct{
    SymbolType type;
    BankPosition *ptrPos;
    size_t capacity; //size_t is unsign type Max position of each account
    size_t count;//how many kind of positions
}BankInventory;

ErrorCode inventory_init(BankInventory *inv,SymbolType type);
int inventory_find_position(BankInventory *inv,const char *symbol);
ErrorCode inventory_add_position(BankInventory *inv,const char *symbol,int quantity,double price);
ErrorCode inventory_update_quantity(BankInventory *inv,const char *symbol,int quantity);
ErrorCode inventory_update_price(BankInventory *inv,const char *symbol,double price);

#endif