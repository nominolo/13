#ifndef THIRTEEN_OPCODES_H
#define THIRTEEN_OPCODES_H

#include <stdint.h>

#ifndef cast
#define cast(t, exp)    ((t)(exp))
#endif

/*********************************************************************

Instruction Formats
-------------------

(Same as Lua 5.1)

   +--------+---------+---------+---------+
   | Opcode | Param A | Param C | Param B |   three arguments
   +--------+---------+---------+---------+
   | Opcode | Param A | Param Bx          |   two arguments
   +--------+---------+-------------------+
   | Opcode | Param A | Param sBx         |   signed Bx
   +--------+---------+-------------------+

 - TODO: Maybe add fourth format with sABx for JMP instruction, so we
   can cover a larger range. (26 bits vs. 18bits)

*********************************************************************/

typedef uint32_t Instruction;

/*
 * Size and position of instruction arguments
 */

#define SIZE_C          9
#define SIZE_B          9
#define SIZE_Bx         (SIZE_C + SIZE_B)
#define SIZE_A          8

#define SIZE_OP         6

/* Position (from the left) */
#define POS_OP          0
#define POS_A           (POS_OP + SIZE_OP)
#define POS_C           (POS_A + SIZE_A)
#define POS_B           (POS_C + SIZE_C)
#define POS_Bx          POS_C

/*
 * Size limits for opcode arguments.
 */
#define MAXARG_Bx       ((1 << SIZE_Bx) - 1)
#define MAXARG_sBx      (MAXARG_Bx >> 1)

#define MAXARG_A        ((1 << SIZE_A) - 1)
#define MAXARG_B        ((1 << SIZE_B) - 1)
#define MAXARG_C        ((1 << SIZE_C) - 1)

/*
 * Creates a mask with `n' 1 bits at position `p' (counting from the right).
 * All other bits will be 0.  E.g. MASK1(8,4) = 0x00000ff0
 */
#define MASK1(n,p)      ((~((~(Instruction)0) << n)) << p)

/* 
 * Creates a mask with `n' 0 bits at position `p' (counting from the right).
 * All other bits will be 1.  E.g., MASK0(8,4) = 0xfffff00f
 */
#define MASK0(n,p)      (~MASK1(n,p))

/*
 * Instruction manipulation
 */
#define GET_OPCODE(i)   (cast(OpCode, ((i) >> POS_OP) & MASK1(SIZE_OP,0)))
#define SET_OPCODE(i,o) ((i) = (((i) & MASK0(SIZE_OP,POS_OP)) | \
                                ((cast(Instruction, o) << POS_OP) \
                                 & MASK1(SIZE_OP,POS_OP))))

#define GETARG_A(i)     (cast(int, ((i) >> POS_A) & MASK1(SIZE_A,0)))
#define SETARG_A(i,u)   ((i) = (((i) & MASK0(SIZE_A,POS_A)) | \
                                ((cast(Instruction, u) << POS_A) \
                                 & MASK1(SIZE_A,POS_A))))

#define GETARG_B(i)     (cast(int, ((i) >> POS_B) & MASK1(SIZE_B,0)))
#define SETARG_B(i,b)   ((i) = (((i) & MASK0(SIZE_B,POS_B)) | \
                                ((cast(Instruction, b) << POS_B) \
                                 & MASK1(SIZE_B,POS_B))))

#define GETARG_C(i)     (cast(int, ((i)>>POS_C) & MASK1(SIZE_C,0)))
#define SETARG_C(i,b)   ((i) = (((i)&MASK0(SIZE_C,POS_C)) | \
                ((cast(Instruction, b)<<POS_C)&MASK1(SIZE_C,POS_C))))

#define GETARG_Bx(i)    (cast(int, ((i)>>POS_Bx) & MASK1(SIZE_Bx,0)))
#define SETARG_Bx(i,b)  ((i) = (((i)&MASK0(SIZE_Bx,POS_Bx)) | \
                ((cast(Instruction, b)<<POS_Bx)&MASK1(SIZE_Bx,POS_Bx))))

#define GETARG_sBx(i)   (GETARG_Bx(i)-MAXARG_sBx)
#define SETARG_sBx(i,b) SETARG_Bx((i),cast(unsigned int, (b)+MAXARG_sBx))


#define CREATE_ABC(o,a,b,c)     ((cast(Instruction, o)<<POS_OP) \
                        | (cast(Instruction, a)<<POS_A) \
                        | (cast(Instruction, b)<<POS_B) \
                        | (cast(Instruction, c)<<POS_C))

