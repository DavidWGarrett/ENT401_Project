// David Garrett
// ENT401 PROJECT

// Goal: Car drives forward and avoids obstacles
// What the car does: Drives forward. Stops when gets too close to object. Reverse/Turn. Drives forward and repeats cycle.
// Arduino Enables two h-bridges which drives two motors/tires each. Also chooses if tires spin forward, backwards, or stop
// Ping Sensor measures distance in front of car. 
// Servo motor turns the ping sensor. Determines if objects are close on left and right side of car

#include "car.h"


void school_code();

void setup() {

  Serial.begin(38400);
  Wire.begin();

  // set pinmode to output for all the motor pins
  for (int i = 0; i < ARRAY_LENGTH(motorArray); i++) {
    pinMode(motorArray[i], OUTPUT);
  }

  myServo.attach(SERVO_PIN); // servo pin
  myServo.write(SERVO_CENTER_POSITION);  // centers servo

  // Ping Sensor Pins
  pinMode(PING_TRIG, OUTPUT);
  pinMode(PING_ECHO, INPUT);

  pinMode(CONFIRM_BUTTON, INPUT_PULLUP);  // Set pin 4 as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(CONFIRM_BUTTON), isUpright, FALLING);  // Attach interrupt

  moveCar(MAX_SPEED, MOVE_FORWARD, MOVE_CAR_DELAY); // makes the motors work properly, needs to be high speed before going sloW

  calibrateSensor();

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED_BUZZER, OUTPUT);

  delay(1000);
  switchLED(GREEN);
  delay(10000);
}

void loop() {
  moveCar(MAX_SPEED, MOVE_FORWARD, MOVE_CAR_DELAY);
  pingSensorSpin(); // spins servo
  int distance = findPingDistance();
  carDirection(changeRate(distance), distance);
  //school_code();
  
  accelerometer.getAcceleration(&ax, &ay, &az);
  fallenOver = checkFalling();
  Serial.print(fallenOver);
  Serial.print("< fallen over\n4\n");
  if (fallenOver) { Serial.print("5\n"); setAlarm(); }

  //int buttonState = 0;

  //buttonState = digitalRead(CONFIRM_BUTTON);  // Read the state of the button

  //if (buttonState == LOW) {  // Button is pressed (because of pull-up, LOW means pressed)
   // Serial.println("Button pressed");
  //} else {  // Button is not pressed
    //Serial.println("Button not pressed");
  //}

  delay(100);  // Small delay to debounce the button
} 

void moveCar(int rate, bool leftTireForward, bool rightTireForward, int timeDelay)
{
    for (int i = 0; i < ARRAY_LENGTH(hBridgeEn); i++) {
    digitalWrite(hBridgeEn[i], LOW);
  }

  for (int i = 0; i < ARRAY_LENGTH(motorArray); i = i + 4) { 
    digitalWrite(motorArray[i], leftTireForward);
    digitalWrite(motorArray[i + 1], !leftTireForward);
    digitalWrite(motorArray[i + 2], rightTireForward);
    digitalWrite(motorArray[i + 3], !rightTireForward);
  }

  changeCarSpeed(rate);

  delay(timeDelay);
}

void changeCarSpeed(int rate)
{
  for (int i = 0; i < ARRAY_LENGTH(hBridgeEn); i++) {
    analogWrite(hBridgeEn[i], rate);
  }
}

void turnWheelsOff() { // Shuts off all the tires

  Serial.print("9\n");
  for (int i = 0; i < ARRAY_LENGTH(hBridgeEn); i++) {
    digitalWrite(hBridgeEn[i], LOW);
  }
  for (int i = 0; i < ARRAY_LENGTH(motorArray); i++) {
    digitalWrite(motorArray[i], LOW);
  }
  for (int i = 0; i < ARRAY_LENGTH(hBridgeEn); i++) {
    analogWrite(hBridgeEn[i], OFF);
  }
  delay(MOVE_CAR_DELAY);
}

