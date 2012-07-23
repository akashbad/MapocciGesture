/*MapocciTransfer.cpp
	The class that will be used to transfer a set of measured features into a single float value
	used by the mapocci emotional model
*/

#include "Arduino.h"
#include "General.h"
#include "MapocciTransfer.h"


/**
* A constructor for the MapocciTransfer class. Uses predifed minimum
* and maximum values for its output and for
* each type of feature. This allows it to produce accurate mappings
*/
MapocciTransfer::MapocciTransfer()
{
	minForce = 0.00;
	maxForce = 100.0;
	minShaking = 40.0;
	maxShaking = 220.0;
	minRotating = 100;
	maxRotating = 512;
}

/**
* A method to transfer shaking amplitude to Mapocci Force
*
*	@param amplitude the amplitude of the given shake
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferShaking(float amplitude)
{
		float force = constrain(amplitude, minShaking, maxShaking);
		return map(force, minShaking, maxShaking, minForce, maxForce);
}

/**
* A method to transfer flipping velocity to Mapocci Force
*
*	@param velocity the velocity of the given flip
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferFlipping(int velocity)
{
	return map(velocity, minRotating, maxRotating, minForce, maxForce);
}

/**
* A method to transfer rolling velocity to Mapocci Force
*
*	@param velocity the velocity of the given roll
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferRolling(int velocity)
{
	return map(velocity, minRotating, maxRotating, minForce, maxForce);
}

/**
* A method to transfer spinning velocity to Mapocci Force
*
*	@param velocity the velocity of the given spin
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferSpinning(int velocity)
{
	return map(velocity, minRotating, maxRotating, minForce, maxForce);
}

/**
* A method to transfer touching pressure to Mapocci Force
*
*	@param pressure the pressure of the given touch
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferTouching(float pressure)
{
	return pressure;
}
/**
* A method to transfer touching area to Mapocci
*
*	@param std the std of the given touch
*	@return a float force value mapped from 0 to 100
*/
float MapocciTransfer::transferTouchArea(float std)
{
	return std;
}

	
	
	
		