#ifndef __IIC_H__
#define __IIC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "de0-nano.h"

alt_u8   IIC_Init();
void            IIC_Start();
void            IIC_Stop();
void            IIC_PutByte(alt_u8 c);
alt_u8   IIC_GetByte();
alt_u8   IIC_ReceiveACK();
void            IIC_Acknowledge(alt_8 a);
alt_u8   IIC_GeneralReset();

#define IIC_NAKRTN      do\
                        {\
                            if(!IIC_ReceiveACK())\
                            {\
                                IIC_Stop();\
                                return(0);\
                            }\
                        }while(0)

#ifdef __cplusplus
}
#endif

#endif
