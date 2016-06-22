#include <NewPing.h>

//low = back, high = forward

#define TPIN_M   4 //pin we use to send our ping command
#define EPIN_M  5 //pin we use to get data back from the sensor
#define TPIN_L A4
#define EPIN_L A5
#define TPIN_R A2
#define EPIN_R A3
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
int distance_m = 1000;
int distance_l = 1000;
int distance_r = 1000;
bool aut = false;
NewPing sonar_m(TPIN_M, EPIN_M, 400);
NewPing sonar_l(TPIN_L, EPIN_L, 400);
NewPing sonar_r(TPIN_R, EPIN_R, 400);




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
  if(Serial.available()){
     input = Serial.readString();
  }

  if(input.indexOf("A") > -1){
    aut= true;
  }
  else if(input.indexOf("M") > -1){
    aut = false;
  }

  if(!aut){
    if(input.indexOf("<") > -1){
    Serial.print("here");
    input.remove(0,1);
    leftMotorPower = input.toInt();
    input = " ";
    }
    else if(input.indexOf(">") > -1){
      input.remove(0,1);
      rightMotorPower = input.toInt();
      input = " ";
    }
    else if(input == "lr"){
      leftMotorDir = LOW;
    }
    else if(input == "lf"){
      leftMotorDir = HIGH;
    }
    else if(input == "rr"){
      rightMotorDir = LOW;
    }
    else if(input == "rf"){
      rightMotorDir = HIGH;
    }
    spinLeftMotor(leftMotorPower, leftMotorDir);
    spinRightMotor(rightMotorPower, rightMotorDir);
    delay(10);
  }

  else{
    distance_l = sonar_l.ping_cm();
    if(distance_l == 0){
      distance_l = 400;
    }
    delay(50);
    distance_m = sonar_m.ping_cm();
    if(distance_m == 0){
      distance_m = 400;
    }
    delay(50);
    distance_r = sonar_r.ping_cm();
    if(distance_r == 0){
      distance_r = 400;
    }
    delay(50);
    if(distance_r < 8 || distance_m < 8 || distance_l < 8){
      brake();
      reverse(200);
      delay(700);
      brake();
      scan();
    } else {
      if(distance_m < 75){
        turnTowardGreatestDistance();
      }
      else{
        forward(120);
        delay(100);
      }
    }
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
  int rightAverage;
  int leftAverage;
  turnRight(200);
  delay(300);
  brake();
  distance_m = sonar_m.ping_cm();
  distance_l = sonar_l.ping_cm();
  distance_r = sonar_r.ping_cm();
  if(distance_r == 0){
    distance_r = 400;
  }
  if(distance_m == 0){
    distance_m = 400;
  }
  if(distance_l == 0){
    distance_l = 400;
  }
  rightAverage = (distance_m + distance_l + distance_r)/3;
  
  turnLeft(200);
  delay(300);
  brake();
  distance_m = sonar_m.ping_cm();
  distance_l = sonar_l.ping_cm();
  distance_r = sonar_r.ping_cm();
  if(distance_r == 0){
    distance_r = 400;
  }
  if(distance_m == 0){
    distance_m = 400;
  }
  if(distance_l == 0){
    distance_l = 400;
  }
  leftAverage = (distance_m + distance_l + distance_r)/3;

  if(rightAverage > leftAverage){
    turnRight(200);
    delay(600);
  }
}


void turnTowardGreatestDistance(){
  int large = 0;
  String largestDist = " ";
  if(distance_m > large){
    large = distance_m;
    largestDist = "m";
  }
  if(distance_r > large){
    large = distance_r;
    largestDist = "r";
  }
  if(distance_l > large){
    large = distance_l;
    largestDist = "l";
  }

  if(largestDist.equals("l")){
    stopAll();
    turnLeft(200);
    delay(100);
    stopAll();
  }
  else if(largestDist.equals("r")){
    stopAll();
    turnRight(200);
    delay(100);
    stopAll();
  }
}

