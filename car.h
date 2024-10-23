#pragma once

#include <Servo.h>  // includes servo header

#define ARRAY_LENGTH(x) (sizeof(x) / sizeof(x[0]))  // Determines the length of arrow
#define PING_DISTANCE(x) ((x/29)/2)

#define SERVO_CENTER_POSITION 90
#define SERVO_TURN_SPEED 10
#define SERVO_LEFT_POSITION 160
#define SERVO_RIGHT_POSITION 20
#define SERVO_MAX_LEFT_POSITION 180
#define SERVO_MAX_RIGHT_POSITION 0
#define SERVO_MAX_DELAY 650

#define SERVO_PIN 5
#define PING_ECHO 12
#define PING_TRIG 13

#define MOVE_CAR_DELAY 1
#define REVERSE_CAR_DELAY 310
#define TRIGGER_DELAY 2
#define STOP_DISTANCE 10

#define TURN_LEFT true, false
#define TURN_RIGHT false, true
#define MOVE_FORWARD true, true
#define MOVE_BACKWARDS false, false

#define BAUD_RATE 9600
#define OFF 0
#define MAX_SPEED 255
#define CAR_TURN_SPEED 150

#define SERVO_TURNING_LEFT -1
#define SERVO_TURNING_RIGHT 0
#define SERVO_TURNING_CENTER 1

#define MAP_FUNCTION_MIN_DIST 30
#define MAP_FUNCTION_MAX_DIST 100
#define MAP_FUNCTION_MIN_SPEED 100
#define MAP_FUNCTION_MAX_SPEED 200

#define LEFT_SIDE_GREATER_DISTANCE true
#define RIGHT_SIDE_GREATER_DISTANCE false

void setup();
void loop();
void moveCar();
void changeCarSpeed(int rate);
void turnWheelsOff();
void pingSensorSpin();
int findPingDistance();
void trigger();
void printPingDistance();
int changeRate(int distance);
void carDirection(int rate, int direction);
bool turnLeftOrRight();

//////////////////////////////////
//FRONT IN1/IN2 ---- Front Right Tire
//When IN1 HIGH/IN2 LOW --- Forward

//FRONT IN3/IN4 --- Front Left Tire
//When IN3 HIGH/IN4 LOW - Forward

//Back IN1 IN2 ---- Back Right Tire
//IN1 High IN2 Low --- Forward

//Back IN3/IN Back Right Tire
//When IN4 High, IN3 Low -- Forward

///// IN1/2  === RIGHT SIDE
///// IN3/4  === LEFT SIDE
///////////////////////////////////////

// pins for the h-bridge
int motorIn1Front = A5;
int motorIn2Front = A4;
int motorIn3Front = A3;
int motorIn4Front = A2;
int hBridgeEnRight = 6;
int motorIn1Back = 7;
int motorIn2Back = 8;
int motorIn3Back = 9;
int motorIn4Back = 10;
int hBridgeEnLeft = 11;

// Array for motor pins
int motorArray[] = {motorIn1Front, motorIn2Front, motorIn3Front, motorIn4Front,
                    motorIn1Back, motorIn2Back, motorIn3Back, motorIn4Back
                   };
// Array for EN pin for right and left side of tires
int hBridgeEn[] = {hBridgeEnRight, hBridgeEnLeft};


Servo myServo; // creates object myServo
// SERVO --- RED 5V, ORANGE SIG, BROWN GND

// declaration/pin values for ping sensor

int echoVal;

int ping1 = 200;
int ping2 = 200;
int ping3 = 200;
int ping4 = 200; 

// -1 = Ping Sensor Turns Left
// 0 = Turn all the way right
// 1 = Turn center
int servoDirection = SERVO_TURNING_LEFT;
int servoPosition = SERVO_CENTER_POSITION;
