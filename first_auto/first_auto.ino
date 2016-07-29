#include <NewPing.h>

//low = back, high = forward

#define TPIN_M   4 //trigger pin for middle distance sensor
#define EPIN_M  5 //return pin for middle sensor
#define TPIN_L A4 //trigger for left
#define EPIN_L A5 //return for left
#define TPIN_R A2 //trigger for right
#define EPIN_R A3 //return for right
#define rightInitPin 13 //direction initialization pin for right motor
#define rightBrakePin 8 //brake pin for right
#define leftInitPin 12 //init for left
#define leftBrakePin 9 //brake for left
#define rightPowerPin 11 //PWM control pin for right motor
#define leftPowerPin 3 //PWM control for left

String input = " "; //will hold serial input
int rightMotorPower; //will hold desired power level (0 - 255) of right motor
int leftMotorPower; //will hold desired power level (0 - 255) of left motor
int leftMotorDir; //specifies left motor direction - HIGH = Forward, LOW = Backward
int rightMotorDir; //specifies right motor direction - HIGH = Forward, LOW = Backward
//set all measured distance values to be arbitrarily high as a placeholder
int distance = 1000;
int distance_m = 1000;
int distance_l = 1000;
int distance_r = 1000;
bool aut = true; //specifies which operation mode the rover is in - true = autonomous, false = manual
//create NewPing sensor objects for controlling ultrasonic sensors
NewPing sonar_m(TPIN_M, EPIN_M, 400);
NewPing sonar_l(TPIN_L, EPIN_L, 400);
NewPing sonar_r(TPIN_R, EPIN_R, 400);




void setup() { //runs one time at Arduino startup
  Serial.begin(9600);
  //Setup Drive
  pinMode(rightInitPin, OUTPUT); //Initializes right motor init pin
  pinMode(rightBrakePin, OUTPUT); //Initializes right motor brake pin

  //Setup Steer
  pinMode(leftInitPin, OUTPUT); //Initializes Left motor init pin
  pinMode(leftBrakePin, OUTPUT);  //Initializes Left motor brake pin

  //set motor power levels to 0 by default
  analogWrite(leftPowerPin, 0);
  analogWrite(rightPowerPin, 0);

  //set desired motor power levels to 0 and directions to 0
  rightMotorPower = 0;
  leftMotorPower = 0;
  leftMotorDir = HIGH;
  rightMotorDir = HIGH;

  //seed the random number generator
  randomSeed(69);
}

void loop(){ //runs continuously while arduino is on
  //check to see if there is new serial input
  if(Serial.available()){
     input = Serial.readString();
  }

  //if a operation mode command is sent, update operation mode
  // "A" = Autonoumous
  // "M" = Manual
  if(input.indexOf("A") > -1){
    aut = true;
  }
  else if(input.indexOf("M") > -1){
    aut = false;
  }

  if(!aut){ //if rover is operating manually
    //the "<" character specifies the arduino is receiving a new power level command
    //for the left motor. Remove the "<" character, convert the remaining string to an int
    //and update the left motor power level
    if(input.indexOf("<") > -1){
      input.remove(0,1);
      leftMotorPower = input.toInt();
      input = " ";
    }
    //the ">" character specifies the arduino is receiving a new power level command
    //for the right motor. Remove the ">" character, convert the remaining string to an int
    //and update the right motor power level
    else if(input.indexOf(">") > -1){
      input.remove(0,1);
      rightMotorPower = input.toInt();
      input = " ";
    }
    //"lr" specifies that the left motor should be set to the reverse direction
    else if(input == "lr"){
      leftMotorDir = LOW;
    }
    //"lf" specifies that the left motor should be set to the forward direction
    else if(input == "lf"){
      leftMotorDir = HIGH;
    }
    //"rr" specifies that the right motor should be set to the reverse direction
    else if(input == "rr"){
      rightMotorDir = LOW;
    }
    //"rf" specifies that the right motor should be set to the forward direction
    else if(input == "rf"){
      rightMotorDir = HIGH;
    }
    //after all available input has been parsed and desired power levels and directions
    //have been updated, update the motors with the desired power and direction and delay for 10ms
    spinLeftMotor(leftMotorPower, leftMotorDir);
    spinRightMotor(rightMotorPower, rightMotorDir);
    delay(10);
  }

  else{ //the rover is in autonomous mode
    //take a distance measurement from all sensors, delaying 50 ms between each measurement to avoid
    //interference, if the distance returned is 0, the sensor did not find any obstruction within its max
    //range, distance will be set the arbitrarily high value of 400
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
    //if any distance measurement returns below 8cm, collision is unavoidable
    if(distance_r < 8 || distance_m < 8 || distance_l < 8){
      brake();
      reverse(200); //spin the motors in reverse at a PWM level of 200
      delay(700); //wait for 700 ms
      brake();
      scan(); //find and proceed in the least obstructed direction
    } else { //there are no very close objects in our immediate path
      if(distance_m < 75){ //if there are some relatively close objects
        turnTowardGreatestDistance(); //measure distance and proceed in the clearest direction
      }
      else{ //if there is nothing remotely close in the rover's path, proceed on
        forward(120);
        delay(100);
      }
    }
  }
}

