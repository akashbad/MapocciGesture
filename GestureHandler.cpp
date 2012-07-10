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
	shakeCount = 0;
	shakePointer = 0;
	shakingUpper = 10.0;
	shakingLower = 10.0;
	accelerometerNominal = 338;
	
	//Rotation variable initialization
	nominalRotation = 512;
	isSpinning = false;
	isRolling = false;
	isFlipping = false;
	rotationThreshold = 50;
	
	//Falling variable initialization
	isFalling = false;
	
	//Hug variable initialization
	hugModeThresholdHigh;
	hugStdThresholdHigh;
	hugModeThresholdLow;
	hugStdThresholdLow;
	strongestHugStd = 0.0;
	strongestHugMode = 0;
	hugCount = 0;
	isHugging = false;
	
	//Hand hold variable initialization
	legsThreshold;
	for(int i=0; i<4; i++)
	{
		isHolding[i] = false;
		maxTouch[i] = 0;
		holdCount[i] = 0;
	}
	
	//Poke variable initialization
	pokeStdThreshold;
	pokeModeThreshold;
	
	//Slap variable initialization
	slapStdThreshold;
	slapStdThreshold;
	
	//Kiss variable initialization
	kissThreshold;
}

void GestureHandler::report(sensorData data)
{
	rawData = data;
	getTouchPadFeatures(means, stds, modes);
	for(int i=0; i<16; i++)
	{
		oldTorso[i] = 0.4*rawData.torso[i] + .6*oldTorso[i];
		float difference = rawData.stomach[i] - oldStomach[i];
		if(difference > 50)
		{
			oldStomach[i] = 0.05*rawData.stomach[i] + .95*oldStomach[i];
		}
		else
		{
			oldStomach[i] = 0.4*rawData.stomach[i] + .6*oldStomach[i];
		}
		// oldStomach[i] = rawData.stomach[i];
		oldBottom[i] = 0.4*rawData.bottom[i] + .6*oldBottom[i];
		Serial.print(oldStomach[i]);
		Serial.print('\t');
	}
	Serial.println("");
	//Serial.print("|\t");
	//Serial.println("");
}

//------------------------------------------------------------------------------
String GestureHandler::getShaking()
{
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
			pow(rawData.accel[1]-accelerometerNominal,2) + 
			pow(rawData.accel[2]-accelerometerNominal,2));
	if(testShake(rawData.accel, shakingUpper)&&!isShaking&&shakeCount==5)
	{
		isShaking = true;

		float force = x > strongestShake ? x : strongestShake;
		return getShakeGesture(force);
	}
	if(testShake(rawData.accel, shakingUpper))
	{
		shakeCount++;
		shakeCount = shakeCount>5 ? 5 : shakeCount;
		strongestShake = x > strongestShake ? x : strongestShake;
	}
	if(isShaking && shakeCount==0)
	{
		isShaking = false;
		strongestShake = 0.0;
		return "Shake=ended:0.00!";
	}
	if(!testShake(rawData.accel, shakingLower)&&shakeCount>0)
	{
		shakeCount--;
	}
	if(isShaking)
	{
		return getShakeGesture(x);
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

String GestureHandler::getShakeGesture(float amplitude)
{
		float force = transfer.transferShaking(amplitude);
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String gesture = "Shake=initiated:" + String(temp)+"!";
		return (gesture);
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
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String spin = "Spin=initiated:" + String(temp)+"!";
		gesture+=spin;
		isSpinning = true;
	}
	else if(!rotationAxis[0]&&isSpinning)
	{
		isSpinning = false;
		gesture+= "Spin=ended:0.00!";
	}
	else if(isSpinning)
	{
		float force = transfer.transferSpinning(abs(rawData.gyro[0] - nominalRotation));
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String spin = "Spin=initiated:" + String(temp)+"!";
		gesture+=spin;
	}
	
	if(rotationAxis[1]&&!isFlipping)
	{
		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String flip = "Flip=initiated:" + String(temp)+"!";
		gesture+=flip;
		isFlipping = true;
	}
	else if(!rotationAxis[1]&&isFlipping)
	{
		isFlipping = false;
		gesture+= "Flip=ended:0.00!";
	}
	else if(isFlipping)
	{
		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String flip = "Flip=initiated:" + String(temp)+"!";
		gesture+=flip;
	}
	
	if(rotationAxis[2]&&!isRolling)
	{
		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String roll = "Roll=initiated:" + String(temp)+"!";
		gesture+=roll;
		isRolling = true;
	}
	else if(!rotationAxis[2]&&isRolling)
	{
		isRolling = false;
		gesture+= "Roll=ended:0.00!";
	}
	else if(isRolling)
	{
		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
		char temp[10];
		dtostrf(force, 0, 3, temp);
		String roll = "Roll=initiated:" + String(temp)+"!";
		gesture+=roll;
	}

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
		return "Falling=started:0.00!";
	}
	else if(isFalling&&magnitude >20.0){
		isFalling = false;
		return "Falling=ended:0.00!";
	}
	if(isFalling)
	{
		return "Falling=started:0.00!";
	}
	return ("");
}

