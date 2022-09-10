#include "../include/global.h"
#include <stdlib.h>

#define NOTFOUND if(!variable_exists(ctx, vname)) { \
  clix_error(filename, valera_get(elem, "line")->num, -1, -1, "Variable '%s' not found!", vname); \
  exit(1);\
}

#define NOTFOUND_(vn) if(!variable_exists(ctx, vn)) { \
  clix_error(filename, valera_get(elem, "line")->num, -1, -1, "Variable '%s' not found!", vn); \
  exit(1);\
}

#define NOTFOUND__(elem, vn) if(!variable_exists(ctx, vn)) { \
  clix_error(filename, valera_get(elem, "line")->num, -1, -1, "Variable '%s' not found!", vn); \
  exit(1);\
}

#define NUMEXC if(get_variable_type(ctx, vname)==VAR_STRING) { \
  printf("\033[31mERROR\033[0m: RuntimeError: Excepted number, but variable type is string!\n"); \
  exit(1); \
}

ClixContext make_context() {
  ClixContext obj;
  obj.variables = valera_array_new();
  obj.cfuncs = valera_array_new();
  obj.functions = valera_array_new();
  return obj;
}

void destroy_context(ClixContext ctx) {
  valera_array_destroy(ctx.variables);
  valera_array_destroy(ctx.functions);
  valera_array_destroy(ctx.cfuncs);
}

void initalize_variable(ClixContext* ctx, char* name, valera_value_t* value) {
  valera_node_t* obj = valera_new();
  valera_push_string(obj, "name", name);
  valera_push(obj, "value", value);
  valera_array_push_object(ctx->variables, obj);
}

int find_variable_idx(ClixContext* ctx, char* name) {
  unsigned int len = valera_array_length(ctx->variables);
  
  for(unsigned int idx=0; idx<len; idx++) {
    char* elemname = valera_get(valera_array_get(ctx->variables, idx)->obj, "name")->str;
    if(strcmp(elemname, name)==0) {
      return idx;
    }
  }
  return -1;
}

char variable_exists(ClixContext* ctx, char* name) {
  return find_variable_idx(ctx, name)<=-1?0:1;
}

ClixVariableType valera2clixVT(int valera_type) {
  if(valera_type==VAL_NUM) return VAR_NUMBER;
  if(valera_type==VAL_STR) return VAR_STRING;
  return -1;
}

valera_value_t* get_variable_value(ClixContext *ctx, char* name) {
  int _ = find_variable_idx(ctx, name);
  if(_<=-1) {return 0;}
  valera_value_t* v = valera_get(valera_array_get(ctx->variables, _)->obj, "value");
  return v;
}

ClixVariableType get_variable_type(ClixContext *ctx, char* name) {
  valera_value_t* val = get_variable_value(ctx, name);
  if(val==0) return -1;
  return valera2clixVT(val->type);
}

valera_array_t* filter_args(valera_array_t* args) {
  valera_array_t* new = valera_array_new();
  
  for(int i=0, argslen=valera_array_length(args); i<argslen; i++) {
    valera_array_push(new, valera_get(valera_array_get(args, i)->obj, "token"));
  }
  
  valera_array_destroy(args);
  return new;
}

// Makes new array with values extracted from variables, skipping other values
valera_array_t* eval_variables_over(char* filename, ClixContext* ctx, valera_array_t* array) {
  valera_array_t* new = valera_array_new();
  
  int arl = valera_array_length(array);
  
  for(int i=0; i<arl; i++) {
    valera_node_t* curobj = valera_array_get(array, i)->obj;
    char* arg = valera_get(curobj, "token")->str;
    ClixLexType argtype = valera_get(curobj, "type")->num;
    
    if(argtype==LEX_NAME) {
      NOTFOUND__(curobj, arg);
      valera_value_t* var = get_variable_value(ctx, arg);
      
      valera_node_t* obj = valera_new();
      valera_push(obj, "token", var);
      valera_push_number(obj, "type", var->type==VAL_NUM?LEX_NUMBER:(var->type==VAL_STR?LEX_STRING:LEX_NAME));
      
      valera_array_push_object(new, obj);
    }else{
      valera_node_t* preparer = valera_new();
      
      if(argtype==LEX_NUMBER) {
        valera_push_number(preparer, "token", atoi(arg));
      }else if(argtype==LEX_STRING) {
        valera_push_string(preparer, "token", arg);
      }else{
        printf("[Clix] Other types?\n");
      }
      valera_push_number(preparer, "type", argtype);
      
      valera_array_push_object(new, preparer);
    }
  }
  
  valera_array_destroy(array);
  
  return new;
}

