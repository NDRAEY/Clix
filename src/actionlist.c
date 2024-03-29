#include "../include/global.h"
#include <stdlib.h>

#define NERR(msg) printf("\033[31mERROR\033[0m: %s:%d %s\n", __FILE__, __LINE__, msg); exit(1);
#define INCORERR if(idx>=len) { NERR("Unexcepted EOF"); }
#define CHECKCOMMA if(strcmp(valera_get(valera_array_get(lexed, idx)->obj, "token")->str, ",")!=0) { NERR("Excepted comma"); }; idx++

valera_node_t* acl_gen_object(ClixActions action, char* token, ClixLexType type, int line) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "token", token);
  valera_push_number(obj, "type", type);
  valera_push_number(obj, "line", line);
  return obj;
}

valera_node_t* acl_gen_object_arr_if(ClixActions action,
                                     char* tok1, char* tok2, char* tok3,
                                     ClixLexType t1, ClixLexType t2, ClixLexType t3,
                                     valera_array_t* array, int line) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "tok1", tok1);
  valera_push_string(obj, "tok2", tok2);
  valera_push_string(obj, "tok3", tok3);
  valera_push_number(obj, "typ1", t1);
  valera_push_number(obj, "typ2", t2);
  valera_push_number(obj, "typ3", t3);
  valera_push_array(obj, "tokens", array);
  valera_push_number(obj, "line", line);
  return obj;
}

valera_node_t* acl_gen_object2(ClixActions action, char* t1, char* t2,
                               ClixLexType p1, ClixLexType p2, int line) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "tok1", t1);
  valera_push_string(obj, "tok2", t2);
  valera_push_number(obj, "typ1", p1);
  valera_push_number(obj, "typ2", p2);
  valera_push_number(obj, "line", line);
  return obj;
}

valera_node_t* acl_gen_object_array(ClixActions action, char* t1, ClixLexType p1,
                                    valera_array_t* array, int line) {
  valera_node_t* obj = valera_new();
  valera_push_number(obj, "action", action);
  valera_push_string(obj, "tok1", t1);
  valera_push_number(obj, "typ1", p1);
  valera_push_array(obj, "tokens", array);
  valera_push_number(obj, "line", line);
  return obj;
}

char* acl_to_string(enum ClixActions action) {
  if(action==ACL_DEFINE) return (char*)"DEFINE";
  if(action==ACL_SET) return (char*)"SET";
  if(action==ACL_ADD) return (char*)"ADD";
  if(action==ACL_SUB) return (char*)"SUB";
  if(action==ACL_MUL) return (char*)"MUL";
  if(action==ACL_DIV) return (char*)"DIV";
  if(action==ACL_SHR) return (char*)"SHR";
  if(action==ACL_SHL) return (char*)"SHL";
  if(action==ACL_AND) return (char*)"AND";
  if(action==ACL_OR) return (char*)"OR";
  if(action==ACL_XOR) return (char*)"XOR";
  if(action==ACL_NOT) return (char*)"NOT";
  if(action==ACL_IF) return (char*)"IF";
  if(action==ACL_PRINT) return (char*)"PRINT";
  if(action==ACL_CALL) return (char*)"CALL";
  if(action==ACL_WHILE) return (char*)"WHILE";
  return (char*)"UNKNOWN";
}

