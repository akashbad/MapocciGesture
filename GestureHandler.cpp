/*GestureHandler.cpp
	The  class responsible for processing all of the data and deciding if a gesture has been detected

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-implementation of shake detection and dummy functionality 01/06/12
-completion of the first version of working shake detection and reporting 04/06/12
-completion of the first version of working rotation detection
			KNOWN BUG: false positives especially when shaking, think of ways to isolate this,
									more real world data needed to adequately threshold
***********************************************************************************************
*/

#include "Arduino.h"
#include "General.h"
#include "MapocciTransfer.h"
#include "GestureHandler.h"



GestureHandler::GestureHandler(MapocciTransfer model)
{
	transfer = model;

	//Shaking variables initialization
	isShaking = false;
	filteredShake = 0.00;
	shakeCount = 0;
	shakePointer = 0;
	shakingUpper = 15.0;
	shakingLower = 10.0;
	accelerometerNominal = 338;
	
	//Rotation variable initialization
	nominalRotation = 512;
	isSpinning = false;
	isRolling = false;
	isFlipping = false;
	rotationThreshold = 100;
	
	//Falling variable initialization
	isFalling = false;
	fallCount = 0;
	
	//Upside down variable initialization
	isUpsideDown = false;
	
	//Touch variable initialization
	touchCount[0] = 0;
	touchCount[1] = 0;
	touchCount[2] = 0;
	isTouching[0] = false;
	isTouching[1] = false;
	isTouching[2] = false;
	Serial.println(isTouching[0]);

	//Tail variable initialization
	tailThreshold;
	
	//Kiss variable initialization
	kissThreshold = 20;

	isTorso=false;
	torsoCount = 0;
}

void GestureHandler::report(sensorData data)
{
	rawData = data;
	for(int i=0; i<9; i++)
	{
		oldTorso[i] = rawData.torso[i] > 20 ? rawData.torso[i] : 0;
		oldBottom[i] = rawData.bottom[i] > 20 ? rawData.bottom[i] : 0;
		oldStomach[i] = rawData.stomach[i] > 20 ? rawData.stomach[i] : 0;
	}
	for(int i=9; i<14; i++)
	{
		oldTorso[i] = rawData.torso[i] > 20 ? rawData.torso[i] : 0;
		oldStomach[i] = rawData.stomach[i] > 20 ? rawData.stomach[i] : 0;
	}
	for(int i=14; i<16; i++)
	{
		oldTorso[i] = rawData.torso[i] > 20 ? rawData.torso[i] : 0;
	}
	
	// for(int i=0; i<16; i++)
	// {
	// 	Serial.print(oldTorso[i]);
	// 	Serial.print('\t');
	// }
	// // Serial.print('|');
	// for(int i=0; i<9; i++)
	// {
	// 	Serial.print(oldBottom[i]);
	// 	Serial.print('\t');
	// }
	// // Serial.print('|');
	// 	for(int i=0; i<14; i++)
	// {
	// 	Serial.print(oldStomach[i]);
	// 	Serial.print('\t');
	// }
	// //Serial.print('|');
	// //Serial.println("");
	// Serial.print(rawData.mouth);
	// Serial.print('\t');
	// Serial.println(rawData.bodyTouches[0]);
	
	getTouchPadFeatures(means, stds, modes);
}

//------------------------------------------------------------------------------
String GestureHandler::getShaking()
{
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
			pow(rawData.accel[1]-accelerometerNominal,2) + 
			pow(rawData.accel[2]-accelerometerNominal,2));
	int axis = ftiMode(rawData.accel, 3);
	if(testShake(rawData.accel, shakingUpper)&&!isShaking&&!isFalling&&shakeCount==5)
	{
		isShaking = true;

		filteredShake = x*0.6 + filteredShake*0.4;
		float force = transfer.transferShaking(filteredShake);
		return "Shake=initiated:Acceleration=" + ftos(force) + 
			":Axis=" + String(axis)+"!";
	}
	if(testShake(rawData.accel, shakingUpper))
	{
		shakeCount++;
		shakeCount = shakeCount>5 ? 5 : shakeCount;
		filteredShake = x*0.6 + filteredShake*0.4;
	}
	if(isShaking && shakeCount==0)
	{
		isShaking = false;
		filteredShake = 0.00;
		return "Shake=ended!";
	}
	if(!testShake(rawData.accel, shakingLower)&&shakeCount>0)
	{
		shakeCount--;
	}
	if(isShaking)
	{
		float force = transfer.transferShaking(filteredShake);
		return "Shake=initiated:Acceleration=" + ftos(force) + 
			":Axis=" + String(axis)+"!";
	}
	return "";
}

