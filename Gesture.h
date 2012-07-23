/*Gesture.h
	The interface definition for the Gesture class which will run the three main processes
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


/**
* This is the top level class for the Mapocci gesture recognition system.
*	An instance of this class is created per Mapocci and the three
*	methods within this class segment the three major steps of the
*	gesture recognition process: data collection, data processing,
*	and gesture sending.
*/
class Gesture
{
	//The three methods refer to the three stages to the process. These will be repeated over and over
	public:
					
		sensorData collect(SensorHandler handler);
		
		String process(GestureHandler *pHandler, sensorData data);
		
		void send(String gestures);		

};

#endif
		
		
		
