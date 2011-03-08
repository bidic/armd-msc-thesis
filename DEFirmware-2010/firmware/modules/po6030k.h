/*
 * po6030k.h
 *
 *  Created on: Mar 5, 2011
 *      Author: lhanusiak
 */

#ifndef PO6030K_H_
#define PO6030K_H_

#include <dbgu/dbgu.h>
#include <utility/trace.h>
#include <utility/assert.h>

#include <drivers/twi/twid.h>
#include "lib_AT91SAM7S256.h"
#include "at45db321d.h"

//adres kamery do zapisu przez TWI
#define PO6030K_DEVICE_ID 0x6E

// AT91C_BASE_PIOA->PIO_PDSR & 0x2000000 - wyciagnij informacje tylko o 25 pinie (VSYNC)
#define GET_VSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x2000000)

// AT91C_BASE_PIOA->PIO_PDSR & 0x4000000 - wyciagnij informacje tylko o 26 pinie (HSYNC)
#define GET_HSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x4000000)

// AT91C_BASE_PIOA->PIO_PDSR & 0x40000000 - wyciagnij informacje tylko o 30 pinie (PCLK)
#define GET_PSYNC (AT91C_BASE_PIOA->PIO_PDSR & 0x40000000)

#define BUFFER_SIZE 512
#define MAX_BUFF_IDX 100
#define SHIFT_BUFF_IDX(idx) ( idx = ((idx)+1 < MAX_BUFF_IDX) ? ((idx)+1) : 0)
#define PREV_BUFF_IDX(idx) ( ((idx)-1 < 0) ? MAX_BUFF_IDX : (idx)-1)

#define CamR(data) { register unsigned int buff= (AT91C_BASE_PIOA->PIO_PDSR & 0x95F0000) >> 14;\
                        data = (( buff & 0x7C ) )\
                               | ((buff & 0x2000) >> 6)\
                               | ((buff & 0x400) >> 10)\
                               | ((buff & 0x100) >> 7);\
                      }

void PO6030K_Initalize();

void PO6030K_InitRegisters(Twid *twid);

void PO6030K_TakePicture();

#endif /* PO6030K_H_ */
