/*SensorHandler.cpp
	The  class responsible for collecting all raw data from the sensors

CHANGE LOG	
***********************************************************************************************
-file created 31/05/12
-implementation of all methods and use of copyInt 01/06/12
***********************************************************************************************
*/

#include "Arduino.h"
#include "General.h"
#include "SensorHandler.h"



SensorHandler::SensorHandler(int Apins[], int Gpins[], int FpinT, int FpinS, int FpinB, 
					int ConTpins[], int ConSpins[], int ConBpins[], int TLpins[], int TpinM)
{
		//Assign all of the pins to their correct variables so that handler can read from the
		//correct place
		copyInt(accelPins, Apins, 3);
		copyInt(gyroPins, Gpins, 3);
		torsoPin = FpinT;
		stomachPin = FpinS;
		bottomPin = FpinB;
		copyInt(torsoControl, ConTpins, 4);
		copyInt(stomachControl, ConSpins, 4);
		copyInt(bottomControl, ConBpins, 4);
		copyInt(legPins, TLpins, 4);
		mouthPin = TpinM;
		
		deadPixThreshold = 800;
		
		for(int i = 0; i<4; i++)
		{
			pinMode(torsoControl[i], OUTPUT);
		}
		for(int i = 0; i<4; i++)
		{
			pinMode(stomachControl[i], OUTPUT);
		}
		for(int i = 0; i<4; i++)
		{
			pinMode(bottomControl[i], OUTPUT);
		}
}

void SensorHandler::getAccelData(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = analogRead(accelPins[i]);
	}
}

void SensorHandler::getGyroData(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = analogRead(gyroPins[i]);
	}
}

void SensorHandler::getTorsoData(int data[])
{
	for(int i=0; i<16; i++)
	{
		torsoMultiplexer(i);
		int temp = analogRead(torsoPin)-300;
		data[i] = temp > 0 ? temp : 0;
	}
	for(int i=0; i<16; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
}

void SensorHandler::getStomachData(int data[])
{

	for(int i=0; i<16; i++)
	{
		stomachMultiplexer(i);
		
		int temp = analogRead(stomachPin);//-300;
		data[i] = temp > 0 ? temp : 0;
		
	}
	for(int i=0; i<16; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
	
}

void SensorHandler::getBottomData(int data[])
{
	for(int i=0; i<8; i++)
	{
		bottomMultiplexer(i);
		int temp = analogRead(bottomPin)-300;
		data[i] = temp > 0 ? temp : 0;
	}
	for(int i=8; i<16; i++)
	{
		data[i] = 0;
	}
}

int SensorHandler::removeDeadPixels(int data[], int index, int size)
{
	int head = index < size-1 ? index+1 : index-2;
	int tail = index > 0 ? index-1 : index+2;
	if(head == index)
	{
		head = tail;
	}
	if(data[index] > deadPixThreshold)
	{
		return (data[head]+data[tail]) /2;
	}
	else return data[index];
}

void SensorHandler::getLegsData(int data[])
{
	for(int i=0; i<4; i++)
	{
		data[i] = analogRead(legPins[i]);
	}
}

int SensorHandler::getMouthData()
{
	return (analogRead(mouthPin));
}

void SensorHandler::torsoMultiplexer(int node)
{
	digitalWrite(torsoControl[0], node%2==0 ? LOW : HIGH);
	digitalWrite(torsoControl[1], node/2%2==0 ? LOW : HIGH);
	digitalWrite(torsoControl[2], node/4%2==0 ? LOW : HIGH);
	digitalWrite(torsoControl[3], node/8%2==0 ? LOW : HIGH);
}

void SensorHandler::stomachMultiplexer(int node)
{
	digitalWrite(stomachControl[0], node%2==0 ? LOW : HIGH);
	digitalWrite(stomachControl[1], node/2%2==0 ? LOW : HIGH);
	digitalWrite(stomachControl[2], node/4%2==0 ? LOW : HIGH);
	digitalWrite(stomachControl[3], node/8%2==0 ? LOW : HIGH);
}

void SensorHandler::bottomMultiplexer(int node)
{
	digitalWrite(bottomControl[0], node%2==0 ? LOW : HIGH);
	digitalWrite(bottomControl[1], node/2%2==0 ? LOW : HIGH);
	digitalWrite(bottomControl[2], node/4%2==0 ? LOW : HIGH);
	digitalWrite(bottomControl[3], node/8%2==0 ? LOW : HIGH);
}
	
	
	
		