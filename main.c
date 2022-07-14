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
    #error "在bsp.h中选择配置 XPT2046_DRV 或者 GT1151_DRV"
           "XPT2046_DRV:  用于800*480 横屏的触摸屏."
           "GT1151_DRV:   用于480*800 竖屏的触摸屏."
           "如果都不选择, 注释掉本 error 信息, 然后自定义: LCD_display_mode[]"
  #endif
#endif

//-------------------------------------------------------------------------------------------------
// 主程序
//-------------------------------------------------------------------------------------------------

ErrorStatus HSEStartUpStatus;

#define PIX_LONG     ((u32)0x12c00)

#define GUI_CONST_STORAGE const
/* Private function prototypes -----------------------------------------------*/
void Demo_Init(void);
/* Private functions ---------------------------------------------------------*/

void SYS_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //?D??ó??è??·?×éoˉêy
	delay_init();	    							 //?óê±oˉêy3?ê??ˉ
	uart_init(115200);	 	                         //′??ú3?ê??ˉ?a115200
	LED_Init();		                               	 //3?ê??ˉó?LEDá??óμ?ó2?t?ó?ú
	KEY_Init();                                      //°′?ü3?ê??ˉ
	Lcd_Init();                                      //??ê??á3?ê??ˉ
	Lcd_Clear(GRAY0);                                //?á??3?ê??ˉ?a?òé?
	Gui_DrawFont_GBK16(4,30,BLACK,GRAY0,"MPU6050êy?Y2é?ˉ");//??×???ê?
	LCD_LED_SET;									 //?a???á??±31a


	MPU_Init();	                                     //3?ê??ˉMPU6050
	while(mpu_dmp_init())                            //3?ê??ˉmpu_dmp?a
 	{
		Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?ê??ˉê§°ü!");//??×???ê?
		LED1 = !LED1;delay_ms(50);LED1 = !LED1;     //LEDéá????ê?
		Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?ê??ˉê§°ü!");//??×???ê?
		printf("Initialization failed￡?\r\n");		//′??ú3?ê??ˉê§°üé?±¨
	}
	printf("Initialization succeed￡?\r\n");			//′??ú3?ê??ˉ3é1|é?±¨
	Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?ê??ˉ3é1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?ê??ˉ3é1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?ê??ˉ3é1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,GRAY0,GRAY0,"3?ê??ˉ3é1|!");delay_ms(200);Gui_DrawFont_GBK16(20,62,RED,GRAY0,"3?ê??ˉ3é1|!");//3?ê??ˉ3é1|ê±éá??èy′?
	LED2 = !LED2;delay_ms(50);LED2 = !LED2;		//LEDéá????ê?
	delay_ms(999);         /
	mpu6050.flag = 0;                               
	mpu6050.speed = 0;        
	mpu6050.mode = 255;								
	key = 255;									
}

void MPU_Mode(void)
{
	Lcd_Clear(GRAY0);							    //?á??3?ê??ˉ?a?òé?
	Gui_DrawFont_GBK16(24,16,BLUE,GRAY0,"?￡ê??????");//??×???ê?
	Gui_DrawFont_GBK16(12,60,BLUE,GRAY0,"KEY0:êμê±êy?Y");//??×???ê?
	Gui_DrawFont_GBK16(12,80,BLUE,GRAY0,"KEY1:?￡?a???á");//??×???ê?
	Gui_DrawFont_GBK16(12,100,BLUE,GRAY0,"WK_UP:·μ??");//??×???ê?
	while(1)//?-?·
	{
		key = KEY_Scan(0);//°′?üé¨?è
		if(key == KEY0_PRES)//KEY0′￥·￠
		{
			mpu6050.mode = 1;//1¤×÷?￡ê?éè???a1
			uart_init(115200);//3?ê??ˉ′??ú
			Lcd_Clear(GRAY0);//?á??3?ê??ˉ?a?òé?
			Gui_DrawFont_GBK16(32,16,BLACK,GRAY0,"êμê±êy?Y");
			Gui_DrawFont_GBK16(8,40,BLUE,GRAY0,"??????:      ??");//??×???ê?
			Gui_DrawFont_GBK16(8,60,BLUE,GRAY0,"??o???:      ??");//??×???ê?
			Gui_DrawFont_GBK16(8,80,BLUE,GRAY0,"·-1???:      ??");//??×???ê?
			Gui_DrawFont_GBK16(8,100,RED,GRAY0,"???è?μ:      ??");//??×???ê?
			goto X;//ì?×aμ?3??ú
		}
		else if(key == KEY1_PRES)//KEY1′￥·￠
		{
			mpu6050.mode = 2;//1¤×÷?￡ê?éè???a2
			uart_init(500000);//3?ê??ˉ′??ú
			Lcd_Clear(GRAY0);//?á??3?ê??ˉ?a?òé?
			Gui_DrawFont_GBK16(32,16,BLACK,GRAY0,"?￡?a???á");//??×???ê?
			Gui_DrawFont_GBK16(28,60,RED,GRAY0,"é?′??D...");//??×???ê?

			goto X;//ì?×aμ?3??ú
		}
		else ;//·à?¨?à
		delay_ms(10);//é¨?è°′?ü?óê±
	}
	X:;//3??ú
}

