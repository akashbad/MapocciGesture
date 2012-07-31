/*Gesture.cpp
	The main thread for sensor input and gesture detection in the Mapocci System
*/

#include "Arduino.h"
#include "SensorHandler.h"
#include "GestureHandler.h"
//#include "Sender.h"
#include "General.h"
#include "Gesture.h"
#include <string>


/**
* This is the method within the Gesture that is responsible for aquiring
*	all of the data from within a sensor. The current linear design of
* 	the system means this should be the first method called each cycle
*	but a multithreaded approach could allow this method to run
*	asynchronously with the processing
*
*	@param handler an instance of the SensorHandler class which
*		is responsible for dealing with connected hardware. This
*		handler should have already been set up with the correct
*		input pins during arduino initialization
*	@return an instance of the sensorData class which contains
*		nearly raw data from the sensors, the exceptions include
*		a few simple numeric modifications to touch sensor data
*/
sensorData Gesture::collect(SensorHandler handler)
{					
	sensorData data;
	//Go through each set of sensors, collect the data from the ADC and then
	handler.getAccelData(data.accel);
	handler.getGyroData(data.gyro);
	handler.getTorsoData(data.torso);
	handler.getStomachData(data.stomach);
	handler.getBottomData(data.bottom);
	data.tail = handler.getTailData();
	data.mouth = handler.getMouthData();
	handler.getBodyTouches(data.bodyTouches);
	return data;
}

/**
* This method that is responsible for the processing piece of the model.
*	In this step the raw data will be set through a series of algorithms
*	in search of each identifiable gesture. Each discovered gesture will
*	be appended to a master string which is returned. This string
*	represents the sum of all gestures occuring this cycle.	
*
*	@param *pHandler a pointer to an instance of the GestureHandler
*		class. We pass in a pointer here so we can modify the instance
*		allowing us to store the state of gestures within that class.
*		pHandler is modified in this method
*	@param data an instance of a sensorData struct containing the 
*		new set of raw data to be processed. This is usually the output
*		of the collect() method. 
*	@return a String which has each identified gesture appended to it
*		according to the grammar
*/
String Gesture::process(GestureHandler *pHandler, sensorData data)
{
	String results = "";
	// Go through each of the gesture models and decide if the data fits for any of them
	// Some require the data from only one sensor, others for more
	GestureHandler handler = *pHandler;
	handler.report(data);
	results+= handler.getShaking();
	results+= handler.getRotating();
	results+= handler.getFalling();
	results+= handler.getUpsideDown();
	// results+= handler.getTorso();
	// results+= handler.getBottom();
	// results+= handler.getStomach();
	results+= handler.getTailTouch();
	results+= handler.getKiss();
	handler.update();
	*pHandler = handler;
	return (results);
}

/**
* An unimplimented method as of yet that exists in case a more complicated
*	message sending framework is required for the list of gestures recognized
*	this cycle
*
*	@param gestures the string containing the list of gestures identified
*		this cycle
*/
void Gesture::send(String gestures)
{
	//A process of sending the given gestures to the other model, will figure out how later
	return;
}
	
	
	
		