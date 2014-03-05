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
		vector<DisplayInfo*> displays;

    private:
};