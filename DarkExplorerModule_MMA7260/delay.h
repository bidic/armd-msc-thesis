/*
 * delay.h
 *
 *  Created on: Jul 21, 2010
 *      Author: lhanusiak
 */

#ifndef DELAY_H_
#define DELAY_H_

#define SYS_CLK 48
#define delay(arg) wait(arg*SYS_CLK/22)

void wait(volatile unsigned int d);

#endif /* DELAY_H_ */
