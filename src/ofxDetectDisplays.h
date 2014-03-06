#pragma once

// OF
#include "ofMain.h"

struct DisplayInfo {
	int width;
	int height;
};

//--------------------------------------------------------------
//--------------------------------------------------------------
class ofxDetectDisplays {
    
    public:    
        ofxDetectDisplays();
        ~ofxDetectDisplays();
    
        int detectDisplays();
    
        int getNumDisplays();
        ofRectangle getDisplayFrame(int displayID);
    
        const vector<DisplayInfo*> & getDisplays();

    private:
        vector<DisplayInfo*> _displays;

};