#define CREATE_ABx(o,a,bc)      ((cast(Instruction, o)<<POS_OP) \
                        | (cast(Instruction, a)<<POS_A) \
                        | (cast(Instruction, bc)<<POS_Bx))

/* this bit 1 means constant (0 means register) */
#define BITRK           (1 << (SIZE_B - 1))

/* test whether value is a constant */
#define ISK(x)          ((x) & BITRK)

/* gets the index of the constant */
#define INDEXK(r)       ((int)(r) & ~BITRK)

#define MAXINDEXRK      (BITRK - 1)

/* code a constant index as a RK value */
#define RKASK(x)        ((x) | BITRK)


typedef enum {
/*
  name            args    description
  --------------------------------
 */
  OP_MOVE,     /* A B 0   R(A) := R(B) */
  OP_LOADK,    /* A Bx    R(A) := Kst(Bx) */

  /* Unboxed int primitives, the rest must go via C */
  
  OP_ADDI,     /* A B C   R(A) := RK(B) + RK(C) */
  OP_SUBI,     /* A B C   R(A) := RK(B) - RK(C) */
  OP_MULI,     /* A B C   R(A) := RK(B) * RK(C) */
  OP_DIVI,     /* A B C   R(A) := RK(B) / RK(C) */
  OP_MODI,     /* A B C   R(A) := RK(B) % RK(C) */
  OP_NEGI,     /* A B 0   R(A) := -RK(B) */  /* TODO: use SUBI instead? */

  /* Unboxed float primitives. */

  OP_ADDF,     /* A B C   R(A) := RK(B) + RK(C) */
  OP_SUBF,     /* A B C   R(A) := RK(B) - RK(C) */
  OP_MULF,     /* A B C   R(A) := RK(B) * RK(C) */
  OP_DIVF,     /* A B C   R(A) := RK(B) / RK(C) */
  OP_MODF,     /* A B C   R(A) := RK(B) % RK(C) */
  OP_NEGF,     /* A B 0   R(A) := -RK(B) */

  /* Unboxed double primitives */

  OP_ADDD,     /* A B C   R(A) := RK(B) + RK(C) */
  OP_SUBD,     /* A B C   R(A) := RK(B) - RK(C) */
  OP_MULD,     /* A B C   R(A) := RK(B) * RK(C) */
  OP_DIVD,     /* A B C   R(A) := RK(B) / RK(C) */
  OP_MODD,     /* A B C   R(A) := RK(B) % RK(C) */
  OP_NEGD,     /* A B 0   R(A) := -RK(B) */

  /* Bitwise stuff */

  OP_AND,      /* A B C   R(A) := RK(B) & RK(C) */
  OP_OR,       /* A B C   R(A) := RK(B) | RK(C) */
  OP_NOT,      /* A B 0   R(A) := ~RK(B) */

  /* Comparisons */

  OP_EQ,       /* A B C   if (RK(B) == RK(C)) ~= A then PC++ */
  OP_LT,       /* A B C   if (RK(B) <  RK(C)) ~= A then PC++ */
  OP_LE,       /* A B C   if (RK(B) <= RK(C)) ~= A then PC++ */
  /* OP_CASE,   .. no idea how that would work yet */

  /* Memory access */

  OP_STORE,    /* A B C   R(A) := Hp, M[Hp:Hp+C-1] := R(B:B+C-1)      */
  OP_LOAD,     /* A B C   R(A:A+C-1) := M[R(B:B+C-1)]                 */
  OP_UPDATE,   /* A B C   M[R(A):R(A)+C-1] := R(B:B+R-1)              */

  /* Control flow */

  OP_EVAL,     /* A B 0   r(A) := eval(R(A)), update if B = 1         */
  OP_JMP,      /* sBx     pc += sBx                                   */
  OP_CALL,     /* A B C   R(A:A+C-2) := R(A)(R(A+1), ... ,R(A+B-1))   */
  OP_TAILCALL, /* A B C   return R(A)(R(A+1), ... ,R(A+B-1))          */
  OP_RETURN,   /* A B 0   return R(A), ... ,R(A+B-2)                  */

  OP_LAST      /* dummy, make sure it's always the last one           */
} OpCode;

#define NUM_OPCODES     (cast(int, OP_LAST))

const char *const thirteen_opnames[NUM_OPCODES + 1];

#endif /* THIRTEEN_OPCODES_H */
