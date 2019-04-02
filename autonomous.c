#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(Sensor, in1,    pot,            sensorPotentiometer)
#pragma config(Sensor, in2,    gyro,           sensorGyro)
#pragma config(Sensor, in3,    absolute_gyro,  sensorGyro)
#pragma config(Sensor, dgtl1,  encoderleft,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  encoderright,   sensorQuadEncoder)
#pragma config(Motor,  port2,           leftdrive,     tmotorVex393_MC29, openLoop, driveLeft)
#pragma config(Motor,  port3,           rightdrive,    tmotorVex393_MC29, openLoop, reversed, driveRight)
#pragma config(Motor,  port5,           claw,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           armmotor,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           shooter,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           spintake,      tmotorVex393_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
borken - Pole
Pickle - Flag
Lemonade - Platform
Nuin - Intake
pinot - Cap
*/

#pragma platform(VEX2)
#pragma competitionControl(Competition) //competition configuration
#include "Vex_Competition_Includes.c"

#pragma systemFile

int selectedAutonomousColor = 2; //1 is red, 2 is blue, 0 is none
int selectedAutonomousSquare = 1; //1 is close, 2 is far, 0 none

/*
LCD INSTRUCTIONS

Buttons: 1-Left 2-Center 3-Right

Voltage: Press any button to continue
Color Selection: Select 1 for red, 2 for blue, 3 for other
Red: 1 for close to flag, 2 for far from flag
Blue: 1 for close to falg, 2 for far from flag
Other: 1 for no auton, 2 for skills
Finished screen: No action needed, wait for match start

Skills: High flag, low flag, cap, platform (10 pts)

Matches: Close to flag: High flag, low flag, cap, platform (10 pts)
				 Far from flag: High flag, cap (5 pts)

				 TEST AIM FOR MIDDLE FLAG
*/

void stopDrive() {
	motor[leftdrive] = -10;
	motor[rightdrive] = -10;
	wait1Msec(10);
	motor[leftdrive] = 0;
	motor[rightdrive] = 0;
	//applies small brake and stops chassis
}

void resetSensors() {
	SensorValue[encoderleft] = 0;
	SensorValue[encoderright] = 0;
	//resets encoder values to 0
}

void setGyro() {
	SensorType[in2] = sensorNone;
	SensorType[in2] = sensorGyro;
	wait1Msec(1118);
	//removes gyro sensor, and adds again, then calibrating
}

void clear() {
	clearLCDLine(0);
	clearLCDLine(1);
	//clear lcd line for scene refresh
}