bool GestureHandler::testShake(int current[], float threshold)
{
	float deltas[3];
	for(int i = 0; i<3; i++)
	{
		deltas[i] = fabs(current[i] - pastAccel[i]);
	}
	bool x = (deltas[0] > threshold && deltas[1] > threshold) ||
		(deltas[1] > threshold && deltas[2] > threshold) ||
		(deltas[2] > threshold && deltas[0] > threshold);
	return x;
}

//------------------------------------------------------------------------------
String GestureHandler::getRotating()
{
	bool rotationAxis[3];
	for(int i=0; i<3; i++)
	{
		rotationAxis[i] = checkRotation(i);
	}
	String gesture = "";
	
	if(rotationAxis[0]&&!isSpinning)
	{
		float force = transfer.transferSpinning(abs(rawData.gyro[0] - nominalRotation));
		String spin = "Spin=initiated:Speed=" + ftos(force)+"!";
		gesture+=spin;
		isSpinning = true;
	}
	else if(!rotationAxis[0]&&isSpinning)
	{
		isSpinning = false;
		gesture+= "Spin=ended!";
	}
	else if(isSpinning)
	{
		float force = transfer.transferSpinning(abs(rawData.gyro[0] - nominalRotation));
		String spin = "Spin=initiated:Speed=" + ftos(force)+"!";
		gesture+=spin;
	}
	
//	if(rotationAxis[1]&&!isFlipping)
//	{
//		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
//		String flip = "Flip=initiated:Speed=" + ftos(force)+"!";
//		gesture+=flip;
//		isFlipping = true;
//	}
//	else if(!rotationAxis[1]&&isFlipping)
//	{
//		isFlipping = false;
//		gesture+= "Flip=ended!";
//	}
//	else if(isFlipping)
//	{
//		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
//		String flip = "Flip=initiated:Speed=" + ftos(force)+"!";
//		gesture+=flip;
//	}
//	
//	if(rotationAxis[2]&&!isRolling)
//	{
//		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
//		String roll = "Roll=initiated:Speed=" + ftos(force)+"!";
//		gesture+=roll;
//		isRolling = true;
//	}
//	else if(!rotationAxis[2]&&isRolling)
//	{
//		isRolling = false;
//		gesture+= "Roll=ended!";
//	}
//	else if(isRolling)
//	{
//		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
//		String roll = "Roll=initiated:Speed=" + ftos(force)+"!";
//		gesture+=roll;
//	}

	return gesture;
}

bool GestureHandler::checkRotation(int axis)
{
	return abs(rawData.gyro[axis] - nominalRotation)>rotationThreshold;
}

//------------------------------------------------------------------------------
String GestureHandler::getFalling()
{
	float deltas[3];
	for(int i = 0; i<3; i++)
	{
		deltas[i] = fabs(rawData.accel[i] - accelerometerNominal);
	}
	float magnitude  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
							pow(rawData.accel[1]-accelerometerNominal,2) + 
							pow(rawData.accel[2]-accelerometerNominal,2));
	if(magnitude < 10.0&&!isFalling)
	{
		isFalling = true;
		return "Falling=initiated!";
	}
	else if(magnitude < 10.0)
	{
		fallCount++;
	}
	else if(isFalling&&magnitude >20.0 && fallCount == 0){
		isFalling = false;
		return "Falling=ended!";
	}
	else if(magnitude > 20.0 && fallCount > 0)
	{
		fallCount--;
	}
	if(isFalling)
	{
		return "Falling=initiated!";
	}
	return ("");
}

