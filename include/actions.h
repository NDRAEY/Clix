#ifndef ACTIONS_H_
#define ACTIONS_H_

typedef enum ClixActions {
  ACL_DEFINE,
  ACL_SET,
  ACL_ADD,
  ACL_SUB,
  ACL_MUL,
  ACL_DIV,
  ACL_SHR,
  ACL_SHL,
  ACL_AND,
  ACL_OR,
  ACL_XOR,
  ACL_NOT,
  ACL_IF,
  ACL_PRINT,
  ACL_CALL
} ClixActions;

void make_actionlist(char* filename, valera_array_t* lexed, valera_array_t* out);

#endif
