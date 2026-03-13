#include "symbolmap.h"
#include "error.h"
#include <stddef.h>
#include <string.h>

/*
symbol_add
symbol_expand
symbol_init
*/
static SymbolEntry symbol_table[] = {
    {"AAAB", Symbol_Stock},
    {"BBBC", Symbol_Stock},
    {"CCCD", Symbol_Fund},
    {"DDDE", Symbol_Fund}
};

static const size_t symbol_count = sizeof(symbol_table)/sizeof(SymbolEntry);
SymbolType symbol_lookup(const char * symbol){
    if(symbol == NULL)
        return Symbol_Invalid;
    for(size_t i=0; i<symbol_count;i++){
        if(strcmp(symbol_table[i].symbol,symbol)==0){
            return symbol_table[i].type;
        }
    }
    return Symbol_Invalid;

}