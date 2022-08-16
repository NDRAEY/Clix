#include <stdio.h>
#include <ctype.h>
#include "../include/global.h"

const char* keywords[] = {
  "DEFINE", "define",
  "SET",    "set",
  "ADD",    "add",
  "SUB",    "sub",
  "MUL",    "mul",
  "DIV",    "div",
  "PRINT",  "print",
};

unsigned int dictlen = 14;

int isin(const char* str, const char* dict[], unsigned int dictlen) {
  for(unsigned int i=0; i<dictlen; i++) {
    if(strcmp(dict[i], str)==0) { return i; }
  }
  return -1;
}

valera_node_t* gen_lex_object(ClixLexType type, char* token, unsigned int start, unsigned int end, int line) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "type", type);
  valera_push_string(obj, "token", token);
  valera_push_number(obj, "start", start);
  valera_push_number(obj, "line", line);
  valera_push_number(obj, "end", end);
  
  return obj;
}

int is_number(char *str) {
  for (int i = 0; str[i]!= '\0'; i++) {
    if (isdigit(str[i]) == 0) return 0;
  }
  return 1;
}

void lex(valera_array_t* tokenized, valera_array_t* out) {
  unsigned int idx = 0;
  
  while(idx<(unsigned int)valera_array_length(tokenized)) {
    valera_node_t* elem = valera_array_get(tokenized, idx)->obj;
    char* token = valera_get(elem, "token")->str;
    
    int kwpos = isin(token, keywords, dictlen);
    if(kwpos>-1) {
      valera_array_push_object(
        out, 
        gen_lex_object(
          LEX_KEYWORD,
          valera_get(elem, "token")->str,
          valera_get(elem, "start")->num,
          valera_get(elem, "end")->num,
          valera_get(elem, "line")->num
        )
      );
      
      idx++;
      continue;
    }else if(is_number(token)) {
      valera_array_push_object(
        out,
        gen_lex_object(
          LEX_NUMBER,
          valera_get(elem, "token")->str,
          valera_get(elem, "start")->num,
          valera_get(elem, "end")->num,
          valera_get(elem, "line")->num

        )
      );
    }else if(strlen(token)<=1 && strchr(TOKENS, token[0])) {
      valera_array_push_object(
        out,
        gen_lex_object(
          LEX_KEY,
          valera_get(elem, "token")->str,
          valera_get(elem, "start")->num,
          valera_get(elem, "end")->num,
          valera_get(elem, "line")->num
        )
      );
    }else if(token[0]=='\"' && token[strlen(token)-1]=='\"') {
      token++;
      token[strlen(token)-1]=0;
      //printf("String! >%s\n", token);
      valera_array_push_object(
        out,
        gen_lex_object(
          LEX_STRING,
          token,
          valera_get(elem, "start")->num,
          valera_get(elem, "end")->num,
          valera_get(elem, "line")->num

        )
      );
    }else{
      valera_array_push_object(
        out,
        gen_lex_object(
          LEX_NAME,
          valera_get(elem, "token")->str,
          valera_get(elem, "start")->num,
          valera_get(elem, "end")->num,
          valera_get(elem, "line")->num
        )
      );
    }
    
    idx++;
  }
}
