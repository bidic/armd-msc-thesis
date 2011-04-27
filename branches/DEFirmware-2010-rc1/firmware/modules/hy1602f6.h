/*
 * hy1602f6.h
 *
 *  Created on: Apr 6, 2011
 *      Author: lhanusiak
 */

#ifndef HY1602F6_H_
#define HY1602F6_H_

// Slave address of AT24C chips.
#define AT24C_ADDRESS   56

void HY1602F6_Init(void);
void HY1602F6_PrintText(const char*);
void HY1602F6_ClearDisplay(void);
void HY1602F6_SetCursorPos(unsigned char);
void HY1602F6_SetCursorHome(void);
void HY1602F6_StartNextLine();
void HY1602F6_SetProgress(unsigned char val, unsigned char clear);
void HY1602F6_Log(const char* line1, const char* line2);

#endif /* HY1602F6_H_ */
