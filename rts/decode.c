#include <stdio.h>

#include "decode.h"

void printInstruction(const Instruction *i_p)
{
  Instruction i = *i_p;
  int b;
  int opc = GET_OPCODE(i);

  printf("%16p: %08x   %-8s ", i_p, i, opc < NUM_OPCODES ? thirteen_opnames[opc] : "?");

  switch (opc) {

  case OP_MOVE:
    printf("r(%i), r(%i)", GETARG_A(i), GETARG_B(i));
    break;

  case OP_RETURN:
    b = GETARG_B(i);
    switch (b) {
    case 0: printf("*"); break;
    case 1: break;
    case 2: printf("r(%i)", GETARG_A(i)); break;
    default:
      printf("r(%i)..r(%i)", GETARG_A(i), GETARG_A(i) + b - 2);
    }
    break;

  case OP_ADDI: case OP_SUBI: case OP_MULI: case OP_DIVI: case OP_MODI:
  case OP_ADDF: case OP_SUBF: case OP_MULF: case OP_DIVF: case OP_MODF:
  case OP_ADDD: case OP_SUBD: case OP_MULD: case OP_DIVD: case OP_MODD:
  case OP_AND: case OP_OR:
    printf("r(%i), %c(%i), %c(%i)",
	   GETARG_A(i),
	   ISK(GETARG_B(i)) ? 'k' : 'r', GETARG_B(i),
	   ISK(GETARG_C(i)) ? 'k' : 'r', GETARG_C(i));
    break;

  default:
    printf("Opc=%02x A=%i B=%i C=%i (Bx=%05x/%i)",
	   GET_OPCODE(i), GETARG_A(i),
	   GETARG_B(i), GETARG_C(i),
	   GETARG_Bx(i), GETARG_sBx(i));
  }
  printf("\n");
}

void printStack(StgPtr base, StgPtr top) {
  int c = 0;
  printf("Stack: ");
  while (c < 255 && top != base) {
    printf("%16lx ", (unsigned long)*base);
    ++base; ++c;
  }
  printf("\n");
}

