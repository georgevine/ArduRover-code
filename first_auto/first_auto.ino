#include <NewPing.h>

//low = back, high = forward

#define TPIN   4 //pin we use to send our ping command
#define EPIN   5 //pin we use to get data back from the sensor
#define rightInitPin 13
#define rightBrakePin 8
#define leftInitPin 12
#define leftBrakePin 9
#define rightPowerPin 11
#define leftPowerPin 3
#define rightPowerFlag ">"
#define leftPowerFlag "<"

String input = " ";
int rightMotorPower;
int leftMotorPower;
int leftMotorDir;
int rightMotorDir;
int distance = 1000;

NewPing sonar(TPIN, EPIN, 400);



void setup() {
  Serial.begin(9600);
  //Setup Drive
  pinMode(rightInitPin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(rightBrakePin, OUTPUT); //Initiates Brake Channel A pin

  //Setup Steer
  pinMode(leftInitPin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(leftBrakePin, OUTPUT);  //Initiates Brake Channel A pin

  analogWrite(leftPowerPin, 0);
  analogWrite(rightPowerPin, 0);

  rightMotorPower = 0;
  leftMotorPower = 0;
  leftMotorDir = HIGH;
  rightMotorDir = HIGH;

  randomSeed(69);
}

void loop(){
  distance = sonar.ping_cm();
  if(distance < 30){
    brake();
    back(200);
    delay(500);
    scan();
  }
  else if(distance < 75){
    stopAll();
    int rand = (int)(random(2));
    if(rand == 1){
      stopAll();
      turnRight(100);
      delay(random(150,400));
      stopAll();
    }
    else{
      stopAll();
      turnLeft(100);
      delay(random(250,400));
      stopAll();
    }
  }
  else{
    forward(200);
  }

  
  
}


void spinLeftMotor(int power, int dir){
  digitalWrite(leftInitPin, dir); //Sets drive direction to back
  digitalWrite(leftBrakePin, LOW);   //Turn off left brake
  analogWrite(leftPowerPin, power);   //Spins the left motor at full speed
}

void spinRightMotor(int power, int dir){
  if(power < 20){
    power = 20;
  }
  digitalWrite(rightInitPin, dir); //Sets drive direction to back
  digitalWrite(rightBrakePin, LOW);   //Turn off right brake
  analogWrite(rightPowerPin, power - 20);   //Spins the right motor at full speed
}

void forward(int power){
  spinLeftMotor(power, HIGH);
  spinRightMotor(power, HIGH);
}

void back(int power){
  spinLeftMotor(power, LOW);
  spinRightMotor(power, LOW);
}

void stopAll(){
  spinLeftMotor(0, HIGH);
  spinRightMotor(0, HIGH);
}

void reverse(int power){
  spinLeftMotor(power, LOW);
  spinRightMotor(power, LOW);
}

void turnRight(int power){
  spinLeftMotor(power, HIGH);
}

void turnLeft(int power){
  spinRightMotor(power, HIGH);
}

void brake(){
  stopAll();
  digitalWrite(leftBrakePin, HIGH);
  digitalWrite(rightBrakePin, HIGH);
  delay(250);
  digitalWrite(leftBrakePin, HIGH);
  digitalWrite(rightBrakePin, HIGH);
}

void scan(){
  int rightDistance;
  int leftDistance;
  turnRight(200);
  delay(700);
  brake();
  rightDistance = sonar.ping_cm();
  turnLeft(200);
  delay(700);
  brake();
  leftDistance = sonar.ping_cm();

  if(rightDistance > leftDistance){
    turnRight(1400);
    delay(200);
  }
}