void execute(char* filename, ClixContext* ctx, valera_array_t* actions) {
  unsigned int idx = 0;
  unsigned int len = valera_array_length(actions);
  
  while(idx<len) {
    valera_node_t* elem = valera_array_get(actions, idx)->obj;
    unsigned int action = valera_get(elem, "action")->num;
    //VPRINT("Data: ", elem);
    
    if(action==ACL_DEFINE) {
      valera_value_t* _ = valera_value_new();
      valera_value_set_number(_, 0);
      initalize_variable(ctx, valera_get(elem, "token")->str, _);
      
      // VPRINTARR("Context dump: ", ctx->variables);
    }else if(action==ACL_ADD) {
      char* vname = valera_get(elem, "tok1")->str;
      NOTFOUND;
      NUMEXC;
      valera_value_t* var = get_variable_value(ctx, vname);
      unsigned int number = atoi(valera_get(elem, "tok2")->str);
      valera_value_set_number(var, var->num+number);
    }else if(action==ACL_SUB) {
      char* vname = valera_get(elem, "tok1")->str;
      NOTFOUND;
      NUMEXC;
      valera_value_t* var = get_variable_value(ctx, vname);
      unsigned int number = atoi(valera_get(elem, "tok2")->str);
      valera_value_set_number(var, var->num-number);
    }else if(action==ACL_MUL) {
      char* vname = valera_get(elem, "tok1")->str;
      NOTFOUND;
      NUMEXC;
      valera_value_t* var = get_variable_value(ctx, vname);
      unsigned int number = atoi(valera_get(elem, "tok2")->str);
      valera_value_set_number(var, var->num*number);
    }else if(action==ACL_DIV) {
      char* vname = valera_get(elem, "tok1")->str;
      NOTFOUND;
      NUMEXC;
      valera_value_t* var = get_variable_value(ctx, vname);
      unsigned int number = atoi(valera_get(elem, "tok2")->str);
      valera_value_set_number(var, var->num/number);
    }else if(action==ACL_PRINT) {
      char* token = valera_get(elem, "token")->str;
      ClixLexType tt = valera_get(elem, "type")->num;
      if(tt==LEX_NUMBER || tt==LEX_STRING) {
        printf("%s\n", token);
      }else if(tt==LEX_NAME) {
        if(!variable_exists(ctx, token)) {
          printf("\033[31mERROR\033[0m: RuntimeError: Variable '%s' was not found!\n", token);
          exit(1);
        }
        
        valera_value_t* a = get_variable_value(ctx, token);
        
        if(a->type==VAL_NUM) {
          printf("%d\n", a->num);
        }else if(a->type==VAL_STR) {
          printf("%s\n", a->str);
        }/*else{
          printf("VT: %d\n", a->type);
        }*/
      }else{
        printf("\033[31mERROR\033[0m: RuntimeError: Other types to print is unimplemeted!\n");
      }
    }else if(action==ACL_SET) {
      char* t1 = valera_get(elem, "tok1")->str;
      ClixLexType tt = valera_get(elem, "typ1")->num;

      NOTFOUND_(t1);
      
      char* t2 = valera_get(elem, "tok2")->str;
      ClixLexType tk = valera_get(elem, "typ2")->num;
      
      valera_value_t* var = get_variable_value(ctx, t1);
      
      if(tk==LEX_NUMBER) { valera_value_set_number(var, atoi(t2)); }
      if(tk==LEX_STRING) { valera_value_set_string(var, t2); }
    }else if(action==ACL_IF) {
      char* what = valera_get(elem, "tok1")->str;
      ClixLexType wt = valera_get(elem, "typ1")->num;
      char* comp = valera_get(elem, "tok2")->str;
      char* with = valera_get(elem, "tok3")->str;
      ClixLexType wit = valera_get(elem, "typ3")->num;

      valera_array_t* execode = valera_get(elem, "tokens")->arr;
      
      if(wt==LEX_NAME) {
        NOTFOUND_(what);
      }
      // EQ/IS - EQUAL
      // LESS - LESS
      // GREATER - GREATER
      // LEQ - LESS OR EQUAL
      // GEQ - GREATER OR EQUAL
      // NEQ - NOT EQUAL
      
      if(strcmp(comp, "EQ")==0 || strcmp(comp, "IS")==0) {
        if(wt==LEX_NAME) {
          ClixVariableType vtyp = get_variable_type(ctx, what);
          if(vtyp==VAR_NUMBER) {
            int vval = get_variable_value(ctx, what)->num;
            
            if(wit==LEX_NUMBER) {
              int wval = atoi(with);
              
              if(vval==wval) {
                // Execute code
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_STRING){
              clix_error(
                filename, valera_get(elem, "line")->num,
                -1, -1, "Cannot compare unsupported types: Number and String!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables\n");
              exit(1);
            }
          }else if(vtyp==VAR_STRING) {
            char* vval = get_variable_value(ctx, what)->str;
            
            if(wit==LEX_STRING) {
              if(strcmp(vval, with)==0) {
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_NUMBER) {
              clix_error(
                filename,
                valera_get(elem, "line")->num,
                -1, -1,
                "Cannot compare unsupported types (EQ): String and Number!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables (EQ)\n");
              exit(1);
            }
          }
        }else{
          printf("[UNIMPL] Number|Number and String|String comprasion temporaily unavailable!\nExiting...\n");
          exit(1);
        }
      }else if(strcmp(comp, "LESS")==0) {
        if(wt==LEX_NAME) {
          ClixVariableType vtyp = get_variable_type(ctx, what);
          if(vtyp==VAR_NUMBER) {
            int vval = get_variable_value(ctx, what)->num;

            if(wit==LEX_NUMBER) {
              int wval = atoi(with);
              
              if(vval<wval) {
                // Execute code
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_STRING){
              clix_error(
                filename, valera_get(elem, "line")->num,
                -1, -1, "Cannot compare unsupported types (LESS): Number and String!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables (LESS)\n");
              exit(1);
            }
          }else if(vtyp==VAR_STRING) {
            clix_error(
              filename, valera_get(elem, "line")->num,
              -1, -1, "Cannot compare unsupported types (LESS): String and String!"
            );
          }
        }else{
          printf("[UNIMPL] Number|Number and String|String comprasion temporaily unavailable!\nExiting...\n");
          exit(1);
        }
      }else if(strcmp(comp, "GREATER")==0) {
        if(wt==LEX_NAME) {
          ClixVariableType vtyp = get_variable_type(ctx, what);
          if(vtyp==VAR_NUMBER) {
            int vval = get_variable_value(ctx, what)->num;

            if(wit==LEX_NUMBER) {
              int wval = atoi(with);
              
              if(vval>wval) {
                // Execute code
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_STRING){
              clix_error(
                filename, valera_get(elem, "line")->num,
                -1, -1, "Cannot compare unsupported types (GREATER): Number and String!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables (GREATER)\n");
              exit(1);
            }
          }else if(vtyp==VAR_STRING) {
            clix_error(
              filename, valera_get(elem, "line")->num,
              -1, -1, "Cannot compare unsupported types (GREATER): String and String!"
            );
          }
        }else{
          printf("[UNIMPL] Number|Number and String|String comprasion temporaily unavailable!\nExiting...\n");
          exit(1);
        }
      }else if(strcmp(comp, "NEQ")==0) {
        if(wt==LEX_NAME) {
          ClixVariableType vtyp = get_variable_type(ctx, what);
          if(vtyp==VAR_NUMBER) {
            int vval = get_variable_value(ctx, what)->num;
            
            if(wit==LEX_NUMBER) {
              int wval = atoi(with);
              
              if(vval!=wval) {
                // Execute code
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_STRING){
              clix_error(
                filename, valera_get(elem, "line")->num,
                -1, -1, "Cannot compare unsupported types: Number and String!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables\n");
              exit(1);
            }
          }else if(vtyp==VAR_STRING) {
            char* vval = get_variable_value(ctx, what)->str;
            
            if(wit==LEX_STRING) {
              if(strcmp(vval, with)!=0) {
                valera_array_t* track01 = valera_array_new();
                make_actionlist(filename, execode, track01);
                execute(filename, ctx, track01); // SHARED CONTEXT
                valera_array_destroy(track01);
              }
            }else if(wit==LEX_NUMBER) {
              clix_error(
                filename,
                valera_get(elem, "line")->num,
                -1, -1,
                "Cannot compare unsupported types (NEQ): String and Number!"
              );
            }else{
              printf("[UNIMPL] Currently cannot compare variables (NEQ)\n");
              exit(1);
            }
          }
        }else{
          printf("[UNIMPL] Number|Number and String|String comprasion temporaily unavailable!\nExiting...\n");
          exit(1);
        }
      }else{
        printf("Other type - unavailable\n");
        exit(1);
      }
      
      printf("[WARN] IF is not fully implemented!\n");
    }else if(action==ACL_CALL) {
      //0rintf("NOT YET IMPLEMENTED (CALL)\n\n");
      
      char* fname = valera_get(elem, "tok1")->str;
      valera_array_t* fargs = valera_get(elem, "tokens")->arr;
      
      //printf("Function name: %s\n", fname);
      
      fargs = eval_variables_over(filename, ctx, fargs);
      //VPRINTARR("Function arguments: ", fargs);

      fargs = filter_args(fargs);
      //VPRINTARR("Function arguments: ", fargs);
      
      //void (*func)(valera_array_t*) = find_FFI_func(ctx, fname);
      ClixFFIFunction* func = find_FFI_func(ctx, fname);
      if(func==0) {
        clix_error(filename, valera_get(elem, "line")->num, -1, -1, "Function '%s' not found!", fname);
        exit(1);
      }
      func->func(ctx, fargs);
      
      valera_array_destroy(fargs);
      //exit(1);
    }else{
      printf("Unimpelemented functions found! => %s (%d)\n", acl_to_string(action), action);
      exit(1);
    }
    
    idx++;
  }
}
