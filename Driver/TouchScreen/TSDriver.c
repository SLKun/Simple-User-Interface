/*
 * TSDriver.c
 *
 *  Created on: 2015-04-08
 *      Author: SL
 */

#include "TSDriver.h"

static void sort(alt_u16 *array);

int GUI_TOUCH_X_MeasureX(void){
	alt_u8 isTouched = 0;
	alt_u16 x[CNT_OF_BUFFER];
	alt_u16 x_val;
	int i;
	IIC_ReadReg(TS_WR, TS_RD, GESTURE_ID, &isTouched);
	if(isTouched == 1){
		for(i = 0; i < CNT_OF_BUFFER; i++){
			alt_u8 buf[2];
			IIC_ReadReg(TS_WR, TS_RD, X1_H, buf);
			IIC_ReadReg(TS_WR, TS_RD, X1_L, buf + 1);
			x[i] = buf[0] << 8 | buf[1];
		}
		sort(x);
		x_val = (x[CNT_OF_BUFFER/2-1]+x[CNT_OF_BUFFER/2]+x[CNT_OF_BUFFER/2+1]) / 3;
		//printf("x_val = %d\n", x_val);
		return x_val;
	}else{
		return -1;
	}
}

int  GUI_TOUCH_X_MeasureY(void){
	alt_u8 isTouched = 0;
	alt_u16 y[CNT_OF_BUFFER];
	alt_u16 y_val;
	int i;
	IIC_ReadReg(TS_WR, TS_RD, GESTURE_ID, &isTouched);
	if(isTouched == 1){
		for(i = 0; i < CNT_OF_BUFFER; i++){
			alt_u8 buf[2];
			IIC_ReadReg(TS_WR, TS_RD, Y1_H, buf);
			IIC_ReadReg(TS_WR, TS_RD, Y1_L, buf + 1);
			y[i] = buf[0] << 8 | buf[1];
		}
		sort(y);
		y_val = (y[CNT_OF_BUFFER/2-1]+y[CNT_OF_BUFFER/2]+y[CNT_OF_BUFFER/2+1]) / 3;
		//printf("y_val = %d\n", y_val);
		return y_val;
	}else{
		return -1;
	}
}

static void sort(alt_u16 *array){
	alt_u16 i, j, temp;
    alt_u8 exchange;
    for (i = 0; i < CNT_OF_BUFFER; i++){
            exchange = 0;
            for(j = CNT_OF_BUFFER-1; j > 0; j--){
                    if (array[j] < array[j-1]){
                            temp = array[j];
                            array[j] = array[j-1];
                            array[j-1] = temp;
                            exchange = 1;
                    }
            }
            if (!exchange){
                    break;
            }
    }
}

// void sort(alt_u16 *array){
// 	alt_u16 i, temp;
// 	for(i = 0; i < CNT_OF_BUFFER - 1; i++){
// 		if(array[i] > array[i+1]){
// 			temp = array[i];
// 			array[i] = array[i+1];
// 			array[i+1] = temp;
// 		}
// 	}
// }

void TS_Init(){
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_TS_RST_BASE, 0x0); //reset
	ALT_USLEEP(100);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_TS_RST_BASE, 0x1); //stop reset
	IIC_WriteReg(TS_WR, SENSOR, 0x80); 			       //enable sensor
}
