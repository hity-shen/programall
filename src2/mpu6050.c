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

//?y¦Ì??-¡Á¨®MPU6050?y?¡¥

//3?¨º??¡¥MPU6050
//¡¤¦Ì???¦Ì:0,3¨¦1|
//    ????,¡ä¨ª?¨®¡ä¨²??
u8 MPU_Init(void)
{
	u8 res;
	MPU_IIC_Init();//3?¨º??¡¥IIC¡Á¨¹??
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X80);	//?¡ä??MPU6050
    delay_ms(100);
	MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X00);	//??D?MPU6050
	MPU_Set_Gyro_Fsr(3);					//¨ª¨®?Y¨°?¡ä??D?¡Â,?¨¤2000dps
	MPU_Set_Accel_Fsr(0);					//?¨®?¨´?¨¨¡ä??D?¡Â,?¨¤2g
	MPU_Set_Rate(50);						//¨¦¨¨??2¨¦?¨´?¨º50Hz
	MPU_Write_Byte(MPU_INT_EN_REG,0X00);	//1?¡À??¨´¨®D?D??
	MPU_Write_Byte(MPU_USER_CTRL_REG,0X00);	//I2C?¡Â?¡ê¨º?1?¡À?
	MPU_Write_Byte(MPU_FIFO_EN_REG,0X00);	//1?¡À?FIFO
	MPU_Write_Byte(MPU_INTBP_CFG_REG,0X80);	//INT¨°y??¦Ì¨ª¦Ì???¨®DD¡ì
	res=MPU_Read_Byte(MPU_DEVICE_ID_REG);
	if(res==MPU_ADDR)//?¡Â?tID?y¨¨¡¤
	{
		MPU_Write_Byte(MPU_PWR_MGMT1_REG,0X01);	//¨¦¨¨??CLKSEL,PLL X?¨¢?a2???
		MPU_Write_Byte(MPU_PWR_MGMT2_REG,0X00);	//?¨®?¨´?¨¨¨®?¨ª¨®?Y¨°???1¡è¡Á¡Â
		MPU_Set_Rate(50);						//¨¦¨¨??2¨¦?¨´?¨º?a50Hz
 	}else return 1;
	return 0;
}
//¨¦¨¨??MPU6050¨ª¨®?Y¨°?¡ä??D?¡Â?¨²¨¢?3¨¬¡¤??¡ì
//fsr:0,?¨¤250dps;1,?¨¤500dps;2,?¨¤1000dps;3,?¨¤2000dps
//¡¤¦Ì???¦Ì:0,¨¦¨¨??3¨¦1|
//    ????,¨¦¨¨??¨º¡ì¡ã¨¹
u8 MPU_Set_Gyro_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_GYRO_CFG_REG,fsr<<3);//¨¦¨¨??¨ª¨®?Y¨°??¨²¨¢?3¨¬¡¤??¡ì
}
//¨¦¨¨??MPU6050?¨®?¨´?¨¨¡ä??D?¡Â?¨²¨¢?3¨¬¡¤??¡ì
//fsr:0,?¨¤2g;1,?¨¤4g;2,?¨¤8g;3,?¨¤16g
//¡¤¦Ì???¦Ì:0,¨¦¨¨??3¨¦1|
//    ????,¨¦¨¨??¨º¡ì¡ã¨¹
u8 MPU_Set_Accel_Fsr(u8 fsr)
{
	return MPU_Write_Byte(MPU_ACCEL_CFG_REG,fsr<<3);//¨¦¨¨???¨®?¨´?¨¨¡ä??D?¡Â?¨²¨¢?3¨¬¡¤??¡ì
}
//¨¦¨¨??MPU6050¦Ì?¨ºy¡Á?¦Ì¨ª¨ª¡§??2¡§?¡Â
//lpf:¨ºy¡Á?¦Ì¨ª¨ª¡§??2¡§?¦Ì?¨º(Hz)
//¡¤¦Ì???¦Ì:0,¨¦¨¨??3¨¦1|
//    ????,¨¦¨¨??¨º¡ì¡ã¨¹
u8 MPU_Set_LPF(u16 lpf)
{
	u8 data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6;
	return MPU_Write_Byte(MPU_CFG_REG,data);//¨¦¨¨??¨ºy¡Á?¦Ì¨ª¨ª¡§??2¡§?¡Â
}
//¨¦¨¨??MPU6050¦Ì?2¨¦?¨´?¨º(?¨´?¡§Fs=1KHz)
//rate:4~1000(Hz)
//¡¤¦Ì???¦Ì:0,¨¦¨¨??3¨¦1|
//    ????,¨¦¨¨??¨º¡ì¡ã¨¹
u8 MPU_Set_Rate(u16 rate)
{
	u8 data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU_SAMPLE_RATE_REG,data);	//¨¦¨¨??¨ºy¡Á?¦Ì¨ª¨ª¡§??2¡§?¡Â
 	return MPU_Set_LPF(rate/2);	//¡Á??¡¥¨¦¨¨??LPF?a2¨¦?¨´?¨º¦Ì?¨°?¡ã?
}