//------------------------------------------------------------------------------
void GestureHandler::getTouchPadFeatures(float means[], float stds[], int modes[])
{
	//Create a huge array
	float body[48];
	for(int i=0; i<16; i++)
	{
		body[i] = oldTorso[i];
	}
	for(int i=0; i<16; i++)
	{
		body[i+16] = oldBottom[i];
	}
	for(int i=0; i<16; i++)
	{
		body[i+32] = oldStomach[i];
	}

	//Get the means
	means[0] = ftfMean(oldTorso, 16);
	means[1] = ftfMean(oldBottom,16);
	means[2] = ftfMean(oldStomach, 16);
	means[4] = ftfMean(body, 48);
	
	//Get the stds
	stds[0] = ftfStd(oldTorso, 16);
	stds[1] = ftfStd(oldBottom,16);
	stds[2] = ftfStd(oldStomach, 16);
	stds[4] = ftfStd(body, 48);
	
	//Get the modes
	modes[0] = ftiMode(oldTorso, 16);
	modes[1] = ftiMode(oldBottom,16);
	modes[2] = ftiMode(oldStomach, 16);
	modes[4] = ftiMode(body, 48);
	
/* 	Serial.print(means[2]);
	Serial.print('\t');
	//Serial.print("\t|\t");
	Serial.print(stds[2]);
	Serial.print('\t');
	//Serial.print("\t|\t");
	Serial.println(modes[2]); */
}

String GestureHandler::getHug()
{
	bool isModeEnough = modes[3] > hugModeThresholdHigh;
	bool isStdEnough = stds[3] > hugStdThresholdHigh;
	if(isModeEnough&&isStdEnough&&!isHugging&&hugCount==3)
	{
		isHugging = true;
		float std = stds[3] > strongestHugStd ? stds[3] : strongestHugStd;
		float mode = modes[3] > strongestHugMode ? modes[3] : strongestHugMode;
		float force = transfer.transferHugging(std, mode);
		char temp[10];
		dtostrf(force, 0, 3, temp);
		return "Hug=initiated:" + String(temp)+"!";
	}
	if(isModeEnough&&isStdEnough)
	{
		hugCount++;
		hugCount = hugCount>3 ? 3 : hugCount;
		strongestHugStd = stds[3] > strongestHugStd ? stds[3] : strongestHugStd;
		strongestHugMode = modes[3] > strongestHugMode ? modes[3] : strongestHugMode;
	}
	if(isHugging && hugCount==0)
	{
		isHugging = false;
		strongestHugStd = 0.0;
		strongestHugMode = 0;
		return "Hug=ended:0.00!";
	}
	isModeEnough = modes[3] > hugModeThresholdLow;
	isStdEnough = stds[3] > hugStdThresholdLow;
	if((!isModeEnough || !isStdEnough) && hugCount>0)
	{
		hugCount--;
	}
	if(isHugging)
	{
		float std = stds[3] > strongestHugStd ? stds[3] : strongestHugStd;
		float mode = modes[3] > strongestHugMode ? modes[3] : strongestHugMode;
		float force = transfer.transferHugging(std, mode);
		char temp[10];
		dtostrf(force, 0, 3, temp);
		return "Hug=initiated:" + String(temp)+"!";
	}
	return ("");
}

//------------------------------------------------------------------------------
//For petting we will not send intermediate information because it is somewhere
//between a continuous gesture and an event based one, each pet may well be treated
//as an event but it is still important to know when each starts and stops.
String GestureHandler::getPet()
{
	float movement[3];
	for(int i=0; i<3; i++)
	{
		movement[i] = fabs(means[i] - lastMeans[i]);
		if(movement[i]>petThresholdHigh && !isPetting &&petCount[i]>5)
		{
			isPetting = true;
			petCount[(i+1)%3] = -1;
			petCount[(i+2)%3] = -1;
			int mode = modes[i] > strongestPetMode[i] ? modes[i] : strongestPetMode[i];
			float speed = movement[i] > fastestPetSpeed[i] ? movement[i] : fastestPetSpeed[i];
			return getPetGesture(i, mode, speed);
		}
		if(movement[i]>petThresholdHigh)
		{
			petCount[i]++;
			petCount[i] = petCount[i]>3 ? 3 : petCount[i];
			strongestPetMode[i] = modes[i] > strongestPetMode[i] ? modes[i] : strongestPetMode[i];
			fastestPetSpeed[i] = movement[i] > fastestPetSpeed[i] ? movement[i] : fastestPetSpeed[i];
		}
		if(isPetting && petCount[i] == 0)
		{
			isPetting = false;
			strongestPetMode[i] = 0;
			fastestPetSpeed[i] = 0.0;
			petCount[(i+1)%3] = 0;
			petCount[(i+2)%3] = 0;
			return "Pet=ended:0.00!";
		}
		if(movement[i]<petThresholdLow && petCount[i]>0)
		{
			petCount[i]--;
		}
	}
	return "";
}