void pre_auton() {
	bStopTasksBetweenModes = true;

	//clear previously existing text on display
	clear();

	bLCDBacklight = true;

	string scene = "voltage";
	float voltage = nAvgBatteryLevel;
	string lastScene = "";

	while (bIfiRobotDisabled) {
		if (lastScene != scene) {
			clear(); //Only refreshes screen on scene change
		}

		//first displays average battery voltage before match in millivolts
		if (scene == "voltage") {
			displayLCDCenteredString(0, "Voltage (mV):");
			displayLCDNumber(0, 14, voltage);
			if (nLCDButtons != 0) { //upon any button press, enters color selection
				scene = "colorSelection";
				wait1Msec(500); //pause to prevent jumping too far forward
			}
		} else if (scene == "colorSelection") { //select autonomous color (red, blue, none/skills)
			displayLCDCenteredString(0, "1: RED, 2: BLUE");
			displayLCDCenteredString(1, "3: SKILLS/NONE");
			if (nLCDButtons == 1) { //red alliance autonomous selected (left button)
				scene = "redSelection";
				selectedAutonomousColor = 1;
				wait1Msec(500);
			} else if (nLCDButtons == 2) { //blue alliance autonomous selected (center button)
				scene = "blueSelection";
				selectedAutonomousColor = 2;
				wait1Msec(500);
			} else if (nLCDButtons == 4) { //other selected (no autonomous or skills) (right button)
				scene = "other";
				wait1Msec(500);
			}
		} else if (scene == "redSelection") {  //select autonomous side for red (flag side, far side)
			displayLCDCenteredString(0, "1: FLAG SIDE");
			displayLCDCenteredString(1, "2: OTHER SIDE");
			if (nLCDButtons == 1) { //red flag side selected (left button)
				scene = "finished";
				selectedAutonomousSquare = 1;
				wait1Msec(500);
			} else if (nLCDButtons == 2) { //red far side selected (center button)
				scene = "finished";
				selectedAutonomousSquare = 2;
			}
		} else if (scene == "blueSelection") { //select autonomous side for blue (flag side, far side)
			displayLCDCenteredString(0, "1: FLAG SIDE");
			displayLCDCenteredString(1, "2: OTHER SIDE");
			if (nLCDButtons == 1) { //blue flag side selected (left button)
				scene = "finished";
				selectedAutonomousSquare = 1;
				wait1Msec(500);
			} else if (nLCDButtons == 2) { //blue far side selected (right button)
				scene = "finished";
				selectedAutonomousSquare = 2;
			}
		} else if (scene == "finished") { //displays selected autonomous
			if (selectedAutonomousColor == 1) {
				if (selectedAutonomousSquare == 1) {
					displayLCDCenteredString(0, "Selected Auton:");
					displayLCDCenteredString(1, "RED, CLOSE SIDE");
				} else {
					displayLCDCenteredString(0, "Selected Auton:");
					displayLCDCenteredString(1, "RED, FAR SIDE");
				}
			} else if (selectedAutonomousColor == 2) {
				if (selectedAutonomousSquare == 1) {
					displayLCDCenteredString(0, "Selected Auton:");
					displayLCDCenteredString(1, "BLUE, CLOSE SIDE");
				} else {
					displayLCDCenteredString(0, "Selected Auton:");
					displayLCDCenteredString(1, "BLUE, FAR SIDE");
				}
			} else if (selectedAutonomousColor == 0 && selectedAutonomousSquare == 0) {
				displayLCDCenteredString(0, "Selected auton:");
				displayLCDCenteredString(1, "NONE");
			} else {
				displayLCDCenteredString(0, "Selected auton:");
				displayLCDCenteredString(1, "SKILLS");
			}
		} else if (scene == "other") { //select other autonomous modes
			displayLCDCenteredString(0, "1: NO AUTON");
			displayLCDCenteredString(1, "2: SKILLS");
			if (nLCDButtons == 1) { //selected no autonomous (left button), usually due to errors or risk of crossing line
				scene = "finished";
				selectedAutonomousColor = 0;
				selectedAutonomousSquare = 0;
				wait1Msec(500);
			} else if (nLCDButtons == 2) { //selected skills autonomous
				scene = "finished";
				selectedAutonomousSquare = 3;
				selectedAutonomousColor = 3;
			}
		} else {
			displayLCDCenteredString(0, "ERROR: Failed");
			displayLCDCenteredString(1, "to match scenes");
		}
		lastScene = scene;

	}

	stopDrive();
	resetSensors(); //final resets before match

	bLCDBacklight = false;

}



void moveBackward(int ticks, int speed) { //move backward ticks (degrees on encoders), where speed is negative
resetSensors();

	int leftpower = speed;


	while (abs(SensorValue[encoderright]) < abs(ticks)) { //activate motors while traveling


			motor[leftdrive] = leftpower;
			motor[rightdrive] = leftpower;
	}

 //when finished, stop and apply small brake
/*
motor[leftdrive] = speed;
motor[rightdrive] = speed;
wait1Msec(ticks);
motor[leftdrive] = 0;
motor[rightdrive] = 0;
*/
}

void moveForward(int ticks, int speed) { //move forward ticks (degrees on encoders) speed is positive
	 //reset encoders
	resetSensors();

	int leftpower = speed; //set speed

	while (abs(SensorValue[encoderright]) < ticks) { //activate motors while traveling
		motor[rightdrive] = leftpower;
		motor[leftdrive] = leftpower;
	}
	stopDrive(); //when finished, stop and apply small brake
}
/*
motor[leftdrive] = speed;
motor[rightdrive] = speed;
wait1Msec(ticks);
motor[leftdrive] = 0;
motor[rightdrive] = 0;
*/


void turnRight(int degrees, int speed) { //turn right using gyro sensor values



	int degrees10 = degrees * 9.4; //scale factor
	int error = 5;

	while (abs(SensorValue[gyro]) < degrees10 - 100) { //stop slightly before to account for slight coasting due to omni wheels
		motor[rightdrive] = -speed;
		motor[leftdrive] = speed;
	}

	motor[rightdrive] = 5; //apply small brake
	motor[leftdrive] = -5;

	wait1Msec(50);

	while (abs(SensorValue[gyro]) < degrees10 - error) { //turns very slightly in other direction if there is error
		motor[rightdrive] = speed * -0.6;
		motor[leftdrive] = speed * 0.6;
	}

	stopDrive(); //stops and applies brake
	resetSensors();
}

