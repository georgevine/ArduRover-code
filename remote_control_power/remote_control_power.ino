//A SIMPLE TEST TO VERIFY THAT IT IS POSSIBLE TO CONTROL THE MOTORS USING COMMANDS
//SENT OVER A SERIAL CONNECTION


//low = back, high = forward


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



void setup() {
  Serial.begin(9600);
  //Setup Drive
  pinMode(rightInitPin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(rightBrakePin, OUTPUT); //Initiates Brake Channel A pin

  //Setup Steer
  pinMode(leftInitPin, OUTPUT); //Initiates Motor Channel A pin
  pinMode(leftBrakePin, OUTPUT);  //Initiates Brake Channel A pin

  analogWrite(leftPowerPin, LOW);
  analogWrite(rightPowerPin, LOW);

  rightMotorPower = 0;
  leftMotorPower = 0;
  leftMotorDir = HIGH;
  rightMotorDir = HIGH;
}

void loop(){

  if(Serial.available()){
     input = Serial.readString();
  }
  Serial.print(input);

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
  //delay(10);
}


void spinLeftMotor(int power, int dir){
  digitalWrite(leftInitPin, dir); //Sets drive direction to back
  digitalWrite(leftBrakePin, LOW);   //Turn off left brake
  analogWrite(leftPowerPin, power);   //Spins the left motor at full speed
}

void spinRightMotor(int power, int dir){
  digitalWrite(rightInitPin, dir); //Sets drive direction to back
  digitalWrite(rightBrakePin, LOW);   //Turn off right brake
  analogWrite(rightPowerPin, power);   //Spins the right motor at full speed
}
