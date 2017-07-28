#pragma config(Sensor, in5,    AccelerometerX, sensorAccelerometer)
#pragma config(Sensor, in6,    AccelerometerY, sensorAccelerometer)
#pragma config(Sensor, in7,    AccelerometerZ, sensorAccelerometer)
#pragma config(Sensor, in8,    Gyroscope,      sensorGyro) //counter clockwise +, clockwise -
#pragma config(Sensor, dgtl1,  rightEncode,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncode,     sensorQuadEncoder)
#pragma config(Sensor, dgtl5,  frontSonar,     sensorSONAR_inch)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           backRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           frontLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           backLeft,      tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//(c) 2017 David Meenan and Chris Barlas

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

//Setup variables, floats allow for decimals

float straight = 0;
float gyroRAW = 0;
float gyroCalibrated = 0;
float gyroSimple = 0;
float X2 = 0, Y1 = 0, X1 = 0, threshold = 15;
float gyroInc = 0;
float M_PI = 3.14159265359; //pi
float wheelSize = 4; //in Inches, mec
float accelerometerDistanceX = 0;
float accelerometerDistanceY = 0;
float accelerometerVelocityX = 0;
float accelerometerVelocityY = 0;
float accelerometerBiasX = 0;
float accelerometerBiasY = 0;
float trackDistance = 0;
float trackTheta = 0;
float trackPositionX = 0;
float trackPositionY = 0;
float WHEEL_BASE = 0; //change this
float locationInArrayX = 0;
float locationInArrayY = 0;
int i = 0;

//temp
int fieldArray[24][24] = {  // 0 = nothing. 1 = cone, 2 = mobile goal, 3 == stationary goal, 4 == scoring 24/6 (24 x 24, 6in padding) cone is almost 6in (5.68in), stationary goal = ~8in
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
};

void accessFieldArray(int x, int y) {
}

