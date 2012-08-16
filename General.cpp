/*General.cpp
	The definition of general functions used to enable some capability
*/

#include "General.h"
#include "Arduino.h"


/**
* A method to copy an array of ints into another array
*
*	@param write[] an array of ints to have data copied into it,
*		it must be at least of size length and will have previous
*		data overwritten
*	@param read[] an array of ints to have the data read from,
*		not mutated, must have size length at minimum
*	@param length an int representing the amount of data to be
*		copied
*/
void copyInt(int write[], int read[], int length)
{
	for(int i=0; i<length; i++)
	{
		write[i] = read[i];
	}
}

/**
* A method to copy an array of floats into another array
*
*	@param write[] an array of floats to have data copied into it,
*		it must be at least of size length and will have previous
*		data overwritten
*	@param read[] an array of floats to have the data read from,
*		not mutated, must have size length at minimum
*	@param length an int representing the amount of data to be
*		copied
*/
void copyFloat(float write[], float read[], int length)
{
	for(int i=0; i<length; i++)
	{
		write[i] = read[i];
	}
}

/**
* A method to obtain the maximum value within an array of floats
*
*	@param data[] the array of floats to be searched, of size length
*		at least
*	@param length an int representing the size of the array to be searched
*	@return the maximum float in the data
*/
float floatMax(float data[], int length)
{
	float max = data[0];
	for(int i=1; i<length; i++)
	{
		max = data[i]>max ? data[i] : max;
	}
	return max;
}

/**
* A method to obtain the maximum value within an array of ints
*
*	@param data[] the array of ints to be searched, of size length
*		at least
*	@param length an int representing the size of the array to be searched
*	@return the maximum int in the data
*/
int intMax(int data[], int length)
{
	int max = data[0];
	for(int i=1; i<length; i++)
	{
		max = data[i]>max ? data[i] : max;
	}
	return max;
}

/**
* A method to obtain the sum of data in an array of ints
*
*	@param data[] the array of ints to be summed, of size length
*		at least
*	@param length an int representing the size of the array to be summed
*	@return the sum of ints in the data
*/
int intSum(int data[], int length)
{
	int sum = 0;
	for(int i=0; i<length; i++)
	{
		sum += data[i];
	}
	return sum;
}

/**
* A method to obtain the sum of data in an array of floats
*
*	@param data[] the array of floats to be summed, of size length
*		at least
*	@param length an int representing the size of the array to be summed
*	@return the sum of floats in the data
*/
float floatSum(float data[], int length)
{
	float sum = 0;
	for(int i=0; i<length; i++)
	{
		sum += data[i];
	}
	return sum;
}

/**
* A method to obtain the float valued mean of an array of data.
*	This mean represents the mean of the indicies using the values
*	at each index as the weight, thus the returned value will always
*	be between 0 and length.
*
*	@param data[] the array of ints to be meaned, of size length
*		at least
*	@param length an int representing the size of the array to be meaned
*	@return the mean index of the data with the data as the weigths
*	@see intSum()
*/
float fMean(int data[], int length)
{
	int sum = intSum(data, length);
	float mean = 0.0;
	for(int i=0; i<length; i++)
	{
		mean+= data[i]*(float)i;
	}
	mean = mean/sum;
	if(mean != mean) return 0;
	return mean;
}

/**
* A method to obtain the float valued standard deviation of an array of data.
*	This std represents the std of the indicies using the values
*	at each index as the weight and the value of fMean as the mean
*
*	@param data[] the array of ints to use, of size length
*		at least
*	@param length an int representing the size of the array to use
*	@return the standard deviation of the data from the mean returned by fMean
*	@see fMean()
*	@see intSum()
*/
float fStd(int data[], int length)
{
	int sum = intSum(data, length);
	float mean = fMean(data, length);
	float variance = 0.0;
	for(int i=0; i<length; i++)
	{
		variance += data[i]*pow(i-mean,2);
	}
	return sqrt(variance/sum);
}

/**
* A method to return the mode of the given data. The mode actually
*	represents the index of the maximum in the set of data, considered
* 	to be a mode here because of the definition of mean and std above.
*
*	@param data[] the array of ints to use, of size length
*		at least
*	@param length an int representing the size of the array to use
*	@return the location of the maximum, the mode by our definition
*/
int iMode(int data[], int length)
{
	int mode = 0;
	for(int i=1; i<length; i++)
	{
		mode = data[i]>data[mode] ? i : mode;
	}
	return mode;
}

/**
* A method of converting a float to a string
*
*	@param input a float to be converted
*	@return a string representing the input float to 3 decimals of precision
*/
String ftos(float input)
{
	char temp[10];
	dtostrf(input, 0, 3, temp);
	return String(temp);
}
	
	
	
		