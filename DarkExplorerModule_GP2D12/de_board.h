#ifndef de_board_h
#define de_board_h

#include <board.h>
#include <at91sam7s256/AT91SAM7S256.h>
#define __inline static inline
//#include "lib_AT91SAM7S256.h"

#define __ramfunc

#define EXT_OC      18432000   // Exetrnal ocilator MAINCK
//#define MCK       47923200   // MCK (PLLRC div by 2)
//#define MCK       95846400   // MCK (PLLRC)
#define BOARD_MCK   48000000
#define MCK BOARD_MCK
#define MCKKHz      (MCK/1000)

#endif /* de_board_h */
