#include <U8g2lib.h>
#include <FlexiTimer2.h>
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE);

float cyTime = 0;

float Ang = 0;
float Angoal = 0;
float Ang1 = 0;
float Ang2 = 0;
float Ang3 = 0;
float Ang1Init = 0;
float Ang2Init = 0;

void setup() 
{
  pinMode(A0,INPUT);
  pinMode(A2,INPUT);
  Valve_setup();
  Button_setup();
  Cy_Init();
  
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  FlexiTimer2::set(200, Ang_ReadNFilt);//每100ms执行角度计算
  FlexiTimer2::start();//定时中断开启
}
boolean Run=0;
void loop() 
{ 
  but1act();
  but2act();
  but5act();
  but6act();
  if(Run == 1)  Cy_Ctrl();
  
  //显示屏打印
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0,10,"Angel");
    u8g2.setCursor(70,10);
    u8g2.print(Ang1);
    
    u8g2.drawStr(0,25,"Angoal");
    u8g2.setCursor(70,25);
    u8g2.print(Angoal);
    
    u8g2.drawStr(0,40,"cyTime");
    u8g2.setCursor(70,40);
    u8g2.print(cyTime);
    u8g2.drawStr(100,40,"ms");
    
    u8g2.drawStr(0,55,"Run");
    u8g2.setCursor(70,55);
    u8g2.print(Run);
  }while(u8g2.nextPage());
}
