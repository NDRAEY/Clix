#ifndef EXECUTOR_H_
#define EXECUTOR_H_

#include "../thirdparty/ValeraC/valera.h"

typedef struct ClixContext {
  valera_array_t* variables;
  valera_array_t* functions;
  valera_array_t* cfuncs;
} ClixContext;

typedef enum ClixVariableType {
  VAR_NUMBER,
  VAR_STRING
} ClixVariableType;

ClixContext make_context();
void destroy_context(ClixContext ctx);
void execute(char* filename, ClixContext* ctx, valera_array_t* actions);

#endif
