/*
 * main4.c
 *
 * created: 2022/7/14
 *  author: 
 */

void errorLog(int num);
void parseGpsBuffer(void);
void printGpsBuffer(void);

int main(void)
{
	delay_init();

	NVIC_Configuration(); 	 
	uart_init(9600);	
	Init_LEDpin();
	LED1 = 1;

	clrStruct();

	printf("Welcome to use!\r\n");
	printf("ILoveMcu.taobao.com!\r\n");
	while(1)
	{
		parseGpsBuffer();
		printGpsBuffer();


	}
}

void errorLog(int num)
{

	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf(Save_Data.GPS_Buffer);


		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//?a??¡ä¨ª?¨®
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2];
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//??¨¨?UTC¨º¡À??
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//??¨¨?UTC¨º¡À??
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//??¨¨??3?¨¨D??¡é
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//??¨¨?N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//??¨¨??-?¨¨D??¡é
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//??¨¨?E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//?a??¡ä¨ª?¨®
				}
			}


		}
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;

		printf("Save_Data.UTCTime = ");
		printf(Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			printf("Save_Data.latitude = ");
			printf(Save_Data.latitude);
			printf("\r\n");


			printf("Save_Data.N_S = ");
			printf(Save_Data.N_S);
			printf("\r\n");

			printf("Save_Data.longitude = ");
			printf(Save_Data.longitude);
			printf("\r\n");

			printf("Save_Data.E_W = ");
			printf(Save_Data.E_W);
			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}

	}
}







