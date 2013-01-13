/* This is an example of how to integrate maximilain into openFrameworks, 
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */


#include "testApp.h"



//-------------------------------------------------------------
testApp::~testApp() {
	
	//delete beat.myData; // you should probably delete myData for any sample object that you've created in testApp.h
    
//    //from Sequencing.cpp
//    delete beats.myData;
    
//    //from DrumMachine.cpp
//    delete kick.myData;
//    delete snare.myData;
    
    // actually including these freezes the sketch on close ??
	
}


//--------------------------------------------------------------
void testApp::setup(){
	/* some standard setup stuff*/
	
	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetVerticalSync(true);
	
	/* This is stuff you always need.*/
	
	sampleRate 			= 44100; /* Sampling Rate */
	initialBufferSize	= 512;	/* Buffer Size. you have to fill this buffer with sound*/
	
	
	/* Now you can put anything you would normally put in maximilian's 'setup' method in here. */
	
	// from example project
//	beat.load(ofToDataPath("beat2.wav"));
//	beat.getLength();
    
//    //from Sequencing.cpp
//    b.amplitude=env[0];//starting value for envelope b
//	f.amplitude=env2[0];//same for f
//	beats.load(ofToDataPath("beat2.wav"));//put a path to a soundfile here. Wav format only.
//	//printf("Summary:\n%s", beats.getSummary());//get info on samples if you like
    
    //from DrumMachine.cpp
    kick.load(ofToDataPath("808kick.wav"));//load in your samples. Provide the full path to a wav file.
	snare.load(ofToDataPath("808snare.wav"));//load in your samples. Provide the full path to a wav file.
	//printf("Summary:\n%s", kick.getSummary());//get info on samples if you like.
	//beats.getLength();

	
	
	ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);/* Call this last ! */
}

//--------------------------------------------------------------
void testApp::update(){
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	/* You can use any of the data from audio received and audiorequested to draw stuff here.
	 Importantly, most people just use the input and output arrays defined above.
	 Clever people don't do this. This bit of code shows that by default, each signal is going to flip
	 between -1 and 1. You need to account for this somehow. Get the absolute value for example.
	 */
	
	ofSetColor(255, 255, 255,255);
	ofRect(600, 300, sample*150, sample*150); /* audio sigs go between -1 and 1. See?*/
	ofCircle(200, 300, wave*150);
	
}

//--------------------------------------------------------------
void testApp::audioRequested 	(float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
		
		/* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
		 
		 For information on how maximilian works, take a look at the example code at
		 
		 http://www.maximilian.strangeloop.co.uk
		 
		 under 'Tutorials'.
		 
		 */
		
		
//		// from example project
//		sample=beat.play(0.25, 0, beat.length);
//		wave=sine1.sinebuf(abs(mouseX));/* mouse controls sinewave pitch. we get abs value to stop it dropping below zero and killing the soundcard*/
        
        //from Counting.cpp example
//        CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
//        wave=mySquare.square(CurrentCount*100);
//        //now we send the sounds to some stereo busses.
//		mymix.stereo(sample + wave, outputs, 0.5);
//        //mixing
//		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
//		output[i*nChannels + 1] = outputs[1];
        
        //from Counting2.cpp example
//        CurrentCount=myCounter.phasor(1, 1, 9);//phasor can take three arguments; frequency, start value and end value.
//        if (CurrentCount<5)//simple if statement
//            myOscOutput=mySwitchableOsc.square(CurrentCount*100);
//        else if (CurrentCount>=5)//and the 'else' bit.
//            myOscOutput=mySwitchableOsc.sinewave(CurrentCount*50);//one osc object can produce whichever waveform you want.
//        //*output=myOscOutput;//point me at your speakers and fire.
//        wave=myOscOutput;//point me at your speakers and fire.
//        //now we send the sounds to some stereo busses.
//		mymix.stereo(sample + wave, outputs, 0.5);
//        //mixing
//		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
//		output[i*nChannels + 1] = outputs[1];
        
        
//        //from Sequencing.cpp example
//        beat=((int) c.phasor(8));//this oscillator is now a counter
//        morebeats=((int) e.phasor(0.5,0,16));//so is this one
//        patch1=b.line(4,env);//here's envelope b
//        patch2=f.line(6,env2);//here's envelop f
//        tune=g.saw(melody[morebeats]*0.25);//here's the melody, which occurs at certain times
//        
//        if (lastbeat!=beat) {//this is a nice sample and hold routine for the kick drum
//            f.trigger(0, env2[0]);//it runs off the hi hat.
//            
//            
//            if (rhythm1[morebeats]==1) {
//                b.trigger(0, env[0]);//and gets played when it's time.
//            }
//        }
//        lastbeat=beat;//let's start again. It's a loop
//        ramp=OSCi.phasor(0.5,1,2048);//create a basic ramp
//        pan=j.phasor(0.25);//some panning from a phasor (object is equal power)
//        delayed=delay.dl(tune, ramp, 0.9)*0.125;//the delay line
//        //then it all gets mixed.
//        mixed=((a.sinewave(patch1)*0.5)+((d.saw(patch2))*0.125)+(delayed*0.3)*0.5);
//        //add some noise
//        noise=OSCi.noise()*0.25;
//        filtered2=beats.play(1*(1./16.),0,beats.length);
//        //	filtered2=beats.play(-1);
//        
//        more=squarewave.pulse(melody[morebeats],pan)*0.05;
//        //filter the noise! this lores takes values between 1 and 100 for res, and freq for cutoff.
//        filtered=myfilter.lores(filtered2, 1+(pan*10000), 10)*0.4;
//        
//        //now we send the sounds to some stereo busses.
//        mymix.stereo(more+mixed+delayed, outputs, 1-pan);
//        bobbins.stereo(filtered, moreoutputs, pan);//invert the pan
//        //mixing
//		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
//		output[i*nChannels + 1] = outputs[1];
        
        
        //from DrumMachine.cpp
        currentCount=(int)timer.phasor(8);//this sets up a metronome that ticks 8 times a second
        
        if (lastCount!=currentCount) {//if we have a new timer int this sample, play the sound
            kicktrigger=kickhit[playHead%16];//get the value out of the array for the kick
            snaretrigger=snarehit[playHead%16];//same for the snare
            playHead++;//iterate the playhead
            lastCount=0;//reset the metrotest
        }
        
        if (kicktrigger==1) {//if the sequence has a 1 in it
            kick.trigger();//reset the playback position of the sample to 0 (the beginning)
        }
        if (snaretrigger==1) {
            snare.trigger();//likewise for the snare
        }
        
        sampleOut=kick.playOnce()+snare.playOnce();//just play the file. No looping.
        
		//now we send the sounds to some stereo busses.
		mymix.stereo(sampleOut, outputs, 0.5);
        //mixing
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
        
        kicktrigger = 0;//set trigger to 0 at the end of each sample to guarantee retriggering.
        snaretrigger = 0;

	}
	
}

//--------------------------------------------------------------
void testApp::audioReceived 	(float * input, int bufferSize, int nChannels){	
	
	
	/* You can just grab this input and stick it in a double, then use it above to create output*/
	
	for (int i = 0; i < bufferSize; i++){
		
		/* you can also grab the data out of the arrays*/
		
		
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
	
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}



//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}