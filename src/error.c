#include "../include/global.h"
#include <stdlib.h>

unsigned int digit_count(int n) {
  int num = n, digits;
  if (num < 0) num = -num;
  else if (num == 0) digits=1;
  
  for (digits = 0; num > 0; digits++) num /= 10;
  return digits;
}

void clix_error(char* filename, unsigned int line,
                unsigned int hls, unsigned int hle,
                char* msg) {
  FILE* fp = fopen(filename, "r");
  if(!fp) {
    printf("\033[31mDoubleFault\033[0m: Failed to open '%s' to extract a line.\n", filename);
    exit(1);
  }
  
  char* linebuf;
  size_t linebuf_sz;
  int linesize = getline(&linebuf, &linebuf_sz, fp);
  int lastlinesize = linesize;
  
  for(int i=0; i<line-1; i++) {
    lastlinesize = getline(&linebuf, &linebuf_sz, fp);
    linesize += lastlinesize;
  }
  
  printf("\033[31mERROR \033[0m[%s:%d]: %s\n", filename, line, msg);
  printf("    %d |  %s", line, linebuf);
  
  int hs = (linesize-lastlinesize)-hls;
  int hlen = hle-hs;
  
  printf("    ");
  for(int _=0; _<digit_count(line); _++) printf(" ");
  printf("    ");
  for(int _=0; _<hs; _++) printf(" ");
  for(int _=0; _<hlen; _++) printf("~");
  printf("\n");
  
  exit(1);
}