//function: rotate the left motor
//RECIEVES: int power - the power with which to rotate (0 - 255), int dir - the direction in which to rotate (HIGH or LOW)
void spinLeftMotor(int power, int dir){
  digitalWrite(leftInitPin, dir); //Sets drive direction to back
  digitalWrite(leftBrakePin, LOW);   //Turn off left brake
  analogWrite(leftPowerPin, power);   //Spins the left motor at full speed
}

//function: rotate the right motor
//RECEIVES: int power - the power with which to rotate (0 - 255), int dir - the direction in which to rotate (HIGH or LOW)
void spinRightMotor(int power, int dir){
  if(power < 20){
    power = 20;
  }
  digitalWrite(rightInitPin, dir); //Sets drive direction to back
  digitalWrite(rightBrakePin, LOW);   //Turn off right brake
  analogWrite(rightPowerPin, power - 20);   //Spins the right motor at full speed
}

//function: rotate both motors forward
//RECEIVES: int power - the power with which to rotate (0 - 255)
void forward(int power){
  spinLeftMotor(power, HIGH);
  spinRightMotor(power, HIGH);
}

//function: set both motor power levels to 0 and directions to forward
void stopAll(){
  spinLeftMotor(0, HIGH);
  spinRightMotor(0, HIGH);
}

//function: rotate both motors in reverse
//RECEIVES: int power - the power with which to rotate (0 - 255)
void reverse(int power){
  spinLeftMotor(power, LOW);
  spinRightMotor(power, LOW);
}


//function: turn the rover to the right
//RECEIVES: int power - the power level with which to rotate the motor (0 - 255)
void turnRight(int power){
  spinLeftMotor(power, HIGH);
}

//function: turn the rover to the left
//RECEIVES: int power - the power level with which to rotate the motor (0 - 255)
void turnLeft(int power){
  spinRightMotor(power, HIGH);
}

//function: Stop all motor function, engage both breaks, wait for a time to allow the rover to stop,
//then disengage brakes
void brake(){
  stopAll();
  digitalWrite(leftBrakePin, HIGH);
  digitalWrite(rightBrakePin, HIGH);
  delay(250);
  digitalWrite(leftBrakePin, LOW);
  digitalWrite(rightBrakePin, LOW);
}

//function: determine the clearest overall direction and proceed in that direction
void scan(){
  int rightAverage; //will hold the mean of all of the sensor readings when the rover is turned to the right
  int leftAverage; //will hold the mean of all of the sensor readings when the rover is turned to the right
  //turn right with a power of 200 for 300 ms and stop
  turnRight(200);
  delay(300);
  brake();
  //measure all distances
  distance_m = sonar_m.ping_cm();
  delay(50);
  distance_l = sonar_l.ping_cm();
  delay(50);
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
  //calculate the average of all distances
  rightAverage = (distance_m + distance_l + distance_r)/3;

  //turn left at a power level of 200 for 300 ms and stop
  turnLeft(200);
  delay(300);
  brake();
  //measure all distances
  distance_m = sonar_m.ping_cm();
  delay(50);
  distance_l = sonar_l.ping_cm();
  delay(50);
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
  //calculate average of measurements
  leftAverage = (distance_m + distance_l + distance_r)/3;

  //proceed in the clearest direction
  if(rightAverage > leftAverage){
    turnRight(200);
    delay(600);
  }
}


void turnTowardGreatestDistance(){
  //find which direction measures the greatest distance to an obstruction
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

  //turn towards the least obstructed direction
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

  //if forward is the least obstructed direction, do nothing
}
