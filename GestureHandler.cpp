/*GestureHandler.cpp
	The  class responsible for processing all of the data and deciding if a gesture has been detected
*/

#include "Arduino.h"
#include "General.h"
#include "MapocciTransfer.h"
#include "GestureHandler.h"


/**
* A constructor for the GestureHandler class.
*	This constructor sets up and initializes all of the private
*	variables in the GestureHandler, including all of the important
*	algorithm thresholds and state variables. In addition it also
*	includes variables to hold on to a raw or processed history of
*	some sensor data. Changes to variables here should produce changes
*	in the sensitivity of gesture detection.
*
*	@param model an instance of the MapocciTransfer object which is used
*		to transfer features to number understandable by the emotional model
*
*/
GestureHandler::GestureHandler(MapocciTransfer model)
{
	transfer = model;

	//Shaking variables initialization
	isShaking = false;
	filteredShake = 0.00;
	shakeCount = 0;
	shakingUpper = 15.0;
	shakingLower = 10.0;
	accelerometerNominal = 338;
	
	//Rotation variable initialization
	nominalRotation = 512;
	isSpinning = false;
	isRolling = false;
	isFlipping = false;
	rotationThreshold = 150;
	
	//Falling variable initialization
	isFalling = false;
	fallThresholdHigh = 20.0;
	fallThresholdLow = 10.0;
	
	//Upside down variable initialization
	upsideDownThreshold = 290;
	isUpsideDown = false;
	standingUpThreshold = 390;
	isStandingUp = false;
	headDownThreshold = 280;
	isHeadDown = false;

	//Tail variable initialization
	tailThreshold = 50;
	isTailTouching = true;
	
	//Kiss variable initialization
	kissThreshold = 20;
	isKissing = false;

	//Touch detection initialization
	isTorso=false;
	torsoCount = 0;
	torsoCapThresh = 100;
	torsoSumThresh = 100;
	isBottom=false;
	bottomCount = 0;
	bottomCapThresh = 70;
	bottomSumThresh = 50;
	isStomach=false;
	stomachCount = 0;
	stomachCapThresh = 120;
	stomachSumThresh = 50;
}

/**
* A method to feed new raw data into the GestureHandler.
*	This allows the GestureHandler to have the most up to date
*	data for this cycle. In addition there is some additional
*	data processing here, including the smoothing or segmenting
*	of different data. Here is also the correct place to print
*	raw or slightly processed data for debugging. Finally,
*	this method generates the features for each fabric sensor.
*
*	@param data the sensorData object containing the newest rawData
*	@see getTouchPadFeatures()
*/
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


	getTouchPadFeatures();


}

