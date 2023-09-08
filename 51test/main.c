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
unsigned char code Buffer[] = "uart send";  //��Ҫ���͵�����
unsigned char *p;
unsigned char uart_receive_buffer[30];  //�Ӵ��ڽ��յ�����
unsigned char uart_receive_number=0;    //ָʾ���ڽ��ո���
static unsigned char i_com = 7;  //����Ϊ��̬������i ��ֵ���ᷢ���ı�

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
	P0M1 = 0x00;	P0M0 = 0x00;	//׼˫���
	P1M0 = 0x00;	P1M1 = 0x00; 
	P3M1 = 0x00;	P3M0 = 0x00;	  
	P2M0 = 0xff;	P2M1 = 0x00; //�������
	P4M0 = 0xff;	P4M1 = 0x00;
	P2=0;	P4=0;
	
	OLED_Init();
	OLED_Clear();
	OLED_ShowString(0,0,"init",16);
	
	
	TR0 = 0;//ֹͣ����

	//��ʱ��0����������
	AUXR &= 0x7F;			//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;			//���ö�ʱ��ģʽ
	TL0 = 0xDC;				//���ö�ʱ��ʼֵ
	TH0 = 0x0B;				//���ö�ʱ��ʼֵ
	TF0 = 0;				//���TF0��־
	TR0 = 1;				//��ʱ��0��ʼ��ʱ

	INT_CLKO &= ~0x01;//�����ʱ��
	ET0 = 1; //����ʱ��0�ֿ���
	
	p = Buffer;
  
	Angle_Init();
	Cy_Init();
	
	
	setUART1();//JY61P��ʹ�ô���1_2 & ��ʱ��1
	HC08_Init();//HC08������ʹ�ô���4 & ��ʱ��4
	Cy_Timer_Init();//��ʱ��2�������׿���
	loop1=0;
	loop2=0;
	loop3=0;
	delay_ms(100);
	
	OLED_Clear();
	PS = 1;//���п��ж�����
	ES = 1;//ʹ�ܴ����ж�
	EA = 1;//�����ж�
	
  while(1)
	{

  }
}

void mainloop() interrupt 1
{//25ms��ʱ�ж�
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
		AngleRoll = (float)stcAngle.Angle[0]/32768*180;//�����
		AnglePitch = (float)stcAngle.Angle[1]/32768*180;//������
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
{//JY901���ڽ����ж�
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
	{  //��Ӳ�����յ�һ������ʱ��RI����λ
    S4CON &= ~S4RI;         //���S4RIλ
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
	S4CON = 0x10;		//8λ����,�ɱ䲨����
	S4CON |= 0x40;		//����4ѡ��ʱ��4Ϊ�����ʷ�����
	T4T3M |= 0x20;		//��ʱ��ʱ��1Tģʽ
	T4L = 0xF3;			//���ö�ʱ��ʼֵ
	T4H = 0xFC;			//���ö�ʱ��ʼֵ
	T4T3M |= 0x80;		//��ʱ��4��ʼ��ʱ
	IE2 = 0x10;       //ʹ�ܴ���4�ж�
}