
#include <Wire.h>
#include <PID_v1.h>
#define MotEnable 6 //Motor Enamble pin Runs on PWM signal
#define MotFwd 7 // Motor Forward pin
#define MotRev 4 // Motor Reverse pin

int User_Input = 0; // This while convert input string into integer
int16_t bigNum; //value I2C receive

int encoderPin1 = 2; //Encoder Output 'A' must connected with intreput pin of arduino.
int encoderPin2 = 3; //Encoder Otput 'B' must connected with intreput pin of arduino.
volatile int lastEncoded = 0; // Here updated value of encoder store.
volatile long encoderValue = 0; // Raw encoder value


int REV = 0; // Set point REQUIRED ENCODER VALUE

int lastMSB = 0;
int lastLSB = 0;
double kp = 7.2, ki = 0.035 , kd = 0.01; // modify for optimal performance
double input = 0, output = 0, setpoint = 0;
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT);
void setup() {
  Wire.begin(1);
  Wire.onReceive(receiveEvent);
  pinMode(MotEnable, OUTPUT);
  pinMode(MotFwd, OUTPUT);
  pinMode(MotRev, OUTPUT);
  Serial.begin(9600); //initialize serial comunication
  pinMode(encoderPin1, INPUT_PULLUP);
  pinMode(encoderPin2, INPUT_PULLUP);
  digitalWrite(encoderPin1, HIGH); //turn pullup resistor on
  digitalWrite(encoderPin2, HIGH); //turn pullup resistor on
  //call updateEncoder() when any high/low changed seen
  //on interrupt 0 (pin 2), or interrupt 1 (pin 3)
  attachInterrupt(0, updateEncoder, CHANGE);
  attachInterrupt(1, updateEncoder, CHANGE);
  TCCR1B = TCCR1B & 0b11111000 | 1; // set 31KHz PWM to prevent motor
  myPID.SetMode(AUTOMATIC); //set PID in Auto mode
  myPID.SetSampleTime(1); // refresh rate of PID controller
  myPID.SetOutputLimits(-255 , 255); // this is the MAX PWM value to move motor, here change in value reflect change in speed of motor.
}
void loop()
{
  while ( Wire.available())
  {
    byte a, b;
    a = Wire.read();
    b = Wire.read();
    bigNum = a;
    bigNum = bigNum << 8 | b;
    // a = 1001
    //   b = 0100
    //   c = a | b = 1101
  }
  REV = bigNum;
  double Sp = map(REV, 0, 180 , 0,228 );
  setpoint += Sp;
  input = encoderValue ; // data from encoder consider as a Process value
  myPID.Compute(); // calculate new output
  pwmOut(output);
  Serial.println(output);
  Serial.println(bigNum);
  //Serial.println(Sp);
}
///////////////////////////
void receiveEvent(int howMany) {
}
////////////////////
void pwmOut(int out) {
  if (out > 0) { // if REV > encoderValue motor move in forward direction.
    analogWrite(MotEnable, out); // Enabling motor enable pin to reach the desire angle
    forward(); // calling motor to move forward
  }
  else {
    analogWrite(MotEnable, abs(out)); // if REV < encoderValue motor move in forward direction.
    reverse(); // calling motor to move reverse
  }
}
void updateEncoder() {
  int MSB = digitalRead(encoderPin1); //MSB = most significant bit
  int LSB = digitalRead(encoderPin2); //LSB = least significant bit
  int encoded = (MSB << 1) | LSB; //converting the 2 pin value to single number
  int sum = (lastEncoded << 2) | encoded; //adding it to the previous encoded value
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) encoderValue ++;
  // 13 || 4 || 2 || 11
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) encoderValue --;
  //  14 || 7 || 1 || 8
  lastEncoded = encoded; //store this value for next time
}
void forward () {
  digitalWrite(MotFwd, HIGH);
  digitalWrite(MotRev, LOW);
}
void reverse () {
  digitalWrite(MotFwd, LOW);
  digitalWrite(MotRev, HIGH);
}
void finish () {
  digitalWrite(MotFwd, LOW);
  digitalWrite(MotRev, LOW);
}
