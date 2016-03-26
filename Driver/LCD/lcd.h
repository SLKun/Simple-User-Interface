/*
 * lcd.h
 *
 *  Created on: Mar 13, 2015
 *      Author: qi
 */

#ifndef LCD_H_
#define LCD_H_

#include "de0-nano.h"

#define RAMWriteCmd				0x2C00
#define RAMReadCmd				0x2E00
#define RAMWriteContinueCmd		0x3C00
#define RAMReadContinueCmd		0x3E00
#define DisplayOnCmd			0x2900
#define DisplayOffCmd			0x2800
#define XStartPositionSet		0x2A00
#define XEndPositionSet			0x2A02
#define YStartPositionSet		0x2B00
#define YEndPositionSet			0x2B02

// Configure

// Switch the HORIZONTAL and the VERTICAL
#define HORIZONTAL

inline alt_u16 LCD_Read(void);
inline void LCD_WriteCmd(alt_u16 cmd);
inline void LCD_WriteData(alt_u16 data);

inline alt_u16 LCD_ReadReg(alt_u16 LCD_Reg);
inline void LCD_WriteReg(alt_u16 LCD_Reg, alt_u16 LCD_RegValue);

inline void LCD_SetCursor(alt_u16 Xpos, alt_u16 Ypos);
void LCD_SetWindow(alt_u16 XS, alt_u16 XE, alt_u16 YS, alt_u16 YE);
void LCD_Clear(alt_u16 color);
void LCD_DrawPoint(alt_u16 x, alt_u16 y, alt_u16 color);
void LCD_Color_Fill(alt_u16 sx, alt_u16 sy, alt_u16 ex, alt_u16 ey, alt_u16 color);
void LCD_light(alt_u8 num);
alt_u16 LCD_ReadPoint(alt_u16 x, alt_u16 y);

void LCD_Init(void);

#endif /* LCD_H_ */