//¦Ì?¦Ì????¨¨?¦Ì
//¡¤¦Ì???¦Ì:???¨¨?¦Ì(¨¤?¡ä¨®¨¢?100¡À?)
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
//¦Ì?¦Ì?¨ª¨®?Y¨°??¦Ì(?-¨º??¦Ì)
//gx,gy,gz:¨ª¨®?Y¨°?x,y,z?¨¢¦Ì??-¨º??¨¢¨ºy(¡ä?¡¤?o?)
//¡¤¦Ì???¦Ì:0,3¨¦1|
//    ????,¡ä¨ª?¨®¡ä¨²??
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
//¦Ì?¦Ì??¨®?¨´?¨¨?¦Ì(?-¨º??¦Ì)
//gx,gy,gz:¨ª¨®?Y¨°?x,y,z?¨¢¦Ì??-¨º??¨¢¨ºy(¡ä?¡¤?o?)
//¡¤¦Ì???¦Ì:0,3¨¦1|
//    ????,¡ä¨ª?¨®¡ä¨²??
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
//IIC¨¢?D?D¡ä
//addr:?¡Â?t¦Ì??¡¤
//reg:??¡ä??¡Â¦Ì??¡¤
//len:D¡ä¨¨?3¡è?¨¨
//buf:¨ºy?Y??
//¡¤¦Ì???¦Ì:0,?y3¡ê
//    ????,¡ä¨ª?¨®¡ä¨²??
u8 MPU_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
	u8 i;
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+D¡ä?¨¹¨¢?
	if(MPU_IIC_Wait_Ack())	//¦Ì¨¨¡äy¨®|¡äe
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D¡ä??¡ä??¡Â¦Ì??¡¤
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
	for(i=0;i<len;i++)
	{
		MPU_IIC_Send_Byte(buf[i]);	//¡¤¡é?¨ª¨ºy?Y
		if(MPU_IIC_Wait_Ack())		//¦Ì¨¨¡äyACK
		{
			MPU_IIC_Stop();
			return 1;
		}
	}
    MPU_IIC_Stop();
	return 0;
}
//IIC¨¢?D??¨¢
//addr:?¡Â?t¦Ì??¡¤
//reg:¨°a?¨¢¨¨?¦Ì???¡ä??¡Â¦Ì??¡¤
//len:¨°a?¨¢¨¨?¦Ì?3¡è?¨¨
//buf:?¨¢¨¨?¦Ì?¦Ì?¨ºy?Y¡ä?¡ä¡é??
//¡¤¦Ì???¦Ì:0,?y3¡ê
//    ????,¡ä¨ª?¨®¡ä¨²??
u8 MPU_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
 	MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|0);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+D¡ä?¨¹¨¢?
	if(MPU_IIC_Wait_Ack())	//¦Ì¨¨¡äy¨®|¡äe
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D¡ä??¡ä??¡Â¦Ì??¡¤
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((addr<<1)|1);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+?¨¢?¨¹¨¢?
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
	while(len)
	{
		if(len==1)*buf=MPU_IIC_Read_Byte(0);//?¨¢¨ºy?Y,¡¤¡é?¨ªnACK
		else *buf=MPU_IIC_Read_Byte(1);		//?¨¢¨ºy?Y,¡¤¡é?¨ªACK
		len--;
		buf++;
	}
    MPU_IIC_Stop();	//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t
	return 0;
}
//IICD¡ä¨°???¡Á??¨²
//reg:??¡ä??¡Â¦Ì??¡¤
//data:¨ºy?Y
//¡¤¦Ì???¦Ì:0,?y3¡ê
//    ????,¡ä¨ª?¨®¡ä¨²??
u8 MPU_Write_Byte(u8 reg,u8 data)
{
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+D¡ä?¨¹¨¢?
	if(MPU_IIC_Wait_Ack())	//¦Ì¨¨¡äy¨®|¡äe
	{
		MPU_IIC_Stop();
		return 1;
	}
    MPU_IIC_Send_Byte(reg);	//D¡ä??¡ä??¡Â¦Ì??¡¤
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
	MPU_IIC_Send_Byte(data);//¡¤¡é?¨ª¨ºy?Y
	if(MPU_IIC_Wait_Ack())	//¦Ì¨¨¡äyACK
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
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|0);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+D¡ä?¨¹¨¢?
	MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
    MPU_IIC_Send_Byte(reg);	//D¡ä??¡ä??¡Â¦Ì??¡¤
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
    MPU_IIC_Start();
	MPU_IIC_Send_Byte((MPU_ADDR<<1)|1);//¡¤¡é?¨ª?¡Â?t¦Ì??¡¤+?¨¢?¨¹¨¢?
    MPU_IIC_Wait_Ack();		//¦Ì¨¨¡äy¨®|¡äe
	res=MPU_IIC_Read_Byte(0);//?¨¢¨¨?¨ºy?Y,¡¤¡é?¨ªnACK
    MPU_IIC_Stop();			//2¨²¨¦¨²¨°???¨ª¡ê?1¨¬??t
	return res;
}



