/*
 * mpuiic.h
 *
 * created: 2022/7/14
 *  author: 
 */

#ifndef _MPUIIC_H
#define _MPUIIC_H

#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"

#define MPU_SDA_IN()  {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=8<<12;}
#define MPU_SDA_OUT() {GPIOC->CRH&=0XFFFF0FFF;GPIOC->CRH|=3<<12;}

#define MPU_IIC_SCL    PCout(12) 		//SCL
#define MPU_IIC_SDA    PCout(11) 		//SDA
#define MPU_READ_SDA   PCin(11) 		//¨º?¨¨?SDA

void MPU_IIC_Delay(void);				//MPU IIC?¨®¨º¡Ào¡¥¨ºy
void MPU_IIC_Init(void);                //3?¨º??¡¥IIC¦Ì?IO?¨²
void MPU_IIC_Start(void);				//¡¤¡é?¨ªIIC?a¨º?D?o?
void MPU_IIC_Stop(void);	  			//¡¤¡é?¨ªIIC¨ª¡ê?1D?o?
void MPU_IIC_Send_Byte(u8 txd);			//IIC¡¤¡é?¨ª¨°???¡Á??¨²
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC?¨¢¨¨?¨°???¡Á??¨²
u8 MPU_IIC_Wait_Ack(void); 				//IIC¦Ì¨¨¡äyACKD?o?
void MPU_IIC_Ack(void);					//IIC¡¤¡é?¨ªACKD?o?
void MPU_IIC_NAck(void);				//IIC2?¡¤¡é?¨ªACKD?o?

void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);
#endif

#endif // _MPUIIC_H

