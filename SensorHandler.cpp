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


/**
* A constructor for the SensorHandler class. It takes in the pins
*	for each of the connected sensors. It also assigns some
*	variables for the small amount of data processing done in the
*	collection process. Finally it initializes pins it will use as
*	output.
*
*	@param Apins[] the accelerometer pins
*	@param Gpins[] the gyroscope pins
*	@param FpinT the fabric pin for the torso
*	@param FpinS the fabric pin for the stomach
*	@param FpinB the fabric pin for the bottom
*	@param ConTpins[] the torso multiplexer control pins
*	@param ConSpins[] the stomach multiplexer control pins
*	@param ConBpins[] the bottom multiplexer control pins
*	@param tPin the tail input pin
*	@param mPin the mouth input pin
*	@param btPins[] the body touch pins
*/
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
		
		//initialize the fabric processing values
		deadPixThreshold = 1024; //250;
		fabricScaleFactor = 0;//270;
		
		//Set the multiplexer pins to output
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

/**
* Get the data from the accelerometer pins
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 3
*/
void SensorHandler::getAccelData(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = analogRead(accelPins[i]);
	}
}

/**
* Get the data from the gyroscope pins
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 3
*/
void SensorHandler::getGyroData(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = analogRead(gyroPins[i]);
	}
}

/**
* Get the data from the fabric torso pins.
*	This data is scaled by the correct factor and then filtered
*	for dead pixels.
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 16
*	@see fabricMultiplexer()
*	@see removeDeadPixels()
*/
void SensorHandler::getTorsoData(int data[])
{
	for(int i=0; i<16; i++)
	{
		fabricMultiplexer(15-i, torsoControl);
		int temp = analogRead(torsoPin)-fabricScaleFactor;
		data[i] = temp > 0 ? temp : 0;
	}
	for(int i=0; i<16; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
}

/**
* Get the data from the fabric stomach pins.
*	This data is scaled by the correct factor and then filtered
*	for dead pixels.
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 14
*	@see fabricMultiplexer()
*	@see removeDeadPixels()
*/
void SensorHandler::getStomachData(int data[])
{

	for(int i=1; i<15; i++)
	{
		fabricMultiplexer(i, stomachControl);
		int temp = analogRead(stomachPin)-fabricScaleFactor;
		data[i-1] = temp > 0 ? temp : 0;		
	}
	for(int i=0; i<14; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
	
}

/**
* Get the data from the fabric bottom pins.
*	This data is scaled by the correct factor and then filtered
*	for dead pixels.
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 9
*	@see fabricMultiplexer()
*	@see removeDeadPixels()
*/
void SensorHandler::getBottomData(int data[])
{
	for(int i=4; i<13; i++)
	{
		fabricMultiplexer(i, bottomControl);
		int temp = analogRead(bottomPin)-fabricScaleFactor;
		data[i-4] = temp > 0 ? temp : 0;
	}
	for(int i=0; i<9; i++)
	{
		data[i] = removeDeadPixels(data, i, 16);
	}
}

/**
* A method to remove dead pixels from fabric data.
*	These is both a simple and complicated algorithm that can be
*	used in this method. The simple method grounds all dead pixels.
*	The more complicated method attempts to solve for averages, 
*	so as to prevent a dead pixel from skewing the mean.
*	The complicated method fails when dealing with many adjacent dead
*	pixels.
*
*	@param data[] the data to be smoothed
*	@param index the index of the current pixel being processed
*	@param size the size of the input array
*	@return the value for the pixel after processing
*/
int SensorHandler::removeDeadPixels(int data[], int index, int size)
{
/*	int head = index < size-1 ? index+1 : index-2;
	int tail = index > 0 ? index-1 : index+2;
	if(head == index)
	{
		head = tail;
	}
	if(data[index] > deadPixThreshold)
	{
		return (data[head]+data[tail]) /2;
	}*/
	if(data[index] > deadPixThreshold) return 0;
	else return data[index];
}

/**
* A method to read the capacitive sensor on the tail.
*	This reads the value of the tail pin using a capacitive
*	method.
*	
*	@return a value representing the capacitance on the tail
*	@see readCapacitivePin()
*/
int SensorHandler::getTailData()
{
	return readCapacitivePin(tailPin);
}

/**
* A method to read the capacitive sensor on the mouth.
*	This reads the value of the mouth pin using a capacitive
*	method.
*	
*	@return a value representing the capacitance on the mouth
*	@see readCapacitivePin()
*/
int SensorHandler::getMouthData()
{
	return readCapacitivePin(mouthPin);
}

/**
* A method to read the capacitive sensors on the body.
*	This reads the value of the body pins using a capacitive
*	method.
*	@param data[] the array where the raw data will be stored,
*		this array is mutated and must be at least of length 4
*	@see readCapacitivePin()
*/
void SensorHandler::getBodyTouches(int data[])
{
	for(int i=0; i<3; i++)
	{
		data[i] = readCapacitivePin(bodyTouchPins[i]);
	}
}

/**
* A method to read a capacitive sensor.
*	This works by pulling the pin down and timing
*	to see how long it takes to return high, the longer
*	the more capacitance.
*
*	@param pinToMeasure the arduino pin number to test
*	@return an int representing how long it took the pin to
*		return high
*/
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

/**
* A method to set the control pins of a multiplexer to read
*	the correct node. Uses binary logic to determin the highs
*	and lows.
*
*	@param node the index to be read on the multiplexer
*	@param control[] the pins to be used for this control
*/
void SensorHandler::fabricMultiplexer(int node, int control[])
{
	digitalWrite(control[0], node%2==0 ? LOW : HIGH);
	digitalWrite(control[1], node/2%2==0 ? LOW : HIGH);
	digitalWrite(control[2], node/4%2==0 ? LOW : HIGH);
	digitalWrite(control[3], node/8%2==0 ? LOW : HIGH);
}
	
	
	
		