#define valve1o 9
#define valve1i 8
#define valve2o 7
#define valve2i 6
#define valve3o 5
#define valve3i 4
#define valve4o 3
#define valve4i 2

void Valve_setup()
{
  pinMode(valve1i,OUTPUT);//电磁阀1
  pinMode(valve1o,OUTPUT);//电磁阀2
}


void pullCy(float pulltime)
{
    digitalWrite(valve1i,HIGH);
    delay(pulltime);
    digitalWrite(valve1i,LOW);
    delay(100);
}

void pushCy(float pushtime)
{
    digitalWrite(valve1o,HIGH);
    delay(pushtime);
    digitalWrite(valve1o,LOW);
    delay(50);
}
