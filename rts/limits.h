/*====================================================================
 
Basic Types
-----------

====================================================================*/

#ifndef THIRTEEN_LIMITS_H
#define THIRTEEN_LIMITS_H

#include <stdint.h>

/* Sized data types */

typedef int8_t          StgInt8;
typedef uint8_t         StgWord8;

typedef int16_t         StgInt16;
typedef uint16_t        StgWord16;

typedef int32_t         StgInt32;
typedef uint32_t        StgWord32;

typedef int64_t         StgInt64;
typedef uint64_t        StgWord64;


#ifdef HAVE_UINTPTR_T
typedef uintptr_t       StgWord;
#else
// #error "System does not define uintptr_t.  Maybe add autoconf check?"
typedef StgWord64       StgWord;
#endif

typedef StgWord*        StgPtr; /* pointer into heap or stack */

#endif /* THIRTEEN_LIMITS_H */
