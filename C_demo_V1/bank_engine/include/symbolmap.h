#ifndef SYMBOLMAP_H
#define SYMBOLMAP_H
typedef enum{
    Symbol_Stock = 0,
    Symbol_Fund,
    Symbol_Invalid
}SymbolType;

typedef struct{
    char symbol[16];
    SymbolType type;
}SymbolEntry;

SymbolType symbol_lookup(const char * symbol);
#endif