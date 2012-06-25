/*Gesture.h
	The interface definition for the Gesture class which will run the three main processes

CHANGE LOG	
***********************************************************************************************
-file created 31/05/12
-primary interface design and definition 31/05/12
-addition of general.h includes 01/06/12
***********************************************************************************************
*/

#ifndef Gesture_H_
#define Gesture_H_
#include "Arduino.h"

//Any neccessary includes for the gesture set
#include "SensorHandler.h"
#include "GestureHandler.h"
//#include "Sender.h"
#include "General.h"
#include <string>
//End of the includes

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

#ifdef __cplusplus
} //extern "C"
#endif


//Create a class  for the arduino to use
class Gesture
{
	//The three methods refer to the three stages to the process. These will be repeated over and over
	public:
					
		sensorData collect(SensorHandler handler);
		
		String process(GestureHandler *pHandler, sensorData data);
		
		void send(String gestures);		

};

#endif
		
		
		
