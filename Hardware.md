Hardware		{#hardware}
========

Overview
--------

This is the overview of interesting hardware notes, including diagrams of the fabric sensor circuit boards as well as information on how to test the hardware to make sure it is functioning.

Fabric and Touch Sensors
--------------
The fabric and touch sensors require some extra hardware in order to interface with the arduino and be compatible with the MapocciGesture libraries. 

###Fabric Sensor Hardware###
The fabric sensors consist of between 9 and 16 different lines which output an analog voltage to be read. This is too many analog sensors even for an Arduino Mega so we need to run these sensors through an analog multiplexer in order to read them all. In addition we must also pass the signal through an op-amp to increase the gain for human touches. 

####Board Schematic####
This schematic of the board shows diagramatically the important features on the board. Most noticable is the large multiplexer and the op-amp configured as a non-inverting differential amplifier.
![](MapocciSchematic.png "Fabric Sensor Board Schematic")

The parts, including relevant values, are labeled on the schematic but repeated here for ease of reading.

*	IC1 = 4067D (the multiplxer and biggest box)
*	R1-R16 = 5.4KOhm (the resistors connecting sensor input to ground)
*	R17-R18 = 3.9KOhm (the vertically oriented resistors to the bottom left of the op-amp)
*	R19-R20 = 6.8KOhm (the resistor below the op-amp and the second op-amp resistor from the top)
*	R21-R22 = 3.3KOhm (the remaining two op-amp resistors)
*	IC2 = TLV071

####Board Layout####
The layout of the board does a better job of showing how things are plugged in. On the left-hand side you have the control pins to the board. From top to bottom they are:

*	GND
*	Control 0
*	Control 1
*	Control 2
*	Control 3
*	Analog Out
*	VCC
*	GND


![](MapocciBoard.png "Fabric Sensor Board Layout")

The pins on the right-hand side are all connected to the different sensors on the fabric. These connections can be made using ribbon cables to hook up the board to the arduino.


Testing
-------

