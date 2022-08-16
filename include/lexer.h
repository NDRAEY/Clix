#ifndef LEXER_H_
#define LEXER_H_

#include "../thirdparty/ValeraC/valera.h"

typedef enum ClixLexType {
  LEX_KEYWORD = 0,
  LEX_KEY,
  LEX_STRING,
  LEX_NUMBER,
  LEX_NAME
} ClixLexType;

int is_number(char *str);
void lex(valera_array_t* tokenized, valera_array_t* out);

#endif
