#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
    ofSetVerticalSync(true);
    // this uses depth information for occlusion
	// rather than always drawing things on top of each other
	glEnable(GL_DEPTH_TEST);
    //ofSetFrameRate(30);
    
    /// ofxOSC
    // open an outgoing connection to HOST:PORT
	sender.setup(HOST, PORT);
    
    /// ofxGrabCam
    randomiseViewport();
	useViewport = false;
    
    /// ofxMonomControl
	//check the console to see your options for devices. With only one device plugged in,
	//you probably want #0
	monomeControl.setup(0);
    
    // a start up LED test - all for 2 seconds then off for 2 secconds
	monomeControl.ClearLEDs(true);
    ofSleepMillis(2000);
    monomeControl.ClearLEDs(false);
    ofSleepMillis(2000);
    
    mcolsdevice = 8;
    mrowsdevice = 8;
    mcols = 16;
    mrows = 16;
    buttonsize = 19;
    buttonspacing = 6;
    xoffset = 1;
    yoffset = 0;
    x = 0;
    y = 0;
    mratio = 1.0;
    
    for (int i=0; i<mcols; i++) {
		for (int j=0; j<mrows; j++) {
			LEDs[i][j]=false;
            monomeControl.SetLED(i,j,false);
        }
    }
    
    // setup flashing LEDs on monome64 as per the Lamdoma matrix colour scheme
    for (int i=0; i<mcols; i++) {
		for (int j=0; j<mrows; j++) {
            LEDToggle[i][j] = false;
        }
    }
    blinkInterval = 200;
    
    // load scene & GUI assets
    lamdoma.loadImage("lamdoma.png");
    //old OF default is 96 - but this results in fonts looking larger than in other programs.
	ofTrueTypeFont::setGlobalDpi(72);
	NewMediaFett30.loadFont("NewMediaFett.ttf", 30, true, true);
	NewMediaFett30.setLineHeight(34.0f);
	NewMediaFett30.setLetterSpacing(1.035);
    
    /// ofxUI
    ofEnableSmoothing();
    //ofEnableAlphaBlending();
    
    float dim = 24;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 260-xInit;
    
    unsigned int newMin = 0;
    unsigned int newMax = 0;
    unsigned int newValue = 0;
    
    gui = new ofxUICanvas(0,400,300,300);
    gui->addWidgetDown(new ofxUILabel("MONOME64 OFFSET", OFX_UI_FONT_LARGE));
    gui->addWidgetDown(new ofxUISpacer(length-xInit,2));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 0.0, 8.0, xoffset, "X", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, 0.0, 8.0, yoffset, "Y", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui->addWidgetDown(new ofxUIFPSSlider(length-xInit, dim*.25, 0.0, 100, 60.0, "FPS SLIDER"));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim*.25, 1.0, 3000.0, 1000.0, "blinkInterval"));
    gui->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui->addWidgetDown(new ofxUIMinimalSlider(length-xInit, dim*.25, 0.0, 1000.0, 440.0, "rootFrequency"));
    
    ofAddListener(gui->newGUIEvent, this, &testApp::guiEvent);
    gui->loadSettings("GUI/guiSettings.xml");
    
    gui->toggleVisible();
    
}

//--------------------------------------------------------------
void testApp::update(){
    
    mcol = x+1+xoffset;
    mrow = y+1+yoffset;
    mratio = (float(x+xoffset)+1.0)/(float(y+yoffset)+1.0);
    mfrequency = rootFrequency*mratio;
    string mratioString = ofToString(mratio, 3);
    string mfrequencyString = ofToString(mfrequency, 3);
    
	bool down = false;
	while(monomeControl.GetButtonPress(x,y,down))
	{
        LEDs[x+xoffset][y+yoffset] = true;
		if(down == false && x<mcolsdevice && y<mrowsdevice)
		{
			LEDs[x+xoffset][y+yoffset] = false;
		}
        
        // only sends value on button release - get a previous mfrequency value otherwise ??
        if(down == false) { 
            ofxOscMessage m;
            m.setAddress("/monome");
            m.addIntArg(mcol);
            m.addIntArg(mrow);
            m.addFloatArg(mratio);
            m.addFloatArg(mfrequency);
            sender.sendMessage(m);
        }
        
        //By Full Row:
        for(int row=0;row<mrowsdevice;row++)
        {
            unsigned char val1 = 0;
            unsigned char val2 = 0;
            for(int i=0;i<mrowsdevice;i++)
                if(LEDs[i+xoffset][row+yoffset])
                    val1 |= 1<<i;
            monomeControl.SetRow1(row,val1);
        }
        
    } // end of while(monomeControl.GetButtonPress(x,y,down)) loop
    
    // Flashing LEDs on monome64 as per the Lamdoma matrix colour scheme
    m = ofGetElapsedTimeMillis();
    
    for (int i=0; i<mcols; i++) {
		for (int j=0; j<mrows; j++) {
            if (m - timeLED[i][j] > blink[i][j]) {
                if (LEDToggle[i][j] == true) {
                    monomeControl.SetLED(i-xoffset,j-yoffset,true);
                    //blink[i][j] = blinkInterval * PWM_map[i][j]/100;
                    blink[i][j] = blinkInterval * PWMarray[j][i]/100;
                } else if (LEDToggle[i][j] == false) {
                    monomeControl.SetLED(i-xoffset,j-yoffset,false);
                    //blink[i][j] = blinkInterval * (100-PWM_map[i][j])/100;
                    blink[i][j] = blinkInterval * (100-PWMarray[j][i])/100;
                }
                LEDToggle[i][j] = !LEDToggle[i][j];
                timeLED[i][j] = m;
            }
        }
    }
    
}


