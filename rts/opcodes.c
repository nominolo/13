#include "opcodes.h"

const char *const thirteen_opnames[NUM_OPCODES + 1] = {
  "move",
  "loadk",

  "addi",
  "subi",
  "muli",
  "divi",
  "modi",
  "negi",

  "addf",
  "subf",
  "mulf",
  "divf",
  "modf",
  "negf",

  "addd",
  "subd",
  "muld",
  "divd",
  "modd",
  "negd",
 
  "and",
  "or",
  "not",

  "eq",
  "lt",
  "le",
  
  "store",
  "load",
  "update",
 
  "eval",
  "jmp",
  "call",
  "tailcall",

  "return",
  0
};
