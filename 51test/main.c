//#include	<STC15Fxxxx.H>
#include	<config.h>
#include	<delay.h>

#include 	"oled.h"
#include 	"JY901.h"
#include 	"AngelADC.h"
#include  "Global.h"
#include	"Cylinder.h"

unsigned char loop1,loop2,loop3;
unsigned char timeloop=0;
unsigned char num=0;
unsigned char g=0;
unsigned char rcv=0;

#define S4RI  0x01              //S4CON.0
#define S4TI  0x02              //S4CON.1
#define S4RB8 0x04              //S4CON.2
#define S4TB8 0x08              //S4CON.3

unsigned char uart_receive_number_old=0;
unsigned char code Buffer[] = "uart send";  //所要发送的数据
unsigned char *p;
unsigned char uart_receive_buffer[30];  //从串口接收的数据
unsigned char uart_receive_number=0;    //指示串口接收个数
static unsigned char i_com = 7;  //定义为静态变量，i 的值不会发生改变

float AngleRoll = 0;
float AnglePitch = 0;
float deltang0,deltang1;

void SetGoal(void);
float IK_AngK(float x,float y);
float IK_AngH(float x, float y);
void Balance(void);

void oledShow(void);
void HC08_Init(void);


void main()
{	
	P0M1 = 0x00;	P0M0 = 0x00;	//准双向口
	P1M0 = 0x00;	P1M1 = 0x00; 
	P3M1 = 0x00;	P3M0 = 0x00;	  
	P2M0 = 0xff;	P2M1 = 0x00; //推挽输出
	P4M0 = 0xff;	P4M1 = 0x00;
	P2=0;	P4=0;
	
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,0,"init",16);
	
	
	TR0 = 0;//停止计数

	//定时器0用于主程序
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0xDC;				//设置定时初始值
	TH0 = 0x0B;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时

	INT_CLKO &= ~0x01;//不输出时钟
	ET0 = 1; //开定时器0分开关
	
	p = Buffer;
  
	Angle_Init();
	Cy_Init();
	
	
	setUART1();//JY61P，使用串口1_2 & 定时器1
	HC08_Init();//HC08蓝牙，使用串口4 & 定时器4
	Cy_Timer_Init();//定时器2用于气缸控制
	loop1=0;
	loop2=0;
	loop3=0;
	delay_ms(100);
	
	OLED_Clear();
	PS = 1;//串行口中断优先
	ES = 1;//使能串口中断
	EA = 1;//打开总中断
	
  while(1)
	{

  }
}

void mainloop() interrupt 1
{//25ms定时中断
	loop1++;loop2++;loop3++;
	if(loop1>=2)//50ms
	{
		Anglefilt();
		oledShow();
		loop1=0;
	}
	if(loop2>=6)//150ms
	{
		P55 = ~P55;
		CyCtrl();
		AngleRoll = (float)stcAngle.Angle[0]/32768*180;//横滚角
		AnglePitch = (float)stcAngle.Angle[1]/32768*180;//俯仰角
		loop2=0;
	}
	if(loop3>=40)//1s
	{
		loop3=0;
		SetGoal();
	}
}

void Cy_PushTimer() interrupt 12
{
	Cy_Push();
}

void Uart1_JY901() interrupt 4
{//JY901串口接收中断
	if (RI)
	{
		RI=0;
		CopeSerialData(SBUF);	    
	}
}

void Uart4_HC08(void) interrupt 18
{
	rcv++;
  if(S4CON & S4RI)
	{  //当硬件接收到一个数据时，RI会置位
    S4CON &= ~S4RI;         //清除S4RI位
    uart_receive_buffer[uart_receive_number] = S4BUF;
    uart_receive_number++;
  }
}

void oledShow(void)
{
	if(ang_kf[0]>ang_adc[0])
	{deltang0 = ang_kf[0] - ang_adc[0];}
	else
	{deltang0 = ang_adc[0] - ang_kf[0];}
	if(ang_kf[1]>ang_adc[1])
	{deltang1 = ang_kf[1] - ang_adc[1];}
	else
	{deltang1 = ang_adc[1] - ang_kf[1];}
	
	OLED_numf(20,0,AngleRoll,3,16);
	OLED_numf(60,0,AnglePitch,3,16);
	
	OLED_numf(0,2,ang_kf[0],3,12);
	OLED_numf(50,2,ang_kf[1],3,12);
	OLED_numf(0,3,ang_kf[2],3,12);
	OLED_numf(50,3,ang_kf[3],3,12);
	
	OLED_numf(0,4,ang_goal[0],3,12);
	OLED_numf(50,4,ang_goal[1],3,12);
	
	OLED_num(0,5,rcv,3,12);
}

void HC08_Init(void) 
{
	S4CON = 0x10;		//8位数据,可变波特率
	S4CON |= 0x40;		//串口4选择定时器4为波特率发生器
	T4T3M |= 0x20;		//定时器时钟1T模式
	T4L = 0xF3;			//设置定时初始值
	T4H = 0xFC;			//设置定时初始值
	T4T3M |= 0x80;		//定时器4开始计时
	IE2 = 0x10;       //使能串口4中断
}