//--------------------------------------------------------------
void testApp::draw(){
    
	ofBackground(0,0,0);
	int w = ofGetWidth();
    int h = ofGetHeight();
    
    //// GrabCam
	//draw scene
	if (useViewport)
		camera.begin(viewport);
	else
		camera.begin();
    
    if (resetDefaultFlag)
        camera.resetDefault();
        resetDefaultFlag = false;
    
    ofPushMatrix();

    ofTranslate(0.0, float(h)/2-43, 0.0);
    ofRotateZ(135.0);
    ofRotateY(180.0);
    
    glDisable(GL_DEPTH_TEST); // can't see ofEnableAlphaBlending() otherwise
    ofEnableAlphaBlending();
    
    // John telfer's Lamdoma matrix diagram Fig. 20
    ofPushMatrix();
    ofSetColor(255,255,255, 100);
    ofRotateZ(-45.0);
    if (LMVisible)
        lamdoma.draw(-float(w/2)+15, 22, 570, 570);
    ofPopMatrix();
    
    // the physical device outline and fill
    ofSetColor(255,128,0, 255);
    ofNoFill();
    ofSetLineWidth(2);
    ofRect(18+xoffset*(buttonsize+buttonspacing), 18+yoffset*(buttonsize+buttonspacing), mcolsdevice*(buttonsize+buttonspacing), mrowsdevice*(buttonsize+buttonspacing));
    ofSetColor(255,255,255, 40);
    ofFill();
    ofRect(18+xoffset*(buttonsize+buttonspacing), 18+yoffset*(buttonsize+buttonspacing), mcolsdevice*(buttonsize+buttonspacing), mrowsdevice*(buttonsize+buttonspacing));
    
    // the LEDS...
    // the set that stays on the XY plane
    for(int x=0;x<mcols;x++)
		for(int y=0;y<mrows;y++)
		{
			if(LEDs[x][y])
				ofSetColor(255,100,50,230);
			else
				ofSetColor(50,50,50,200);
			ofRect(20+(buttonsize+buttonspacing)*x,20+(buttonsize+buttonspacing)*y,buttonsize,buttonsize);
		}
    // the set that offset on Z-axis according to their ratio of the root frequency
    if (LEDzVisible)
    {
        ofFill();
        for(int x=0;x<mcols;x++)
            for(int y=0;y<mrows;y++)
            {
                if(LEDs[x][y])
                    ofSetColor(255,255,255,50);
                else
                    ofSetColor(150,150,150,50);
                ofPushMatrix();
                ofTranslate(0.0,0.0,(rootFrequency-(rootFrequency*float((x+1.0)/(y+1.0))))/zdivider);
                ofRect(22+((buttonsize-2)+(buttonspacing+2))*x,22+((buttonsize-2)+(buttonspacing+2))*y,buttonsize-4,buttonsize-4);
                ofPopMatrix();
            }
    }
    
    ofPopMatrix();
    ofDisableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    
    /// 3D axis
    if (ThreeDAxisVisible)
        threeDAxis();

    camera.end(); /// ofxGrabCam end
    
    if (useViewport) {
		ofPushStyle();
		ofSetColor(255,255,255);
		ofNoFill();
		ofRect(viewport);
		ofPopStyle();
	}
    
    // ratio, mratio and mfrequency to screen:
    ofSetColor(255);
    char reportStr[1024];
    sprintf(reportStr, "Ratio: %i/%i\nmratio: %.3f\nmfrequency: %.2f", mcol, mrow, mratio, mfrequency);
    NewMediaFett30.drawString(reportStr, 20, 40);
    
}

