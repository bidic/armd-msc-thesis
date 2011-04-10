/*
 * hy1602f6.h
 *
 *  Created on: Apr 6, 2011
 *      Author: lhanusiak
 */

#ifndef HY1602F6_H_
#define HY1602F6_H_

#include <pio/pio.h>
#include <dbgu/dbgu.h>
#include <twi/twi.h>
#include <utility/assert.h>
#include <utility/trace.h>
#include <drivers/async/async.h>
#include <drivers/twi/twid.h>

// Slave address of AT24C chips.
#define AT24C_ADDRESS   56

void HY1602F6_Init(void);
void HY1602F6_PrintText(const char*);
void HY1602F6_ClearDisplay(void);
void HY1602F6_SetCursorPos(unsigned char);
void HY1602F6_SetCursorHome(void);
void HY1602F6_StartNextLine();
void HY1602F6_SetProgress(unsigned char val, unsigned char clear);

#endif /* HY1602F6_H_ */
