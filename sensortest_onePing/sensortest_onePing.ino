//A SIMPLE TEST TO VERIFY THAT IT IS POSSIBLE TO SEND COMMANDS OVER A SERIAL CONNECTION TO CONTROL
//AN ULTRASONIC DISTANCE SENSOR
#include <NewPing.h>

#define TPIN   12 //pin we use to send our ping command
#define EPIN   11 //pin we use to get data back from the sensor
#define BLINK_PIN 13 //this guy will blink

NewPing sonar(TPIN, EPIN, 400); // 400 cms will be our max distance

char blueToothVal;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial.available()){
    blueToothVal=Serial.read();
  }

  if(blueToothVal == 'r'){
    digitalWrite(13, HIGH);
    Serial.print("Ping: ");
    Serial.print(sonar.ping_cm());
    Serial.println("cm");
    delay(1000);
    digitalWrite(13, LOW);
    blueToothVal = 'x';
  }
}
