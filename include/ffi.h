#ifndef FFI_H
#define FFI_H

#include "../thirdparty/ValeraC/valera.h"

typedef struct ClixFFIFunction {
	char* name;
	unsigned int argc;
	void (*func)(valera_array_t*);
} ClixFFIFunction;

ClixFFIFunction* find_FFI_func(ClixContext* ctx, char* name);
void add_FFI_func(ClixContext* ctx, ClixFFIFunction* func);

#endif