void MPU_Read(void)
{

	if(mpu_dmp_get_data(&pitch,&roll,&yaw)==0)//dmp′|àíμ?μ?êy?Y￡???·μ???μ??DD?D??
	{
		temp=MPU_Get_Temperature();	                //μ?μ????è?μ
		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//μ?μ??ó?ù?è′??D?÷êy?Y
		MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//μ?μ?íó?Yò?êy?Y
		mpu6050.speed++;                            //é?±¨?ù?è×??ó
		if(mpu6050.speed == 1)						//é?±¨?ù?è?D?μéè??
		{
			mpu6050.flag = 1;						//é?±¨±ê?????aóDD§
			mpu6050.speed = 0;						//é?±¨?ù?è1éá?
		}
	}
	else 											//2é?ˉ2?3é1|
	{
		mpu6050.flag = 0;							//é?±¨±ê?????a?TD§
	}
}

void DATA_Report(void)
{
	if(mpu6050.mode==1)								//??è?1¤×÷?￡ê?1
	{
		if(mpu6050.flag == 1)						//é?±¨±ê?????aóDD§
		{
			if(temp<0)								//??êy?Y?y?o?D??￡??D???a?oê±
			{
				temp=-temp;							//???oêy?Yè?·′
				sprintf(tmp_buf, "-%3d.%d",temp/100,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,100,RED,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			else                                    //?D???a?yê±
			{
				sprintf(tmp_buf, " %3d.%d",temp/100,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,100,RED,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			printf("temp:%d.%d,  ",temp/100,temp%10); //í¨1y′??ú1ê?3????è

			temp=pitch*10;							 //?3temp?apitch
			if(temp<0)								//??êy?Y?y?o?D??￡??D???a?oê±
			{
				temp=-temp;						    //???oêy?Yè?·′
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,40,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			else                                    //?D???a?yê±
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,40,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			printf("pitch:%d.%d,  ",temp/10,temp%10); //í¨1y′??ú1ê?3?pitch

			temp=yaw*10;                           //?3temp?ayaw
			if(temp<0)								//??êy?Y?y?o?D??￡??D???a?oê±
			{
				temp=-temp;						    //???oêy?Yè?·′
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,60,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			else                                    //?D???a?yê±
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,60,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			printf("yaw:%d.%d,  ",temp/10,temp%10);//í¨1y′??ú1ê?3?yaw

			temp=roll*10;                            //?3temp?aroll
			if(temp<0)								//??êy?Y?y?o?D??￡??D???a?oê±
			{
				temp=-temp;						    //???oêy?Yè?·′
				sprintf(tmp_buf, "-%3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,80,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			else                                    //?D???a?yê±
			{
				sprintf(tmp_buf, " %3d.%d",temp/10,temp%10);//×?·?′???ê??ˉ?üá?
				Gui_DrawFont_GBK16(64,80,BLUE,GRAY0,(u8*)tmp_buf);//LCD??ê?16o?′óD?×?·?′?
			}
			printf("roll:%d.%d,  ",temp/10,temp%10);//í¨1y′??ú1ê?3?roll
			printf("gyrox:%5d,  gyroy:%5d,  gyroz:%5d,  aacx:%5d,  aacy:%5d,  aacz:%5d\r\n",gyrox,gyroy,gyroz,aacx,aacy,aacz);//é?±¨???ù?èêy?Y￡????ó?ù?èêy?Y

			LED2=!LED2;								 			//LEDéá??
			key = KEY_Scan(0);	                      			//°′?üé¨?è
			if(key == WKUP_PRES)                     			//°′?ü?μ?aWK_UPê±
			{
				mpu6050.flag = 0;                               //é?±¨±ê????3?ê??ˉ
				mpu6050.speed = 0;								//é?±¨?ù?è3?ê??ˉ
				mpu6050.mode = 255;								//1¤×÷?￡ê?3?ê??ˉ
				key = 255;										//°′?ü?μ3?ê??ˉ
				MPU_Mode();										//??è??￡ê?????
			}
			mpu6050.flag = 0;									//é?±¨±ê????éè???a?TD§
		}
	}
	else if(mpu6050.mode==2)									//??è?1¤×÷?￡ê?2
	{
		if(mpu6050.flag == 1)									//é?±¨±ê?????aóDD§
		{
			mpu6050_send_data(aacx,aacy,aacz,gyrox,gyroy,gyroz);//·￠?í?ó?ù?è′??D?÷êy?Yoííó?Yò?êy?Y
			usart1_report_imu(aacx,aacy,aacz,gyrox,gyroy,gyroz,(int)(roll*100),(int)(pitch*100),(int)(yaw*10));//é?±¨?á??oóμ?×?ì?êy?Y
			key = KEY_Scan(0);								
			if(key == WKUP_PRES)                     		
			{
				mpu6050.flag = 0;                              
				mpu6050.speed = 0;							
				mpu6050.mode = 255;							
				key = 255;										//°′?ü?μ3?ê??ˉ
				MPU_Mode();										//??è??￡ê?????
			}
			mpu6050.flag = 0;									//é?±¨±ê????éè???a?TD§
		}
	}
	else ;														//·à?¨?à
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
    LCD_WriteRAM_Prepare(); // ?a??′?
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

			   if((temp7670 == 1))// ??×??ú||(value & 0x0200) ||(LCD_PCLK_STATE)
                {
                  	val1=value& 0x00ff;

                }
                else // μí×??ú	 if((temp7670 != 1)||(LCD_PCLK_STATE))
                {
                   	val2= value<<8  ; 	 //
					val =ili9320_BGR2RGB(val1 |val2);
                    temp7670 = 0;
                    LCD_WriteRAM(val); //TFT GRAM êy?Y
                    TimerCnt ++;
                }
            }


		}
  }
  	SYS_Init();//?μí33?ê??ˉ×üoˉêy
	MPU_Mode();//?μí31¤×÷?￡ê?????
	while(1)   //?÷?-?·
	{
		MPU_Read();    //MPU6050êy?Y?áè?
		DATA_Report(); //MPU6050êy?Yé?±¨
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
