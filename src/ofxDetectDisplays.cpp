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
    CGDirectDisplayID displays[32];
    CGGetActiveDisplayList(32, displays, &displayCount);

    for (int i=0; i<displayCount; i++) {
        cout<< i << " - display ID: " << displays[i] << " - display size: " << CGDisplayPixelsWide(displays[i]) << " - " <<  CGDisplayPixelsHigh(displays[i]) << endl;
    }
    
#endif
    
#ifdef TARGET_WIN32
	DisplaysParam displaysParam;
	displaysParam.count = 0;
	displaysParam.displays = &displays;

    if (EnumDisplayMonitors(NULL, NULL, monitorEnumProc, (LPARAM)&displaysParam)) {
		return displaysParam.count;
	}
    return -1;

#endif

}




