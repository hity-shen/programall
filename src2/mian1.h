/*
 * mian1.h
 *
 * created: 2022/7/14
 *  author: 
 */

#ifndef _MIAN1_H
#define _MIAN1_H
#include "sys.h"
#include "usart.h"
#include "mpu6050.h"
#include "niming.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 

char tmp_buf[33];			//��?��?��?��y����
float pitch,roll,yaw; 		//?����-??:??????��???o???��?1?��a??
short gyrox,gyroy,gyroz;	//����?Y��??-��?��y?Y  gyroscope
short aacx,aacy,aacz;		//?��?��?����??D?��?-��?��y?Y  angular acceleration
short temp;					//???��
u8 key;						//���?��?��


struct MPU6050				//MPU6050?��11��?
{
	u8 flag;				//��?��������????
	u8 speed;				//��?����?��?��
	u8 mode;				//1�����?�꨺?
}mpu6050;					//?����??��11��?��?��?


int main(void);				//?��o����y
void SYS_Init(void);		//?�̨�33?��??������o����y
void MPU_Mode(void);        //?�̨�31�����?�꨺?????o����y
void MPU_Read(void);		//MPU6050��y?Y?����?o����y
void DATA_Report(void);		//MPU6050��y?Y��?����



#endif // _MIAN1_H

