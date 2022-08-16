#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "../thirdparty/ValeraC/valera.h"

const static char* TOKENS = "(){}[],.:;*+-!?/@#$\"\' =^~|%<>\n";
const static char* COMPARESIGNS = "<>!=";

void tokenize(const char* code, valera_array_t* tokenized);

#endif
