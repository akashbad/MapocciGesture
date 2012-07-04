/*GestureHandler.h
	The interface definition for the GestureHandler class which will take input data and 

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-listing of public methods and of shaking private variables and methods 01/06/31
***********************************************************************************************
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

//Create a class  for the GestureHandler to use
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
		String getHug();
		String getPet();
		String getScratch();
		String getHandHold();
		String getFeed();
		String getPoke();
		String getSlap();
		String getKiss();
		
		void update();
		
	private:
		sensorData rawData;
		MapocciTransfer transfer;
		
		//Helper methods for shaking
		bool testShake(int current[], float threshold);
		String getShakeGesture(float amplitude);
		
		//Helper variables for shaking, save state and such
		int pastAccel[3];
		float strongestShake;
		bool isShaking;
		int shakeCount;
		int shakePointer;
		float shakingUpper;
		float shakingLower;
		float maxShaking;
		int accelerometerNominal;
		
		//Helper methods for rotation
		bool checkRotation(int axis);
		
		//Variables for rotation
		int nominalRotation;
		bool isSpinning;
		bool isFlipping;
		bool isRolling;
		int rotationThreshold;
		
		//Helper variables for falling
		bool isFalling;
		
		//Helper function for the touchPads
		void getTouchPadFeatures(float means[], float stds[], int modes[]);
		
		//Helper variables for each of the touchPads
		float means[4];
		float stds[4];
		int modes[4];
		
		float oldTorso[16];
		float oldStomach[16];
		float oldBottom[16];
		
		float lastMeans[4];
		float lastStds[4];
		int lastModes[4];
		
		//Hug detection variables
		int hugModeThresholdHigh;
		float hugStdThresholdHigh;
		int hugModeThresholdLow;
		float hugStdThresholdLow;
		float strongestHugStd;
		int strongestHugMode;
		int hugCount;
		bool isHugging;
		
		//Pet helper functions
		String getPetGesture(int position, int mode, float speed);
		
		//Scratch detection variables
		float petThresholdHigh;
		float petThresholdLow;
		float petCount[3];
		float filtered[3];
		bool isPetting;
		int strongestPetMode[3];
		float fastestPetSpeed[3];
		
		//Hand hold helper variables
		float legsThreshold;
		bool isHolding[4];
		int maxTouch[4];
		int holdCount[4];
		
		//Poke detection helper variables
		float pokeStdThreshold;
		int pokeModeThreshold;
		
		//Slap detection helper variables
		float slapStdThreshold;
		int slapModeThreshold;
		
		//Kiss detection helper variables
		int kissThreshold;
		
};

#endif
		
		
		
