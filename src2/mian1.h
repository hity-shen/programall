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

char tmp_buf[33];			//℅?﹞?∩?那y℅谷
float pitch,roll,yaw; 		//?﹞角-??:??????㏒???o???㏒?1?℅a??
short gyrox,gyroy,gyroz;	//赤車?Y辰??-那?那y?Y  gyroscope
short aacx,aacy,aacz;		//?車?迄?豕∩??D?¯?-那?那y?Y  angular acceleration
short temp;					//???豕
u8 key;						//～∩?邦?米


struct MPU6050				//MPU6050?芍11足?
{
	u8 flag;				//谷?㊣“㊣那????
	u8 speed;				//谷?㊣“?迄?豕
	u8 mode;				//1∟℅¯?㏒那?
}mpu6050;					//?“辰??芍11足?㊣?芍?


int main(void);				//?¯o‘那y
void SYS_Init(void);		//?米赤33?那??‘℅邦o‘那y
void MPU_Mode(void);        //?米赤31∟℅¯?㏒那?????o‘那y
void MPU_Read(void);		//MPU6050那y?Y?芍豕?o‘那y
void DATA_Report(void);		//MPU6050那y?Y谷?㊣“



#endif // _MIAN1_H

