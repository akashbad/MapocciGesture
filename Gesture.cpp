/*Gesture.cpp
	The main thread for sensor input and gesture detection in the Mapocci System

CHANGE LOG	
***********************************************************************************************
-file created 31/05/12
-primary implementation of collect and process methods 31/05/12
-update to use of copyInt 01/06/12
***********************************************************************************************
*/

#include "Arduino.h"
#include "SensorHandler.h"
#include "GestureHandler.h"
//#include "Sender.h"
#include "General.h"
#include "Gesture.h"
#include <string>


//The method to collect the data from all of the sensors
sensorData Gesture::collect(SensorHandler handler)
{					
	sensorData data;
	//Go through each set of sensors, collect the data from the ADC and then
	handler.getAccelData(data.accel);
	handler.getGyroData(data.gyro);
	handler.getTorsoData(data.torso);
	handler.getStomachData(data.stomach);
	handler.getBottomData(data.bottom);
	handler.getLegsData(data.legs);
	data.mouth = handler.getMouthData();
	return (data);
}

String Gesture::process(GestureHandler *pHandler, sensorData data)
{
	String results = "";
	//Go through each of the gesture models and decide if the data fits for any of them
	//Some require the data from only one sensor, others for more
	GestureHandler handler = *pHandler;
	handler.report(data);
	results+= handler.getShaking();
	results+= handler.getRotating();
	results+= handler.getFalling();
	results+= handler.getUpsideDown();
	results+= handler.getTouching();
	results+= handler.getHandHold();
	results+= handler.getKiss();
	handler.update();
	*pHandler = handler;
	return (results);
}

void Gesture::send(String gestures)
{
	//A process of sending the given gestures to the other model, will figure out how later
	return;
}
	
	
	
		