#include "../include/global.h"
#include <stdlib.h>

#define NERR(msg) printf("\033[31mERROR\033[0m: %s\n", msg); exit(1);
#define INCORERR if(idx>=len) { NERR("Unexcepted EOF"); }
#define CHECKCOMMA if(strcmp(valera_get(valera_array_get(lexed, idx)->obj, "token")->str, ",")!=0) { NERR("Excepted comma"); }; idx++

valera_node_t* acl_gen_object(ClixActions action, char* token, ClixLexType type) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "token", token);
  valera_push_number(obj, "type", type);
  return obj;
}

valera_node_t* acl_gen_object2(ClixActions action, char* t1, char* t2, ClixLexType p1, ClixLexType p2) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "tok1", t1);
  valera_push_string(obj, "tok2", t2);
  valera_push_number(obj, "typ1", p1);
  valera_push_number(obj, "typ2", p2);
  return obj;
}

void make_actionlist(char* filename, valera_array_t* lexed, valera_array_t* out) {
  unsigned int idx = 0;
  
  for(int px=0, lm=valera_array_length(lexed); px<lm; px++) {
    if(strcmp(valera_get(valera_array_get(lexed, px)->obj, "token")->str, " ")==0) {
      valera_array_removeat(lexed, px);
      lm--;
      continue;
    }
  }
  
  unsigned int len = valera_array_length(lexed);
  unsigned int line = 1;
  while(idx<len) {
    valera_node_t* elem = valera_array_get(lexed, idx)->obj;
    int ttype = valera_get(elem, "type")->num;
    char* token = valera_get(elem, "token")->str;
    
    if(ttype == LEX_KEYWORD) {
      if(strcmp(token, "DEFINE")==0 || strcmp(token, "define")==0) {
        idx++; INCORERR;
        
        char* deftok = valera_get(
          valera_array_get(lexed, idx)->obj,
          "token"
        )->str;
        
        ClixLexType lt = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        if(idx+1<len && strcmp(
            valera_get(valera_array_get(lexed, idx+1)->obj, "token")->str,"\n"
           )==0) { idx++; line++; }

        valera_array_push_object(out,acl_gen_object(ACL_DEFINE, deftok, lt));
        idx++;

        continue;
      }else if(strcmp(token, "SET")==0 || strcmp(token, "set")==0) {
        idx++; INCORERR;
        
        char* settok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType settype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++;
        
        CHECKCOMMA;
        
        char* whattok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType whattype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        //printf("SET: %s -> %s\n", settok, whattok);
        
        if(idx+1<len && strcmp(
           valera_get(valera_array_get(lexed, idx+1)->obj, "token")->str,"\n"
          )==0) { idx++; line++; }
        
        valera_array_push_object(out, acl_gen_object2(ACL_SET, settok, whattok, settype, whattype));

        idx++; continue;
      }else if(strcmp(token, "ADD")==0 || strcmp(token, "add")==0
            || strcmp(token, "SUB")==0 || strcmp(token, "sub")==0
            || strcmp(token, "MUL")==0 || strcmp(token, "mul")==0
            || strcmp(token, "DIV")==0 || strcmp(token, "div")==0) {
        ClixActions action;
        if(strcmp(token, "ADD")==0 || strcmp(token, "add")==0) {action=ACL_ADD;}
        if(strcmp(token, "SUB")==0 || strcmp(token, "sub")==0) {action=ACL_SUB;}
        if(strcmp(token, "MUL")==0 || strcmp(token, "mul")==0) {action=ACL_MUL;}
        if(strcmp(token, "DIV")==0 || strcmp(token, "div")==0) {action=ACL_DIV;}
        
        idx++; INCORERR;
        
        char* settok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType settype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++;

        INCORERR;
        CHECKCOMMA;
        
        char* whattok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType whattype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        //printf("MATH: %s -> %s\n", settok, whattok);
        
        if(idx+1<len && strcmp(
          valera_get(valera_array_get(lexed, idx+1)->obj, "token")->str,"\n"
          )==0) { idx++; line++; }
        
        valera_array_push_object(out, acl_gen_object2(action, settok, whattok, settype, whattype));
        
        idx++; continue;
      }else if(strcmp(token, "PRINT")==0 || strcmp(token, "print")==0) {
        idx++; INCORERR;
      
        char* settok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType settype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++;
        
        if(idx+1<len && strcmp(
          valera_get(valera_array_get(lexed, idx+1)->obj, "token")->str,"\n"
        )==0) { idx++; line++; }
        
        valera_array_push_object(out, acl_gen_object(ACL_PRINT, settok, settype));
        
        idx++; continue;
      }
    }else{
      if(strcmp(token, "\n")!=0) {
        //NERR("Invalid syntax");
        clix_error(filename, line,
                   valera_get(elem, "start")->num,
                   valera_get(elem, "end")->num,
                   "Invalid syntax.");
      }
    }
    
    idx++;
  }
}
