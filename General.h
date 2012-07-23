/*General.h
	The interface definition for a General list of functions to be used by all programs
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

/**
* The struct to hold the raw data read by our sensors
*/
typedef struct {
	int accel[3];	///<An array to hold acceleration data on 3 axis: x,y,z
	int gyro[3]; 	///<An array to hold gyroscope data on 3 axis: y,p,r
	int torso[16];	///<An array to hold touch sensor data on the 16 torso sensors
	int stomach[14];///<An array to hold touch sensor data on the 14 stomach sensors
	int bottom[9];	///<An array to hold touch sensor data on the 9 bottom sensors
	int tail;		///<An int to hold capacitive sensor data for the tail
	int mouth;		///<An int to hold capacitive sensor data for the mouth
	int bodyTouches[3];	///<An array to capacitive sensor data for the torso, bottom, and stomach
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
	
//The float valued mean method
	float fMean(int data[], int length);
	
//The float valued std method
	float fStd(int data[], int length);

//The int valued mode method
	int iMode(int data[], int length);
	
//The float to string method
	String ftos(float input);
#endif


		
		
		
