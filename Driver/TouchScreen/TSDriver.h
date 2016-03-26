/*
 * TSDriver.h
 *
 *  Created on: 2015-04-08
 *      Author: SL
 */

#ifndef TSDRIVER_H_
#define TSDRIVER_H_

#include "IIC/IIC_Ctrl.h"
#include "de0-nano.h"

#define CNT_OF_BUFFER 10

#define TS_BASE		 0x59
#define TS_WR		 0xb2
#define TS_RD		 0xb3
#define GESTURE_ID	 0x00
#define SENSOR 		 0x0d

#define RES_XH 		 0x09
#define RES_XL 		 0x0a
#define RES_YH 		 0x0b
#define RES_YL 		 0x0c

#define X1_H		 0x01
#define X1_L		 0x02
#define Y1_H		 0x03
#define Y1_L		 0x04

void TS_Init();
int GUI_TOUCH_X_MeasureX(void);
int GUI_TOUCH_X_MeasureY(void);

#endif /* TSDRIVER_H_ */
