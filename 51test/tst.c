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