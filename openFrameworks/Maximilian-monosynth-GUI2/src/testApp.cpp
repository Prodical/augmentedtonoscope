/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 You can copy and paste this and use it as a starting example.
 */

#include "testApp.h"


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
    
    VCO1F = 55;
    VCO1PW = 0.6;
    VCO2F = 110;
    VCO2PW = 0.2;
    waveTypeVCO1 = 4;
    waveTypeVCO2 = 4;
    LFO1F = 0.0;
    LFO2F = 0.2;
    filterTypeVCF = 0;
    VCFCutoff = 250;
    VCFResonance = 10;
    cutoffMin = -5000.0;
    cutoffMax = 5000.0;
    resonanceMin = 0.0;
    resonanceMax = 100.0;
    // tried these settings for bandpass, lopass and hipass - don't work
    //    VCFCutoff = 0.0;
    //    VCFResonance = 0.0;
    //    cutoffMin = 0.0;
    //    cutoffMax = 1.0;
    //    resonanceMin = 0.0;
    //    resonanceMax = 1.0;
    autoTriggerToggle = false;
    triggerTime = 1.0;
    
    // initialise  2D vectors for the ADSR
    adsrA.set(5.0, 1.0);
    adsrD.set(250.0, 0.125);
    adsrS.set(125.0, 0.125);
    adsrR.set(500.0, 0.0);
    // initialise a second ADSR array
    double adsrEnv2[8]={adsrA.x,adsrA.y,adsrD.x,adsrD.y,adsrS.x,adsrS.y,adsrR.x,adsrR.y};
    
// alternative pointer method for updating ADSR array
//    double * p = adsrEnv;
//    *p = AT;
//    p++;
//    *p = AV;
//    p++;
//    *p = DT;
//    p++;
//    *p = DV;
//    p++;
//    *p = ST;
//    p++;
//    *p = SV;
//    p++;
//    *p = RT;
//    p++;
//    *p = RV;
//    
//    for (int i = 0; i<8; i++) {
//    cout << adsrEnv[i] << endl;
//    }

    ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);/* Call this last ! */
    
    //ofxUI
    ofEnableSmoothing();
    setGUI4();
    gui4->setDrawBack(false);
}

//--------------------------------------------------------------
void testApp::update(){
    
    if (ATLast != AT || AVLast != AV) {
        ATLast = AT;
        AVLast = AV;
        adsrA.set(ATLast, AVLast);
        ADSRUpdate = true;
    }
    if (DTLast != DT || DVLast != DV) {
        DTLast = DT;
        DVLast = DV;
        adsrD.set(DTLast, DVLast);
        ADSRUpdate = true;
    }
    if (STLast != ST || SVLast != SV) {
        STLast = ST;
        SVLast = SV;
        adsrS.set(STLast, SVLast);
        ADSRUpdate = true;
    }
    if (RTLast != RT || RVLast != RV) {
        RTLast = RT;
        RVLast = RV;
        adsrR.set(RTLast, RVLast);
        ADSRUpdate = true;
    }
    if (ADSRUpdate) {
        double adsrEnv2[8]={adsrA.x,adsrA.y,adsrD.x,adsrD.y,adsrS.x,adsrS.y,adsrR.x,adsrR.y};
        // output adsrEnv2 values to console
        for (int i = 0; i < 8; i++) {
            cout << "adsrEnv2[" << i << "]: " << adsrEnv2[i] << endl;
        }
        // copy ADSREnv2 values to ADSREnv
        // doesn't work if I do this - even if I repeat this in audioRequested() as advised by Mick
        //for (int i = 0; i < 8; i++) {
        //    adsrEnv[i] = adsrEnv2[i];
        //    cout << "adsrEnv[" << i << "]: " << adsrEnv[i] << endl;
        //}
        ADSRUpdate = false;
    }
    
}


//--------------------------------------------------------------
void testApp::draw(){
	
	/* You can use any of the data from audio received and audiorequested to draw stuff here.
	 Importantly, most people just use the input and output arrays defined above.
	 Clever people don't do this. This bit of code shows that by default, each signal is going to flip
	 between -1 and 1. You need to account for this somehow. Get the absolute value for example.
	 */
	
	ofSetColor(255, 255, 255,255);
    ofCircle(200, 300, wave*150);
    
}

