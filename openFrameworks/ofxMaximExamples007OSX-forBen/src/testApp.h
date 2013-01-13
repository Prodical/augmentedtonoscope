#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"


class testApp : public ofBaseApp{

	public:
		~testApp();/* destructor is very useful */
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
	
		void audioRequested 	(float * input, int bufferSize, int nChannels); /* output method */
		void audioReceived 	(float * input, int bufferSize, int nChannels); /* input method */
	
		int		initialBufferSize; /* buffer size */
		int		sampleRate;
	
	
		/* stick you maximilian stuff below */
    // commeted out these from example project
	//		ofxMaxiOsc sine1;
    //		ofxMaxiSample beats,beat;
    // kept these
		double wave,sample,outputs[2];
		ofxMaxiMix mymix;
    
    //from Counting.cpp example
//    maxiOsc myCounter,mySquare;//these oscillators will help us count and play sound
//    int CurrentCount;//we're going to put the current count in this variable so that we can use it more easily.

     //from Counting2.cpp example
//    maxiOsc myCounter,mySwitchableOsc;//these oscillators will help us count and make sound.
//    int CurrentCount;//we're going to put the current count in this variable so that we can use it more easily.
//    double myOscOutput;//we're going to stick the output here to make it easier to mess with stuff.
    
    //from Sequencing.cpp
//    //double outputs[2],moreoutputs[2]; //some track outputs
//    double moreoutputs[2]; //some track outputs
//    double filtered,patch1,patch2,tune,delayed,
//    mixed,ramp,filtered2,noise,pan,more;//a bunch of patch cables
//    int beat,lastbeat,morebeats,lastmorebeats;//some rhythmic elemts
//    double env[4]={200,0,0,50};//the kick drum pitch envelope data
//    double env2[6]={10000,0,9000,5,0,5};//the hi hat pitch envelope dat
//    double melody[14]={600,0,0,650,0,0,400,0,0,425,0,300,0,315};//the melody data
//    int rhythm1[16]={1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0};//another way of doing a rhythm
//    maxiOsc a,c,d,e,g,h,OSCi,j,squarewave;//some oscillators
//    maxiEnvelope b,f;//two envelopers
//    maxiDelayline delay;//a delay line
//    maxiFilter myfilter,antia;// a FAT filter
//    //maxiMix mymix,bobbins;//some panning busses
//    maxiMix bobbins;//some panning busses
//    maxiSample beats;
    
    //from DrumMachine.cpp
    maxiSample kick,snare; //we've got two sampleplayers
    maxiOsc timer; //and a timer
    int currentCount,lastCount,playHead;
    int kickhit[16]={1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1}; //This is the sequence for the kick
    int snarehit[16]={0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0};//This is the sequence for the snare
    int kicktrigger,snaretrigger;
    double sampleOut;

		
};
