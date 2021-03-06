#include <Wire.h>
int16_t bigNum1, bigNum2, bigNum3, bigNum4;
byte myArray1[2],  myArray2[2], myArray3[2], myArray4[2];

#include <Stepper.h>
int infrared = 8;
int Relay1 = 6;//dc
int Relay2 = 7;//led
//int value=0;

const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 9, 11, 10, 12);



String readString;
#define ledpin 13
int blue;
int x;
void setup() 
{
Wire.begin();
Serial.begin(9600);
pinMode(ledpin,OUTPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Relay1,HIGH);//trang thai off cua rl 1
  digitalWrite(Relay2,LOW);//trang thai off cua rl 2
  pinMode(infrared, INPUT_PULLUP);
  myStepper.setSpeed(15);
  Serial.println("Connection established...");


}
void loop() {
  while (Serial.available()) {
      delay(3);
      char a = Serial.read();
      if (a == '#'){break;}
      readString += a;
    }
    ///////////////////////////////////////////////////////////////////////
   if (readString.length()>0) 
   {
   blue = readString.toInt();
   readString = "";
   delay(100);
   } 
/////////////////////////////////////////////
 if (blue==5)// Reverse
    {
    digitalWrite(ledpin,1);
    x=135;
    tinh_gui();
    } 
 if (blue==6)// turnLeft
    {
    digitalWrite(ledpin,1);
    x=45;
    tinh_gui();
    } 
 if (blue==7)//turnRight
     {
    digitalWrite(ledpin,1);
    x=225;
    tinh_gui();
     } 
 if (blue==8) // Forward
    {
    digitalWrite(ledpin,1);
    x=315;
    tinh_gui();
    } 
if (blue== 0)
    {
    digitalWrite(ledpin,0);
    bigNum1 = 0;
    bigNum2 = 0;
    gui();
    }

    // code Kien
    if (blue == 11)
    {
      vaoa();
  
    }
    else if (blue == 12)
    {
     vaob(); 
    }
    else if (blue == 14)
    {
     stopall(); 
    }
//    else if (blue == d)
//    {
//     lenx(); 
//    }
     if ( (blue == 3) or (blue == 4)) 
      { 
      slave3();
//    delay(500);
    slave30();
//    Serial.clear();
      }
//    delay(500);
//    slave30();
//    Serial.clear();
     
//        Serial.flush();
Serial.println(bigNum1);
Serial.println(bigNum2);
}
void slave3()
{
bigNum3 = blue;
myArray3[0] = (bigNum3 >> 8) & 0xFF;
myArray3[1] = bigNum3 & 0xFF;  
Wire.beginTransmission(3); // transmit to device #3
Wire.write(myArray3, 2);
Wire.endTransmission(); // stop transmitting
delay(10) ;

}
void slave30()
{
bigNum3 = 4;
myArray3[0] = (bigNum3 >> 8) & 0xFF;
myArray3[1] = bigNum3 & 0xFF;  
Wire.beginTransmission(3); // transmit to device #3
Wire.write(myArray3, 2);
Wire.endTransmission(); // stop transmitting
}

void tinh_gui()
{
 ////////////////Tinh//////////
bigNum1 = 30*sin(x*PI/180);
bigNum2 = -30*cos(x*PI/180);

////////////////////////////////////////////
myArray1[0] = (bigNum1 >> 8) & 0xFF;
myArray1[1] = bigNum1 & 0xFF;
myArray2[0] = (bigNum2 >> 8) & 0xFF;
myArray2[1] = bigNum2 & 0xFF;

////////////////////Gui/////////////////////////
Wire.beginTransmission(1); // transmit to device #1
Wire.write(myArray1, 2);
Wire.endTransmission(); // stop transmitting
Wire.beginTransmission(2); // transmit to device #2
Wire.write(myArray2, 2);
Wire.endTransmission(); // stop transmitting

//Wire.beginTransmission(4); // transmit to device #4
//Wire.write(myArray2, 2);
//Wire.endTransmission(); // stop transmitting
delay(10);
 }
void gui()
 {
myArray1[0] = (bigNum1 >> 8) & 0xFF;
myArray1[1] = bigNum1 & 0xFF;
myArray2[0] = (bigNum2 >> 8) & 0xFF;
myArray2[1] = bigNum2 & 0xFF;
/////////////////////////////////////////////
Wire.beginTransmission(1); // transmit to device #1
Wire.write(myArray1, 2);
Wire.endTransmission(); // stop transmitting
Wire.beginTransmission(2); // transmit to device #2
Wire.write(myArray2, 2);
Wire.endTransmission(); // stop transmitting
//Wire.beginTransmission(3); // transmit to device #3
//Wire.write(myArray1, 2);
//Wire.endTransmission(); // stop transmitting
//Wire.beginTransmission(4); // transmit to device #4
//Wire.write(myArray2, 2);
//Wire.endTransmission(); // stop transmitting
delay(10);
}
// code Kien
void vaoa(){
      digitalWrite(Relay2, HIGH);
      if(digitalRead(infrared) == 0)
        {
          digitalWrite(Relay1, LOW);
          delay(9000);
          digitalWrite(Relay1, HIGH);
          lenx();
          delay(50);
        }
      else
        {   
          digitalWrite(Relay1, HIGH);
          delay(50);
        }
}
void vaob(){
  digitalWrite(Relay2, HIGH);
  if(digitalRead(infrared) == 0)
  {
  len();
  digitalWrite(Relay1, LOW);
  delay(8000);
  xuon();
  digitalWrite(Relay1, HIGH);
  delay(50);
  }
  else {
  digitalWrite(Relay1, HIGH);
  delay(50);
}
}
void lenx(){
  myStepper.step(0.3*stepsPerRevolution);
  myStepper.step(-0.3*stepsPerRevolution);
    myStepper.step(0.3*stepsPerRevolution);
  myStepper.step(-0.3*stepsPerRevolution);
    myStepper.step(0.3*stepsPerRevolution);
  myStepper.step(-0.3*stepsPerRevolution);
}
void len(){
  myStepper.step(0.1*stepsPerRevolution);
}
void xuon(){
  myStepper.step(-0.1*stepsPerRevolution);
}
void stopall(){
  //digitalWrite(Relay1, HIGH);
  digitalWrite(Relay2, LOW);
  delay(50);
}
void ledon(){
  digitalWrite(Relay2, HIGH);
  
}
void ledoff(){
  digitalWrite(Relay2, LOW);
}