/**
* The method to return shake gestures.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. More specifically
*	this algorithm is described <a href="http://stackoverflow.com/questions/150446/how-do-i-detect-when-someone-shakes-an-iphone">
*	here</a>. In addition, this method performed a high pass filter on the
*	acceleration magnitude which is then used as a feature. Also the axis
*	on which the acceleration was highest is used as a feature. Finally
*	a special condition is added which prevents shaking from being activated
*	if the Mapocci is in free fall.
*
*	@return an empty String if no gesture is detected or "Shake=initated" or
*		"Shake=ended" gesture if one is detected.
*	@see testShake() the test condition for the algorithm
*	@see MapocciTransfer::transferShaking()
*	@see ftos()
*/
String GestureHandler::getShaking()
{
	//Obtain the total magnitude of the acceleration and store it in x
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
			pow(rawData.accel[1]-accelerometerNominal,2) + 
			pow(rawData.accel[2]-accelerometerNominal,2));
	//Also find the axis on which the acceleration is highest
	int axis = iMode(rawData.accel, 3);
	//See the general gesture algorithm skeleton
	if(testShake(rawData.accel, shakingUpper)&&!isShaking&&!isFalling&&shakeCount==5)
	{
		isShaking = true;

		filteredShake = x*0.6 + filteredShake*0.4;
		float force = transfer.transferShaking(filteredShake);
		//The correctly formatted acceleration message
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

/**
* The test condition for the shake gesture.
*	This method checks to see if the difference in acceleration
*	between this cycle and the last on at least 2 of the axis 
*	is more than the given threshold.
*	
*	@param current[] the new acceleration data for this cycle
*	@param threshold the difference threshold
*	@return a boolean determining if the threshold was exceeded on
*		at least 2 axis
*	@see getShaking()
*/
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

/**
* The method to check for rotation on any of the three axis.
*	This method uses the standard gesture detection algorithm which
*	is explained in more depth on the @ref algorithms page. The test
*	condition for this is a value above the rotation threshold. Each
*	axis is evaluated seperately because they connote slightly
*	different gestures.
*
*	@return an empty String if no gesture is detected or "Spin=initated" or
*		"Spin=ended" gesture if one is detected. The same format
*		follows to Rolling and Flipping
*	@see checkRotation the method to get the test condition
*	@see MapocciTransfer::transferSpinning
*	@see MapocciTransfer::transferRolling
*	@see MapocciTransfer::transferFlipping
*	@see ftos()
*/
String GestureHandler::getRotating()
{
	bool rotationAxis[3];
	for(int i=0; i<3; i++)
	{
		rotationAxis[i] = checkRotation(i);
	}
	String gesture = "";
	
	if(rotationAxis[0]&&!isSpinning&&!isShaking)
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
	
/*	if(rotationAxis[1]&&!isFlipping)
	{
		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
		String flip = "Flip=initiated:Speed=" + ftos(force)+"!";
		gesture+=flip;
		isFlipping = true;
	}
	else if(!rotationAxis[1]&&isFlipping)
	{
		isFlipping = false;
		gesture+= "Flip=ended!";
	}
	else if(isFlipping)
	{
		float force = transfer.transferFlipping(abs(rawData.gyro[1] - nominalRotation));
		String flip = "Flip=initiated:Speed=" + ftos(force)+"!";
		gesture+=flip;
	}
	
	if(rotationAxis[2]&&!isRolling)
	{
		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
		String roll = "Roll=initiated:Speed=" + ftos(force)+"!";
		gesture+=roll;
		isRolling = true;
	}
	else if(!rotationAxis[2]&&isRolling)
	{
		isRolling = false;
		gesture+= "Roll=ended!";
	}
	else if(isRolling)
	{
		float force = transfer.transferRolling(abs(rawData.gyro[2] - nominalRotation));
		String roll = "Roll=initiated:Speed=" + ftos(force)+"!";
		gesture+=roll;
	}
*/
	return gesture;
}

/**
* The method that checks if a given gyroscope axis is rotating.
*	This checks that by subtracting the nominal gyroscope value
*	and then comparing it to the threshold.
*
*	@param axis the number of the axis to be checked
*	@return a boolean determining if it is rotating or not
*	@see getRotating()
*/
bool GestureHandler::checkRotation(int axis)
{
	return abs(rawData.gyro[axis] - nominalRotation)>rotationThreshold;
}

/**
* The method to check for free fall.
*	This method uses the standard gesture detection algorithm which
*	is explained in more depth on the @ref algorithms page. The test
*	condition for this is if the total magnitude of acceleration is near
*	to 0, indicating free fall. This gesture has no features.
*
*	@return an empty String if no gesture is detected or "Falling=initated" or
*		"Falling=ended" gesture if one is detected.
*/
String GestureHandler::getFalling()
{
	float magnitude  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
							pow(rawData.accel[1]-accelerometerNominal,2) + 
							pow(rawData.accel[2]-accelerometerNominal,2));
	if(magnitude < fallThresholdLow&&!isFalling)
	{
		isFalling = true;
		return "Falling=initiated!";
	}
	if(isFalling&&magnitude > fallThresholdHigh)
	{
		isFalling = false;
		return "Falling=ended!";
	}
	if(isFalling)
	{
		return "Falling=initiated!";
	}
	return ("");
}

