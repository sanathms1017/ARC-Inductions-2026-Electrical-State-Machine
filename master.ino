#include<IRremote.h>
#include<Wire.h>


int IR_pin=2;
int photoval;
int gasval;
int tempval;
int systemState=0;
bool displayMode;
void setup()
{ Wire.begin();
  Serial.begin(9600);
  IrReceiver.begin(IR_pin);
   
  
}

void loop()
{  Wire.requestFrom(8,3);
    if (Wire.available()>=3)
    { photoval=Wire.read();
      gasval=Wire.read();
      tempval=Wire.read();
    }
  
      Serial.println(photoval);
      Serial.println(gasval);
      Serial.println(tempval);
     
     if(systemState!=4)
     { if(tempval>45) 
     { systemState=4;}
     
     else if (gasval>130 && photoval<20)
     { systemState=5;}
     
     else if(gasval>180)
     { systemState=2;}
     
     else if(photoval<20)
     { systemState=3;}
      
      else if (systemState!=0)
          systemState=1;
     }
     
     
     
   switch(systemState)
   { case 0: Serial.println("AWAITING RITUAL");
             Wire.beginTransmission(8);
             Wire.write(2);
             Wire.endTransmission();
             Wire.beginTransmission(8);
             Wire.write(4);
             Wire.endTransmission();
    
            if (IrReceiver.decode()) 
            { systemState = 1; 
              IrReceiver.resume(); }

      break;
    
      case 1: Wire.beginTransmission(8);
              Wire.write(2);
              Wire.endTransmission();
              Wire.beginTransmission(8);
              Wire.write(4);
              Wire.endTransmission();
  
  
             if (IrReceiver.decode()) 
             { displayMode = !displayMode; 
              IrReceiver.resume();
                delay(200);}

              
             if(displayMode==0)
             { Serial.print("Light value: ");
               Serial.println(photoval);
             }
               else { Serial.print("gas value: ");
                     Serial.println(gasval);}
      break;
    
    

     case 5: Serial.println("MULTIPLE PROBLEMS DETECTED");
             Wire.beginTransmission(8);
             Wire.write(3);
             Wire.endTransmission();
      break;

     case 2: Serial.println("TOXIC PURGE");
      break;

     case 3: Serial.println("NOCTIS PROTOCOL");
      break;

     case 4: Serial.println("COOKED");
             Wire.beginTransmission(8);
             Wire.write(1);
             Wire.endTransmission();
    
    
              if (IrReceiver.decode()) 
              {systemState = 1; 
              IrReceiver.resume(); }

      break;
          
   }
 
      delay(100);
     
}