#include<IRremote.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

 LiquidCrystal_I2C lcd(0x27,16,2);
int last_state=-1;

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
 lcd.init();
 lcd.backlight();
 lcd.clear();
   delay(1000);
  
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
     
     if(systemState!=0 && systemState!=4)
     { if(tempval>45) 
     { systemState=4;}
     
     else if (gasval>180 && photoval<20)
     { systemState=5;}
     
     else if(gasval>180)
     { systemState=2;}
     
     else if(photoval<20)
     { systemState=3;}
      
      else if (systemState!=0)
          systemState=1;
     }
 
      if(systemState!=last_state)
      { lcd.clear();
        last_state=systemState;
      }
     
     
     
   switch(systemState)
   { case 0: Serial.println("AWAITING RITUAL");
              lcd.setCursor(0,0);
              lcd.print("AWAITING RITUAL");
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
               lcd.clear();
              IrReceiver.resume();
                delay(200);}

              
             if(displayMode==0)
             { Serial.print("Light value: ");
               Serial.println(photoval);
               lcd.setCursor(0,0);
               lcd.print("Light value: ");
               lcd.setCursor(0,1);
               lcd.print(photoval);
               lcd.print("  ");
             }
               else { Serial.print("Gas value: ");
                     Serial.println(gasval);
                     lcd.setCursor(0,0);
               lcd.print("Gas value: ");
               lcd.setCursor(0,1);
               lcd.print(gasval);
               lcd.print("  ");}
      break;
    
    

     case 5: 
  Serial.println("MULTIPLE PROBLEMS DETECTED");
  lcd.setCursor(0, 0);
  lcd.print("MULTIPLE ISSUES ");
  lcd.setCursor(0, 1);
  lcd.print("DETECTED        ");
  
  Wire.beginTransmission(8);
  Wire.write(3);
  Wire.endTransmission();
  break;

case 2: 
  Serial.println("TOXIC PURGE");
  lcd.setCursor(0, 0);
  lcd.print("TOXIC PURGE     ");
  break;

case 3: 
  Serial.println("NOCTIS PROTOCOL");
  lcd.setCursor(0, 0);
  lcd.print("NOCTIS PROTOCOL ");
  break;

case 4: 
  Serial.println("COOKED");
  lcd.setCursor(0, 0);
  lcd.print("COOKED          ");
  
  Wire.beginTransmission(8);
  Wire.write(1);
  Wire.endTransmission();

  if (IrReceiver.decode()) 
  {
    systemState = 1; 
    lcd.clear();
    IrReceiver.resume(); 
  }
  break;
          
   }
 
      delay(100);
     
}