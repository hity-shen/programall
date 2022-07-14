/*
 * mpu6050.c
 *
 * created: 2022/7/14
 *  author: 
 */

#include "mpu6050.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

//?y��??-����MPU6050?y?��

//3?��??��MPU6050
//����???��:0,3��1|
//    ????,�䨪?���䨲??
u8 MPU_Init(void)
{
	u8 res;
	MPU_IIC_Init();//3?��??��IIC����??
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//?��??MPU6050
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//??D?MPU6050
	MPU_Set_Gyro_Fsr(3);					//����?Y��?��??D?��,?��2000dps
	MPU_Set_Accel_Fsr(0);					//?��?��?����??D?��,?��2g
	MPU_Set_Rate(50);						//����??2��?��?��50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//1?��??����D?D??
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C?��?�꨺?1?��?
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//1?��?FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT��y??�̨���???��DD��
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//?��?tID?y����
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//����??CLKSEL,PLL X?��?a2???
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//?��?��?����?����?Y��???1�����
		MPU_Set_Rate(50);						//����??2��?��?��?a50Hz
 	}else return 1;
	return 0;
}
//����??MPU6050����?Y��?��??D?��?����?3����??��
//fsr:0,?��250dps;1,?��500dps;2,?��1000dps;3,?��2000dps
//����???��:0,����??3��1|
//    ????,����??����㨹
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//����??����?Y��??����?3����??��
}
//����??MPU6050?��?��?����??D?��?����?3����??��
//fsr:0,?��2g;1,?��4g;2,?��8g;3,?��16g
//����???��:0,����??3��1|
//    ????,����??����㨹
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//����???��?��?����??D?��?����?3����??��
}
//����??MPU6050��?��y��?�̨�����??2��?��
//lpf:��y��?�̨�����??2��?��?��(Hz)
//����???��:0,����??3��1|
//    ????,����??����㨹
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU_Write_Byte(MPU_CFG_REG,data);//����??��y��?�̨�����??2��?��
}
//����??MPU6050��?2��?��?��(?��?��Fs=1KHz)
//rate:4~1000(Hz)
//����???��:0,����??3��1|
//    ????,����??����㨹
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//����??��y��?�̨�����??2��?��
 	return MPU_Set_LPF(rate/2);	//��??������??LPF?a2��?��?����?��?��?
}

//��?��????��?��
//����???��:???��?��(��?�䨮��?100��?)
short MPU_Get_Temperature(void)
{
    u8 buf[2];
    short raw;
	float temp;
	MPU_Read_Len(MPU_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((u16)buf[0]<<8)|buf[1];
    temp=36.53+((double)raw)/340;
    return temp*100;;
}
//��?��?����?Y��??��(?-��??��)
//gx,gy,gz:����?Y��?x,y,z?����??-��??����y(��?��?o?)
//����???��:0,3��1|
//    ????,�䨪?���䨲??
u8 MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    u8 buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((u16)buf[0]<<8)|buf[1];
		*gy=((u16)buf[2]<<8)|buf[3];
		*gz=((u16)buf[4]<<8)|buf[5];
	}
    return res;;
}
//��?��??��?��?��?��(?-��??��)
//gx,gy,gz:����?Y��?x,y,z?����??-��??����y(��?��?o?)
//����???��:0,3��1|
//    ????,�䨪?���䨲??
u8 MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    u8 buf[6],res;
	res=MPU_Read_Len(MPU_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((u16)buf[0]<<8)|buf[1];
		*ay=((u16)buf[2]<<8)|buf[3];
		*az=((u16)buf[4]<<8)|buf[5];
	}
    return res;;
}
//IIC��?D?D��
//addr:?��?t��??��
//reg:??��??�¦�??��
//len:D�䨨?3��?��
//buf:��y?Y??
//����???��:0,?y3��
//    ????,�䨪?���䨲??
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i;
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);//����?��?��?t��??��+D��?����?
	if(MPU_IIC_Wait_Ack())	//�̨���y��|��e
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D��??��??�¦�??��
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//����?����y?Y
		if(MPU_IIC_Wait_Ack())		//�̨���yACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
    MPU_IIC_Stop();
	return 0;
}
//IIC��?D??��
//addr:?��?t��??��
//reg:��a?����?��???��??�¦�??��
//len:��a?����?��?3��?��
//buf:?����?��?��?��y?Y��?���??
//����???��:0,?y3��
//    ????,�䨪?���䨲??
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
 	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);//����?��?��?t��??��+D��?����?
	if(MPU_IIC_Wait_Ack())	//�̨���y��|��e
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D��??��??�¦�??��
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//����?��?��?t��??��+?��?����?
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//?����y?Y,����?��nACK
		else *buf=MPU_IIC_Read_Byte(1);		//?����y?Y,����?��ACK
		len--;
		buf++;
	}
    MPU_IIC_Stop();	//2��������???����?1��??t
	return 0;
}
//IICD�䨰???��??��
//reg:??��??�¦�??��
//data:��y?Y
//����???��:0,?y3��
//    ????,�䨪?���䨲??
u8 MPU_Write_Byte(u8 reg,u8 data)
{
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����?��?��?t��??��+D��?����?
	if(MPU_IIC_Wait_Ack())	//�̨���y��|��e
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D��??��??�¦�??��
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
	MPU_IIC_Send_Byte(data);//����?����y?Y
	if(MPU_IIC_Wait_Ack())	//�̨���yACK
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Stop();
	return 0;
}

u8 MPU_Read_Byte(u8 reg)
{
	u8 res;
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//����?��?��?t��??��+D��?����?
	MPU_IIC_Wait_Ack();		//�̨���y��|��e
    MPU_IIC_Send_Byte(reg);	//D��??��??�¦�??��
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//����?��?��?t��??��+?��?����?
    MPU_IIC_Wait_Ack();		//�̨���y��|��e
	res=MPU_IIC_Read_Byte(0);//?����?��y?Y,����?��nACK
    MPU_IIC_Stop();			//2��������???����?1��??t
	return res;
}



