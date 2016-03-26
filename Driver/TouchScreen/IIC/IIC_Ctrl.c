/*
 * IIC_Ctrl.c
 *
 *  Created on: 2014-8-17
 *      Author: SL
 */


#include "IIC_Ctrl.h"

alt_8 IIC_WriteReg(alt_u8 waddr,alt_u8 reg, alt_u8 data)
{
	IIC_Start();
	IIC_PutByte(waddr);
	IIC_NAKRTN;
	IIC_PutByte(reg);
	IIC_NAKRTN;
	IIC_PutByte(0xff);
	IIC_NAKRTN;
	IIC_PutByte(data);
	IIC_NAKRTN;
	IIC_Stop();
	return 1;
}

alt_8 IIC_ReadReg(alt_u8 waddr, alt_u8 raddr, alt_u8 reg, alt_u8 *data)
{
	IIC_Start();
	IIC_PutByte(waddr);
	IIC_NAKRTN;
	IIC_PutByte(reg);
	IIC_NAKRTN;
	IIC_PutByte(0xff);
	IIC_NAKRTN;
	IIC_Start();
	IIC_PutByte(raddr);
	IIC_NAKRTN;
	*data = IIC_GetByte();
	IIC_Acknowledge(0);
	IIC_Stop();
	return 1;
}
/*data types needs to be change*/
alt_8 IIC_ReadNRegs(alt_u8 waddr, alt_u8 raddr, alt_u8 reg, alt_u16 *data, alt_u8 N)
{
	IIC_Start();
	IIC_PutByte(waddr);
	IIC_NAKRTN;
	IIC_PutByte(reg);
	IIC_NAKRTN;
	IIC_PutByte(0xff);
	IIC_NAKRTN;
	IIC_Start();
	IIC_PutByte(raddr);
	IIC_NAKRTN;
	*(alt_u8 *)data = IIC_GetByte();
	alt_u8 i;
	for(i = 0; i < N; i++){
		IIC_Acknowledge(1);
		*(i + (alt_u8 *)data) = IIC_GetByte();
	}
	IIC_Acknowledge(0);
	IIC_Stop();
	return 1;
}
