/*GestureHandler.h
	The interface definition for the GestureHandler class which will take input data and 
*/

#ifndef GestureHandler_H_
#define GestureHandler_H_
#include "Arduino.h"

//Any neccessary includes for the gesture set
#include "General.h"
#include "MapocciTransfer.h"
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
* The class which is responsible for the processing of all gestures.
* 	This class is responsible for step 2 of the Mapocci gesture model.
* 	The methods of this class contain all of the important algorithms
*	used to classify gestures. There is one public method for each gesture
*	and methods to feed the handler new data and to update its internal
*	state. Main changes to future Mapocci's should be here. Each method
*	documentation should contain a layout of the current algorithm used.
*/
class GestureHandler
{
	//The public methods for the GestureHandler which involve analyzing the raw data to see
	//if it qualifies for one of the gestures
	public:
		GestureHandler(MapocciTransfer model);
		
		void report(sensorData data);
		
		String getShaking();
		String getRotating();
		String getFalling();
		String getUpsideDown();
		String getTorso();
		String getBottom();
		String getStomach();
		String getTailTouch();
		String getKiss();
		
		void update();

		//Kiss detection helper variables
		int kissThreshold; ///<The capacitance threshold for the mouth
		bool isKissing; ///<The boolean determining mouth touch state

	//private:
		sensorData rawData;			///<The storage place for all raw data, usually collected by a SensorHandler
		MapocciTransfer transfer;	///<The transfer function object used in converting features to forces
		
		//Helper methods for shaking
		bool testShake(int current[], float threshold);
		
		//Helper variables for shaking, save state and such
		int pastAccel[3];	///<A storage place for previous acceleration data
		float filteredShake;	///<A variable used to high pass filter the shake amplitude
		bool isShaking;	///<The boolean determining shaking state
		int shakeCount;	///<A counter used in hysterisis of shaking
		float shakingUpper; ///<The upper threshold for shaking
		float shakingLower;	///<The lower threshold for shaking
		float maxShaking;	///<The ceiling of shaking amplitudes
		int accelerometerNominal;	///<The nominal acceleration magnitude, corresponds to g
		
		//Helper methods for rotation
		bool checkRotation(int axis);
		
		//Variables for rotation
		int nominalRotation;	///<The nominal value for no rotation on the gyroscope
		bool isSpinning;	///<The boolean determining spinning state
		bool isFlipping;	///<The boolean determining flipping state
		bool isRolling;		///<The boolean determining rolling state
		int rotationThreshold;	///<The threshold for rotation to be valid
		
		//Helper variables for falling
		bool isFalling; ///<The boolean determining falling state
		int fallThresholdHigh; ///<The the upper threshold for falling
		int fallThresholdLow; ///<The lower threshold for falling

		//Helper variable for upside down
		int upsideDownThreshold; ///<The threshold used for upside down detection
		bool isUpsideDown;	///<The boolean determining upsidedown state
		
		//Helper function for the touchPads
		void getTouchPadFeatures();

		String getTouchGestureString(int i);

		//Helper variables for each of the touchPads
		float means[3]; ///<The mean touch locations of each fabric sensor
		float stds[3]; ///<The standard deviations of touch on each fabric sensor
		int modes[3]; ///<The location of the strongest touch on each fabric sensor
		int sums[3]; ///<The sum of the pressure values for each fabric sensor
		
		int oldTorso[16]; ///<An array used to high pass filter torso data
		int oldBottom[9];	///<An array used to high pass filter bottom data
		int oldStomach[4];	///<An array used to high pass filter stomach data
	
		//Tail detection helper variables
		int tailThreshold;	///<The capacitance threshold for the tail
		bool isTailTouching; ///<The boolean determining tail touch state
		
		bool isTorso;	///<The boolean determining torso touch state
		int torsoCount;	///<A counter used in hysterisis of torso touches
		bool isBottom;	///<The boolean determining bottom touch state
		int bottomCount; ///<A counter used in hysterisis of bottom touches
		bool isStomach;	///<The boolean determining stomach touch state
		int stomachCount; ///<A counter used in hysterisis of stomach touches
		
		float randomMemoryPlaceholder0; ///<For some reason we need this to work...
		float randomMemoryPlaceholder1; ///<For some reason we need this to work...
};

#endif
		
		
		