void pingSensorSpin() {

  // -1 = Ping Sensor Turns Left
  // 0 = Turn all the way right
  // 1 = Turn center 
 
  // While loops continues till servo returns to original position
  // Ping sensors spins all the way left, all the way right, and back to center

    myServo.write(servoPosition); // turn servo

    if (servoDirection == SERVO_TURNING_LEFT) {
        servoPosition += SERVO_TURN_SPEED;
        Serial.print("Servo SPIN LEFT\n");
        if ( servoPosition == SERVO_LEFT_POSITION ) { servoDirection = SERVO_TURNING_RIGHT; } }
  
    else if (servoDirection == SERVO_TURNING_RIGHT) {
        servoPosition -= SERVO_TURN_SPEED;
        Serial.print("Servo SPIN RIGHT");
        if ( servoPosition == SERVO_RIGHT_POSITION ) { servoDirection = SERVO_TURNING_CENTER; } }
  
    else if (servoDirection == SERVO_TURNING_CENTER) { 
        servoPosition += SERVO_TURN_SPEED; 
        if ( servoPosition == SERVO_CENTER_POSITION ) { servoDirection = SERVO_TURNING_LEFT; } }

    Serial.print("Servo Position: ");
    Serial.print(servoPosition);
    Serial.print("\n");
}

int findPingDistance() { // function converts value from ping sensor to distance in centimeter
  long pingTime;
  trigger(); // triggers ping
  pingTime = pulseIn(PING_ECHO, HIGH);  // value from the ping

  ping4 = ping3;
  ping3 = ping2;
  ping2 = ping1;
  ping1 = PING_DISTANCE(pingTime);  // returns value apprx in centermeter
  // 343 m/s and or 34.3 cm/ms -- speed of sound
  // ping sensor measures echo, time for echo return equals the amount of distance

  Serial.print("Ping1: ");
  Serial.print(ping1);
  Serial.print(", Ping2: ");
  Serial.print(ping2);
  Serial.print(", Ping3: ");
  Serial.print(ping3);
  Serial.print(", Ping4: ");
  Serial.println(ping4);
  
  // Returns the largest distance from four consecutive ping values
  // Corrects for false positives where ping sensor randomly outputs a value of 0
  if ( ping1 > ping2 && ping1 > ping1 && ping3 && ping1 > ping4 ) { return ping1; } 
  else if ( ping2 > ping3 && ping2 > ping4) { return ping2; } 
  else if ( ping3 > ping4 ) { return ping3; } 
  else { return ping4; }
}

void trigger() {  // triggers the ping sensor
  digitalWrite(PING_TRIG, LOW);
  delayMicroseconds(TRIGGER_DELAY);
  digitalWrite(PING_TRIG, HIGH);
  delayMicroseconds(TRIGGER_DELAY);
  digitalWrite(PING_TRIG, LOW);
}

void printPingDistance() {  // Function prints out the distance of ping sensor
  int distance = findPingDistance();
  Serial.print("The Distance is ");
  Serial.print(distance);
  Serial.print(" centimeters.\n");
}

int changeRate(int distance) {
  int cmDist = distance; // figures out the distance in centimeter
  Serial.print("Distance in centimeter = ");
  Serial.println(cmDist);
  int distanceForMapFunction = cmDist; // new value for map function
  if ( distanceForMapFunction > MAP_FUNCTION_MAX_DIST ) { // map function doesn't work without this if statement
    distanceForMapFunction = MAP_FUNCTION_MAX_DIST;       // maximum value is 400. Car won't run if value goes above 400 with map function
  }

  int rate = map(distanceForMapFunction,
             MAP_FUNCTION_MIN_DIST,
             MAP_FUNCTION_MAX_DIST,
             MAP_FUNCTION_MIN_SPEED,
             MAP_FUNCTION_MAX_SPEED);

  // 30cm = only 75 speed, 400cm = 200 speed

  Serial.print("Rate: ");
  Serial.print(rate);
  Serial.print("\n");

  return rate;
}

void carDirection (int rate, int distance) {

  // if distance greater than 40cm, go forward  
  if ( distance > STOP_DISTANCE ) { moveCar(rate, MOVE_FORWARD, MOVE_CAR_DELAY); } 

  // below 40 cm  
  else { 
    moveCar(MAX_SPEED, MOVE_BACKWARDS, REVERSE_CAR_DELAY);
    bool moveRight = turnLeftOrRight();

    Serial.print("moveRight: ");
    Serial.print(moveRight);
    Serial.print("\n\n\n\n\n\n");

    // if left, turn right. If right, turn left
    if (moveRight) { moveCar(CAR_TURN_SPEED, TURN_RIGHT, MOVE_CAR_DELAY); } 
    else { moveCar(CAR_TURN_SPEED, TURN_LEFT, MOVE_CAR_DELAY); }

    delay(1000);

    moveCar(rate, MOVE_FORWARD, MOVE_CAR_DELAY); 
  }
}

