#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxUI.h"


class testApp : public ofBaseApp{
    
public:
    //~testApp();/* destructor is very useful */
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
	
    int	initialBufferSize; /* buffer size */
    int	sampleRate;
	
	
    /* stick you maximilian stuff below */
	
    //monosynth.cpp example
    //This shows how to use maximilian to build a monophonic synth
    //These are the synthesiser bits
    maxiOsc VCO1,VCO2,LFO1,LFO2;
    maxiFilter VCF;
    maxiEnvelope ADSR;
    maxiMix mixer;
    //the control values for the envelope
    double  AT = 5;
    double  AV = 1;
    double  DT = 250;
    double  DV = 0.125;
    double  ST = 125;
    double  SV = 0.125;
    double  RT = 500;
    double  RV = 0;
    //double adsrEnv[8]={AT,AV,DT,DV,ST,SV,RT,RV};
    double adsrEnv[8]={1,5,0.125,250,0.125,125,0,500};
    
    // alternative approach to creating an ADSR array using 2D vectors
    ofVec2f adsrA, adsrD, adsrS, adsrR;
    //    double adsrEnv2[8]={adsrA.x,adsrA.y,adsrD.x,adsrD.y,adsrS.x,adsrS.y,adsrR.x,adsrR.y};
    double adsrEnv2[8];
    //variables to hold last values so ADSR array only updates when the GUI element is changed
    double  ATLast,AVLast,DTLast,DVLast,STLast,SVLast,RTLast,RVLast;
    bool ADSRUpdate = false;
    
        
    //This is a bunch of control signals so that we can hear something
    maxiOsc timer;//this is the metronome
    int currentCount,lastCount;//these values are used to check if we have a new beat this sample
    
    //and these are some variables we can use to pass stuff around
    double VCO1out,VCO2out,LFO1out,LFO2out,VCFout,ADSRout;
    
    //some track outputs
    double wave,outputs[2],moreoutputs[2]; 
    
    // bunch of variables to hold values from GUI element
    //VCO1 and VCO2
    float VCO1F, VCO1FND, VCO1PW, VCO2F, VCO2PW, LFO1F, LFO2F;
    int waveTypeVCO1, waveTypeVCO2;
    //VCF
    double VCFCutoff, VCFResonance;
    int filterTypeVCF;
    float cutoffMin, cutoffMax, resonanceMin, resonanceMax;
    //Trigger
    bool autoTriggerToggle;
    float triggerTime;

    
    //ofxUI
    void setGUI4();
    void guiEvent(ofxUIEventArgs &e);
    void exit();
    ofxUIScrollableCanvas *gui4;
    bool hideGUI;
     
    
    
};
