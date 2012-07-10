/*General.cpp
	The definition of general functions used to enable some capability

CHANGE LOG	
***********************************************************************************************
-file created 01/06/12
-primary implementation of copyInt method 01/06/12
***********************************************************************************************
*/

#include "General.h"
#include "Arduino.h"


//The copyInt method
void copyInt(int write[], int read[], int length)
{
	for(int i=0; i<length; i++)
	{
		write[i] = read[i];
	}
}

//The copyFloat method
void copyFloat(float write[], float read[], int length)
{
	for(int i=0; i<length; i++)
	{
		write[i] = read[i];
	}
}

//The float max method
float floatMax(float data[], int length)
{
	float max = data[0];
	for(int i=1; i<length; i++)
	{
		max = data[i]>max ? data[i] : max;
	}
	return max;
}

//The int max method
int intMax(int data[], int length)
{
	int max = data[0];
	for(int i=1; i<length; i++)
	{
		max = data[i]>max ? data[i] : max;
	}
	return max;
}

//The int sum method
int intSum(int data[], int length)
{
	int sum = 0;
	for(int i=0; i<length; i++)
	{
		sum += data[i];
	}
	return sum;
}

//The float sum method
float floatSum(float data[], int length)
{
	float sum = 0;
	for(int i=0; i<length; i++)
	{
		sum += data[i];
	}
	return sum;
}

//The float valued int mean method
float ftfMean(int data[], int length)
{
	int sum = intSum(data, length);
	float mean = 0.0;
	for(int i=0; i<length; i++)
	{
		//Serial.println(data[i]);
		mean+= data[i]*(float)i;
	}
	mean = mean/sum;
	return mean;
}

//The float values int std method
float ftfStd(int data[], int length)
{
	int sum = intSum(data, length);
	float mean = ftfMean(data, length);
	float variance = 0.0;
	for(int i=0; i<length; i++)
	{
		variance += data[i]*pow(i-mean,2);
	}
	return sqrt(variance/sum);
}

//The int mode method
int ftiMode(int data[], int length)
{
	int mode = 0;
	for(int i=1; i<length; i++)
	{
		mode = data[i]>data[mode] ? i : mode;
	}
	return mode;
}

//The float to string method
String ftos(float input)
{
	char temp[10];
	dtostrf(input, 0, 3, temp);
	return String(temp);
}
	
	
	
		