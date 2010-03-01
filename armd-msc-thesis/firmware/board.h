#ifndef Board_h
#define Board_h

#include "AT91SAM7S256.h"
#define __inline static inline
#include "lib_AT91SAM7S256.h"

#define __ramfunc

#define EXT_OC      18432000   // Exetrnal ocilator MAINCK
//#define MCK       47923200   // MCK (PLLRC div by 2)
#define MCK         95846400   // MCK (PLLRC)
#define MCKKHz      (MCK/1000) //

#endif /* Board_h */
