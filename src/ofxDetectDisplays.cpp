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
	displayInfo->width = lprcMonitor->right  - lprcMonitor->left;
	displayInfo->height = lprcMonitor->bottom - lprcMonitor->top;

	DisplaysParam* displaysParam = (DisplaysParam*) dwData;
	displaysParam->count++;
	displaysParam->displays->push_back(displayInfo);

	cout << displayInfo->width << "::" << displayInfo->height << " - " << lprcMonitor->left << " - " << lprcMonitor->top << " - " << lprcMonitor->right << " - " << lprcMonitor->bottom << endl;

    return TRUE;
}
#endif

//--------------------------------------------------------------
//--------------------------------------------------------------
ofxDetectDisplays::ofxDetectDisplays()
{
    
}

//--------------------------------------------------------------
ofxDetectDisplays::~ofxDetectDisplays()
{

}

//--------------------------------------------------------------
int ofxDetectDisplays::detectDisplays()
{

#ifdef TARGET_OSX
    CGDisplayCount displayCount;
    CGDirectDisplayID displaysID[32];
    CGGetActiveDisplayList(32, displaysID, &displayCount);

    for (int i=0; i<displayCount; i++) {
        cout<< i << " - display ID: " << displaysID[i] << " - display size: " << CGDisplayPixelsWide(displaysID[i]) << " - " <<  CGDisplayPixelsHigh(displaysID[i]) << endl;
        
        DisplayInfo* displayInfo = new DisplayInfo();
        displayInfo->width = CGDisplayPixelsWide(displaysID[i]);
        displayInfo->height = CGDisplayPixelsHigh(displaysID[i]);
        
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
ofRectangle ofxDetectDisplays::getDisplayFrame(int displayID)
{
    int top = 0;
    int left = 0;
    int width = 0;
    int height = 0;
    
    if (_displays.size() > 0 && displayID >= 0 && displayID < _displays.size()) {
        top = 0;
        left = 0;
        width = _displays[displayID]->width;
        height = _displays[displayID]->height;
    }
    
    return ofRectangle(.0f, .0f, width, height);
}

//--------------------------------------------------------------
const vector<DisplayInfo*> & ofxDetectDisplays::getDisplays()
{
    return _displays;
}