/**
* A method to aquire all of the touch fabric features.
*	This method obtains the means, standard deviations, modes,
*	and sums for each of the touch pads. This is also the place
*	to print out the features if debugging.
*
*	@see fMean()
*	@see fStd()
*	@see iMode()
*	@see intSum()
*/
void GestureHandler::getTouchPadFeatures()
{
	//Get the means
	means[0] = fMean(oldTorso, 16);
	means[1] = fMean(oldBottom,9);
	means[2] = fMean(oldStomach, 14);
	
	//Get the stds
	stds[0] = fStd(oldTorso, 16);
	stds[1] = fStd(oldBottom,9);
	stds[2] = fStd(oldStomach, 14);
	
	//Get the modes
	modes[0] = iMode(oldTorso, 16);
	modes[1] = iMode(oldBottom,9);
	modes[2] = iMode(oldStomach, 14);
	
	//Get the sums
	sums[0] = intSum(oldTorso, 16);
	sums[1] = intSum(oldBottom, 9);
	sums[2] = intSum(oldStomach, 14);

	// Serial.print(means[0]);
	// Serial.print('\t');
	// Serial.print(stds[0]);
	// Serial.print('\t');
	// Serial.print(modes[0]);
	// Serial.print('\t');
	// Serial.print(sums[0]);
	// Serial.println("");
}

/**
* The method to check if the Mapocci is upside down.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. The test condition
*	of this gesture is the acceleration on the z-axis of the accelerometer
*	which should change greatly in magnitude if the Mapocci is upside down.
*
*	@return an empty String if no gesture is detected or "Upside_Down=initated" or
*		"Upside_Down=ended" gesture if one is detected.
*/
String GestureHandler::getUpsideDown()
{
	if(rawData.accel[2]<upsideDownThreshold&&!isUpsideDown&&!isShaking)
	{
		isUpsideDown = true;
		return "Upside-Down=initated!";
	}
	if(rawData.accel[2]>upsideDownThreshold&&isUpsideDown)
	{
		isUpsideDown = false;
		return "Upside-Down=ended!";
	}
	if(isUpsideDown)
	{
		return "Upside-Down=initiated!";
	}
	return "";
}

/**
* The method to check if the Mapocci is standing up.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. The test condition
*	of this gesture is the acceleration on the y-axis of the accelerometer
*	which should change greatly in magnitude if the Mapocci is standing up.
*
*	@return an empty String if no gesture is detected or "Standing-Up=initated" or
*		"Standing-Up=ended" gesture if one is detected.
*/
String GestureHandler::getStandingUp()
{
	if(rawData.accel[0]>standingUpThreshold&&!isStandingUp&&!isShaking)
	{
		isStandingUp = true;
		return "Standing-Up=initated!";
	}
	if(rawData.accel[0]<standingUpThreshold&&isStandingUp)
	{
		isStandingUp = false;
		return "Standing-Up=ended!";
	}
	if(isStandingUp)
	{
		return "Standing-Up=initiated!";
	}
	return "";
}

/**
* The method to check if the Mapocci is head down.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. The test condition
*	of this gesture is the acceleration on the y-axis of the accelerometer
*	which should change greatly in magnitude if the Mapocci is head down.
*
*	@return an empty String if no gesture is detected or "Head_Down=initated" or
*		"Head_Down=ended" gesture if one is detected.
*/
String GestureHandler::getHeadDown()
{
	if(rawData.accel[0]<headDownThreshold&&!isHeadDown&&!isShaking)
	{
		isHeadDown = true;
		return "Head-Down=initated!";
	}
	if(rawData.accel[0]>headDownThreshold&&isHeadDown)
	{
		isHeadDown = false;
		return "Head-Down=ended!";
	}
	if(isHeadDown)
	{
		return "Head-Down=initiated!";
	}
	return "";
}

/**
* The method to test for torso touches.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. This method
* 	is identical to getBottom() and getStomach() but they are duplicated
*	because of a bug with storing state in an array. The test condition for
*	a touch is the presence of a capacitive signal on the torso touch sensor
*	and significant activation on the torso fabric (read a sum). The features
*	extracted include the mean (location), pressure (mode), area (standard deviation),
*	and whether or not the accelerometer felt a shake.
*
*	@return an empty String if no gesture is detected or "Touch-0=initated" or
*		"Touch-0=ended" gesture if one is detected.
*	@see getTouchGestureString() a method to construct the proper gesture
*/
String GestureHandler::getTorso()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 0;
	bool touchTest = rawData.bodyTouches[i] > torsoCapThresh && sums[i] > torsoSumThresh;
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
	return gesture;
}