void turnLeft (int degrees, int speed) { //turns left based on gyro values



	int degrees10 = degrees * 9.4; //scale factor
	int error = 5; //preset error

	while (abs(SensorValue[gyro]) < degrees10 - 100) { //turns while turn in incomplete
		motor[rightdrive] = speed;
		motor[leftdrive] = -speed;
	}

	motor[rightdrive] = -5; //small brake
	motor[leftdrive] = 5;

	wait1Msec(50);

	while (abs(SensorValue[gyro]) < degrees10 - error) { //continues based on error
		motor[rightdrive] = speed * 0.6;
		motor[leftdrive] = speed * -0.6;
	}

	stopDrive(); //stops and applies brake
	resetSensors();
}

void arm(int ticks) { //moves arm based on arm potentiometer value, not limited to stay within size
	if (SensorValue[pot] > ticks) {
		while (SensorValue[pot] > ticks) {
			motor[armmotor] = -127; //while arm is below value, moves arm up
		}
	} else if (SensorValue[pot] < ticks) {
		while (SensorValue[pot] < ticks) {
			motor[armmotor] = 127; //while arm is above value, moves arm down
		}
	}
	motor[armmotor] = 0; //stops arm
	
}

void shoot() {
	motor[shooter] = -127;
	wait1Msec(1200);
	motor[shooter] = 0; //shoots catapult
}

void flip(int ticks) {
	motor[claw] = 127;
	wait1Msec(ticks);
	motor[claw] = 0;
}

void intake(int ticks, int speed) {
	motor[spintake] = -speed;
	wait1Msec(ticks);
	motor[spintake] = 0;
}

void skills() {
	/*
	shoot();
	moveBackward(950, -127);
	moveForward(1050, 60);
	turnLeft(90, 50);
	moveForward(950, 60);
	arm(2100);
	arm(2250);
	turnLeft(135, 50);
	moveForward(400, 60);
	flip(300);
	moveBackward(400, -60);
	turnRight(45, 50);
	arm(1900);
	moveForward(400, 60);
	moveBackward(300, 60);
	turnRight(75, 50);
	arm(2200);
	moveForward(800, 60);
	*/
	moveBackward(350, -127);
	shoot();
	arm(1900);
	moveBackward(550, -127);
	moveForward(1300, 127);
	/*
	arm(2100);
	moveForward(900, 127);
	moveBackward(1000, -127);
	*/
	turnLeft(90, 50);
	moveForward(1200, 127);
	//skills autonomous: shoots middle flag, toggles low flag, flips cap, then center parks

	//IN PROGRESS: add another cap or another flag
}

void red_flag() {
	moveBackward(500, -127);
	shoot();
	moveBackward(400, -127);
//red flag side autonomous: shoots middle flag, toggles low flag, flips cap, then alliance parks

//IN PROGRESS: improve reliability to above 70%
}

void red_non_flag() {
	moveBackward(300, -127);
	turnLeft(40, 50);
	moveBackward(100, -127);

	//red far side autonomous: flips cap

	//IN PROGRESS: shoot ball
}

void blue_flag() {
	setGyro();
	//blue flag side autonomous: shoots middle flag, flips cap, then alliance parks
	motor[spintake] = -80;
	moveBackward(480, -60);
	wait1Msec(500);
	moveBackward(490, 80);
	motor[spintake] = 0;
	turnLeft(70, 80);
	arm(560);
	moveBackward(265, 80);
	moveBackward(40, -70);
	motor[leftdrive] = 0;
	motor[rightdrive] = 0;
	arm(1200);
	shoot();
	motor[spintake] = -90;
	moveBackward(360, -80);
	motor[rightdrive] = 0;
	motor[leftdrive] = 0;
	wait1Msec(900);
	shoot();
	motor[spintake] = 0;
	//IN PROGRESS: improve reliability to above 80%
}

void blue_non_flag() {
	shoot();
	moveBackward(300, -127);
	turnLeft(60, 50);
	moveBackward(650, -127);
	//blue far side autonomous: flips cap and shoots ball

}

task autonomous () {
	//playSoundFile();
	if (selectedAutonomousColor == 1) {
		if (selectedAutonomousSquare - 1) {
			red_non_flag();
		} else {
			red_flag();
		}
	} else if (selectedAutonomousColor == 2) {
		if (selectedAutonomousSquare - 1) {
			blue_non_flag();
		} else {
			blue_flag();
		}
	} else if (selectedAutonomousColor == 3) {
		skills();
	}
	//select autonomous based on output values from LCD code
}

