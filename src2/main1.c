/*
 * main1.c
 *
 * created: 2022/7/14
 *  author: 
 */

#include "main1.h"

int main(void)
{
	SYS_Init();//?�̨�33?��??������o����y
	MPU_Mode();//?�̨�31�����?�꨺?????
	while(1)   //?��?-?��
	{
		MPU_Read();    //MPU6050��y?Y?����?
		DATA_Report(); //MPU6050��y?Y��?����
	}
}

void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //?D??��??��??��?����o����y
	delay_init();	    							 //?������o����y3?��??��
	uart_init(115200);	 	                         //��??��3?��??��?a115200
	LED_Init();		                               	 //3?��??����?LED��??����?��2?t?��?��
	KEY_Init();                                      //���?��3?��??��
	Lcd_Init();                                      //??��??��3?��??��
	Lcd_Clear(GRAY0);                                //?��??3?��??��?a?����?
	Gui_DrawFont_GBK16(4,30,BLACK,GRAY0,"MPU6050��y?Y2��?��");//??��???��?
	LCD_LED_SET;									 //?a???��??��31a


	MPU_Init();	                                     //3?��??��MPU6050
	while(mpu_dmp_init())                            //3?��??��mpu_dmp?a
 	{
		Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?��??������㨹!");//??��???��?
		LED1 = !LED1;delay_ms(50);LED1 = !LED1;     //LED����????��?
		Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?��??������㨹!");//??��???��?
		printf("Initialization failed��?\r\n");		//��??��3?��??������㨹��?����
	}
	printf("Initialization succeed��?\r\n");			//��??��3?��??��3��1|��?����
	Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?��??��3��1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?��??��3��1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?��??��3��1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?��??��3��1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?��??��3��1|!");//3?��??��3��1|��������??��y��?
	LED2 = !LED2;delay_ms(50);LED2 = !LED2;		//LED����????��?
	delay_ms(999);									//?������3???????��?
	mpu6050.flag = 0;                               //2��?��3��1|����????3?��??��
	mpu6050.speed = 0;								//��?����?��?��3?��??��
	mpu6050.mode = 255;								//1�����?�꨺?3?��??��
	key = 255;										//���?��?��3?��??��
}
/**
  * @brief  ?�̨�31�����?�꨺?????o����y
  * @param  ?T
  * @retval ?T
  */
void MPU_Mode(void)
{
	Lcd_Clear(GRAY0);							    //?��??3?��??��?a?����?
	Gui_DrawFont_GBK16(24,16,BLUE,GRAY0,"?�꨺??????");//??��???��?
	Gui_DrawFont_GBK16(12,60,BLUE,GRAY0,"KEY0:���̨�����y?Y");//??��???��?
	Gui_DrawFont_GBK16(12,80,BLUE,GRAY0,"KEY1:?��?a???��");//??��???��?
	Gui_DrawFont_GBK16(12,100,BLUE,GRAY0,"WK_UP:����??");//??��???��?
	while(1)//?-?��
	{
		key = KEY_Scan(0);//���?������?��
		if(key == KEY0_PRES)//KEY0�䣤����
		{
			mpu6050.mode = 1;//1�����?�꨺?����???a1
			uart_init(115200);//3?��??����??��
			Lcd_Clear(GRAY0);//?��??3?��??��?a?����?
			Gui_DrawFont_GBK16(32,16,BLACK,GRAY0,"���̨�����y?Y");
			Gui_DrawFont_GBK16(8,40,BLUE,GRAY0,"??????:      ??");//??��???��?
			Gui_DrawFont_GBK16(8,60,BLUE,GRAY0,"??o???:      ??");//??��???��?
			Gui_DrawFont_GBK16(8,80,BLUE,GRAY0,"��-1???:      ??");//??��???��?
			Gui_DrawFont_GBK16(8,100,RED,GRAY0,"???��?��:      ??");//??��???��?
			goto X;//��?��a��?3??��
		}
		else if(key == KEY1_PRES)//KEY1�䣤����
		{
			mpu6050.mode = 2;//1�����?�꨺?����???a2
			uart_init(500000);//3?��??����??��
			Lcd_Clear(GRAY0);//?��??3?��??��?a?����?
			Gui_DrawFont_GBK16(32,16,BLACK,GRAY0,"?��?a???��");//??��???��?
			Gui_DrawFont_GBK16(28,60,RED,GRAY0,"��?��??D...");//??��???��?

			goto X;//��?��a��?3??��
		}
		else ;//����?��?��
		delay_ms(10);//����?�����?��?������
	}
	X:;//3??��
}
/**
  * @brief  MPU6050��y?Y?����?o����y
  * @param  ?T
  * @retval ?T
  */
