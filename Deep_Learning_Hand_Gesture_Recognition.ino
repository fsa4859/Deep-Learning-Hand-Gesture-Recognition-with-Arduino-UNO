#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 12);
// The following library is related to the digital Temperature Sensor
#include <DallasTemperature.h>
#include <OneWire.h>
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

//int flag = 0;
int red = 13;
int green = 11;
int white = 2;
//int yellow=8;
int data;

// the following library is for ultrasonic sensor
#include <NewPing.h>
#define TRIGGER_PIN 10
#define ECHO_PIN 8
#define MAX_DISTANCE 400 // Maximum distance is 400 Cm
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
float distance; // Distance returned from ultrasonic sensor
float updated_distance;// distance after temp correction
// the following are connections of motor to driver
// all the pins assigned are PWM to control speed.
int b1 = 3; // First connection between motor1 and driver
int b2 = 9; // Second connection between motor1 and driver
//int enable=3;
int in3 = 5; // First connection between motor 2 and driver
int in4 = 6; // second connection between motor 2 and driver
int timedelayone = 250;
//int data;




void setup() {
  // put your setup code here, to run once:
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  //pinMode(enable,OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // Low signals from the pins.
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.begin(9600);
  //MyBlue.begin(9600);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  //pinMode(yellow,OUTPUT);
  pinMode(white, OUTPUT);
  mySerial.begin(9600);
  //sensors.begin();
  delay(1000);

}

void loop() {
  //sensors.requestTemperatures();
  //int temp=sensors.getTempCByIndex(0);
  //Serial.println("Temperature is");
  //Serial.print(sensors.getTempCByIndex(0));
  delay(10000);
  // this next while loop will only function if there is no serial or bluetooth data coming from machine learning model.
  // in case of no handgestures coming, arduino will run its own obstacle avoidance robot.
  while (!Serial.available() && !mySerial.available()) {
    digitalWrite(red, HIGH);
    distance = sonar.ping_cm();
    Serial.println(distance);
    delay(300);
    while ((distance >= 50 || distance == 0) && (!Serial.available() && !mySerial.available())) {
      stopallmotors();
      delay(500);
      moveforward();
      delay(500);
      //delay(2000);
      distance = sonar.ping_cm();
      delay(300);
    }
    while ((distance < 50 && distance != 0) && (!Serial.available() && !mySerial.available())) {
      stopallmotors();
      delay(1000);
      movebackward();
      stopallmotors();
      delay(1000);
      turnright();
      distance = sonar.ping_cm();
      delay(300);
    }
  }
  // the exit of this first while loop will indicate that handgestures are being recognized by Arduino
  delay(5000);
  // new code for integrating machine learning
  data = Serial.read(); //reading serial information and storing in an int data
  // forward checking
  // 70 is ASCI Equivalent of F
  while ((Serial.available() > 0 || mySerial.available() > 0) && (Serial.read() == 70)) {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    distance = sonar.ping_cm();
    // this is the forward direction
    // integrating ultrasonic distance with machine learning
    while ((distance > 65) && (Serial.read() == 70)) {
      moveforward();
      //delay(5000);
      distance = sonar.ping_cm();
      data = Serial.read();
    }
    while ((distance < 65) && (Serial.read() == 70)) {
      stopallmotors();
      movebackward();
      turnright();
      distance = sonar.ping_cm();
      int data = Serial.read();
    }
  }
  // the exit of this loop indicates that F is no longer being sent or serial data is not available
  data = Serial.read();
  // left
  // 76 is equivalent ASCI code for L (Left sign).
  while ((Serial.available() > 0 || mySerial.available() > 0) && (Serial.read() == 76)) {
    digitalWrite(white, HIGH);
    digitalWrite(green, LOW);
    digitalWrite(red, LOW);
    distance = sonar.ping_cm();
    while ((distance > 65) && (Serial.read() == 76)) {
      turnleft();
      stopallmotors();
      distance = sonar.ping_cm();
      int data = Serial.read();
    }
    while ((distance < 65) && (Serial.read() == 76)) {
      //digitalWrite(red,HIGH);
      stopallmotors();
      movebackward();
      distance = (sonar.ping_cm()) / (4);
      int data = Serial.read();
    }
  }
  // the exit of the previous loop indicates
  // that either L is no longer being sent or data is not coming
  data = Serial.read();
  // right
  // 82 in Asci is equivalent to R (Right direction)
  while ((Serial.available() > 0 || mySerial.available() > 0) && (Serial.read() == 82)) {
    digitalWrite(green, HIGH);
    distance = sonar.ping_cm();
    while (distance > 65 && data == 82) {
      turnright();
      stopallmotors();
      distance = (sonar.ping_cm()) / (4);
      data = Serial.read();
    }
    while ((distance < 65) && (Serial.read() == 82)) {
      stopallmotors();
      movebackward();
      distance = sonar.ping_cm();
      data = Serial.read();
    }
  }

  data = Serial.read();
  // stop signal
  // 83 is equivalent ASCI in stop sign (S)
  while ((Serial.available() > 0) && (Serial.read() == 83)) {
    digitalWrite(red, HIGH);
    digitalWrite(white, LOW);
    digitalWrite(green, LOW);
    stopallmotors();
  }

}

// put your main code here, to run repeatedly:
stopallmotors();
}

}
void stopallmotors()
{
  analogWrite(b1, 0);
  analogWrite(b2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
}

void movebackward()
{
  analogWrite(b1, 255);
  analogWrite(b2, 0);
  analogWrite(in3, 255);
  analogWrite(in4, 0);

}

void moveforward()
{
  // we can select values for pwm between 0 to 255
  // this is equivalent to 8 bit analog to digital coverter
  analogWrite(b1, 0);
  analogWrite(b2, 255);
  analogWrite(in3, 0);
  analogWrite(in4, 255);
}

void turnright()
{
  analogWrite(b1, 0);
  analogWrite(b2, 255);
  analogWrite(in3, 255);
  analogWrite(in4, 0);
}

void turnleft()
{
  analogWrite(b1, 255);
  analogWrite(b2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 255);
}
