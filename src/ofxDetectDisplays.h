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
    
        void update(ofEventArgs & args);
    
        int detectDisplays();
    
        int getNumDisplays();
        ofRectangle getDisplayBounds(int displayID);
        bool isDisplayPrimary(int displayID);
        bool isMirroringEnabled();
    
        const vector<DisplayInfo*> & getDisplays();
    
        bool placeWindowOnDisplay(int displayID);
        bool fullscreenWindowOnDisplay(int displayID);

    private:
        vector<DisplayInfo*> _displays;
    
        int _actionOnDisplayID;
        bool _doPlaceWindowNextCycle;
        bool _doFullscreenWindowNextCycle;

};