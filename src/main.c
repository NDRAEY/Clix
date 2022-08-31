#include <stdio.h>
#include <stdlib.h>
#include "../include/global.h"

int main(int argc, char** argv) {
	if(argc<=1) {
		printf("Usage: clix file\n");
		exit(1);
	}

	char* file = argv[argc-1];
	FILE* fptr = fopen(file, "r");
	if(!fptr) {
		printf("\033[31mERROR\033[0m: File '%s' was not found!\n", file);
		exit(1);
	}
	fseek(fptr, 0, SEEK_END);
	int filesize = ftell(fptr);
	
	char* buffer = calloc(filesize+1, sizeof(char));
	CHECKMEM(buffer);

	rewind(fptr);

	fread(buffer, 1, filesize, fptr);
	fclose(fptr);
	
	valera_array_t* tokenized = valera_array_new();
	tokenize(buffer, tokenized);
	//VPRINTARR("Tokenized: ", tokenized);
	
	valera_array_t* lexed = valera_array_new();
	lex(tokenized, lexed);
	//VPRINTARR("Lexed: ", lexed);

	free(buffer);
	
	valera_array_t* actionlist = valera_array_new();
	make_actionlist(file, lexed, actionlist);
	// VPRINTARR("Action list: ", actionlist);
	
	ClixContext ctx = make_context();
	execute(file, &ctx, actionlist);

	valera_array_destroy(tokenized);
	valera_array_destroy(lexed);
	destroy_context(ctx);
	
	return 0;
}
