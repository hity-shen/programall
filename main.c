#include "ls1b.h"
#include "mips.h"
#include "ili9320.h"
#include "OV7670.h"
//-------------------------------------------------------------------------------------------------
// BSP
//-------------------------------------------------------------------------------------------------

#include "bsp.h"

#ifdef BSP_USE_FB
  #include "ls1x_fb.h"
  #ifdef XPT2046_DRV
    char LCD_display_mode[] = LCD_800x480;
  #elif defined(GT1151_DRV)
    char LCD_display_mode[] = LCD_480x800;
  #else
    #error "��bsp.h��ѡ������ XPT2046_DRV ���� GT1151_DRV"
           "XPT2046_DRV:  ����800*480 �����Ĵ�����."
           "GT1151_DRV:   ����480*800 �����Ĵ�����."
           "�������ѡ��, ע�͵��� error ��Ϣ, Ȼ���Զ���: LCD_display_mode[]"
  #endif
#endif

//-------------------------------------------------------------------------------------------------
// ������
//-------------------------------------------------------------------------------------------------

ErrorStatus HSEStartUpStatus;

#define PIX_LONG     ((u32)0x12c00)

#define GUI_CONST_STORAGE const
/* Private function prototypes -----------------------------------------------*/
void Demo_Init(void);
/* Private functions ---------------------------------------------------------*/

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
	delay_ms(999);         /
	mpu6050.flag = 0;                               
	mpu6050.speed = 0;        
	mpu6050.mode = 255;								
	key = 255;									
}

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
			key = KEY_Scan(0);								
			if(key == WKUP_PRES)                     		
			{
				mpu6050.flag = 0;                              
				mpu6050.speed = 0;							
				mpu6050.mode = 255;							
				key = 255;										//���?��?��3?��??��
				MPU_Mode();										//??��??�꨺?????
			}
			mpu6050.flag = 0;									//��?��������????����???a?TD��
		}
	}
	else ;														//����?��?��
}


int main(void)
{

    static unsigned long TimerCnt = 0;
    static unsigned int temp7670 = 0;
	u16 value,val,val1,val2;

	Demo_Init();
    ili9320_Initializtion();
    ili9320_Clear(0xffff);
    delay_ms(100);
 	ili9320_SetCursor(0,0);
    LCD_WriteReg(0x0050, 0);
    LCD_WriteReg(0x0052, 0);
    LCD_WriteReg(0x0051, 239);
    LCD_WriteReg(0x0053, 319);
    LCD_WriteRAM_Prepare(); // ?a??��?
	//delay_ms(10);

	while(1!=OV7670_init());
    while(1)
    {
        TimerCnt = 0;
        temp7670 = 0;
        CLK_init_ON(); // OV7670 XCLK ?a
        while(value & 0x0800)    value = GPIOC->IDR;   // Vsync=H
        while((~value) & 0x0800) value = GPIOC->IDR;   // Vhync=L

        CLK_init_OFF(); //OV7670 XCLK 1?
		while(TimerCnt < 76800)
        {
			XCLK_L;
            XCLK_H;
			value = GPIOC->IDR;
			temp7670 ++;

			if(value & 0x0100) // HREF = H ||(LCD_PCLK_STATE)
            {
			  	 //value = GPIOC->IDR;
			  	 //display[TimerCnt]=value;

			   if((temp7670 == 1))// ??��??��||(value & 0x0200) ||(LCD_PCLK_STATE)
                {
                  	val1=value& 0x00ff;

                }
                else // �̨���??��	 if((temp7670 != 1)||(LCD_PCLK_STATE))
                {
                   	val2= value<<8  ; 	 //
					val =ili9320_BGR2RGB(val1 |val2);
                    temp7670 = 0;
                    LCD_WriteRAM(val); //TFT GRAM ��y?Y
                    TimerCnt ++;
                }
            }


		}
  }
  	SYS_Init();//?�̨�33?��??������o����y
	MPU_Mode();//?�̨�31�����?�꨺?????
	while(1)   //?��?-?��
	{
		MPU_Read();    //MPU6050��y?Y?����?
		DATA_Report(); //MPU6050��y?Y��?����
	}
}


/*******************************************************************************
* Function Name  : Demo_Init
* Description    : Initializes the demonstration application.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Demo_Init(void)
{
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_16);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
      /* Enable USART1 and GPIOA clock */
     RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
  }

  /* Enable GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOC
         | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG
         | RCC_APB2Periph_AFIO, ENABLE);

/*------------------- Resources Initialization -----------------------------*/
     /* ADCCLK = PCLK2/8 */
   RCC_ADCCLKConfig(RCC_PCLK2_Div8);

    /* Enable DMA clock */
   RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

   /* Enable ADC1 and GPIOC clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALL, ENABLE);

  /* If HSE is not detected at program startup */
  if(HSEStartUpStatus == ERROR)
  {
    /* Generate NMI exception */
    NVIC_SetSystemHandlerPendingBit(SystemHandler_NMI);
  }


}


#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number */
}
#endif



/*
 * @@ End
 */
