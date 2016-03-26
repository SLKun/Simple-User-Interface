/*
 * IIC_Ctrl.h
 *
 *  Created on: 2014-8-17
 *      Author: SL
 */

#ifndef IIC_CTRL_H_
#define IIC_CTRL_H_

#include "IIC.h"
#include "de0-nano.h"

alt_8 IIC_WriteReg(alt_u8 waddr,alt_u8 reg, alt_u8 data);
alt_8 IIC_ReadReg(alt_u8 waddr, alt_u8 raddr, alt_u8 reg, alt_u8 *data);
alt_8 IIC_ReadNRegs(alt_u8 waddr, alt_u8 raddr, alt_u8 reg, alt_u16 *data, alt_u8 N);
extern alt_u8  IIC_Init();


#endif /* IIC_CTRL_H_ */
