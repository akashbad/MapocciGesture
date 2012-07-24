Algorithms					{#algorithms}
===========

Overview
--------

This page outlines the generic structure of the algorithms used to detect all of the gestures. The two algorithms used are very similar but the second includes hysterisis

Generic Algorithm  			{#generic}
-----------------

This is the simple algorithm that is used in the detection of rotations, falling, tail touches, and kisses.

### Components ###
The necessary components in running this algorithm are a **test case** and a **state** variable. 

1.	Each different gesture will have its own **test case**, most often checking that some raw or lightly processed data is above or below a threshold.
2. 	The **state** variable is also unique to each gesture, but is just a boolean representing whether or not the gesture is active or not.

### Procedure ###

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

### Components ###
In addition to the components needed in the original algorithm, **test case** and **state**, the inclusion of hysterisis requires a **count** variable. 

### Procedure ###

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

### Benefits ###
This count variable presents significant room for modification to the functioning of these algorithms. By changing the amount the count is incremented by and the count threshold for different conditions one can change a gesture's tendency to be recognized as well as how easy it is to stop the gesture.

Most importantly, however, this count allows us to filter out random noise that might otherwise cause a gesture, and it will allow gestures to be sufficiently well segmented in time.