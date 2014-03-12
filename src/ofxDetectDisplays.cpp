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
	MONITORINFO mi;
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
    while(!_displays.empty()) delete _displays.back(), _displays.pop_back();
	_displays.clear();
}

//--------------------------------------------------------------
int ofxDetectDisplays::detectDisplays()
{
    
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
    
        _displays.push_back(displayInfo);
    }
    
    return displayCount;
    
#elif defined(TARGET_WIN32)
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
#if defined(TARGET_OSX)
    return CGDisplayIsInMirrorSet(CGMainDisplayID());

#elif defined(TARGET_WIN32)
	return false;
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

#if defined(TARGET_OSX)
    ofSetFullscreen(false);
    ofSetWindowPosition(_displays[displayID]->left, _displays[displayID]->top);
    ofSetWindowShape(_displays[displayID]->width, _displays[displayID]->height);

#elif defined(TARGET_WIN32)
	HWND hwnd = ofGetWin32Window();
 
	DWORD EX_STYLE = WS_EX_OVERLAPPEDWINDOW;
	DWORD STYLE = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	SetWindowLong(hwnd, GWL_EXSTYLE, EX_STYLE);
	SetWindowLong(hwnd, GWL_STYLE, STYLE);
	SetWindowPos(hwnd, HWND_TOPMOST, _displays[displayID]->left, _displays[displayID]->top, _displays[displayID]->width, _displays[displayID]->height, SWP_SHOWWINDOW);
#endif

	return true;
}

//--------------------------------------------------------------
bool ofxDetectDisplays::fullscreenWindowOnDisplay(int displayID)
{
#if defined(TARGET_OSX)
    ofSetFullscreen(true);
    ofSetWindowShape(_displays[displayID]->width, _displays[displayID]->height);
    ofSetWindowPosition(_displays[displayID]->left, _displays[displayID]->top);

#elif defined(TARGET_WIN32)
	HWND hwnd = ofGetWin32Window();
 
	SetWindowLong(hwnd, GWL_EXSTYLE, 0);
  	SetWindowLong(hwnd, GWL_STYLE, WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	SetWindowPos(hwnd, HWND_TOPMOST, _displays[displayID]->left, _displays[displayID]->top, _displays[displayID]->width, _displays[displayID]->height, SWP_SHOWWINDOW);
#endif

    return true;
}