void MPU_Read(void)
{

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp��|������?��?��y?Y��???����???��??DD?D??
	{
		temp=MPU_Get_Temperature();	                //��?��????��?��
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//��?��??��?��?����??D?�¨�y?Y
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//��?��?����?Y��?��y?Y
		mpu6050.speed++;                            //��?����?��?����??��
		if(mpu6050.speed == 1)						//��?����?��?��?D?�̨���??
		{
			mpu6050.flag = 1;						//��?��������?????a��DD��
			mpu6050.speed = 0;						//��?����?��?��1����?
		}
	}
	else 											//2��?��2?3��1|
	{
		mpu6050.flag = 0;							//��?��������?????a?TD��
	}
}

void DATA_Report(void)
{
	if(mpu6050.mode==1)								//??��?1�����?�꨺?1
	{
		if(mpu6050.flag == 1)						//��?��������?????a��DD��
		{
			if(temp<0)								//??��y?Y?y?o?D??��??D???a?o����
			{
				temp=-temp;							//???o��y?Y��?����
				sprintf(tmp_buf, "-%3d.%d",temp/100,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,100,RED,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			else                                    //?D???a?y����
			{
				sprintf(tmp_buf, " %3d.%d",temp/100,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,100,RED,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			printf("temp:%d.%d,  ",temp/100,temp%10); //����1y��??��1��?3????��

			temp=pitch*10;							 //?3temp?apitch
			if(temp<0)								//??��y?Y?y?o?D??��??D???a?o����
			{
				temp=-temp;						    //???o��y?Y��?����
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,40,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			else                                    //?D???a?y����
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,40,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			printf("pitch:%d.%d,  ",temp/10,temp%10); //����1y��??��1��?3?pitch

			temp=yaw*10;                           //?3temp?ayaw
			if(temp<0)								//??��y?Y?y?o?D??��??D???a?o����
			{
				temp=-temp;						    //???o��y?Y��?����
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,60,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			else                                    //?D???a?y����
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,60,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			printf("yaw:%d.%d,  ",temp/10,temp%10);//����1y��??��1��?3?yaw

			temp=roll*10;                            //?3temp?aroll
			if(temp<0)								//??��y?Y?y?o?D??��??D???a?o����
			{
				temp=-temp;						    //???o��y?Y��?����
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,80,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			else                                    //?D???a?y����
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//��?��?��???��??��?����?
				Gui_DrawFont_GBK16(64,80,BLUE,GRAY0,(u8*)tmp_buf);//LCD??��?16o?�䨮D?��?��?��?
			}
			printf("roll:%d.%d,  ",temp/10,temp%10);//����1y��??��1��?3?roll
			printf("gyrox:%5d,  gyroy:%5d,  gyroz:%5d,  aacx:%5d,  aacy:%5d,  aacz:%5d\r\n",gyrox,gyroy,gyroz,aacx,aacy,aacz);//��?����???��?����y?Y��????��?��?����y?Y

			LED2=!LED2;								 			//LED����??
			key = KEY_Scan(0);	                      			//���?������?��
			if(key == WKUP_PRES)                     			//���?��?��?aWK_UP����
			{
				mpu6050.flag = 0;                               //��?��������????3?��??��
				mpu6050.speed = 0;								//��?����?��?��3?��??��
				mpu6050.mode = 255;								//1�����?�꨺?3?��??��
				key = 255;										//���?��?��3?��??��
				MPU_Mode();										//??��??�꨺?????
			}
			mpu6050.flag = 0;									//��?��������????����???a?TD��
		}
	}
	else if(mpu6050.mode==2)									//??��?1�����?�꨺?2
	{
		if(mpu6050.flag == 1)									//��?��������?????a��DD��
		{
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//����?��?��?��?����??D?�¨�y?Yo������?Y��?��y?Y
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));//��?����?��??o����?��?��?��y?Y
			key = KEY_Scan(0);									//���?������?��
			if(key == WKUP_PRES)                     			//���?��?��?aWK_UP����
			{
				mpu6050.flag = 0;                               //��?��������????3?��??��
				mpu6050.speed = 0;								//��?����?��?��3?��??��
				mpu6050.mode = 255;								//1�����?�꨺?3?��??��
				key = 255;										//���?��?��3?��??��
				MPU_Mode();										//??��??�꨺?????
			}
			mpu6050.flag = 0;									//��?��������????����???a?TD��
		}
	}
	else ;														//����?��?��
}

