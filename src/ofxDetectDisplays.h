#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
struct DisplayInfo {
    bool isPrimary;
    int left;
    int top;
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
        ofRectangle getDisplayBounds(int displayID);
        bool isDisplayPrimary(int displayID);
    
        const vector<DisplayInfo*> & getDisplays();

    private:
        vector<DisplayInfo*> _displays;

};