	/*Pin1 = 0;
	Pin2 = 0;
	num = 0;
	
	EA = 1; //�����ж�
	ET0 = 1; //����ʱ��0�ֿ���
	TMOD = 0x01;
	TH0 = (65535-10000)/256; //10ms
	TL0 = (65535-10000)%256;
	
	while(1)
	{
		if(Pin2 == 1)
		{
			if(num == 0)
			{TR0 = 1;	Pin1 = 1;}
			else
			{TR0 = 0;	Pin1 = 0;}
		}
		else
		{
			TR0 = 0;
			Pin1 = 0;
			num = 0;
		}
	}
}*/

	ADC_config();
	//Pin1 = 0;
	//Pin2 = 0;
	num = 0;
	EA = 1; //�����ж�
	ET0 = 1; //����ʱ��0�ֿ���
	TMOD = 0x01;
	TH0 = (65535-10000)/256; //10ms
	TL0 = (65535-10000)%256;
	OLED_Init();			//��ʼ��OLED  
	Delay_1ms(2000);
	OLED_Clear();
	while(1) 
	{		
		ang = Get_ADC10bitResult(0);
		
		//ang = Anglefilt();
		//ang = ang/2.86;
		OLED_numf(10,10,ang,5,16);
		
		/*TxSend(ang/1000 + '0');	//���ڷ��ͽǶ�����
		TxSend(ang%1000/100 + '0');
		TxSend(ang%100/10 + '0');
		TxSend(ang%10 + '0');
		TxSend(' ');
		TxSend(' ');*/
		//TxSend(ang%10 + '0');
		//TxSend(ang%10 + '0');
		PrintString("\r\n");
		//TxSend('5');
		//PrintString("5");
		//PrintString("AndTest");
		//PrintString("\n");
		Delay_50ms(50);
	//}	  
	}


/*
float Anglefilt()//���˲��ĽǶ�ADC��ȡ
{
	float t;
	char i,j;
	int temp,sum;
	unsigned char filtime=15;
	int buf[15];
	temp=0;
	sum=0;
	for(i=0;i<filtime;i++)
	{
		buf[i] = Get_ADC10bitResult(0);
		Delay_1ms(1);
	}
	for(j=0;j<filtime-1;j++)
	{
		for(i=0;i<filtime-1;i++)
		{
			if(buf[i]>buf[i+1])
			{
				temp=buf[i];
				buf[i]=buf[i+1];
				buf[i+1]=temp;
			}
		}
	}
	for(i=2;i<filtime-2;i++)
	{sum = sum+ buf[i];}
	t = sum/(filtime-4.0);
	return t;
}
	
	
	/*while(1)
	{
		if(Pin2 == 1)
		{
			if(num == 0)
			{TR0 = 1;	Pin1 = 1;}
			else
			{TR0 = 0;	Pin1 = 0;}
		}
		else
		{
			TR0 = 0;
			Pin1 = 0;
			num = 0;
		}
	}
}*/

void dly10ms() interrupt 0
{
	TH0 = (65535-10000)/256; //10ms
	TL0 = (65535-10000)%256;
	num++;
}

void	ADC_config(void)
{
	ADC_InitTypeDef		ADC_InitStructure;				//�ṹ����
	ADC_InitStructure.ADC_Px        = ADC_P10 | ADC_P11 | ADC_P12;	//����Ҫ��ADC��IO,	ADC_P10 ~ ADC_P17(�����),ADC_P1_All
	ADC_InitStructure.ADC_Speed     = ADC_360T;			//ADC�ٶ�			ADC_90T,ADC_180T,ADC_360T,ADC_540T
	ADC_InitStructure.ADC_Power     = ENABLE;			//ADC��������/�ر�	ENABLE,DISABLE
	ADC_InitStructure.ADC_AdjResult = ADC_RES_H8L2;		//ADC�������,	ADC_RES_H2L8,ADC_RES_H8L2
	ADC_InitStructure.ADC_Polity    = PolityLow;		//���ȼ�����	PolityHigh,PolityLow
	ADC_InitStructure.ADC_Interrupt = DISABLE;			//�ж�����		ENABLE,DISABLE
	ADC_Inilize(&ADC_InitStructure);					//��ʼ��
	ADC_PowerControl(ENABLE);							//������ADC��Դ��������, ENABLE��DISABLE
}




