#include "ofxDetectDisplays.h"

//--------------------------------------------------------------
//--------------------------------------------------------------
#if defined(TARGET_WIN32)
struct DisplaysParam {
	int count;
	vector<DisplayInfo*> * displays;
};

BOOL CALLBACK monitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	MONITORINFO mi;
	mi.cbSize = sizeof(MONITORINFO);
	GetMonitorInfo(hMonitor, &mi);

	DisplayInfo* displayInfo = new DisplayInfo();
	if (mi.dwFlags == MONITORINFOF_PRIMARY) {
		displayInfo->isPrimary = true;
	} else {
		displayInfo->isPrimary = false;
	}
	
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

}

//--------------------------------------------------------------
ofxDetectDisplays::~ofxDetectDisplays()
{
    clearDisplays();
}

//--------------------------------------------------------------
int ofxDetectDisplays::detectDisplays()
{
    clearDisplays();
    
#if defined(TARGET_OSX)
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
    
        displays.push_back(displayInfo);
    }
    
    return displayCount;
    
#elif defined(TARGET_WIN32)
	DisplaysParam displaysParam;
	displaysParam.count = 0;
	displaysParam.displays = &displays;

    if (EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)&displaysParam)) {
		return displaysParam.count;
	}
    return -1;

#endif
}

//--------------------------------------------------------------
void ofxDetectDisplays::clearDisplays()
{
    while (!displays.empty()) delete displays.back(), displays.pop_back();
	displays.clear();
}

//--------------------------------------------------------------
int ofxDetectDisplays::getNumDisplays()
{
    return displays.size();
}

//--------------------------------------------------------------
ofRectangle ofxDetectDisplays::getDisplayBounds(int displayID)
{
    int top = 0;
    int left = 0;
    int width = 0;
    int height = 0;
    
    if (displays.size() > 0 && displayID >= 0 && displayID < displays.size()) {
        left = displays[displayID]->left;
        top = displays[displayID]->top;
        width = displays[displayID]->width;
        height = displays[displayID]->height;
    }
    
    return ofRectangle(left, top, width, height);
}

//--------------------------------------------------------------
bool ofxDetectDisplays::isDisplayPrimary(int displayID)
{
    if (displays.size() == 0 || displayID > displays.size()-1) {
        return false;
    }
    
    return displays[displayID]->isPrimary;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::isMirroringEnabled()
{
#if defined(TARGET_OSX)
    return CGDisplayIsInMirrorSet(CGMainDisplayID());

#elif defined(TARGET_WIN32)
	// have not found yet how to do this on Windows
	return false;
#endif
}

//--------------------------------------------------------------
const vector<DisplayInfo*> & ofxDetectDisplays::getDisplays()
{
    return displays;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::placeWindowOnDisplay(int displayID)
{

#if defined(TARGET_OSX)
    ofSetFullscreen(false);
    ofSetWindowPosition(displays[displayID]->left, displays[displayID]->top);
    ofSetWindowShape(displays[displayID]->width, displays[displayID]->height);

#elif defined(TARGET_WIN32)
	HWND hwnd = ofGetWin32Window();
 
	DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW;
	DWORD STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	SetWindowLong(hwnd, GWL_EXSTYLE, EX_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, STYLE);
	SetWindowPos(hwnd, HWND_TOPMOST, displays[displayID]->left, displays[displayID]->top, displays[displayID]->width, displays[displayID]->height, SWP_SHOWWINDOW);
#endif

	return true;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::fullscreenWindowOnDisplay(int displayID)
{
#if defined(TARGET_OSX)
    ofSetFullscreen(true);
    ofSetWindowShape(displays[displayID]->width, displays[displayID]->height);
    ofSetWindowPosition(displays[displayID]->left, displays[displayID]->top);

#elif defined(TARGET_WIN32)
	HWND hwnd = ofGetWin32Window();
 
	SetWindowLong(hwnd, GWL_EXSTYLE, 0);
  	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	SetWindowPos(hwnd, HWND_TOPMOST, displays[displayID]->left, displays[displayID]->top, displays[displayID]->width, displays[displayID]->height, SWP_SHOWWINDOW);
#endif

    return true;
}
