#ifndef ACTIONS_H_
#define ACTIONS_H_

typedef enum ClixActions {
  ACL_DEFINE,
  ACL_SET,
  ACL_ADD,
  ACL_SUB,
  ACL_MUL,
  ACL_DIV,
  ACL_PRINT
} ClixActions;

void make_actionlist(valera_array_t* lexed, valera_array_t* out);

#endif
