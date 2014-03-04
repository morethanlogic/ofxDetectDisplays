#include "ofxDetectDisplays.h"


//--------------------------------------------------------------
ofxDetectDisplays::ofxDetectDisplays()
{
    
}

//--------------------------------------------------------------
ofxDetectDisplays::~ofxDetectDisplays()
{

}

//--------------------------------------------------------------
void ofxDetectDisplays::detectDisplays()
{

#ifdef TARGET_OSX
    CGDisplayCount displayCount;
    CGDirectDisplayID displays[32];
    CGGetActiveDisplayList(32, displays, &displayCount);

    for (int i=0; i<displayCount; i++) {
        cout<< i << " - display ID: " << displays[i] << " - display size: " << CGDisplayPixelsWide(displays[i]) << " - " <<  CGDisplayPixelsHigh(displays[i]) << endl;
    }
    
#endif
    
#ifdef TARGET_WIN32
    
#endif

}



