//
//  README
//  Text
//  ----------------------------------
//  Developed with embedXcode
//
//  Project AT-monome-ratio-z
//  Created by Lewis Sykes on 31/12/2012
//  Licence CC = BY SA NC
//

This tested on OSX 10.7 using Xcode 4.5 and oF0073

uses addons from default folder:
ofxXmlSettings
ofxOsc
uses additional addons from addonsOther folder at same root:
ofxUI
ofxMonomeControl
ofxGrabCam

What it does:
- loads John Telfer's colour coded Lamdoma Matrix Fig. 20
- overlays a virtual grid of 16x16 LEDs on top
- overlays a slightly smaller set of 16x16 LEDs on top - these shift on the z-axis according to rootFrequency set by ofxUI
- overlays an 8x8 device outline over the virtual 16x16 grid - this is moveable via keys and ofxUI
- displays Ratio, mratio and mfrequency values
- flashes monome64 LEDs using a PWM style blinking to display a monochrome version of the colour coded Lamdoma Matrix on the monome64 - blinkInterval set by ofxUI
- detects monome64 button presses, lights monome64 LEDs on button press, lights virtual LEDs
- sends mfrequency value to Ronin Synth via OSC
- uses slightly modified version of ofxGrabCam and viewports 

added to ofxGrabCam.cpp
//--------------------------
void ofxGrabCam::resetDefault() {
	ofCamera::resetTransform();
	ofCamera::setPosition(0.0f, 0.0f, 600.0f);
	ofCamera::lookAt( ofVec3f(0.0f, 0.0f, 0.0f) );
}
added to ofxGrabCam.h
void    resetDefault();

Key Presses:
- UP, DOWN, LEFT & RIGHT keys move the 8x8 device outline around the virtual 16x16 matrix
// ofxGrabCam
- g or G - toggle GUI
- s - save camera pose
- l - load camera pose
- SPACE - randomise Viewport
- v - use Viewports
- r - reset camera
- t - reset camera to default settings
// screen assets
- p - toggle visibility of Lamdoma Matrix
- o - toggle visibility of z-axis offset LEDs
- i - toggle visibility of 3DAxis





