#define but1 22
#define but2 24
#define but3 26
#define but4 28
#define but5 30
#define but6 32


void Button_setup()
{
  pinMode(but1,INPUT);
  pinMode(but2,INPUT);
  pinMode(but3,INPUT);
  pinMode(but4,INPUT);
  pinMode(but5,INPUT);
  pinMode(but6,INPUT);
}

boolean prvb1 = 0;
boolean prvb2 = 0;
boolean prvb3 = 0;
boolean prvb4 = 0;
boolean prvb5 = 0;
boolean prvb6 = 0;

void but1act()
{
  if((digitalRead(but1)==1)&&(prvb1==0))
  {Cy_Init();}
  prvb1 = digitalRead(but1);
}

void but2act()
{
  if((digitalRead(but2)==1)&&(prvb2==0))
  {
    if(Run ==0) Run=1;
    else if(Run ==1)  Run=0;
    delay(100);
  }
  prvb2 = digitalRead(but2);
}

void but5act()
{
  if((digitalRead(but5)==1)&&(prvb5==0))
  {pullCy(500);}
  prvb5 = digitalRead(but5);
}
void but6act()
{
  if((digitalRead(but6)==1)&&(prvb6==0))
  {pushCy(500);}
  prvb6 = digitalRead(but6);
}
