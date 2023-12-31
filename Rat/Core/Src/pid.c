/*
 * pid.c
 */

#include "main.h"
#include "motors.h"
#include "encoders.h"

int angleError = 0;
int oldAngleError = 0;
int goalAngle = 0;
float angleCorrection;
float oldAngleCorrection = 0;
float distanceError = 0;
float oldDistanceError = 0;
float goalDistance = 0;
float distanceCorrection;
float oldDistanceCorrection = 0;;
const float kPw = 1; //1
const float kDw = 0.5; //1
const float kPx = 0.3; //1
const float kDx = 0.5; //1
int finishCount = 0;
const float maxCorrectA = 0.42; //0.35
const float maxCorrectD = 0.58; //0.3
const float accelConstD = 0.0001;
float leftRPM = 0;
float rightRPM = 0;
void resetPID() {
	/*
	 * For assignment 3.1: This function does not need to do anything
	 * For assignment 3.2: This function should reset all the variables you define in this file to help with PID to their default
	 *  values. You should also reset your motors and encoder counts (if you tell your rat to turn 90 degrees, there will be a big
	 * difference in encoder counts after it turns. If you follow that by telling your rat to drive straight without first
	 * resetting the encoder counts, your rat is going to see a huge angle error and be very unhappy).
	 *
	 * You should additionally set your distance and error goal values (and your oldDistanceError and oldAngleError) to zero.
	 */
	leftRPM = 0;
	rightRPM = 0;
	angleError = 0;
	oldAngleError = 0;
	distanceError = 0;
	oldDistanceError = 0;
	oldDistanceCorrection = 0;
	oldAngleCorrection = 0;
	distanceCorrection = 0;
	angleCorrection = 0;
	finishCount = 0;
	goalDistance = 0;
	goalAngle = 0;
	resetEncoders();
	resetMotors();
}

void updatePID() {
	/*
	 * This function will do the heavy lifting PID logic. It should do the following: read the encoder counts to determine an error,
	 * use that error along with some PD constants you determine in order to determine how to set the motor speeds, and then actually
	 * set the motor speeds.
	 *
	 * For assignment 3.1: implement this function to get your rat to drive forwards indefinitely in a straight line. Refer to pseudocode
	 * example document on the google drive for some pointers
	 *
	 * TIPS (assignment 3.1): Create kPw and kDw variables, and use a variable to store the previous error for use in computing your
	 * derivative term. You may get better performance by having your kDw term average the previous handful of error values instead of the
	 * immediately previous one, or using a stored error from 10-15 cycles ago (stored in an array?). This is because systick calls so frequently
	 * that the error change may be very small and hard to operate on.
	 *
	 * For assignment 3.2: implement this function so it calculates distanceError as the difference between your goal distance and the average of
	 * your left and right encoder counts. Calculate angleError as the difference between your goal angle and the difference between your left and
	 * right encoder counts. Refer to pseudocode example document on the google drive for some pointers.
	 */
	angleError = goalAngle - (getLeftEncoderCounts() - getRightEncoderCounts());
	angleCorrection = kPw * angleError + kDw * (angleError-oldAngleError);
	oldAngleError = angleError;
	if (angleCorrection > maxCorrectA)
		angleCorrection = maxCorrectA;
	else if (angleCorrection < -maxCorrectA)
		angleCorrection = -maxCorrectA;

	distanceError = goalDistance - ((getLeftEncoderCounts() + getRightEncoderCounts())/2);
	//goalDistance - ((getRightEncoderCounts()+getLeftEncoderCounts())/2);
	distanceCorrection = kPx * distanceError + kDx * (distanceError - oldDistanceError);
	oldDistanceError = distanceError;

//	if ((distanceCorrection - oldDistanceCorrection) > accelConstD) //this still doesn't work lol
//		distanceCorrection = oldDistanceCorrection + accelConstD;
//	else if ((distanceCorrection - oldDistanceCorrection) < accelConstD)
//		distanceCorrection = oldDistanceCorrection - accelConstD;

	oldDistanceCorrection = distanceCorrection;


	if (distanceCorrection > maxCorrectD)
		distanceCorrection = maxCorrectD;
	else if (distanceCorrection < -maxCorrectD)
		distanceCorrection = -maxCorrectD;

	leftRPM = distanceCorrection + angleCorrection;
	rightRPM = distanceCorrection - angleCorrection;
	if (angleCorrection == maxCorrectA || angleCorrection == -maxCorrectA)
	{
		if(leftRPM > maxCorrectA)
			leftRPM = maxCorrectA;
		if(leftRPM < -maxCorrectA)
			leftRPM = -maxCorrectA;
		if(rightRPM > maxCorrectA)
			rightRPM = maxCorrectA;
		if(rightRPM < -maxCorrectA)
			rightRPM = -maxCorrectA;
	}
	//setMotorLPWM(distanceCorrection + angleCorrection);
	//setMotorRPWM(distanceCorrection - angleCorrection);
	setMotorLPWM(leftRPM);
	setMotorRPWM(rightRPM);
	if ((distanceError < 3 && distanceError > -3) && (angleError < 2 && angleError > -2) )
		finishCount++;


}

void setPIDGoalD(int16_t distance) {
	/*
	 * For assignment 3.1: this function does not need to do anything.
	 * For assignment 3.2: this function should set a variable that stores the goal distance.
	 */
	goalDistance = distance;
}

void setPIDGoalA(int16_t angle) {
	/*
	 * For assignment 3.1: this function does not need to do anything
	 * For assignment 3.2: This function should set a variable that stores the goal angle.
	 */
	goalAngle = angle;
}

int8_t PIDdone(void) { // There is no bool type in C. True/False values are represented as 1 or 0.
	/*
	 * For assignment 3.1: this function does not need to do anything (your rat should just drive straight indefinitely)
	 * For assignment 3.2:This function should return true if the rat has achieved the set goal. One way to do this by having updatePID() set some variable when
	 * the error is zero (realistically, have it set the variable when the error is close to zero, not just exactly zero). You will have better results if you make
	 * PIDdone() return true only if the error has been sufficiently close to zero for a certain number, say, 50, of SysTick calls in a row.
	 */
	if (finishCount > 500)
		return 1;
	return 0;
}
