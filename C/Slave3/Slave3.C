
#include <Wire.h>
int bigNum;
int i;
 // RFID
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <Stepper.h>
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
#define ACCESS_DELAY 5000  // Độ trể của relay khi đóng
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN);   // tạo MFRC522.
#define LED 7

const int stepsPerRevolution = 2048;
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5);


void setup() {
 Wire.begin(3);
 Wire.onReceive(receiveEvent);  
 Serial.begin(9600); //initialize serial comunication

  pinMode(LED , OUTPUT);

 // RFID
   Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();          // Initiate  SPI bus
  mfrc522.PCD_Init();   // bắt đầu MFRC522
//  pinMode(Relay, OUTPUT);
//  pinMode(BUZZER, OUTPUT);
//  noTone(BUZZER);
//  digitalWrite(Relay, LOW);
  Serial.println("Hãy đặt thẻ của bạn vào cảm biến...");
  Serial.println ();
  myStepper.setSpeed(15);
}
void loop() 
{
if ( Wire.available()) 
{ 
byte a,b;
a = Wire.read();
b = Wire.read(); 
bigNum = a;
bigNum = bigNum << 8 | b;
Serial.println(bigNum);
//Serial.println(receiveEvent);
}
  if (bigNum == 3)
  { 
    i=i+1;
//    Serial.println(i);
    if (i==2) {
      i=0;
   myStepper.step(-3.7*stepsPerRevolution);

    delay(ACCESS_DELAY);

    myStepper.step(3.7*stepsPerRevolution);

  }}
//  else if ( bigNum == 4)
//  {
//  digitalWrite(LED, LOW);  
//  }

// RFID
 if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "F2 35 F9 2C")   //đổi UID của thẻ ở đây/thẻ muốn cấp quyền truy cập
  {
    Serial.println("Cho phép truy cập");
    Serial.println();
    myStepper.step(-3.7*stepsPerRevolution); //len
    delay(ACCESS_DELAY);
    myStepper.step(3.7*stepsPerRevolution);  //xuon
  }
  else   {
    Serial.println(" Từ chối truy cập");
  }
} 
void receiveEvent(int howMany) {
}