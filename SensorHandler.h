/*SensorHandler.h
	The interface definition for the SensorHandler class which involves the collection of raw data 
	from all of the sensors

CHANGE LOG	
***********************************************************************************************
-file created 31/05/12
-primary interface design and definition 31/05/12
-addition of general.h includes 01/06/12
***********************************************************************************************
*/

#ifndef SensorHandler_H_
#define SensorHandler_H_
#include "Arduino.h"
#include "General.h"
#include "wiring_private.h"
#include "pins_arduino.h"


//End of the includes

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
int readCapacitivePin(int pinToMeasure);

#ifdef __cplusplus
} //extern "C"
#endif


/**
* The class responsible for collecting all the raw data. This is  the
*	only class that interacts with the sensors. Performing all of the 
*	data collection and returning it. The methods of this class are
*	called by the instance of Gesture which uses stores the values
*	produced within a sensorData instance.*/
class SensorHandler
{
	//The methods needed to read in the proper data from each of the adc pins, there is a single
	//public method for each of the sensors and private methods to allow for proper handling
	public:
		SensorHandler(int Apins[], int Gpins[], int FpinT, int FpinS, int FpinB, 
					int ConTpins[], int ConSpins[], int ConBpins[], int tPin, int mPin, int btPins[]);
					
					
		void getAccelData(int data[]);
		void getGyroData(int data[]);
		void getTorsoData(int data[]);
		void getStomachData(int data[]);
		void getBottomData(int data[]);
		int getTailData();
		int getMouthData();
		void getBodyTouches(int data[]);
		
	//Methods to multiplex the sensors that share an adc pin and variables to hold pin numbers
	private:
		
		void fabricMultiplexer(int node, int control[]);
		int removeDeadPixels(int data[], int index, int size);
		int deadPixThreshold; ///<The threshold for a dead fabric pixel
		int fabricScaleFactor; ///<The amount to be subtracted from each fabric
		


		int accelPins[3];	///<An array to store the accelerometer pin numbers
		int gyroPins[3];	///<An array to store the gyroscope pin numbers
		int torsoPin;		///<The analog input pin connected to the torso circuitry
		int stomachPin;		///<The analog input pin connected to the stomach circuitry
		int bottomPin;		///<The analog input pin connected to the bottom circuitry
		int torsoControl[4];	///<An array to store pins connected to the torso multiplexer
		int stomachControl[4];	///<An array to store pins connected to the stomach multiplexer
		int bottomControl[4];	///<An array to store pins connected to the bottom multiplexer
		int tailPin;		///<The digital pin connected to the tail
		int mouthPin;		///<The digital pin connected to the mouth
		int bodyTouchPins[3];	///<An array to store digital pin numbers connected to the capacitive body sensors
};

#endif
		
		
		
