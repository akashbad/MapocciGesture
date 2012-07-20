/*MapocciTransfer.cpp
	The class that will be used to transfer a set of measured features into a single float value
	used by the mapocci emotional model

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-primary implementation of copyInt method 01/06/12
***********************************************************************************************
*/

#include "Arduino.h"
#include "General.h"
#include "MapocciTransfer.h"



MapocciTransfer::MapocciTransfer()
{
	minForce = 0.00;
	maxForce = 100.0;
	minShaking = 40.0;
	maxShaking = 220.0;
}

float MapocciTransfer::transferShaking(float amplitude)
{
		float force = constrain(amplitude, minShaking, maxShaking);
		return map(force, minShaking, maxShaking, minForce, maxForce);
}

float MapocciTransfer::transferFlipping(float velocity)
{
	return map(velocity, 50.0, 512, minForce, maxForce);
}

float MapocciTransfer::transferRolling(float velocity)
{
	return map(velocity, 50.0, 512.0, minForce, maxForce);
}

float MapocciTransfer::transferSpinning(float velocity)
{
	return map(velocity, 50.0, 512, minForce, maxForce);
}

float MapocciTransfer::transferTouching(float pressure)
{
	return (float)pressure;
}
float MapocciTransfer::transferTouchArea(float std)
{
	return (float)std;
}

	
	
	
		