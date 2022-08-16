#ifndef ERROR_H_
#define ERROR_H_

void clix_error(char* filename, unsigned int line,
                unsigned int hls, unsigned int hle,
                char* msg);

#endif
