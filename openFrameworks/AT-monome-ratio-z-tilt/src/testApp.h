#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "ofxOsc.h"
#define HOST "192.168.1.102"
#define PORT 10000

#include "ofxGrabCam.h"
#include "ofxMonomeControl.h"
#include "ofxUI.h"


class testApp : public ofBaseApp{
	
public:
	
	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();
    
    void gotMessage(ofMessage msg);
	
    // ofxMonomeControl
	ofxMonomeControl monomeControl;
	bool LEDs[16][16];
    int mcolsdevice, mrowsdevice;
    int x, y;
    int xoffset, yoffset;
    int mcol, mrow, mcols, mrows;
    int buttonsize, buttonspacing;
    float mratio, mfrequency;
    float rootFrequency;
    int zdivider = 25; // this reduces range of z-axis spread of cells by frequency
    
    // ofxOSC
    ofxOscSender sender;
    
    // ofxUI
    void guiEvent(ofxUIEventArgs &e);
    void exit();
    ofxUICanvas *gui;
    bool hideGUI;
    ofTrueTypeFont	NewMediaFett30;
    
    // ofxGrabCam
    ofxGrabCam camera;
    void randomiseViewport();
    bool useViewport;
	ofRectangle viewport;
	ofMatrix4x4 savedPose;
    bool resetDefaultFlag = true;
    
    /// Timer for blinking monome64 LEDs
    unsigned long m;
    unsigned long timeLED[16][16];
    bool LEDToggle[16][16];
    int blinkInterval;
    int blink[16][16];
    
    // 2D array to store individual LED PWM values
    int PWMarray[16][16] = {
        // 1 2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
        {99,99,85,99,71,85,43,99, 1,71, 1,85, 1,43, 1,99}, // 1
        {99,99,85,99,71,85,43,99, 1,71, 1,85, 1,43, 1,99}, // 2
        {85,85,99,85,57,99,29,85,85,57, 1,99, 1,29, 1,85}, // 3
        {99,99,85,99,71,85,43,99, 1,71, 1,85, 1,43, 1,99}, // 4
        {71,71,57,71,99,57,15,71, 1,99, 1,57, 1,15,85,71}, // 5
        {85,85,99,85,57,99,29,85,85,57, 1,99, 1,29, 1,85}, // 6
        {43,43,29,43,15,29,99,43, 1,15, 1,29, 1,99, 1,43}, // 7
        {99,99,85,99,71,85,43,99, 1,71, 1,85, 1,43, 1,99}, // 8
        { 1, 1,85, 1, 1,85, 1, 1,99, 1, 1,85, 1, 1,57, 1}, // 9
        {71,71,57,71,99,57,15,71, 1,99, 1,57, 1,15,85,71}, // 10
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,99, 1, 1, 1, 1, 1}, // 11
        {85,85,99,85,57,99,29,85,85,57, 1,99, 1,29,71,85}, // 12
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,99, 1, 1, 1}, // 13
        {43,43,29,43,15,29,99,43, 1,15, 1,29, 1,99, 1,43}, // 14
        { 1, 1,71, 1,85,71, 1, 1,57,85, 1,71, 1, 1,99, 1}, // 15
        {99,99,85,99,71,85,43,99, 1,71, 1,85, 1,43, 1,99}  // 16
    };
    
    // NOTE: there are only 8 different values in this array
    // could I optomise this by having individual LEDs use one of 8 discreet PWMs
    // rather than this per LED 16x16 PWM array ??

    // scene assets
    void threeDAxis();
    ofImage lamdoma;
    bool LMVisible = true;
    bool LEDzVisible = true;
    bool ThreeDAxisVisible = true;
    
};


#endif