/**
* The method to test for bottom touches.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. This method
* 	is identical to getTorso() and getStomach() but they are duplicated
*	because of a bug with storing state in an array. The test condition for
*	a touch is the presence of a capacitive signal on the bottom touch sensor
*	and significant activation on the bottom fabric (read a sum). The features
*	extracted include the mean (location), pressure (mode), area (standard deviation),
*	and whether or not the accelerometer felt a shake.
*
*	@return an empty String if no gesture is detected or "Touch-1=initated" or
*		"Touch-1=ended" gesture if one is detected.
*	@see getTouchGestureString() a method to construct the proper gesture
*/
String GestureHandler::getBottom()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 1;
	bool touchTest = rawData.bodyTouches[i] > bottomCapThresh && sums[i] > bottomSumThresh;
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
	return gesture;
}

/**
* The method to test for stomach touches.
*	This method uses a version of the general gesture algorithm with hysterisis, which
*	is explained in more depth on the @ref algorithms page. This method
* 	is identical to getBottom() and getTorso() but they are duplicated
*	because of a bug with storing state in an array. The test condition for
*	a touch is the presence of a capacitive signal on the stomach touch sensor
*	and significant activation on the stomach fabric (read a sum). The features
*	extracted include the mean (location), pressure (mode), area (standard deviation),
*	and whether or not the accelerometer felt a shake.
*
*	@return an empty String if no gesture is detected or "Touch-2=initated" or
*		"Touch-2=ended" gesture if one is detected.
*	@see getTouchGestureString() a method to construct the proper gesture
*/
String GestureHandler::getStomach()
{
	String gesture = "";
	float x  = sqrt(pow(rawData.accel[0]-accelerometerNominal,2) + 
		pow(rawData.accel[1]-accelerometerNominal,2) + 
		pow(rawData.accel[2]-accelerometerNominal,2));
	int i = 2;
	bool touchTest = rawData.bodyTouches[i] > stomachCapThresh && sums[i] > stomachSumThresh;
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
	return gesture;
}

/**
* A method to construct the gesture message for body touches.
*	This method takes the index of the gesture and looks through
*	the stored features to properly construct the message according
*	to the grammar.
*
*	@param i the index of the touch
*	@return the String representing the gesture with all of the 
*		features inserted
*	@see getTorso()
*	@see getBottom()
*	@see getStomach()
*/
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

/**
* The method to check for tail touches.
*	This method uses the standard gesture detection algorithm which
*	is explained in more depth on the @ref algorithms page. The test
*	condition for this is if the measured capacitance exceeds the 
*	threshold chosen for the tail. This gesture has no features.
*
*	@return an empty String if no gesture is detected or "Tail=initated" or
*		"Tail=ended" gesture if one is detected.
*/
String GestureHandler::getTailTouch()
{
	if(rawData.tail>tailThreshold&&!isTailTouching)
	{
		isTailTouching = true;
		return "Tail=initated!";
	}
	if(rawData.tail<tailThreshold&&isTailTouching)
	{
		isTailTouching = false;
		return "Tail=ended!";
	}
	if(isTailTouching)
	{
		return "Tail=initiated!";
	}
	return "";
}

/**
* The method to check for mouth touches.
*	This method uses the standard gesture detection algorithm which
*	is explained in more depth on the @ref algorithms page. The test
*	condition for this is if the measured capacitance exceeds the 
*	threshold chosen for the mouth. This gesture has no features.
*
*	@return an empty String if no gesture is detected or "Kiss=initated" or
*		"Kiss=ended" gesture if one is detected.
*/
String GestureHandler::getKiss()
{
	if(rawData.mouth>kissThreshold&&!isKissing)
	{
		isKissing = true;
		return "Kiss=initated!";
	}
	if(rawData.mouth<kissThreshold&&isKissing)
	{
		isKissing = false;
		return "Kiss=ended!";
	}
	if(isKissing)
	{
		return "Kiss=initiated!";
	}
	return "";
}

/**
* A method to store data that needs to be remembered for the GestureHandler
*	This method only saves acceleration data now but could be used
*	for more data if needed in the future.
*/
void GestureHandler::update()
{
	//Copy over the rest of the data
	copyInt(pastAccel, rawData.accel, 3);
}