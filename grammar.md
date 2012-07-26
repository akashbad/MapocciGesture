Gesture Grammar				{#grammar}
================

[TOC]

Overview					{#grammar_overview}
--------

This is the grammar for how identified gesture events will be communicated from the Gesture model to the emotional controller. 

The general idea is that each cycle a String will be produced that follows the following grammar. It contains all of the gesture events that were detected during that cycle. If the string is empty it means no gestures have been identified. 

Within a single gesture string, different gestures are delimited by the '!' character.

Grammar 					{#definitions}
--------

	MESSAGE ::= GESTURE*

	GESTURE ::= (START | END) "!"

	START ::= TYPE "=initated" FEATURES?

	END ::= TYPE "=ended"

	TYPE ::= "Shaking" | "Spinning" | "Flipping" | "Rolling" | "Falling" | 
		"Upside-Down" | "Touch-0" | "Touch-1" | "Touch-2" | "Tail" | "Kiss"

	FEATURES ::= ":" (SHAKING_FEATURES | ROTATING_FEATURES | TOUCH_FEATURES)

	SHAKING_FEATURES ::= "Acceleration=" %f ":Axis" %d

	ROTATING_FEATURES ::= "Speed=" %f

	TOUCH_FEATURES ::= "Pressure=" %f ":Area" %f ":Position" %f ":Acceleration" %d

	%f ::= float

	%d ::= integer