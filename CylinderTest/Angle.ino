int sint=0;
int FiltTime = 30;
void Ang_ReadNFilt()
{
  long readang1=0;
  long readang2=0;
  int FiltCount = FiltTime;
  for(int i=0;i<FiltTime;i++)
  {
    if((analogRead(A0)>=0)&&(analogRead(A0)<1500))
    {readang1 += analogRead(A0);}
    else
    {FiltCount-=1;}
    readang2 += analogRead(A2);
    delay(1);
  }
  Ang1 = -(readang1 /(FiltCount*2.6) - Ang1Init);
  Ang2 = -(readang2 /(30*2.6*2) - Ang2Init);
  Ang3 = (sin(sint/31.4)+1)*22;
  sint++;
  if(sint>6280) sint=0;
  Angoal = Ang3;
  
  Serial.print("Angel:");
  if(Ang1>0)  {Serial.print(Ang1);}
  else        {Serial.print(0);}
  
  Serial.print("  Angoal:");
  if(Angoal>0)  {Serial.println(Angoal);}
  else          {Serial.println(0);}
}