bool turnLeftOrRight() {
  turnWheelsOff(); // makes the car stop moving
  int distanceLeft, distanceRight;

  //////////////////////
  // Figures out which way to turn.
  // measure the ping on left and right side of car

  myServo.write(SERVO_MAX_LEFT_POSITION);
  delay(SERVO_MAX_DELAY);
  for (int i = 0; i < 3; i++) { distanceLeft = findPingDistance(); }

  myServo.write(SERVO_MAX_RIGHT_POSITION);
  delay(SERVO_MAX_DELAY);
  for (int i = 0; i < 3; i++) { distanceRight = findPingDistance(); }

  Serial.print("DistLeft: ");
  Serial.print(distanceLeft);
  Serial.print("DistRight: ");
  Serial.print(distanceRight);
  Serial.print("\n\n\n\n\n");

  ///////////////////

  myServo.write(SERVO_CENTER_POSITION);

  // highest ping on both right and left side gets compared
  // based on the highest ping, it'll either turn left or right
  
  if ( distanceLeft > distanceRight ) { return LEFT_SIDE_GREATER_DISTANCE; } 
  else { return RIGHT_SIDE_GREATER_DISTANCE; }
}

void school_code() 
{

// read raw accel/gyro measurements from device
//accelerometer.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
// these methods (and a few others) are also available
accelerometer.getAcceleration(&ax, &ay, &az);
//accelgyro.getRotation(&gx, &gy, &gz);
// display tab-separated accel/gyro x/y/z values
Serial.print("ax: ");
Serial.print(ax-axOffset); Serial.print("\tay: ");
Serial.print(ay-ayOffset); Serial.print("\taz: ");
Serial.print(az-azOffset+16384); Serial.print("\n");
Serial.print(az); 
Serial.print("\t"); 
Serial.print(ax); 
Serial.print("\n");
delay(100);
}

void calibrateSensor() {
  int32_t axSum = 0, aySum = 0, azSum = 0;
  int32_t axMax = 0, ayMax = 0, azMax = 0;
  int32_t axMin = 0, ayMin = 0, azMin = 0; 

  for (int i = 0; i < 100; i++) {
    // Get acceleration values
    accelerometer.getAcceleration(&ax, &ay, &az);

    // Update sum for averaging
    axSum += ax;
    aySum += ay;
    azSum += az;

    // Update max and min values for each axis
    if (ax > axMax) axMax = ax;
    if (ax < axMin) axMin = ax;

    if (ay > ayMax) ayMax = ay;
    if (ay < ayMin) ayMin = ay;

    if (az > azMax) azMax = az;
    if (az < azMin) azMin = az;

    // Short delay to allow stable readings
    delay(50);
  }

  Serial.print(axSum);

  // Calculate average values
  axOffset = axSum / 100;
  ayOffset = aySum / 100;
  azOffset = azSum / 100;

  Serial.print(axOffset);
}

bool checkFalling() {
  // Check if ay or ax exceeds 10000, if so, stop wheels and servo/ping sensor
  Serial.print("1\n");
      Serial.print(ax-axOffset);
    Serial.print("\n");
    Serial.print(ay-ayOffset);
    Serial.print("\n");
    Serial.print(az-azOffset);
    Serial.print("\n");
    Serial.print(az-azOffset-16384);
    Serial.print("\n");
  if ((abs(ax-axOffset) < 10000) && (abs(ay-ayOffset) < 10000) && (abs(az-azOffset) < 5000)) {
    return false;
  } 
  else  { Serial.print("3\n"); return true; }
}

void setAlarm() {
  while (fallenOver) {
    Serial.print("6\n");
    delay(10);
    turnWheelsOff();  // Stop the wheels
    delay(10);
    Serial.print("8\n");
    switchLED(RED);
    Serial.print("7\n");
    delay(10);  // Pause briefly to let the servo move to center
    Serial.print("setAlarm");
  }
}

void isUpright() {
  fallenOver = false;  // Set fallOver to false when the button is pushed
  switchLED(GREEN);
}

void switchLED(bool light) {
  if (light == GREEN)
  {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED_BUZZER, HIGH);
  }

  else if (light == RED)
  {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED_BUZZER, LOW);
  }
}