//--------------------------------------------------------------
void testApp::audioRequested (float * output, int bufferSize, int nChannels){
	
	for (int i = 0; i < bufferSize; i++){
		
		/* Stick your maximilian 'play()' code in here ! Declare your objects in testApp.h.
		 
		 For information on how maximilian works, take a look at the example code at
		 
		 http://www.maximilian.strangeloop.co.uk
		 
		 under 'Tutorials'.
		 
		 */
        
        //monosynth.cpp example
        
        //so this first bit is just a basic metronome so we can hear what we're doing.
        currentCount=(int)timer.phasor(triggerTime);//this sets up a metronome that ticks every 2 seconds
        // hit "SPACE" to trigger - or click on autotrigger button in GUI 
        if (autoTriggerToggle){
            if (lastCount!=currentCount) {//if we have a new timer int this sample, play the sound
                ADSR.trigger(0, adsrEnv[0]);//trigger the envelope from the start
                cout << "tick\n";//the clock ticks
                lastCount=0;//set lastCount to 0
            }
        } else {
        }
        
        //and this is where we build the synth
        
        // copy ADSREnv2 values to ADSREnv in audioRequested as suggested by Mick
        // doesn't work if I do this
        //for (int i = 0; i < 8; i++) {
        //    adsrEnv[i] = adsrEnv2[i];
        //    //cout << "adsrEnv[" << i << "]: " << adsrEnv[i] << endl;
        //}

        //our ADSR env has 8 value/time pairs.
        ADSRout=ADSR.line(8,adsrEnv);
        
        //this lfo is a sinewave at 0.2 hz
        LFO1out=LFO1.sinebuf(LFO1F);
        LFO2out=LFO2.sinebuf(LFO2F);
        
        //here's VCO1. it's a pulse wave at 55 hz, with a pulse width of 0.6
        if (waveTypeVCO1 == 0)
            VCO1out=VCO1.sinewave(VCO1F+LFO1out);
        else if (waveTypeVCO1 == 1)
            VCO1out=VCO1.triangle(VCO1F+LFO1out);
        else if (waveTypeVCO1 == 2)
            VCO1out=VCO1.saw(VCO1F+LFO1out);
        else if (waveTypeVCO1 == 3)
            VCO1out=VCO1.square(VCO1F+LFO1out);
        else if (waveTypeVCO1 == 4)
            VCO1out=VCO1.pulse(VCO1F+LFO1out,VCO1PW);
        else if (waveTypeVCO1 == 5)
            VCO1out=VCO1.phasor(VCO1F+LFO1out);
        else if (waveTypeVCO1 == 6)
            VCO1out=VCO1.noise();
        
        //here's VCO2. it's a pulse wave at 110hz with LFO modulation on the frequency, and width of 0.2
        if (waveTypeVCO2 == 0)
            VCO2out=VCO2.sinewave(VCO2F+LFO2out);
        else if (waveTypeVCO2 == 1)
            VCO2out=VCO2.triangle(VCO2F+LFO2out);
        else if (waveTypeVCO2 == 2)
            VCO2out=VCO2.saw(VCO2F+LFO2out);
        else if (waveTypeVCO2 == 3)
            VCO2out=VCO2.square(VCO2F+LFO2out);
        else if (waveTypeVCO2 == 4)
            VCO2out=VCO2.pulse(VCO2F+LFO2out,VCO2PW);
        else if (waveTypeVCO2 == 5)
            VCO2out=VCO2.phasor(VCO2F+LFO2out);
        else if (waveTypeVCO2 == 6)
            VCO2out=VCO2.noise();
        
        //now we stick the VCO's into the VCF, using the ADSR as the filter cutoff
        if (filterTypeVCF == 0)
        VCFout=VCF.lores((VCO1out+VCO2out)*0.5, VCFCutoff+(ADSRout*15000), VCFResonance);
        else if (filterTypeVCF == 1)
        VCFout=VCF.hires((VCO1out+VCO2out)*0.5, VCFCutoff+(ADSRout*15000), VCFResonance);
        else if (filterTypeVCF == 2)
        VCFout=VCF.bandpass((VCO1out+VCO2out)*0.5, VCFCutoff+(ADSRout*15000), VCFResonance);
        else if (filterTypeVCF == 3)
        VCFout=VCF.lopass((VCO1out+VCO2out)*0.5, VCFCutoff+(ADSRout*15000));
        else if (filterTypeVCF == 4)
        VCFout=VCF.hipass((VCO1out+VCO2out)*0.5, VCFCutoff+(ADSRout*15000));
        

        wave=VCFout*ADSRout;//finally we add the ADSR as an amplitude modulator
        
        //now we send the sounds to some stereo busses.
        mixer.stereo(wave, outputs, 0.5);
        //mixing
		output[i*nChannels    ] = outputs[0]; /* You may end up with lots of outputs. add them here */
		output[i*nChannels + 1] = outputs[1];
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
    
    if (key == ' '){
        ADSR.trigger(0, adsrEnv[0]);//trigger the envelope from the start
    }
    if (key == 'v') {
        gui4->toggleVisible();
    }
	
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

/// ofxUI stuff
//--------------------------------------------------------------
void testApp::exit()
{
    gui4->saveSettings("GUI/guiSettings.xml");
    delete gui4;
}

//--------------------------------------------------------------
void testApp::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	cout << "got event from: " << name << endl;
    
    
    /// VCO1
	if(name == "VCO1F")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "VCO1F " << slider->getScaledValue() << endl;
		VCO1F = slider->getScaledValue();
	}
    if(name == "VCO1PW")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "VCO1PW " << slider->getScaledValue() << endl;
		VCO1PW = slider->getScaledValue();
	}
    if(name == "1-SINE")
	{
        waveTypeVCO1 = 0;
	}
    if(name == "1-TRIANGLE")
	{
        waveTypeVCO1 = 1;
	}
    if(name == "1-SAW")
	{
        waveTypeVCO1 = 2;
	}
    if(name == "1-SQUARE")
	{
        waveTypeVCO1 = 3;
	}
    if(name == "1-PULSE")
	{
        waveTypeVCO1 = 4;
	}
    if(name == "1-PHASOR")
	{
        waveTypeVCO1 = 5;
	}
    if(name == "1-NOISE")
	{
        waveTypeVCO1 = 6;
	}
    
    /// VCO2
    if(name == "VCO2F")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "VCO2F " << slider->getScaledValue() << endl;
		VCO2F = slider->getScaledValue();
	}
    if(name == "VCO2PW")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "VCO2PW " << slider->getScaledValue() << endl;
		VCO2PW = slider->getScaledValue();
	}
    if(name == "2-SINE")
    {
        waveTypeVCO2 = 0;
    }
    if(name == "2-TRIANGLE")
    {
        waveTypeVCO2 = 1;
    }
    if(name == "2-SAW")
    {
        waveTypeVCO2 = 2;
    }
    if(name == "2-SQUARE")
    {
        waveTypeVCO2 = 3;
    }
    if(name == "2-PULSE")
    {
        waveTypeVCO2 = 4;
    }
    if(name == "2-PHASOR")
    {
        waveTypeVCO2 = 5;
    }
    if(name == "2-NOISE")
    {
        waveTypeVCO2 = 6;
    }
    
    /// LFO
    if(name == "LFO1F")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "LFO1F " << slider->getScaledValue() << endl;
		LFO1F = slider->getScaledValue();
	}
    if(name == "LFO2F")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "LFO2F " << slider->getScaledValue() << endl;
		LFO1F = slider->getScaledValue();
	}
    
    /// AUTO TRIGGER
    if(name == "AUTOTRIGGER")
	{
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        if (toggle->getValue() == 1){
            autoTriggerToggle = true;
        } else {
            autoTriggerToggle = false;
        }
	}
    if(name == "TRIGGERTIME")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "TRIGGERTIME " << slider->getScaledValue() << endl;
		triggerTime = 1/slider->getScaledValue();
	}
    
    /// VCF
    if(name == "CUTOFF")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "CUTOFF " << slider->getScaledValue() << endl;
		VCFCutoff = slider->getScaledValue();
	}
    if(name == "RESONANCE")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "RESONANCE " << slider->getScaledValue() << endl;
		VCFResonance = slider->getScaledValue();
	}
    if(name == "LORES")
    {
        filterTypeVCF = 0;
        cutoffMin = -5000.0;
        cutoffMax = 5000.0;
        resonanceMin = 0.0;
        resonanceMax = 100.0;
        gui4->update();
//        setGUI4();
//        gui4->setDrawBack(false);
    }
    if(name == "HIGHRES")
    {
        filterTypeVCF = 1;
        cutoffMin = -5000.0;
        cutoffMax = 5000.0;
        resonanceMin = 0.0;
        resonanceMax = 100.0;
//        setGUI4();
//        gui4->setDrawBack(false);
    }
    if(name == "BANDPASS")
    {
        filterTypeVCF = 2;
        cutoffMin = 0.0;
        cutoffMax = 1.0;
        resonanceMin = 0.0;
        resonanceMax = 1.0;
//        setGUI4();
//        gui4->setDrawBack(false);
    }
    if(name == "LOPASS")
    {
        filterTypeVCF = 3;
        cutoffMin = 0.0;
        cutoffMax = 1.0;
        resonanceMin = 0.0;
        resonanceMax = 1.0;
//        setGUI4();
//        gui4->setDrawBack(false);
    }
    if(name == "HIPASS")
    {
        filterTypeVCF = 4;
        cutoffMin = 0.0;
        cutoffMax = 1.0;
        resonanceMin = 0.0;
        resonanceMax = 1.0;;
        gui4->update();
//        setGUI4();
//        gui4->setDrawBack(false);
    }
    
    /// ADSR

    if(name == "AT")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "AT " << slider->getScaledValue() << endl;
		AT = slider->getScaledValue();
	}
    if(name == "AV")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "AV " << slider->getScaledValue() << endl;
		AV = slider->getScaledValue();
	}
    if(name == "DT")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "DT " << slider->getScaledValue() << endl;
		DT = slider->getScaledValue();
	}
    if(name == "DV")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "DV " << slider->getScaledValue() << endl;
		DV = slider->getScaledValue();
	}
    if(name == "ST")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "ST " << slider->getScaledValue() << endl;
		ST = slider->getScaledValue();
	}
    if(name == "SV")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "SV " << slider->getScaledValue() << endl;
		SV = slider->getScaledValue();
	}
    if(name == "RT")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "RT " << slider->getScaledValue() << endl;
		RT = slider->getScaledValue();
	}
    if(name == "RV")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "RV " << slider->getScaledValue() << endl;
		RV = slider->getScaledValue();
	}



}


