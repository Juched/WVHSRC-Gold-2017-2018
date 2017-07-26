#pragma config(Sensor, in5,    AccelerometerX, sensorAccelerometer)
#pragma config(Sensor, in6,    AccelerometerY, sensorAccelerometer)
#pragma config(Sensor, in7,    AccelerometerZ, sensorAccelerometer)
#pragma config(Sensor, in8,    Gyroscope,      sensorGyro)
#pragma config(Sensor, dgtl1,  rightEncode,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftEncode,     sensorQuadEncoder)
#pragma config(Motor,  port2,           frontRight,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           backRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           frontLeft,     tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           backLeft,      tmotorVex393_MC29, openLoop, reversed)

//(c) 2017 David Meenan

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
float wheelRotation = 0;
float encodeRotations = 0;
float X2 = 0, Y1 = 0, X1 = 0, threshold = 15;
float gyroInc = 0;
int i = 0;

//functions

void stopRobot() {
	motor[frontRight] = 0;
	motor[backRight] =  0;
	motor[frontLeft] = 0;
	motor[backLeft] =  0;
}

void updateGyro() {
	gyroRAW = SensorValue[Gyroscope]; //in8. Sets gyroRAW to raw* gyroscope data
	gyroCalibrated = gyroRAW - gyroInc; //adds calibration to raw* gyroscope data
	gyroSimple = gyroCalibrated / 10; //converts calibrated raw* gyroscope data (eg: 900) to a degree (eg: 90)
	if (gyroSimple > 180) { //ensures that the gyroscope data is in a easily managable format clockwise* 0->180 and -180->0
		gyroSimple = gyroSimple - 360;
		} else if (gyroSimple < -180) {
		gyroSimple = gyroSimple + 360;
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

void updateStraight() { //sets straight variable to gyro info
	if(X2 > threshold) { //makes sure that robot is not set to turn
		straight = gyroSimple; //sets straight to gyro information
	}
}


void moveToWheelRotation(float wrot) { //multiplied by 360 so each (1) rotation would be a full wheel rotation, not 1 drgree
	stopRobot(); //stops robot
	X2 = 0; //clears out any old driving information
	Y1 = 0;
	X1 = 0;
	wheelRotation = 0; //clears out old wheel rotations
	encodeRotations = 0; //clears out old encode rotations
	while(encodeRotations <= wrot*360) { //while full turns are greater than the encoded rotation, if = X1 will be set to 0
		updateGyro(); //gets new information from gyroscope
		//finds average/median wheel rotations
		if (SensorValue[leftEncode] > SensorValue[rightEncode]) {
			wheelRotation = SensorValue[leftEncode] - SensorValue[rightEncode];
			wheelRotation = SensorValue[rightEncode] + wheelRotation;
			} else if (SensorValue[leftEncode] < SensorValue[rightEncode]) {
			wheelRotation = SensorValue[rightEncode] - SensorValue[leftEncode];
			wheelRotation = SensorValue[leftEncode] + wheelRotation;
			} else {
			wheelRotation = SensorValue[rightEncode];
		}
		encodeRotations = encodeRotations + wheelRotation; //keeps count of all roations
		if(wrot*360 > 0) { //makes sure the robot turns the correct way
			X1 = 127;
			} else if(wrot*360 < 0){
			X1 = -127;
			} else {
			X1 = 0;
			encodeRotations = wrot*360 + 1; //ensures that while loop will stop correctly
		}
		if(wrot*360 == encodeRotations) { //checks if they are the same
			encodeRotations = wrot*360 + 1; //ensures that while loop will stop correctly
		}
		driveGyroCorrection(); //helps robot to drive straight
		motor[frontRight] = Y1 - X2 - X1; //moves/ turns the robot
		motor[backRight] =  Y1 - X2 + X1;
		motor[frontLeft] = Y1 + X2 + X1;
		motor[backLeft] =  Y1 + X2 - X1;
		updateStraight(); //updates with new straight value
	}
	X1 = 0; //helps to stop moving
	stopRobot(); //stops robot
}

void moveTo(float rot, float wrot, int style) { //all in one solution for turning and driving in autonomous mode
	stopRobot(); //stops robot
	if (style == 0) { // rotates, then drives
		moveToRotation(rot);
		moveToWheelRotation(wrot);
		} else if (style == 1) { // drives, then rotates
		moveToWheelRotation(wrot);
		moveToRotation(rot);
		} else if (style == 2) { // drives half, then rotates, then drives other half
		moveToWheelRotation(wrot/2);
		moveToRotation(rot);
		moveToWheelRotation(wrot/2);
		} else { // idk

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
	clearLCDLine(0);
	clearLCDLine(1);
	displayLCDCenteredString(0, "Prepping...");
	SensorType[in8] = sensorNone; //clears sensor port8
	wait1Msec(1000); //gives time to clear
	displayLCDCenteredString(1, "Gyro Engaging...");
	SensorType[in8] = sensorGyro; //sets sensor port8 to gyro (rotation along/around y axis)
	wait1Msec(2000); //gives time for gyro to init

	displayLCDCenteredString(0, "Calibrating...");
	for(i=0;i<1024;i++) {
		gyroInc = gyroInc + SensorValue[in8];
		wait1Msec(1);
	}
	clearLCDLine(1);
	displayLCDCenteredString(0, "Waiting for Task...");
}


task autonomous() //program the robot to do stuff
{
	displayLCDCenteredString(0, "Autonomous"); //messages are fun :)
	//while(true) { // might be needed, might not
	moveTo(45, 10, 0); //testing code here and below
	wait1Msec(2000);
	moveTo(-45, -10, 2);
	wait1Msec(2000);
	moveTo(180, 15, 1); //testing code here and above
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
