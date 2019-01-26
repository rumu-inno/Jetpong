/*
     'q' -> 噴漆短壓
     'w' -> 噴漆長壓
     'e' -> 底座噴漆 歸零

     'a' -> 底座左轉
     's' -> 底座右轉

     'r' -> 待機模式
     't' -> 準備開始
*/
#include <Servo.h>

//Rotate Sensor
const int sensorPin1 = A5;
const int sensorPin2 = A4;
int sensorValue1 = 0;
int sensorValue2 = 0;

//Servo
const int ServoPin1 = 10;                  //2, 3
const int ServoPin2 = 11;
Servo myServo1;                           //spray
Servo myServo2;                           //rotation
int ServoMaxAngle1 = 180;
int ServoMaxAngle2 = 180;

//Button
const int input01 = 2;
const int input02 = 3;

int buttonPushCounter1 = 0;
int buttonState1 = 0;
int lastButtonState1 = 0;
int buttonPushCounter2 = 0;
int buttonState2 = 0;
int lastButtonState2 = 0;

//ServoIdle
int organgle;
bool idleFlag = false;
bool forceStop = true;
int pos1 = 0;
int pos2= 0;

void setup() {
  Serial.begin(9600);
  pinMode(input01, INPUT);
  pinMode(input02, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  myServo1.attach(ServoPin1);
  myServo2.attach(ServoPin2);

  // init Servo
  myServo1.write(180);


  sensorValue2 = analogRead(sensorPin2);
  ServoMaxAngle2 = map(sensorValue2, 0, 1024, 180, 0);
  organgle = ServoMaxAngle2 / 2;

  myServo2.write(organgle);
  Serial.println("Setup");
}

void loop() {

  //  Receive
  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    Serial.print("incomingByte ");
    Serial.println(incomingByte);

    if (incomingByte == 114 ) {      // ASCII -> r
      forceStop = false;                   // idle
    }

    if (incomingByte == 116 ) {     // ASCII -> t
      forceStop = true;                   // awke up
    }

    if (incomingByte == 113)    {                    // ASCII -> q
      forceStop = true;
      Spray_short(ServoMaxAngle1);
    }

    if (incomingByte == 119)   {                     // ASCII -> w
      forceStop = true;
      Spray_long(ServoMaxAngle1);
    }

    if (incomingByte == 101)   {                      // ASCII -> e
      forceStop = true;
      Default();
    }


    if (incomingByte == 97)                         // ASCII -> a
    {
      forceStop = true;
      myServo2.write(0);
      delay(50);
    }

    if (incomingByte == 115)                         // ASCII -> s
    {
      forceStop = true;
      myServo2.write(ServoMaxAngle2);
      delay(50);
    }

    if (incomingByte == 122)                         // ASCII -> z
    {
      forceStop = true;
      myServo2.write(organgle);
      delay(10);
      myServo2.write(0);
      delay(200);
      delay(1000);
      Spray_long(ServoMaxAngle1);
    }

    if (incomingByte == 120)                         // ASCII -> x
    {
      forceStop = true;
      myServo2.write(organgle);
      delay(10);
      myServo2.write(ServoMaxAngle2);
      delay(200);
      delay(1000);
      Spray_long(ServoMaxAngle1);
    }

    Serial.flush();
  } else {
    // Read button1
    buttonState1 = digitalRead(input01);
    if (buttonState1 != lastButtonState1) {
      if (buttonState1 == HIGH)
        buttonPushCounter1++;
      delay(10);
    }
    lastButtonState1 = buttonState1;

    //  // Read button2
    buttonState2 = digitalRead(input02);
    if (buttonState2 != lastButtonState2) {
      if (buttonState2 == HIGH)
        buttonPushCounter2++;
      delay(10);
    }
    lastButtonState2 = buttonState2;

    if (buttonState1) {
      if (buttonPushCounter1 % 2 == 0 ) {
        //      Serial.println("buttonState1");
        myServo1.write(ServoMaxAngle1);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
      } else {
        myServo1.write(0);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
      }
    }

    if (buttonState2) {
      if (buttonPushCounter2 % 2 == 0 ) {
        //      Serial.println("buttonState2");
        myServo2.write(ServoMaxAngle2);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
      } else {
        myServo2.write(0);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
      }
    }

    sensorValue1 = analogRead(sensorPin1);
    ServoMaxAngle1 = map(sensorValue1, 0, 1024, 180, 0);
    //    Serial.print("RotSen1 ");
    //    Serial.println(ServoMaxAngle1);

    sensorValue2 = analogRead(sensorPin2);
    ServoMaxAngle2 = map(sensorValue2, 0, 1024, 180, 0);
    organgle = ServoMaxAngle2 / 2;
    //    Serial.print("RotSen2 ");
    //    Serial.println(ServoMaxAngle2);
    delay(20);

    //Idle State
    if (!idleFlag && !forceStop) {
      {
        idleFlag = true;
        int rand  = random(2);
        Serial.print("state ");
        Serial.println(rand);
        IdleState(rand);
      }
    }

  }

}

void Spray_short(int _max) {
  myServo1.write(0);
  delay(300);
  myServo1.write(_max);
  delay(300);
}

void Spray_long(int _max) {
  myServo1.write(0);
  delay(1500);
  myServo1.write(_max);
  delay(1000);
}

void Default() {
  myServo1.write(180);
  delay(15);
  myServo2.write(0);
  delay(15);
}

void IdleState(int state) {
  switch (state) {
    case 0:
      for (pos1 = 0; pos1 <= ServoMaxAngle2; pos1 += 3) {
        myServo2.write(pos1);
        delay(50);
      }
      for (pos1 = ServoMaxAngle2; pos1 >= 0; pos1 -= 3) {
        myServo2.write(pos1);
        delay(50);
      }
      idleFlag = false;
      break;

    case 1:
      int count = random(2, 3);
      for (int i = 0; i < count; i++)
      {
        int randvalue  = random(ServoMaxAngle2);
        int time1 = random(750, 1500);
        myServo2.write(randvalue);
        delay(10);
        delay(time1);
      }
      idleFlag = false;
      break;

    default:
      idleFlag = false;
      break;
  }
}
