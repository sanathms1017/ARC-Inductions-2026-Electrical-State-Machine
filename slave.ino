#include<Wire.h>
#include<Servo.h>

Servo myservo;

int photo_res=A0;
int gas_sens=A1;
int temp_sens =A2;
int buzzer =8;
int servo_pin=9;

volatile byte receivedCmd=0;


void setup()
{
  pinMode(buzzer,OUTPUT);
  myservo.attach(servo_pin);
  myservo.write(0);
  
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{  if(receivedCmd==1)
       myservo.write(180);
 
  else if (receivedCmd==2)
        myservo.write(0);
    
   else if (receivedCmd==3)
        digitalWrite(buzzer,HIGH);
    
    else if (receivedCmd==4)
         digitalWrite(buzzer,LOW);
    
    delay(50);
   
}

void receiveEvent( int howMany)
{ while (Wire.available())
    { receivedCmd=Wire.read();
    }
}

void requestEvent()
{ int tempval=analogRead(temp_sens);
  int photoval=analogRead(photo_res);
  int gasval=analogRead(gas_sens);
 
 float voltage= tempval*(5.0/1023.0);
 
 int  mappedphotoval=map(photoval,0,1023,0,255);
 int  mappedgasval=map(gasval,0,1023,0,255);
 int mappedtempval=(voltage-0.5)*100;
 
 Wire.write(mappedphotoval);
 Wire.write(mappedgasval);
 Wire.write(mappedtempval);
}