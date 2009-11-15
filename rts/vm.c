#include <stdio.h>

#include "limits.h"
#include "opcodes.h"
#include "vm.h"
#include "decode.h"

#define barf    printf

#define RA(i)   (base + GETARG_A(i))
#define RB(i)   (base + GETARG_B(i))
#define RC(i)   (base + GETARG_C(i))
#define RKB(i)  (ISK(GETARG_B(i)) ? k + INDEXK(GETARG_B(i)) : base + GETARG_B(i))
#define RKC(i)  (ISK(GETARG_C(i)) ? k + INDEXK(GETARG_C(i)) : base + GETARG_C(i))

/*
  Assumes stack base points to a closure / function.
 */
void vm_execute(State *L, int nested_calls)
{
  const Instruction *pc;
  StgWord *k;
  StgPtr base = L->stack;
  const StgBCO *bco;
  StgHeader* header = cast(StgHeader*, *base);
  const StgInfoTable* info = cast(const StgInfoTable*, header->info);

  if (!info->tag == TAG_CLOSURE) barf("vm_execute: Argument not a closure.");
  bco = &info->payload.bco;
  int b;

 reentry:
  pc = bco->code;
  k = bco->k;

  for(;;) {
    printInstruction(pc);
    const Instruction i = *pc++;

    StgPtr ra;  // where R(A) points to
    ra = RA(i);
    
    switch (GET_OPCODE(i)) {

    case OP_MOVE:
      *ra = *RB(i);
      continue;

    case OP_ADDI:
      *ra = *RKB(i) + *RKC(i);
      continue;

    case OP_SUBI:
      *ra = *RKB(i) - *RKC(i);
      continue;

    case OP_MULI:
      *ra = *RKB(i) * *RKC(i);
      continue;

    case OP_DIVI:
      *ra = *RKB(i) / *RKC(i);
      continue;

    case OP_MODI:
      *ra = *RKB(i) % *RKC(i);
      continue;

    case OP_NEGI:
      *ra = -(*RKB(i));
      continue;

    case OP_RETURN:
      b = GETARG_B(i);
      if (b != 0) { L->top = ra + b - 1; }
      if (--nested_calls == 0)
	return;  // Return from interpreter
      else {
	// TODO
	if (b) L->top = 0;
	goto reentry;
      }
    default:
      barf("Unimplemented instruction");
    }
  }
}

void test_call()
{
  StgWord s[16 * 1024];
  State L;
  Instruction is[] =
  // Type:  <Int#, Int#> -> Int#
    { // CREATE_ABC(OP_MOVE, 1, 0, 0),
      CREATE_ABC(OP_ADDI, 0, 1, 2),
      CREATE_ABC(OP_RETURN, 0, 2, 0)
    };
  const StgInfoTable tbl1 =
    { tag: TAG_CLOSURE,
      payload: { bco: { k: 0, sizek: 0,
			code: is, sizecode: sizeof(is)/sizeof(Instruction) } }
    };
    
  StgHeader obj = { info: &tbl1 };

  s[0] = cast(StgWord, &obj);
  s[1] = 17;
  s[2] = 33;

  L.stack = s;
  L.top = s + 3;

  printStack(L.stack, L.top);
  vm_execute(&L, 1);
  printStack(L.stack, L.top);
}

int main(int argc, char* argv[])
{
  printf("Hello world!\n");

  printf("%i\n", NUM_OPCODES);

  test_call();
  //printf("%li %li %li", sizeof(StgWord), sizeof(void*), sizeof(int));

  return 0;
}

