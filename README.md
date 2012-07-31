MapocciGesture				{#mainpage}
===============

The repository for the libraries neccessary for sensor input and gesture recognition. This project consists of libraries responsible for collecting and processing data. It also contains the framework for a more sophisticated send to the emotional model.

In order to appropriately integrate this code with the rest of the project move these files to a folder within the arduino libraries. See the following code block for an example of how to include this functionality into a sketch.

~~~{.c}
//Make sure to include the libraries
#include <Gesture.h>
#include <General.h>
#include <SensorHandler.h>
#include <GestureHandler.h>
#include <MapocciTransfer.h>

//Declare the correct pins for all of the sensors
int Apins[] = {0,1,2};
int Gpins[] = {3,4,5};
int FpinT = 8;
int FpinB = 15;
int FpinS = 9;
int ConTpins[] = {48,46,44,42};
int ConBpins[] = {28,26,24,22};
int ConSpins[] = {49,47,45,43}; 
int tPin = 32;
int mPin = 34;
int btPins[] = {36,40,38};
int ledPins[] = {4,5,6};

//Create instances of the necessary objects
SensorHandler sHandler(Apins, Gpins, FpinT, FpinS, FpinB, ConTpins,ConSpins, ConBpins, tPin, mPin, btPins);
MapocciTransfer model;
GestureHandler gHandler(model);
Gesture gesture;

void setup()
{
	//Add any setup you may need
}

void loop()
{
  //Collect the data
  sensorData s = gesture.collect(sHandler);
  //Process the data
  String gestures = gesture.process(&gHandler, s);
  //This method will not do anything for now, the gestures recieved this loop will be stored in gestures.
  gesture.send(gesture.process(gHandler, gesture.collect(sHandler)));
}
~~~