//------------------------------------------------------------------------------
void GestureHandler::getTouchPadFeatures(float means[], float stds[], int modes[])
{
	//Get the means
	means[0] = ftfMean(oldTorso, 16);
	means[1] = ftfMean(oldBottom,9);
	means[2] = ftfMean(oldStomach, 14);
	
	//Get the stds
	stds[0] = ftfStd(oldTorso, 16);
	stds[1] = ftfStd(oldBottom,9);
	stds[2] = ftfStd(oldStomach, 14);
	
	//Get the modes
	modes[0] = ftiMode(oldTorso, 16);
	modes[1] = ftiMode(oldBottom,9);
	modes[2] = ftiMode(oldStomach, 14);
	
	//Get the sums
	sums[0] = intSum(oldTorso, 16);
	sums[1] = intSum(oldBottom, 9);
	sums[2] = intSum(oldStomach, 14);
}

String GestureHandler::getUpsideDown()
{
	return "";
}

String GestureHandler::getTorso()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 0;
	bool touchTest = rawData.bodyTouches[i] > 60;
	if(touchTest&&!isTorso&&torsoCount==3)
	{
		isTorso = true;
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	if(touchTest)
	{
		torsoCount++;
		torsoCount = torsoCount > 3 ? 3 : torsoCount;
	}
	if(isTorso&&torsoCount==0)
	{
		isTorso = false;
		gesture+= "Touch-"+String(i)+"=ended!";
		return gesture;
	}
	if(!touchTest && torsoCount > 0)
	{
		torsoCount--;
	}
	if(isTorso)
	{
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	touchCount[i] = torsoCount;
	return gesture;
}

String GestureHandler::getBottom()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 1;
	bool touchTest = rawData.bodyTouches[i] > 60;
	if(touchTest&&!isBottom&&bottomCount==3)
	{
		isBottom = true;
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	if(touchTest)
	{
		bottomCount++;
		bottomCount = bottomCount > 3 ? 3 : bottomCount;
	}
	if(isBottom&&bottomCount==0)
	{
		isBottom = false;
		gesture+= "Touch-"+String(i)+"=ended!";
		return gesture;
	}
	if(!touchTest && bottomCount > 0)
	{
		bottomCount--;
	}
	if(isBottom)
	{
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	touchCount[i] = bottomCount;
	return gesture;
}

String GestureHandler::getStomach()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 2;
	bool touchTest = rawData.bodyTouches[i] > 60;
	if(touchTest&&!isStomach&&stomachCount==3)
	{
		isStomach = true;
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	if(touchTest)
	{
		stomachCount++;
		stomachCount = stomachCount > 3 ? 3 : stomachCount;
	}
	if(isStomach&&stomachCount==0)
	{
		isStomach = false;
		gesture+= "Touch-"+String(i)+"=ended!";
		return gesture;
	}
	if(!touchTest && stomachCount > 0)
	{
		stomachCount--;
	}
	if(isStomach)
	{
		gesture+= getTouchGestureString(i);
		return gesture;
	}
	touchCount[i] = stomachCount;
	return gesture;
}

String GestureHandler::getTouchGestureString(int i)
{
	bool didShake = testShake(rawData.accel, shakingUpper);
	float force;
	switch (i)
	{
		case 0:
			 force = transfer.transferTouching(rawData.torso[modes[i]]);
			break;
		case 1:
			 force = transfer.transferTouching(rawData.bottom[modes[i]]);
			break;
		case 2:
			 force = transfer.transferTouching(rawData.stomach[modes[i]]);
			break;
		default:
			 force = 0.00;
	}
	float area = transfer.transferTouchArea(stds[i]);
	return "Touch-"+String(i)+"=initiated:Pressure=" + ftos(force) + 
		":Area=" + ftos(area)+":Position=" + ftos(means[i])+
		":Acceleration=" + String(didShake)+"!";
}

//------------------------------------------------------------------------------
String GestureHandler::getTailTouch()
{
	if(rawData.tail>tailThreshold)
	{
//		return "Tail=Detected!";
	}
	return "";
}

//------------------------------------------------------------------------------
String GestureHandler::getKiss()
{
	if(rawData.mouth>kissThreshold)
	{
		
		return "Kiss=Detected!";
	}
	return "";
}

//------------------------------------------------------------------------------
void GestureHandler::update()
{
	//Copy over the rest of the data
	copyInt(pastAccel, rawData.accel, 3);
}