void make_actionlist(char* filename, valera_array_t* lexed, valera_array_t* out) {
  unsigned int idx = 0;
  
  for(int px=0, lm=valera_array_length(lexed); px<lm; px++) {
    if(strcmp(valera_get(valera_array_get(lexed, px)->obj, "token")->str, " ")==0) {
      valera_array_removeat(lexed, px);
      lm--;
    }else{
    	VPRINT("This not a space: ", valera_array_get(lexed, px)->obj);
    }
  }

  VPRINTARR("Lexed: ", lexed);
  
  unsigned int len = valera_array_length(lexed);
  while(idx<len) {
    valera_node_t* elem = valera_array_get(lexed, idx)->obj;
    int ttype = valera_get(elem, "type")->num;
    char* token = valera_get(elem, "token")->str;
    unsigned int line = valera_get(elem, "line")->num;
    
    if(ttype == LEX_KEYWORD) {
      if(strcmp(token, "DEFINE")==0 || strcmp(token, "define")==0) {
        idx++; INCORERR;
        
        char* deftok = valera_get(
          valera_array_get(lexed, idx)->obj,
          "token"
        )->str;
        
        ClixLexType lt = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        valera_array_push_object(out,acl_gen_object(ACL_DEFINE, deftok, lt, line));

        /*if(idx+1<len && strcmp(
            valera_get(valera_array_get(lexed, idx+1)->obj, "token")->str,"\n"
           )==0) { idx++; line++; }
        */

        idx++; continue;
      }else if(strcmp(token, "SET")==0 || strcmp(token, "set")==0) {
        idx++; INCORERR;
        
        char* settok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType settype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++;
        
        CHECKCOMMA;
        
        char* whattok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType whattype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        //printf("SET: %s -> %s\n", settok, whattok);
        
        valera_array_push_object(out, acl_gen_object2(ACL_SET, settok, whattok, settype,
        whattype, line));

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
        
        valera_array_push_object(out, acl_gen_object2(action, settok, whattok, settype, whattype, line));

        idx++; continue;
      }else if(strcmp(token, "PRINT")==0 || strcmp(token, "print")==0) {
        idx++; INCORERR;
      
        char* settok = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType settype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++;
        
        valera_array_push_object(out, acl_gen_object(ACL_PRINT, settok, settype, line));
        
        idx++; continue;
      }else if(strcmp(token, "IF")==0 || strcmp(token, "if")==0){
        //printf("IFTOKEN!\n");
   
        idx++; INCORERR;
        char* valwhat = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType whattype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
      
        idx++; INCORERR;
        char* valcomp = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType comptype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
      
        idx++; INCORERR;
        char* valwith = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType withtype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        idx++; INCORERR;
      
        //printf("WHAT: %s; COMP: %s; WITH: %s\n", valwhat, valcomp, valwith);
      
        valera_array_t* body = valera_array_new();
      
        valera_node_t* btok = valera_array_get(lexed, idx)->obj;
        while(1) {
          btok = valera_array_get(lexed, idx)->obj;
          if(strcmp(valera_get(btok, "token")->str, "\n")==0) { idx++; continue; } // May cause skew.
          if(strcmp(valera_get(btok, "token")->str, "END")==0) break;
          valera_array_push_object(body, btok);
          idx++;
        }
        /*puts("=======================================");
        VPRINTARR("Body is: ", body);
        puts("=======================================");*/
      
        valera_array_push_object(
          out,
          acl_gen_object_arr_if(
            ACL_IF,
            valwhat,
            valcomp,
            valwith,
            whattype,
            comptype,
            withtype,
            body,
            line
          )
        );
        
        idx++; continue;
      }else if(strcmp(token, "CALL")==0 || strcmp(token, "call")==0) {
       	idx++; INCORERR;
      	char* funcname = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
        ClixLexType fntype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
        
        //printf("Funcname is: %s\n", funcname);
        
        valera_array_t* funcargs = valera_array_new();
        while(1) {
          idx++;
          if(idx>=len) break; // INCORERR;
          
          char* tkn = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
          ClixLexType ttype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;
          
          if(strcmp(tkn, "\n")==0) break;
          if(strcmp(tkn, " ")==0) continue;
          if(strcmp(tkn, ",")==0) continue;
          
          //printf("Token: %s\n", tkn);
          
          valera_node_t* tot = valera_new();
          valera_push_string(tot, "token", tkn);
          valera_push_number(tot, "type", ttype);
          
          valera_array_push_object(funcargs, tot);
        }
        
        //VPRINTARR("Args: ", funcargs);
        
        valera_array_push_object(
          out,
          acl_gen_object_array(
            ACL_CALL,
            funcname,
            fntype,
            funcargs,
            line
          )
        );

      	//exit(1);
      }
    }else if(strcmp(token, "WHILE")==0 || strcmp(token, "while")==0) {
    	idx++; INCORERR;

    	char* valwhat = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
    	ClixLexType whattype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;

		idx++; INCORERR;
    	char* valnext = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
    	ClixLexType nexttype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;

    	idx++; INCORERR;
    	char* valtotal = valera_get(valera_array_get(lexed, idx)->obj, "token")->str;
    	ClixLexType totaltype = valera_get(valera_array_get(lexed, idx)->obj, "type")->num;

		printf("Value: %s; Comparison operator: %s; Value: %s;\n", valwhat, valnext, valtotal);

		valera_array_t* body = valera_array_new();
      
        valera_node_t* btok = valera_array_get(lexed, idx)->obj;
        while(1) {
          btok = valera_array_get(lexed, idx)->obj;
          if(strcmp(valera_get(btok, "token")->str, "\n")==0) { idx++; continue; } // May cause skew.
          if(strcmp(valera_get(btok, "token")->str, "END")==0) break;
          valera_array_push_object(body, btok);
          idx++;
        }

        valera_array_push_object(
          out,
          acl_gen_object_arr_if(
            ACL_WHILE,
            valwhat,
            valnext,
            valtotal,
            whattype,
            nexttype,
            totaltype,
            body,
            line
          )
        );

		/*
    	clix_error(filename, line,
    			   valera_get(elem, "start")->num,
    			   valera_get(elem, "end")->num,
    			   "WHILE not supported yet.");
    	*/
    	idx++; continue;
    }else {
   	  if(strcmp(token, "\n")!=0) {
        VPRINT("Error on: ", elem);
        clix_error(filename, line,
                   valera_get(elem, "start")->num,
                   valera_get(elem, "end")->num,
                   "Invalid syntax.");
      }
    }
    
    idx++;
  }
}
