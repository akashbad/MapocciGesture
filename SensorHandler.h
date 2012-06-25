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

//End of the includes

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

#ifdef __cplusplus
} //extern "C"
#endif


//Create a class  for the SensorHandler to use
class SensorHandler
{
	//The methods needed to read in the proper data from each of the adc pins, there is a single
	//public method for each of the sensors and private methods to allow for proper handling
	public:
		SensorHandler(int Apins[], int Gpins[], int FpinT, int FpinS, int FpinB, 
					int ConTpins[], int ConSpins[], int ConBpins[], int TLpins[], int TpinM);
					
					
		void getAccelData(int data[]);
		void getGyroData(int data[]);
		void getTorsoData(int data[]);
		void getStomachData(int data[]);
		void getBottomData(int data[]);
		void getLegsData(int data[]);
		int getMouthData();
		
	//Methods to multiplex the sensors that share an adc pin and variables to hold pin numbers
	private:
		
		void torsoMultiplexer(int node);
		void stomachMultiplexer(int node);
		void bottomMultiplexer(int node);
		
		int removeDeadPixels(int data[], int index, int size);
		int deadPixThreshold;
		
		int accelPins[3];
		int gyroPins[3];
		int torsoPin;
		int stomachPin;
		int bottomPin;
		int torsoControl[4];
		int stomachControl[4];
		int bottomControl[4];
		int legPins[4];
		int mouthPin;
};

#endif
		
		
		
