#include "ofxDetectDisplays.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
#ifdef TARGET_WIN32
struct DisplaysParam {
	int count;
	vector<DisplayInfo*> * displays;
};

BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	
	DisplayInfo* displayInfo = new DisplayInfo();
    displayInfo->top = lprcMonitor->top;
    displayInfo->left = lprcMonitor->left;
	displayInfo->width = lprcMonitor->right  - lprcMonitor->left;
	displayInfo->height = lprcMonitor->bottom - lprcMonitor->top;

	DisplaysParam* displaysParam = (DisplaysParam*) dwData;
	displaysParam->count++;
	displaysParam->displays->push_back(displayInfo);

    return TRUE;
}
#endif

//--------------------------------------------------------------
//--------------------------------------------------------------
ofxDetectDisplays::ofxDetectDisplays()
{
    ofAddListener(ofEvents().update, this, &ofxDetectDisplays::update);
    
    while(!_displays.empty()) delete _displays.back(), _displays.pop_back();
	_displays.clear();
}

//--------------------------------------------------------------
ofxDetectDisplays::~ofxDetectDisplays()
{
    ofRemoveListener(ofEvents().update, this, &ofxDetectDisplays::update);
}

//--------------------------------------------------------------
void ofxDetectDisplays::update(ofEventArgs & args)
{
    if(_doPlaceWindowNextCycle && _actionOnDisplayID >= 0) {
        placeWindowOnDisplay(_actionOnDisplayID);
        _doPlaceWindowNextCycle = false;
        _actionOnDisplayID = -1;
    }
    
    if (_doFullscreenWindowNextCycle && _actionOnDisplayID >= 0) {
        fullscreenWindowOnDisplay(_actionOnDisplayID);
        _doFullscreenWindowNextCycle = false;
        _actionOnDisplayID = -1;
    }
}

//--------------------------------------------------------------
int ofxDetectDisplays::detectDisplays()
{
    _actionOnDisplayID = false;
    _doPlaceWindowNextCycle = false;
    _doFullscreenWindowNextCycle = -1;
    
#ifdef TARGET_OSX
    CGDisplayCount displayCount;
    CGDirectDisplayID mainDisplayID = CGMainDisplayID();
    CGDirectDisplayID displaysID[32];
    CGGetActiveDisplayList(32, displaysID, &displayCount);

    for (int i=0; i<displayCount; i++) {
        CGRect displayRect = CGDisplayBounds(displaysID[i]);
        
        DisplayInfo* displayInfo = new DisplayInfo();
        displayInfo->isPrimary = false;
        if (displaysID[i] == mainDisplayID) {
            displayInfo->isPrimary = true;
        }
        
        displayInfo->left = displayRect.origin.x;
        displayInfo->top = displayRect.origin.y;
        displayInfo->width = displayRect.size.width;
        displayInfo->height = displayRect.size.height;
    
        _displays.push_back(displayInfo);
    }
    
    return displayCount;
    
#endif
    
#ifdef TARGET_WIN32
	DisplaysParam displaysParam;
	displaysParam.count = 0;
	displaysParam.displays = &_displays;

    if (EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)&displaysParam)) {
		return displaysParam.count;
	}
    return -1;

#endif

}

//--------------------------------------------------------------
int ofxDetectDisplays::getNumDisplays()
{
    return _displays.size();
}

//--------------------------------------------------------------
ofRectangle ofxDetectDisplays::getDisplayBounds(int displayID)
{
    int top = 0;
    int left = 0;
    int width = 0;
    int height = 0;
    
    if (_displays.size() > 0 && displayID >= 0 && displayID < _displays.size()) {
        left = _displays[displayID]->left;
        top = _displays[displayID]->top;
        width = _displays[displayID]->width;
        height = _displays[displayID]->height;
    }
    
    return ofRectangle(left, top, width, height);
}

//--------------------------------------------------------------
bool ofxDetectDisplays::isDisplayPrimary(int displayID)
{
    if (_displays.size() == 0 || displayID > _displays.size()-1) {
        return false;
    }
    
    return _displays[displayID]->isPrimary;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::isMirroringEnabled()
{
#ifdef TARGET_OSX
    return CGDisplayIsInMirrorSet(CGMainDisplayID());
#endif
    
#ifdef TARGET_WIN32
    
#endif
}

//--------------------------------------------------------------
const vector<DisplayInfo*> & ofxDetectDisplays::getDisplays()
{
    return _displays;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::placeWindowOnDisplay(int displayID)
{
    // All the following can not be done in the same cycle, this is why we split method on 2 cycles we are in fullscreen
    if (ofGetWindowMode() == OF_FULLSCREEN) {
        ofSetFullscreen(false);
        _doPlaceWindowNextCycle = true;
        _actionOnDisplayID = displayID;
        return false;
    }

    ofSetWindowPosition(_displays[displayID]->left, _displays[displayID]->top);
    ofSetWindowShape(_displays[displayID]->width, _displays[displayID]->height);

    return true;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::fullscreenWindowOnDisplay(int displayID)
{
    // All the following can not be done in the same cycle, this is why we split method on 2 cycles we are in fullscreen
    if (ofGetWindowMode() == OF_FULLSCREEN) {
        ofSetFullscreen(false);
        _doFullscreenWindowNextCycle = true;
        _actionOnDisplayID = displayID;
        return false;
    }

    int offset = 1; // if we just move the windows to the top left corner of the display, it will go back to the primary display when going fullscreen.
    ofSetWindowPosition(_displays[displayID]->left + offset, _displays[displayID]->top + offset);
    ofSetFullscreen(true);
    
    return true;
}




