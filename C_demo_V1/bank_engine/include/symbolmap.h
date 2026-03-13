#ifndef SYMBOLMAP_H
#define SYMBOLMAP_H
#define MAX_CHAR 16

/*
Symbol Map Interface

The module maps a symbol string to its
corresponding SymbolType and declares
the lookup functions used by the engine.
*/


typedef enum{
    Symbol_Stock = 0,
    Symbol_Fund,
    Symbol_Invalid
}SymbolType;

typedef struct{
    char symbol[MAX_CHAR];
    SymbolType type;
}SymbolEntry;

/* Lookup SymbolType by symbol string */
SymbolType symbol_lookup(const char * symbol);
#endif