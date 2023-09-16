#include	<config.h>
#include	"STC15Fxxxx.H"
#include "intrins.h"
#include <delay.h>
#include "oled.h"
#include 	"AngelADC.h"
#include  "Global.h"

unsigned char uart_receive_number_old=0;
unsigned char i = 0;
unsigned char code Buffer[] = "uart send";  //所要发送的数据
unsigned char *p;
unsigned char uart_receive_buffer[30];  //从串口接收的数据
unsigned char uart_receive_number=0;    //指示串口接收个数
	
//角度传感器及其卡尔曼滤波

void Com_Init(void);

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
	

	p = Buffer;
  Com_Init();
  
  while(1){
    //收
    if(uart_receive_number_old != uart_receive_number){
      uart_receive_number_old = uart_receive_number;
    }
    
    //如果单片机接收来自HC-xx模块不少于1字节的串口数据，一定要加延时后再判断是否
    //还有串口数据在发送，这样才能完整的接收一帧串口数据
    delay_ms(30);
    
    //一段时间之后仍然相等，表示上位机发来的一串数据结束
    if(uart_receive_number_old==uart_receive_number){
      if(uart_receive_number)		break;
    }
		
		//发
		SBUF = *p;
    while(!TI){  //如果发送完毕，硬件会置位TI
      _nop_();   //此句似乎可以去掉
    }
    TI = 0;  //TI清零
    p++;
    if(*p == '\0')		break;  //在每个字符串的最后，会有一个'\0'
		
  }
	
	
	
  if(uart_receive_number)
	{
    //串口有数据时，该做些什么就做什么吧
  }
	
}

void Com_Init(void) 
{
  /*TMOD = 0x20;
  PCON = 0x00;
  SCON = 0x50;
  TH1 = 0xFd;  //设置波特率 9600
  TL1 = 0xFd;
  TR1 = 1;  //启动定时器1
  ES = 1;  //开串口中断
  EA = 1;  //开总中断*/
}
