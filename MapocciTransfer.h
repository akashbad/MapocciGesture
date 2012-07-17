/*MapocciTransfer.h
	The interface for a set of transfer function for a variety of gestures, taking measured features
	and transfering them to the "force" parameter used to measure the Mapocci arousal
	based on the gesture

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-primary interface design and definition 01/06/12
***********************************************************************************************
*/

#ifndef MapocciTransfer_H_
#define MapocciTransfer_H_

#include "Arduino.h"
#include "General.h"


#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

#ifdef __cplusplus
} //extern "C"
#endif

class MapocciTransfer
{
	public:
		MapocciTransfer();
		//The transfer function for shaking
		float transferShaking(float amplitude);
		//The transfer functions for rotation
		float transferFlipping(float velocity);
		float transferRolling(float velocity);
		float transferSpinning(float velocity);
		float transferTouching(float pressure);
		float transferTouchArea(float std);
		float transferTail(int touch);
		float transferKiss(int mouth);
		
	private:
		float minForce;
		float maxForce;
		float minShaking;
		float maxShaking;
};
#endif
		
		
		