int angle;
string user = "Christian";
bool k = false;
task usercontrol() { //driver control
	while (0==0) {
		angle = SensorValue[pot]; //update potentiometer value
		//Driving with Joysticks
		if (user == "Christian") { //boot offline lolz fortnutz hacks must eat cheeze itz
			motor[leftdrive] = vexRT[Ch3];
			motor[rightdrive] = vexRT[Ch2];
			if (k) {
				if (vexRT[Btn6U] == 1 && vexRT[Btn6D] == 0) {
					motor[armmotor] = -127;
				} else if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 0) {
					motor[armmotor] = 127;
				} else {
					motor[armmotor] = 0;
				}
			} else {
				if (vexRT[Btn6U] == 1 && vexRT[Btn6D] == 0 && angle > 1850) {
					motor[armmotor] = -127;
				} else if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 0) {
					motor[armmotor] = 127;
				} else {
					motor[armmotor] = 0;
				}
			}
			if (vexRT[Btn6U] == 1) {
				k = true;
			} else if (vexRT[Btn6D] == 1) {
				k = false;
			} else {
				k = false;
			}
			if (vexRT[Btn5U] == 1 && vexRT[Btn5D] == 0) { //claw is controlled by 5U and 5D
				motor[claw] = 55;
			} else if (vexRT[Btn5D] == 1 && vexRT[Btn5U] == 0) {
				motor[claw] = -55;
			} else {
				motor[claw] = 0;
			}
			if (vexRT[Btn8U] == 1 && vexRT[Btn8D] == 0) {
				motor[shooter] = 127;
			} else if (vexRT[Btn8U] == 0 && vexRT[Btn8D] == 1) {
				motor[shooter] = -127;
			} else {
				motor[shooter ] = 0;
			}
			if (vexRT[Btn7U] == 1 && vexRT[Btn7D] == 0) {
				motor[spintake] = 127;
			} else if (vexRT[Btn7U] == 0 && vexRT[Btn7D] == 1) {
				motor[spintake] = -127;
			} else {
				motor[spintake] = 0;
			}
		} else if (user == "Daniel") {
			motor[leftdrive] = vexRT[Ch3Xmtr2];
			motor[rightdrive] = vexRT[Ch2Xmtr2];
			if (vexRT[Btn8UXmtr2] == 1 && vexRT[Btn8DXmtr2] == 0) { //claw is controlled by 5U and 5D
				motor[armmotor] = 55;
			} else if (vexRT[Btn8DXmtr2] == 1 && vexRT[Btn8UXmtr2] == 0) {
				motor[armmotor] = -55;
			} else {
				motor[armmotor] = 0;
			}
			if (vexRT[Btn7UXmtr2] == 1 && vexRT[Btn7DXmtr2] == 0) { //claw is controlled by 5U and 5D
				motor[claw] = 55;
			} else if (vexRT[Btn7DXmtr2] == 1 && vexRT[Btn7UXmtr2] == 0) {
				motor[claw] = -55;
			} else {
				motor[claw] = 0;
			}
			if (vexRT[Btn5UXmtr2] == 1 && vexRT[Btn5DXmtr2] == 0) {
				motor[shooter] = 127;
			} else if (vexRT[Btn5UXmtr2] == 0 && vexRT[Btn5DXmtr2] == 1) {
				motor[shooter] = -127;
			} else {
				motor[shooter] = 0;
			}
			if (vexRT[Btn6UXmtr2] == 1 && vexRT[Btn6DXmtr2] == 0) {
				motor[spintake] = 127;
			} else if (vexRT[Btn6UXmtr2] == 0 && vexRT[Btn6DXmtr2] == 1) {
				motor[spintake] = -127;
			} else {
				motor[spintake] = 0;
			}
		}








		if (vexRT[Btn8L] == 1) {
			user = "Daniel";
		}
		if (vexRT[Btn5UXmtr2] == 1 && vexRT[Btn5DXmtr2] == 1 && vexRT[Btn6UXmtr2] == 1 && vexRT[Btn6DXmtr2] == 1) {
			user = "Christian";
		}
//According to all known laws of aviation,  there is no way a beeshould be able to fly. Its wings are too small to get its fat little body off the ground.The bee, of course, flies anyway because bees don't care what humans think is impossible.


	}
}
