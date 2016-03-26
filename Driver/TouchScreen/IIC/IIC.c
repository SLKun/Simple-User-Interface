// loywong.HUST 20080722

#include "IIC.h"

#define PIO_IIC_SDA_BASE PIO_TS_SDA_BASE
#define PIO_IIC_SCL_BASE PIO_TS_SCL_BASE

#define IIC_RLS_SDA     do\
                        {\
                            IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_IIC_SDA_BASE, ALTERA_AVALON_PIO_DIRECTION_INPUT);\
                        }while(0)                        //float input,with external 4.7Kohm resistor pulling up
#define IIC_CLR_SDA     do\
                        {\
                            IOWR_ALTERA_AVALON_PIO_DATA(PIO_IIC_SDA_BASE, 0x0);\
                            IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_IIC_SDA_BASE, ALTERA_AVALON_PIO_DIRECTION_OUTPUT);\
                        }while(0)                        //output LOW
#define IIC_RLS_SCL     do\
                        {\
                            IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_IIC_SCL_BASE, ALTERA_AVALON_PIO_DIRECTION_INPUT);\
                        }while(0)                        //float input,with external 4.7Kohm resistor pulling up
#define IIC_CLR_SCL     do\
                        {\
                            IOWR_ALTERA_AVALON_PIO_DATA(PIO_IIC_SCL_BASE, 0x0);\
                            IOWR_ALTERA_AVALON_PIO_DIRECTION(PIO_IIC_SCL_BASE, ALTERA_AVALON_PIO_DIRECTION_OUTPUT);\
                        }while(0)                        //output LOW

#define IIC_SDA_VAL     (IORD_ALTERA_AVALON_PIO_DATA(PIO_IIC_SDA_BASE))
#define IIC_SCL_VAL     (IORD_ALTERA_AVALON_PIO_DATA(PIO_IIC_SCL_BASE))
#define IIC_SCL_SYNC    while(!IIC_SCL_VAL)

#define IIC_NAKRTN      do\
                        {\
                            if(!IIC_ReceiveACK())\
                            {\
                                IIC_Stop();\
                                return(0);\
                            }\
                        }while(0)

//Start Bus
void IIC_Start()
{
    IIC_RLS_SDA;
    ALT_USLEEP(3);
    IIC_RLS_SCL;
    ALT_USLEEP(4);        //Start condition set up time >4.7us
    IIC_SCL_SYNC;
    IIC_CLR_SDA;    //Send Start signal
    ALT_USLEEP(4);        //Start condition lock time >4us
    IIC_CLR_SCL;    //Seize Bus.Prepare to send or receive data
}

//Stop Bus
void IIC_Stop()
{
    IIC_CLR_SDA;
    ALT_USLEEP(3);
    IIC_RLS_SCL;
    ALT_USLEEP(4);        //Stop condition set up time >4us
    IIC_SCL_SYNC;
    IIC_RLS_SDA;    //Send Stop signal
    ALT_USLEEP(4);
}

//Put Byte to bus
void IIC_PutByte(alt_u8 c)
{
    alt_u8 BitCnt;
    for(BitCnt = 0; BitCnt < 8; BitCnt++)
    {
        if(c & 0x80)
            IIC_RLS_SDA;
        else
            IIC_CLR_SDA;
        c <<= 1;
        ALT_USLEEP(2);
        IIC_RLS_SCL;            //Pull SCL High.Order device to receive data bits
        ALT_USLEEP(3);                //the High period of SCL >4us
        IIC_SCL_SYNC;
        IIC_CLR_SCL;
    }
}

//Get byte from bus
alt_u8 IIC_GetByte()
{
    alt_u8 Data;
    alt_u8 BitCnt;
    Data = 0;
    IIC_RLS_SDA;
    for(BitCnt = 0; BitCnt < 8; BitCnt++)
    {
        ALT_USLEEP(3);
        IIC_RLS_SCL;
        ALT_USLEEP(2);
        IIC_SCL_SYNC;
        Data <<= 1;
        Data |= IIC_SDA_VAL;    //Receive data bits,put them in "Data"
        IIC_CLR_SCL;                //the Low period of SCL >4.7us
    }
    return Data;
}

//Acknowledge    a=1:ACK;    a=0:NAK
void IIC_Acknowledge(alt_8 a)
{
    if(a)
        IIC_CLR_SDA;
    else
        IIC_RLS_SDA;    //Send ACK or NAK
    ALT_USLEEP(2);
    IIC_RLS_SCL;        //the High period of SCL >4us
    ALT_USLEEP(3);
    IIC_SCL_SYNC;
    IIC_CLR_SCL;        //Seize Bus
}

alt_u8 IIC_ReceiveACK()
{
    alt_u8 IIC_Ack;
    IIC_RLS_SDA;
    ALT_USLEEP(3);
    IIC_RLS_SCL;
    ALT_USLEEP(3);
    IIC_SCL_SYNC;
    IIC_Ack=!IIC_SDA_VAL;        //Whether received acknowledge
    IIC_CLR_SCL;                //Seize Bus
    return IIC_Ack;
}

alt_u8 IIC_GeneralReset()
{
    IIC_Start();
    IIC_PutByte(0x00);
    IIC_NAKRTN;
    IIC_PutByte(0x06);
    IIC_NAKRTN;
    IIC_Stop();
    return(1);
}

alt_u8 IIC_Init()
{
    IIC_RLS_SCL;
    IIC_RLS_SDA;
    IIC_Stop();
//    return IIC_GeneralReset();
    return 1;
}

