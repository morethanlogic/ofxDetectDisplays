#pragma once

#include "ofMain.h"

//--------------------------------------------------------------
struct DisplayInfo
{
    bool isPrimary;
    int left;
    int top;
	int width;
	int height;
};

//--------------------------------------------------------------
class ofxDetectDisplays
{
public:
    
    ofxDetectDisplays();
    ~ofxDetectDisplays();
    
    int detectDisplays();
    void clearDisplays();

    int getNumDisplays();
    ofRectangle getDisplayBounds(int displayID);
    bool isDisplayPrimary(int displayID);
    bool isMirroringEnabled();

    const vector<DisplayInfo*> & getDisplays();

    bool placeWindowOnDisplay(int displayID);
    bool fullscreenWindowOnDisplay(int displayID);
    

private:
    vector<DisplayInfo*> displays;
};

