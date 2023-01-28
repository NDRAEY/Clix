#include "../include/global.h"
#include <string.h>
#include <stdlib.h>

void tokenize(const char* code, valera_array_t* tokenized) {
	valera_array_t* sptokens  = valera_array_new();
	
	unsigned int idx = 0;
	unsigned int len = strlen(code);
	unsigned int tokenslength = strlen(TOKENS);
	unsigned int line = 1;
	while(idx<len) {
		char elm = code[idx];
		
		char* exists = memchr(TOKENS, elm, tokenslength);
		
		if(exists) {
			int splen = valera_array_length(sptokens);
			if(splen) {
				char* collected = calloc(valera_array_join_size(sptokens, "")+1, sizeof(char)); // Don't forget to free it!
				if(collected==NULL) {
					printf("error: calloc() failed! (%s:%d)\n", __FILE__, __LINE__);
					exit(1);
				}
				valera_array_join(sptokens, collected, "");
				
				valera_node_t* obj = valera_new();
				valera_push_string(obj, "token", collected);
				valera_push_number(obj, "start", idx-splen);
				valera_push_number(obj, "line", line);
				valera_push_number(obj, "end", idx);
				
				valera_array_push_object(tokenized, obj);
				
				while(valera_array_length(sptokens)) {
					valera_array_pop(sptokens);
				}
			}
			// printf(">>Pushing token: %c\n", elm);
			
			char* one = malloc(2);
			one[0] = elm;
			one[1] = 0;

			if(elm=='\n') { line++; }
			
			if(elm=='\"') {
				int starti = idx;
				valera_array_t* col = valera_array_new();
				valera_array_push_number(col, elm);
				idx++;
				while(1) {
					elm = code[idx];
					valera_array_push_number(col, elm);
					if(elm=='\"') { idx++; break; }
					if(elm=='\n') { line++; }
					idx++;
				}
				
				char* totstring = malloc(valera_array_length(col)+1);
				memset(totstring, 0, valera_array_length(col)+1);
				for(int _ = 0, len = valera_array_length(col); _<len; _++) {
					totstring[_] = valera_array_get(col, _)->num;
				}
				valera_array_destroy(col);
				
				//printf("COLLECTED STRING: %s\n", totstring);
				
				free(one);
				valera_node_t* obj = valera_new();
				valera_push_string(obj, "token", totstring);
				valera_push_number(obj, "start", starti);
				valera_push_number(obj, "end", idx);
				valera_push_number(obj, "line", line);
				
				valera_array_push_object(tokenized, obj);
				idx++; continue;
			}

			valera_node_t* obj = valera_new();
			valera_push_string(obj, "token", one);
			valera_push_number(obj, "start", idx);
			valera_push_number(obj, "end", idx+1);
			valera_push_number(obj, "line", line);
			
			valera_array_push_object(tokenized, obj);
			
			//valera_array_push_string(tokenized, one);			
		} else {
			//printf("Pushing token: %c\n", elm);
			
			char* one = malloc(2);
			one[0] = elm;
			one[1] = '\0';
			
			/*
			valera_node_t* obj = valera_new();
			valera_push_string(obj, "token", one);
			valera_push_number(obj, "start", idx);
			valera_push_number(obj, "end", idx+1);
			valera_array_push_object(tokenized, obj);
			*/

			// printf("Adding: %c\n", elm);
			
			valera_array_push_string(sptokens, one);
		}
		
		idx++;
	}
	
	if(valera_array_length(sptokens)) {
		char* collected = malloc(valera_array_join_size(sptokens, "")+1);
		valera_array_join(sptokens, collected, "");
		
		valera_node_t* obj = valera_new();
		valera_push_string(obj, "token", collected);
		valera_push_number(obj, "start", idx-valera_array_length(sptokens));
		valera_push_number(obj, "end", idx);
		valera_push_number(obj, "line", line);
		
		valera_array_push_object(tokenized, obj);
		
		
		while(valera_array_length(sptokens)) {valera_array_pop(sptokens);}
	}
	
	valera_array_destroy(sptokens);
}
