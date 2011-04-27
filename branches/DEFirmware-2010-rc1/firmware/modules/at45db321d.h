/*
 * at45db321d.h
 *
 *  Created on: Mar 5, 2011
 *      Author: lhanusiak
 */

#ifndef AT45DB321D_H_
#define AT45DB321D_H_

#include <string.h>
#include <aic/aic.h>

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>
#include <spi-flash/at45d.h>
#include "peripherals.h"

// 1500 - because ... :p
#define PAGES_AMOUNT 1500
#define BLOCKS_AMOUNT (PAGES_AMOUNT / 8)
#define PAGE_SIZE 512
#define BLOCK_SIZE (8 * PAGE_SIZE)

#define MAX_BUFF_IDX2 3
#define SHIFT_BUFF_IDX2(idx) ( idx = ((idx)+1 < MAX_BUFF_IDX2) ? ((idx)+1) : 0)

void AT45DB321D_Initalize();
void AT45DB321D_ClearChip();
void AT45DB321D_SelfTest();
void AT45DB321D_Read(unsigned char *pBuffer, unsigned int size,
		unsigned int address);
At45* AT45DB321D_GetPointer();

inline unsigned char AT45_GetStatus(At45 *pAt45);
inline void AT45_WriteBuffer(At45* pAt45, short int siBuffNr,
		unsigned char* pBuff, unsigned int uiSize);

#endif /* AT45DB321D_H_ */