//--------------------------------------------------------------
void testApp::exit()
{
    //gui->saveSettings("GUI/guiSettings.xml"); // not using to save settings at the moment
    delete gui;
    monomeControl.ClearLEDs(false);
}


//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
    if(e.widget->getName() == "X")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        xoffset = slider->getScaledValue();
        //cout << "x: " << slider->getScaledValue() << endl;
    }
    else if(e.widget->getName() == "Y")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        yoffset = slider->getScaledValue();
        //cout << "y: " << slider->getScaledValue() << endl;
    }
    else if(e.widget->getName() == "blinkInterval")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        blinkInterval = slider->getScaledValue();
        //cout << "blinkInterval: " << slider->getScaledValue() << endl;
    }
    else if(e.widget->getName() == "rootFrequency")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        rootFrequency = slider->getScaledValue();
        //cout << "rootFrequency: " << slider->getScaledValue() << endl;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    // UP, DOWN, LEFT & RIGHT keys to move the 8x8 device outline around the virtual 16x16 matrix
    if (key == OF_KEY_UP) {
        yoffset = ofClamp(yoffset-1,0,8);
        xoffset = ofClamp(xoffset-1,0,8);
    } else if (key == OF_KEY_DOWN) {
        yoffset = ofClamp(yoffset+1,0,8);
        xoffset = ofClamp(xoffset+1,0,8);
    } else if (key == OF_KEY_LEFT) {
        yoffset = ofClamp(yoffset+1,0,8);
        xoffset = ofClamp(xoffset-1,0,8);
    } else if (key == OF_KEY_RIGHT) {
        yoffset = ofClamp(yoffset-1,0,8);
        xoffset = ofClamp(xoffset+1,0,8);
    }
    /// ofxUI
    switch (key){
        case 'g':
        case 'G':
            gui->toggleVisible();
            break;
    }
    /// ofxGrabCam
    //  if (key=='c')
    //		camera.toggleCursorDraw();
    //	if (key=='u')
    //		camera.toggleFixUpwards();
    //
	if (key=='s')
		savedPose = camera.getGlobalTransformMatrix();
    //cout << "camera.getGlobalTransformMatrix(): " << camera.getGlobalTransformMatrix() << endl;
	if (key=='l')
		camera.setTransformMatrix(savedPose);
    if (key==' ')
		randomiseViewport();
	if (key=='v')
		useViewport ^= true;
    if (key=='r')
    camera.reset();
    if (key=='t')
        camera.resetDefault();
    /// scene assets
    if (key=='p')
        LMVisible = !LMVisible;
    if (key=='o')
    LEDzVisible = !LEDzVisible;
    if (key=='i')
        ThreeDAxisVisible = !ThreeDAxisVisible;
}

//--------------------------------------------------------------
void testApp::threeDAxis()
{
    //3D axis
    
    ofColor c = ofGetStyle().color;
    
    float deg = 90.0;
    float rad = ofDegToRad(deg);
    float len = 200;
    float arrowsize = 16;
    ofSetLineWidth(2);
    ofNoFill();
    ofSetColor(0, 255, 0);
    ofLine(0,0,0,len,0,0);
    ofLine(len,0,0,len-arrowsize,+arrowsize/2,0);
    ofLine(len,0,0,len-arrowsize,-arrowsize/2,0);
    ofSetColor(255, 0, 0);
    ofLine(0,0,0,0,len,0);
    ofLine(0,len,0,+arrowsize/2,len-arrowsize,0);
    ofLine(0,len,0,-arrowsize/2,len-arrowsize,0);
    ofSetColor(0, 0, 255);
    ofLine(0,0,0,0,0,len);
    ofLine(0,0,len,+arrowsize/2,0,len-arrowsize);
    ofLine(0,0,len,-arrowsize/2,0,len-arrowsize);
    
}

//--------------------------------------------------------------
void testApp::randomiseViewport() {
	viewport.x = ofRandom(0,ofGetWidth()/2);
	viewport.y = ofRandom(0,ofGetHeight()/2);
	viewport.width = ofRandom(256, ofGetWidth() - viewport.x);
	viewport.height = ofRandom(256, ofGetHeight() - viewport.y);
}


//--------------------------------------------------------------
void testApp::keyReleased  (int key){
	
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
void testApp::mouseReleased(){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
    
}

