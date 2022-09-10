#include "../include/global.h"

ClixFFIFunction* find_FFI_func(ClixContext* ctx, char* name) {
  int clen = valera_array_length(ctx->cfuncs);
  
  for(int i=0; i<clen; i++) {
    valera_node_t* elem = valera_array_get(ctx->cfuncs, i)->obj;
    
    char* elname = valera_get(elem, "name")->str;
    int argc = valera_get(elem, "argc")->num;
    void (*elfunc)(valera_array_t*) = (void*)(valera_array_t*)valera_get(elem, "func")->str;
    
    if(strcmp(elname, name)==0) {
      return &(ClixFFIFunction){
        elname, argc, elfunc
      };
    }
  }
  return 0;
}

void add_FFI_func(ClixContext* ctx, ClixFFIFunction* func) {
  valera_node_t* obj = valera_new();
  
  valera_push_string(obj, "name", func->name);
  valera_push_number(obj, "argc", func->argc);
  valera_push_string(obj, "func", (void*)(valera_array_t*)func->func);
  
  valera_array_push_object(ctx->cfuncs, obj);
}