void updateLocationInArray(int x, int y) {
	if((((locationInArrayX + 3) >= x) || ((locationInArrayX - 3) <= x)) && ((((locationInArrayY + 3) >= Y) || ((locationInArrayY - 3) <= Y)) { //checks if X and Y of robot is in current square, > < might be backwards
		//dont update?
	} else { //if it isnt, update to the next square
		if(((locationInArrayY + 3) >= Y) || ((locationInArrayY - 3) <= Y)) {

		} else if(((locationInArrayX + 3) >= X) || ((locationInArrayX - 3) <= X)) {

		} else { //error?

		}
	}
}

//functions

void stopRobot() {
	motor[frontRight] = 0;
	motor[backRight] =  0;
	motor[frontLeft] = 0;
	motor[backLeft] =  0;
}

void updateGyro() {
	gyroRAW = SensorValue[Gyroscope]; //in8. Sets gyroRAW to raw* gyroscope data
	gyroCalibrated = (gyroRAW - gyroInc) * -1; //adds calibration to raw* gyroscope data and flips the data so clockwise is positive instead of counterclockwise
	gyroSimple = gyroCalibrated / 10; //converts calibrated raw* gyroscope data (eg: 900) to a degree (eg: 90)
	if (gyroSimple > 180) { //ensures that the gyroscope data is in a easily managable format clockwise* 0->180 and -180->0
		gyroSimple = gyroSimple - 360;
		} else if (gyroSimple < -180) {
		gyroSimple = gyroSimple + 360;
	}
}

void updateStraight() { //sets straight variable to gyro info
	if(X2 > threshold) { //makes sure that robot is not set to turn
		straight = gyroSimple; //sets straight to gyro information
	}
}

void driveGyroCorrection() { //ensures that the robot is going as streight as currently possible
	if(abs(vexRT[Ch1]) < threshold) { // checks to see if the joy has been not been moved beond the threshold (to ensure it continues to work when the joy is stuck close to 0)
		if(gyroSimple > straight) { // checks if the robot is heading straight (right)
			X2 = -25; //helps correct error by turning slightly
			} else if (gyroSimple < straight) { // checks if the robot is heading straight (left)
			X2 = 25; //helps correct error by turning slightly
		}
		} else {  // if joy has been moved
		// other stuff
	}
}

void measureDistanceAccelerometer() { //measures distance using accelerometer
	accelerometerVelocityX = (SensorValue[AccelerometerX] - accelerometerBiasX) * 0.001;
	accelerometerVelocityY = (SensorValue[AccelerometerY] - accelerometerBiasY) * 0.001;
	accelerometerDistanceX += accelerometerVelocityX * 0.001;
	accelerometerDistanceY += accelerometerVelocityY * 0.001;
}

void trackMovement() {
	trackDistance = (SensorValue[leftEncode] + SensorValue[rightEncode]) / 2.0; //average the encoder values
	trackTheta = (SensorValue[leftEncode] - SensorValue[rightEncode]) / WHEEL_BASE; //calculate angle from starting point
	trackPositionX = trackDistance * sin(trackTheta); //find x value using trig (cos(theta) = xPos/d) == (xPos = cos(theta)*d)
	trackPositionY = trackDistance * cos(trackTheta); //find y value using trig (sin(theta)=yPos/d) == (yPos=sin(theta)*d)
}
void checkLocationNR() { //no return
	trackMovement();
	measureDistanceAccelerometer();
	updateGyro();
	updateStraight();
}


float checkLocationX() { //returns X
	trackMovement();
	measureDistanceAccelerometer();
	updateGyro();
	updateStraight();
	return trackPositionX;
}

float checkLocationY() { //returns Y
	trackMovement();
	measureDistanceAccelerometer();
	updateGyro();
	updateStraight();
	return trackPositionY;
}

float checkLocationZ() { //returns Z
	trackMovement();
	measureDistanceAccelerometer();
	updateGyro();
	updateStraight();
	return gyroSimple;
}

void goToPoint(float x, float y, float z) {//note: z == rotation, might not work
	if(x > checkLocationX()) { //check
		X1 = 127;
		} else if(x < checkLocationX()) {
		X1 = -127;
		} else {
		X1 = 0;
	}
	if(y > checkLocationY()) { //check
		Y1 = 127;
		} else if(y < checkLocationY()) {
		Y1 = -127;
		} else {
		Y1 = 0;
	}
	if(z > checkLocationZ()) { //check
		X2 = 70;
		} else if (z < checkLocationZ()) {
		X2 = -70;
		} else {
		X2 = 0;
	}
	motor[frontRight] = Y1 - X2 - X1;
	motor[backRight] =  Y1 - X2 + X1;
	motor[frontLeft] = Y1 + X2 + X1;
	motor[backLeft] =  Y1 + X2 - X1;


}

void moveToRotation(float rot) { // rotates to selected rotation. -180 <-> 180 (eg: 43 or -43)
	stopRobot(); //stops robot
	Y1 = 0; //Y1, X1, and X2 set to 0 to ensure that no previous movements will continue
	X1 = 0;
	X2 = 0;
	while (rot != gyroSimple) { //while robot isnt facing the correct direction
		updateGyro(); //updates information with new gyroscope data
		if (rot > gyroSimple) { //checks if the robot is heading straight (right)
			X2 = -25; //starts turning in wanted direction (maybe make turn faster if too slow)
			} else if (rot < gyroSimple) { //checks if the robot is heading straight (left)
			X2 = 25; //starts turning in wanted direction (maybe make turn faster if too slow)
			} else { //if correct don't move
			X2 = 0; //stops turning
		}
		motor[frontRight] = Y1 - X2 - X1; //makes robot turn/ move
		motor[backRight] =  Y1 - X2 + X1;
		motor[frontLeft] = Y1 + X2 + X1;
		motor[backLeft] =  Y1 + X2 - X1;
	}
	stopRobot(); //stops robot
}


void pre_auton() //runs before robot is ready
{
	// Set bStopTasksBetweenModes to false if you want to keep user created tasks
	// running between Autonomous and Driver controlled modes. You will need to
	// manage all user created tasks if set to false.
	bStopTasksBetweenModes = true;
	bDisplayCompetitionStatusOnLcd = false;

	stopRobot(); //stops robot
	bLCDBacklight = true;
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDCenteredString(0, "Prepping...");
	SensorType[in8] = sensorNone; //clears sensor port8
	wait1Msec(1000); //gives time to clear
	displayLCDCenteredString(1, "Gyro Engaging...");
	SensorType[in8] = sensorGyro; //sets sensor port8 to gyro (rotation along/around y axis)
	wait1Msec(2000); //gives time for gyro to init
	SensorScale[in8] = 0; //change this
	SensorFullCount[in8] = 0; //change this

	displayLCDCenteredString(0, "Calibrating...");
	for(i=0;i<1024;i++) {
		gyroInc = gyroInc + SensorValue[in8];
		wait1Msec(1);
	}
	for(i=0;i<1024;i++) {
		accelerometerBiasX = accelerometerBiasX + SensorValue[AccelerometerX];
		wait1Msec(1);
	}
	for(i=0;i<1024;i++) {
		accelerometerBiasY = accelerometerBiasY + SensorValue[AccelerometerY];
		wait1Msec(1);
	}
	clearLCDLine(1);
	displayLCDCenteredString(0, "Waiting for Task...");
}


task autonomous() //program the robot to do stuff
{
	displayLCDCenteredString(0, "Autonomous"); //messages are fun :)
	//while(true) { // might be needed, might not
	//moveTo(45, 10, 0); //testing code here and below
	wait1Msec(2000);
	//moveTo(-45, -10, 2);
	wait1Msec(2000);
	//moveTo(180, 15, 1); //testing code here and above
	stopRobot(); //stops robot
	//}
}


task usercontrol() //drive the robot using a controller
{
	stopRobot(); //stops robot
	displayLCDCenteredString(0, "Driver Control");
	while(true) { //run code multiple times
		updateGyro();
		//Create "deadzone" for Y1/Ch3
		if(abs(vexRT[Ch3]) > threshold) {
			Y1 = vexRT[Ch3];
			} else {
			Y1 = 0;
		}
		//Create "deadzone" for X1/Ch4
		if(abs(vexRT[Ch4]) > threshold) {
			X1 = vexRT[Ch4];
			} else {
			X1 = 0;
		}
		//Create "deadzone" for X2/Ch1
		if(abs(vexRT[Ch1]) > threshold) {
			X2 = vexRT[Ch1];
			} else {
			X2 = 0;
		}
		driveGyroCorrection();
		//Remote Control Commands
		motor[frontRight] = Y1 - X2 - X1;
		motor[backRight] =  Y1 - X2 + X1;
		motor[frontLeft] = Y1 + X2 + X1;
		motor[backLeft] =  Y1 + X2 - X1;
		//logRotation();
		//logGyro();

		updateStraight(); //check location, might have to be moved
	}
	stopRobot(); //stops robot
}
