#include	<config.h>
#include	"STC15Fxxxx.H"
#include "intrins.h"
#include <delay.h>
#include "oled.h"
#include 	"AngelADC.h"
#include  "Global.h"

unsigned char uart_receive_number_old=0;
unsigned char i = 0;
unsigned char code Buffer[] = "uart send";  //��Ҫ���͵�����
unsigned char *p;
unsigned char uart_receive_buffer[30];  //�Ӵ��ڽ��յ�����
unsigned char uart_receive_number=0;    //ָʾ���ڽ��ո���
	
//�Ƕȴ��������俨�����˲�

void Com_Init(void);

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
	

	p = Buffer;
  Com_Init();
  
  while(1){
    //��
    if(uart_receive_number_old != uart_receive_number){
      uart_receive_number_old = uart_receive_number;
    }
    
    //�����Ƭ����������HC-xxģ�鲻����1�ֽڵĴ������ݣ�һ��Ҫ����ʱ�����ж��Ƿ�
    //���д��������ڷ��ͣ��������������Ľ���һ֡��������
    delay_ms(30);
    
    //һ��ʱ��֮����Ȼ��ȣ���ʾ��λ��������һ�����ݽ���
    if(uart_receive_number_old==uart_receive_number){
      if(uart_receive_number)		break;
    }
		
		//��
		SBUF = *p;
    while(!TI){  //���������ϣ�Ӳ������λTI
      _nop_();   //�˾��ƺ�����ȥ��
    }
    TI = 0;  //TI����
    p++;
    if(*p == '\0')		break;  //��ÿ���ַ�������󣬻���һ��'\0'
		
  }
	
	
	
  if(uart_receive_number)
	{
    //����������ʱ������Щʲô����ʲô��
  }
	
}

void Com_Init(void) 
{
  /*TMOD = 0x20;
  PCON = 0x00;
  SCON = 0x50;
  TH1 = 0xFd;  //���ò����� 9600
  TL1 = 0xFd;
  TR1 = 1;  //������ʱ��1
  ES = 1;  //�������ж�
  EA = 1;  //�����ж�*/
}
