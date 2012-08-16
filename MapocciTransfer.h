/*MapocciTransfer.h
	The interface for a set of transfer function for a variety of gestures, taking measured features
	and transfering them to the "force" parameter used to measure the Mapocci arousal
	based on the gesture
*/

#ifndef MapocciTransfer_H_
#define MapocciTransfer_H_

#include "Arduino.h"
#include "General.h"


#ifdef __cplusplus`
extern "C" {
#endif
void loop();
void setup();

#ifdef __cplusplus
} //extern "C"
#endif

/** 
* The class responsible for mapping feature values.
*	This class was created to allow for the extension of the
*	Mapocci to more complicated mathematical models. It takes in
*	a set of features from each gesture and returns a single value.
*	As of now the class acts to simple map values but could be
*	implemented with more complicated transfer functions.
*	
*/
class MapocciTransfer
{
	public:
		MapocciTransfer();
		//The transfer function for shaking
		float transferShaking(float amplitude);
		//The transfer functions for rotation
		float transferFlipping(int velocity);
		float transferRolling(int velocity);
		float transferSpinning(int velocity);
		float transferTouching(float pressure);
		float transferTouchArea(float std);
		
	private:
		float minForce; ///<The minimum force value for Mapocci gestures
		float maxForce; ///<The maximum force value for Mapocci gestures
		float minShaking; ///<The minimum measurable shaking value
		float maxShaking; ///<The maximum shaking value
		float minRotating; ///<The minimum rotation velocity
		float maxRotating; ///<The maxium rotation velocity
		float minPressure; ///<The minimum fabric sensor pressure
		float maxPressure; ///<The maxium fabric sensor pressure
};
#endif

		
		
		
