#ifndef THIRTEEN_VM_H
#define THIRTEEN_VM_H

#include "limits.h"
#include "opcodes.h"

typedef int     Value;
typedef Value*  StkId;

typedef struct _StgHeader    StgHeader;
typedef struct _StgInfoTable StgInfoTable;
typedef struct _StgBCO       StgBCO;

struct _StgHeader {
  const StgInfoTable *info;
};

typedef enum {
  TAG_CLOSURE = 1,
} StgTag;

struct _StgBCO {
  StgWord* k;         // constants referenced by the code
  Instruction* code;  // the byte code instructions
  int sizek;          // number of constants
  int sizecode;       // number of byte code words
};

struct _StgInfoTable {
  StgHeader header;
  StgTag tag;
  union {
    StgBCO bco;
  } payload;
};

/* TODO: This should become a ThreadStateObject (TSO). */

typedef struct _State {
  StgPtr stack;  // bottom of the stack
  StgPtr top;    // top of the stack
} State;

void vm_execute(State *L, int nested_calls);

#endif /* THIRTEEN_VM_H */