String GestureHandler::getPetGesture(int position, int mode, float speed)
{
	float force = transfer.transferPetting(position, mode, speed);
	char temp[10];
	dtostrf(force, 0, 3, temp);
	switch(position)
	{
		case 0:
			return "Pet-torso=initiated:" + String(temp)+"!";
		case 1:
			return "Pet-bottom=initiated:" + String(temp)+"!";
		case 2:
			return "Pet-stomach=initiated:" + String(temp)+"!";
		default:
			return "";
	}
	
}

//------------------------------------------------------------------------------
//Unimplemented for now
String GestureHandler::getScratch()
{
	return "";
}

//------------------------------------------------------------------------------
String GestureHandler::getHandHold()
{
	String gesture = "";
	bool touch[4];
	for(int i=0; i<4; i++)
	{
		touch[i] = rawData.legs[i] > legsThreshold;
		if(touch[i] && !isHolding[i] && holdCount[i]==4)
		{
			isHolding[i] = true;
			float touchStrength = rawData.legs[i] > maxTouch[i] ? rawData.legs[i] : maxTouch[i];
			float force = transfer.transferHold(touchStrength);
			char temp[10];
			dtostrf(force, 0, 3, temp);
			gesture+="Touch-"+String(i)+"=initiated:"+String(temp)+"!";
		}
		if(touch[i])
		{
			holdCount[i]++;
			holdCount[i] = holdCount[i]>4 ? 4 : holdCount[i];
			maxTouch[i] = rawData.legs[i] > maxTouch[i] ? rawData.legs[i] : maxTouch[i];
		}
		if(isHolding[i] && holdCount==0)
		{
			isHolding[i] = false;
			maxTouch[i] = 0;
			gesture+= "Touch-"+String(i)+"=ended:0.00!";
		}
		if(!touch[i] && holdCount[i]>0)
		{
			holdCount[i]--;
		}
		if(isHolding[i])
		{
			float touchStrength = rawData.legs[i] > maxTouch[i] ? rawData.legs[i] : maxTouch[i];
			float force = transfer.transferHold(touchStrength);
			char temp[10];
			dtostrf(force, 0, 3, temp);
			gesture+="Touch-"+String(i)+"=initiated:"+String(temp)+"!";
		}
	}
	return gesture;
}

//------------------------------------------------------------------------------
//Unimplemented for now
String GestureHandler::getFeed()
{
	return "";
}
 
//------------------------------------------------------------------------------
String GestureHandler::getPoke()
{
	for(int i=0; i<3; i++)
	{
		float stdChange = fabs(stds[i] - lastStds[i]);
		float modeChange = fabs(modes[i] - lastModes[i]);
		if(stdChange < pokeStdThreshold && modeChange > pokeModeThreshold && !isHugging)
		{
			float force = transfer.transferPoke(modes[i], stds[i]);
			char temp[10];
			dtostrf(force, 0, 3, temp);
			return "Poke=detected:"+String(temp)+"!";
		}
	}
	return "";
}

//------------------------------------------------------------------------------
String GestureHandler::getSlap()
{
	for(int i=0; i<4; i++)
	{
		float stdChange = fabs(stds[i] - lastStds[i]);
		float modeChange = fabs(modes[i] - lastModes[i]);
		if(stdChange > slapStdThreshold && modeChange > slapModeThreshold && !isHugging)
		{
			float force = transfer.transferSlap(modes[i], stds[i]);
			char temp[10];
			dtostrf(force, 0, 3, temp);
			return "Slap=Detected:"+String(temp)+"!";
		}
	}
	return "";
}

//------------------------------------------------------------------------------
String GestureHandler::getKiss()
{
	if(rawData.mouth>kissThreshold)
	{
		float force = transfer.transferKiss(rawData.mouth);
		char temp[10];
		dtostrf(force, 0, 3, temp);
		return "Kiss=Detected:"+String(temp)+"!";
	}
	return "";
}

//------------------------------------------------------------------------------
void GestureHandler::update()
{
	//Copy over the rest of the data
	copyInt(pastAccel, rawData.accel, 3);
	copyFloat(lastMeans, means, 4);
	copyFloat(lastStds, stds, 4);
	copyInt(lastModes, modes, 4);
	
}