#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "../thirdparty/ValeraC/valera.h"
#include "lexer.h"
#include "tokenizer.h"
#include "actions.h"
#include "executor.h"
#include "error.h"

#define VPRINT(text, obj) printf(text); valera_print(obj); puts("");
#define VPRINTARR(text, obj) printf(text); valera_array_print(obj); puts("");
#define CHECKMEM(ptr) if(ptr==NULL) { printf("[%s:%d] fatal error: malloc() failed!", __FILE__, __LINE__); exit(1); }

#endif
