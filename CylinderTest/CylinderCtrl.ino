float kp = 0.1;
float ki = 0.003;
float kd = 1;
float I = 0;
float Prverr = 0;

void Cy_Init()
{//气缸初始化，先缩短到最低距离，然后读取角度初始值
  delay(500);
  Ang1Init = 0;
  Ang2Init = 0;
  pullCy(1000);
  Ang_ReadNFilt();
  Ang1Init = -Ang1;
  Ang2Init = -Ang2;
  delay(500);
}
void Cy_Ctrl()
{
  Ang_ReadNFilt();
  if(Run == 1)
  {
    float err = Angoal-Ang1;
    float P = kp*abs(err);
    I += ki*abs(err);
    float D = kd*abs(err-Prverr);
    Prverr = err;
    if(err>3)
    {
      cyTime = 6+P+I+D;
      if(cyTime>300) cyTime=300;
      pushCy(cyTime);
    }
    else if(err<-3)
    {
      cyTime = 5+P+I+D;
      if(cyTime>300) cyTime=300;
      pullCy(cyTime);
    }
    delay(2*cyTime);//排气时间，等待角度数值稳定
  }
}