//--------------------------------------------------------------
void testApp::setGUI4()
{
	float dim = 16;
    float sliderHeight = 150;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    //    img = new ofImage();
    //    img->loadImage("nerd_me.png");
    
    vector<string> waveTypeVCO1Items;
    waveTypeVCO1Items.push_back("1-SINE");
    waveTypeVCO1Items.push_back("1-TRIANGLE");
    waveTypeVCO1Items.push_back("1-SAW");
    waveTypeVCO1Items.push_back("1-SQUARE");
    waveTypeVCO1Items.push_back("1-PULSE");
    waveTypeVCO1Items.push_back("1-PHASOR");
    waveTypeVCO1Items.push_back("1-NOISE");
    
    vector<string> waveTypeVCO2Items;
    waveTypeVCO2Items.push_back("2-SINE");
    waveTypeVCO2Items.push_back("2-TRIANGLE");
    waveTypeVCO2Items.push_back("2-SAW");
    waveTypeVCO2Items.push_back("2-SQUARE");
    waveTypeVCO2Items.push_back("2-PULSE");
    waveTypeVCO2Items.push_back("2-PHASOR");
    waveTypeVCO2Items.push_back("2-NOISE");
    
    vector<string> filterTypeVCFItems;
    filterTypeVCFItems.push_back("LORES");
    filterTypeVCFItems.push_back("HIRES");
    // These don't work - even between 0.0 and 1.0 as Mick advises
//    filterTypeVCFItems.push_back("BANDPASS");
//    filterTypeVCFItems.push_back("LOPASS");
//    filterTypeVCFItems.push_back("HIPASS");

    
	gui4 = new ofxUIScrollableCanvas(length*1+xInit*2+6, 0, length*3+xInit, ofGetHeight());
    gui4->addWidgetDown(new ofxUILabel("MAXIMILIAN MONOSYNTH", OFX_UI_FONT_LARGE));
    
    /// VC1 & LFO1
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("VC01", OFX_UI_FONT_MEDIUM));
    
    gui4->addWidgetRight(new ofxUINumberDialer(0, 2000, 55, 2, "VCO1F", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 0.6, 2, "VCO1PW", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 50.0, 0.0, 2, "LFO1F", OFX_UI_FONT_SMALL));
    
    gui4->addWidgetDown(new ofxUIRotarySlider(dim*4, 0.0, 2000.0, 55, "VCO1F"));
    gui4->addWidgetRight(new ofxUIRotarySlider(dim*4, 0.0, 1.0, 0.6, "VCO1PW"));
    gui4->addWidgetRight(new ofxUIRotarySlider(dim*4, 0.0, 50.0, 0.0, "LFO1F"));
    
    gui4->addWidgetDown(new ofxUIRadio( dim, dim, "", waveTypeVCO1Items, OFX_UI_ORIENTATION_HORIZONTAL));
    
    /// VCO2 & LFO2
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("VC02", OFX_UI_FONT_MEDIUM));
    
    gui4->addWidgetRight(new ofxUINumberDialer(0, 2000, 110, 2, "VCO2F", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 0.2, 2, "VCO2PW", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 50.0, 0.2, 2, "LFO2F", OFX_UI_FONT_SMALL));
    
    gui4->addWidgetDown(new ofxUIRotarySlider(dim*4, 0.0, 2000.0, 110, "VCO2F"));
    gui4->addWidgetRight(new ofxUIRotarySlider(dim*4, 0.0, 1.0, 0.2, "VCO2PW"));
    gui4->addWidgetRight(new ofxUIRotarySlider(dim*4, 0.0, 50.0, 0.2, "LFO2F"));
    
    gui4->addWidgetDown(new ofxUIRadio( dim, dim, "", waveTypeVCO2Items, OFX_UI_ORIENTATION_HORIZONTAL));
    
    /// ADSR
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("ADSR", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUINumberDialer(0.0, 500.0, 5.0, 2, "AT", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 1.0, 2, "AV", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 500.0, 250, 2, "DT", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 0.125, 2, "DV", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 500.0, 125, 2, "ST", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 0.125, 2, "SV", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 500.0, 500.0, 2, "RT", OFX_UI_FONT_SMALL));
    gui4->addWidgetRight(new ofxUINumberDialer(0.0, 1.0, 0.0, 2, "RV", OFX_UI_FONT_SMALL));
    
    /// VCF
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("VCF", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIRotarySlider(dim*4, cutoffMin, cutoffMax, VCFCutoff, "CUTOFF"));
    gui4->addWidgetRight(new ofxUIRotarySlider(dim*4, resonanceMin, resonanceMax, VCFResonance, "RESONANCE"));
    
    gui4->addWidgetDown(new ofxUIRadio( dim, dim, "", filterTypeVCFItems, OFX_UI_ORIENTATION_HORIZONTAL));
    
    ///autoTriggerToggle
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("AUTO TRIGGER", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIToggle( dim, dim, false, "AUTOTRIGGER"));
    gui4->addWidgetRight(new ofxUINumberDialer(0.1, 5.0, 1.0, 1, "TRIGGERTIME", OFX_UI_FONT_SMALL));
    
    /// FPS
    gui4->addWidgetDown(new ofxUISpacer(length-xInit, 2));
    gui4->addWidgetDown(new ofxUILabel("FPS", OFX_UI_FONT_MEDIUM));
    gui4->addWidgetDown(new ofxUIFPS(OFX_UI_FONT_SMALL));
    
    
	ofAddListener(gui4->newGUIEvent,this,&testApp::guiEvent);
}




