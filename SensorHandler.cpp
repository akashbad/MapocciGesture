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
#include "wiring_private.h"
#include "pins_arduino.h"



SensorHandler::SensorHandler(int Apins[], int Gpins[], int FpinT, int FpinS, int FpinB, 
					int ConTpins[], int ConSpins[], int ConBpins[], int tPin, int mPin, int btPins[])
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
		tailPin = tPin;
		mouthPin = mPin;
		copyInt(bodyTouchPins, btPins,3);
		
		deadPixThreshold = 250;
		
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
		int temp = analogRead(torsoPin)-270;
		data[i] = temp > 0 ? temp : 0;
	}
	for(int i=0; i<16; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
}

void SensorHandler::getStomachData(int data[])
{

	for(int i=1; i<15; i++)
	{
		stomachMultiplexer(i);
		int temp = analogRead(stomachPin)-270;
		data[i-1] = temp > 0 ? temp : 0;		
	}
	for(int i=0; i<14; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
	
}

void SensorHandler::getBottomData(int data[])
{
	for(int i=4; i<13; i++)
	{
		bottomMultiplexer(i);
		int temp = analogRead(bottomPin)-270;
		data[i-4] = temp > 0 ? temp : 0;
	}
	for(int i=0; i<9; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
}

int SensorHandler::removeDeadPixels(int data[], int index, int size)
{
	// int head = index < size-1 ? index+1 : index-2;
	// int tail = index > 0 ? index-1 : index+2;
	// if(head == index)
	// {
		// head = tail;
	// }
	// if(data[index] > deadPixThreshold)
	// {
		// return (data[head]+data[tail]) /2;
	// }
	if(data[index] > deadPixThreshold) return 0;
	else return data[index];
}

int SensorHandler::getTailData()
{
	return readCapacitivePin(tailPin);
}

int SensorHandler::getMouthData()
{
	return readCapacitivePin(mouthPin);
}

void SensorHandler::getBodyTouches(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = readCapacitivePin(bodyTouchPins[i]);
	}
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a 1+Megaohm resistor pulling
//  it up to +5v.
int readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR

  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  uint8_t bitmask = digitalPinToBitMask(pinToMeasure);
  uint8_t port = digitalPinToPort(pinToMeasure);

  volatile uint8_t* out;
  volatile uint8_t* pin;
  volatile uint8_t* reg;

  out = portOutputRegister(port);
  reg = portModeRegister(port);
  pin = portInputRegister(port);

  // Discharge the pin first by setting it low and output
  *out &= ~(bitmask);
  *reg  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *reg &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *out &= ~(bitmask);
  *reg  |= bitmask;
  
  return cycles;
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
	
	
	
		