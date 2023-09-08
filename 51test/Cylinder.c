#include	"Cylinder.h"
#include	"AngelADC.h"


float kp=0.8;
float ki=0;
float kd=2;
float Angle[8];
float prverr[8];
float pid_i[8];
float pid_d[8];
float cytime[8];
unsigned int cytime_counter[8];
unsigned long int Cy_us=0;//100微秒定时中断的计数

void Cy_Init(void)
{
	unsigned char i;
	P2 |= 11111111;
	/*P24=1;P26=1;
	for(i=0;i<10;i++){delay_ms(100);}
	P25=1;P27=1;
	for(i=0;i<10;i++){delay_ms(100);}
	P21=1;P23=1;
	for(i=0;i<10;i++){delay_ms(100);}
	P20=1;P22=1;*/
	
	for(i=0;i<15;i++){delay_ms(100);}
	P4=0;
	for(i=0;i<15;i++){delay_ms(100);}
	P2=0;
	for(i=0;i<10;i++){delay_ms(100);Anglefilt();}
	
	for(i=0;i<8;i++)
	{//初始值置0
		ang_initial[i]=ang_kf[i];
		ang_goal[i]=0;
		Angle[i]=0;
		pid_i[i]=0;
		pid_d[i]=0;
		prverr[i]=0;
		cytime[i]=0;
	}
	Anglefilt();
	for(i=0;i<10;i++)
	{delay_ms(100);}
}

void CyCtrl()
{//根据角度的气缸控制，PID
	unsigned char i;
	for(i=0;i<8;i++)
	{
		Angle[i] = ang_goal[i] - ang_kf[i];
		if((Angle[i]>5)||(Angle[i]<-5))
		{
			pid_i[i] = pid_i[i]+Angle[i];
			pid_d[i] = Angle[i]-prverr[i];
			prverr[i]=Angle[i];
			cytime[i] = kp*Angle[i] + ki*pid_i[i] + kd*pid_d[i];
			
			if(cytime[i]>0)				{cytime[i]=cytime[i]+6;}
			else if(cytime<0)			{cytime[i]=cytime[i]-6;}
			if(cytime[i]>500)			{cytime[i]=500;}
			else if(cytime[i]<-500){cytime[i]=-500;}
		}
		else
		{cytime[i]=0;}
		
	}
}

void Cy_Timer_Init()
{//定时器2用于气缸控制的计时
	//100微秒,0.1ms
	AUXR |= 0x04;			//定时器时钟1T模式
	T2L = 0x48;				//设置定时初始值
	T2H = 0xF4;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//开定时器2中断
}

void Cy_Push(void)
{
	unsigned char i;
	Cy_us++;
	//if((P2==0x00)&&(P4==0x00)&&(Cy_us>=2000))
	if((Cy_us>=10000)||((P2==0x00)&&(P4==0x00)&&(Cy_us>=2000)))
	{//开关时长大于1秒，或在100ms内所有时间均计时结束
		Cy_us=0;
		CyCtrl();
	}
	//随着该函数运行次数的增加，“目标时间[8]”不断减小
	//如果“目标时间[i]”减小到0，则关闭对应的引脚
	if(Cy_us<=3)
	{//将对应的引脚置为1
		for(i=0;i<8;i++)
		{
			if(cytime[i]>7.0)
			{
				cytime_counter[i]=(unsigned int)(cytime[i]*10.0);
				switch (i)
				{
					case 0:	P20=0;P21=1;	break;
					case 1:	P22=0;P23=1;	break;
					case 2:	P24=0;P25=1;	break;
					case 3:	P26=0;P27=1;	break;
					case 4:	P40=0;P41=1;	break;
					case 5:	P42=0;P43=1;	break;
					case 6:	P44=0;P45=1;	break;
					case 7:	P46=0;P47=1;	break;
					default:	break;
				}
			}
			else if(cytime[i]<-7.0)
			{
				cytime_counter[i]=(unsigned int)(-cytime[i]*15.0);
				switch (i)
				{
					case 0:	P20=1;P21=0;	break;
					case 1:	P22=1;P23=0;	break;
					case 2:	P24=1;P25=0;	break;
					case 3:	P26=1;P27=0;	break;
					case 4:	P40=1;P41=0;	break;
					case 5:	P42=1;P43=0;	break;
					case 6:	P44=1;P45=0;	break;
					case 7:	P46=1;P47=0;	break;
					default:	break;
				}
			}				
		}
	}
	else
	{
		for(i=0;i<8;i++)
		{
			if(cytime_counter[i]>0)
			{cytime_counter[i]--;}
			else
			{
				switch (i)
				{
					case 0 :	P20=0;P21=0;	break;
					case 1 :	P22=0;P23=0;	break;
					case 2 :	P24=0;P25=0;	break;
					case 3 :	P26=0;P27=0;	break;
					case 4 :	P40=0;P41=0;	break;
					case 5 :	P42=0;P43=0;	break;
					case 6 :	P44=0;P45=0;	break;
					case 7 :	P46=0;P47=0;	break;
					default:	break;
				}
			}
		}
	}
}