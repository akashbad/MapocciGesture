Algorithms					{#algorithms}
===========

[TOC]

Overview				{#software_overview}
--------

This page outlines the generic structure of the algorithms used to detect all of the gestures. The two algorithms used are very similar but the second includes hysterisis

Generic Algorithm  			{#generic}
-----------------

This is the simple algorithm that is used in the detection of rotations, falling, tail touches, and kisses.

### Components			{#generic_components}
The necessary components in running this algorithm are a **test case** and a **state** variable. 

1.	Each different gesture will have its own **test case**, most often checking that some raw or lightly processed data is above or below a threshold.
2. 	The **state** variable is also unique to each gesture, but is just a boolean representing whether or not the gesture is active or not.

### Procedure			{#generic_procedure}

The standard procedure for these gestures involves checking 3 conditionals, and returning a gesture for each one or returning no gesture if no conditional is satisfied. Note that the gesture returned for the first and last conditional are the same, this could be changed in the future if it is important to distinguish the first sample where a gesture is detected from subsequent samples where it is still active. Also note that the test case might be slightly different for each conditional (i.e. using a different threshold).

~~~~~~~~~~~~{.c}
if(test_case && !state)
{
	state = true;
	return "Gesture=initiated" + (features);
}
if(!test_case && state)
{
	state = false;
	return "Gesture=ended";
}
if(state)
{
	return "Gesture=initiated" + (features);
}
return "";
~~~~~~~~~~~~

Algorithm + Hysterisis 		{#hysterisis}
----------------------

This follows the same basic structure as the original algorithm but includes 2 more code blocks in order to establish a more time averaged detection scheme. The key difference in these algorithms is the use of a counting variable to prevent a single noisy sample from causing a false positive or negative. This is used for the detection of shakes, upside down, and body touches because the sensors are much noisier.

### Components 			{#hysterisis_components}
In addition to the components needed in the original algorithm, **test case** and **state**, the inclusion of hysterisis requires a **count** variable. 

### Procedure 			{#hysterisis_procedure}

The idea behind including the **count** variable is that it is incremented or decremented if the **test case** is true or false. This adds another 2 conditionals to our existing 3. In addition, the first 2 of the original 3 conditionals have been modified to consider the state of the count. The first will only activate when the count is sufficiently high, and the second will only activate when the count is sufficiently low.

~~~~~~~~~~~~{.c}
if(test_case && !state && count > threshold)
{
	state = true;
	return "Gesture=initiated" + (features);
}
if(test_case)
{
	count++;
}
if(state && count == 0)
{
	state = false;
	return "Gesture=ended";
}
if(!test_case&&count>0)
{
	count--;
}
if(state)
{
	return "Gesture=initiated" + (features);
}
return "";
~~~~~~~~~~~~

### Benefits 				{#benefits}
This count variable presents significant room for modification to the functioning of these algorithms. By changing the amount the count is incremented by and the count threshold for different conditions one can change a gesture's tendency to be recognized as well as how easy it is to stop the gesture.

Most importantly, however, this count allows us to filter out random noise that might otherwise cause a gesture, and it will allow gestures to be sufficiently well segmented in time.

###Testing Gesture Algorithm			{#gesture_algorithm}
This is probably the most confusing and annoying thing to work with in these libraries, especially because something about our current configuration is causing strange effects to the code. For example, GestureHandler::randomMemoryPlaceholder0 and GestureHandler::randomMemoryPlaceholder1 are floats that have been created for no other reason than the fact that allocating those variables memory space allows the arduino to print messages across serial. The fact that the gesture recognition system for the fabrics uses 6 different variables instead of 2 arrays of 3 is also because of some strange behavior on the arduino.

Nevertheless, coming up with these workarounds is not too difficult, what is harder is trying to find out where the problem.
Because the use of arduino prohibits us from using an IDE as a debugger plan on debugging with print statements. 

If a piece of an algorithm is acting strangely (printing repeated ending messages without any initated messages for example) go into each conditional statement and print a message. This will allow you to see which blocks the code is going inside of. Check to see that the test_case, the state, and the count are what you would expect inside of each of these blocks. If things are amiss try commenting in and out blocks of code to see how it changes the behavior and ideally isolate the problem. If you still can't get it to work try replacing some of the more complicated features (arrays and long conditionals) with simple ones. Check that the algorithm works with simpler features so you have a better idea of where to look.

Overall the idea is print, observe, change. Hopefully doing so will allow you to isolate the problem and fix it or come up with a workaround if the problem seems unintelligible.