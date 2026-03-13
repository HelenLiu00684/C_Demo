/*
Symbol Map Implementation

Implements a lookup table that maps asset symbols
to their corresponding SymbolType.
*/
#include "symbolmap.h"
#include <stddef.h>
#include <string.h>

/* Static symbol lookup table */
static SymbolEntry symbol_table[] = {
    {"AAAB", Symbol_Stock},
    {"BBBC", Symbol_Stock},
    {"CCCD", Symbol_Fund},
    {"DDDE", Symbol_Fund}
};

static const size_t symbol_count = sizeof(symbol_table)/sizeof(SymbolEntry);
/* Lookup SymbolType by symbol string */
SymbolType symbol_lookup(const char * symbol){
    if(symbol == NULL)
        return Symbol_Invalid;
    if(strlen(symbol)>=MAX_CHAR)
        return Symbol_Invalid;
    for(size_t i=0; i<symbol_count;i++){
        if(strcmp(symbol_table[i].symbol,symbol)==0){
            return symbol_table[i].type;
        }
    }
    return Symbol_Invalid;

}