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
		String getUpsideDown();
		String getTouching();
		String getTailTouch();
		String getKiss();
		
		void update();
		
	private:
		sensorData rawData;
		MapocciTransfer transfer;
		
		//Helper methods for shaking
		bool testShake(int current[], float threshold);
		
		//Helper variables for shaking, save state and such
		int pastAccel[3];
		float filteredShake;
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
		int fallCount;
		
		//Helper variable for upside down
		bool isUpsideDown;
		
		//Helper function for the touchPads
		void getTouchPadFeatures(float means[], float stds[], int modes[]);
		
		//Helper variables for each of the touchPads
		float means[3];
		float stds[3];
		int modes[3];
		int sums[3];
		
		int oldTorso[16];
		int oldBottom[9];
		int oldStomach[4];
		
		//Touch detection variables
		int touchCount[3];
		bool isTouching[3];
		
		
		//Tail detection helper variables
		int tailThreshold;
		
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
		
		
		
