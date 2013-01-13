//
//  README
//  Text
//  ----------------------------------
//  Developed with embedXcode
//
//  Project emptyExample
//  Created by Dr X on 13/01/2013
//  Copyright © 2013 __MyCompanyName__
//  Licence CC = BY SA NC
//

Hi Ben

I've selected some of the examples from the 'maximilian_examples' folder at the 'Maxilmilian-master' root that involve using Maximilian oscillators as timers and arrays for sequencing and copied and tweaked them to work in this oF sketch.

Counting.cpp
Counting2.cpp
Sequencer.cpp
DrumMachine.cpp

Just uncomment and comment the relevant code blocks...

The Counting examples use of a Maxilmilian phasor as a timer to increase a counter... and then trigger an oscillator each time the counter increases... this avoids having to use ofGetElapsedTimeMillis() as per your last example.

The Sequencer and Drummachine examples extend this use of a Maxilmilian phasor as a timer to increase a counter by stepping through an array rather than directly triggering another oscillator - if the value in the array is 1 the oscillator/sample triggers

I think an approach for the nodal sequencer may be to extend these examples further by adding a record function - populating a dynamic array with either a 0 or a 1 (the latter if a button press is detected) every time the counter increases - and then stepping through this array as above during playback?









