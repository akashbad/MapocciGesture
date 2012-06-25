/*General.h
	The interface definition for a General list of functions to be used by all programs

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-primary interface design and definition 01/06/12
***********************************************************************************************
*/

#ifndef General_H_
#define General_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

#ifdef __cplusplus
} //extern "C"
#endif

//The sensorData struct which is used to store data
typedef struct {
	int accel[3];
	int gyro[3];
	int torso[16];
	int stomach[16];
	int bottom[16];
	int legs[4];
	int mouth;
} sensorData;

//The int array copy method
	void copyInt(int write[], int read[], int length);
	
//The float array copy method
	void copyFloat(float write[], float read[], int length);
	
//The float maximum method

	float floatMax(float data[], int length);

//The int maximum method
	int intMax(int data[], int length);
	
//The int sum method
	int intSum(int data[], int length);
	
//The float sum method
	float floatSum(float data[], int length);
	
//The float valued float mean method
	float ftfMean(float data[], int length);
	
//The float values float std method
	float ftfStd(float data[], int length);

//The float to int mode method
	int ftiMode(float data[], int length);
#endif


		
		
		
