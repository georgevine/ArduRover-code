//low = forward, high = backword on drive motor, i don't know
//low = right, high = left

#define driveInitPin 12
#define driveBrakePin 9
#define steerInitPin 13
#define steerBrakePin 8
#define drivePowerPin 3
#define steerPowerPin 11
#define forwardCommand 'w'
#define backwardCommand 's'
#define leftCommand 'a'
#define rightCommand 'd'
#define brakeCommand ' '
char input = ' ';
bool turnLeft = false;
bool turnRight = false;


void setup() {
  Serial.begin(9600);
  //Setup Drive
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin

  //Setup Steer
  pinMode(13, OUTPUT); //Initiates Motor Channel A pin
  pinMode(8, OUTPUT);  //Initiates Brake Channel A pin

  analogWrite(drivePowerPin, LOW);
  analogWrite(steerPowerPin, LOW);
}

void loop(){
  if(Serial.available()){
     input = (char)Serial.read();  
  }
  else{
    input = brakeCommand;
  }

  if(input == brakeCommand){
    brake();
  }
  if(input == forwardCommand){
    forward();
  }
  if(input == backwardCommand){
    back();
  }
  if(input == leftCommand){
    turnLeft = true;
    turnRight = false;
  }
  if(input == rightCommand){
    turnRight = true;
    turnLeft = false;
  }
  if(input == 'x'){
    turnRight = false;
    turnLeft = false;
  }


  if(turnLeft==true){
    left();
  }
  else if(turnRight==true){
    right();
  }
  else{
    centerWheels();
  }
}

void back(){
  digitalWrite(driveInitPin, HIGH); //Sets drive direction to back
  digitalWrite(driveBrakePin, LOW);   //Turn off drive brake
  analogWrite(drivePowerPin, 255);   //Spins the drive motor at full speed
  delay(500);
}

void forward(){
  digitalWrite(driveInitPin, LOW); //Sets drive direction to forward
  digitalWrite(driveBrakePin, LOW);   
  analogWrite(drivePowerPin, 255);
  delay(500);  
}

void brake(){
  digitalWrite(driveBrakePin, HIGH);
}

void right(){
  digitalWrite(steerInitPin, LOW); //Sets steer direction to right
  digitalWrite(steerBrakePin, LOW);   
  analogWrite(steerPowerPin, 255);  
}

void left(){
  digitalWrite(steerInitPin, HIGH); //Sets sets steer direction to left
  digitalWrite(steerBrakePin, LOW);   
  analogWrite(steerPowerPin, 255);  
}

void centerWheels(){
  analogWrite(steerPowerPin, 0); 
}

