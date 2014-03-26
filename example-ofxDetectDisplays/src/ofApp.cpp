#include "ofApp.h"

bool _bMouseLeftButtonPressed = false;
bool _bMouseRightButtonPressed = false;

//--------------------------------------------------------------
void ofApp::setup()
{
    // You should only use the shared instance of ofxDetectDisplays,
    // otherwise event registration will be messed up.
    ofAddListener(ofxDetectDisplaysSharedInstance().displayConfigurationChanged, this, &ofApp::displayConfigurationChanged);
}

//--------------------------------------------------------------
void ofApp::update()
{

}

//--------------------------------------------------------------
void ofApp::draw()
{
    ofxDetectDisplays& detectDisplays = ofxDetectDisplaysSharedInstance();
    // You can also get the shared instance as a pointer.
    //ofxDetectDisplays* detectDisplays = ofxDetectDisplays::sharedInstance();
    
    string msg = "";
    msg = "Number of displays: " + ofToString(detectDisplays.getNumDisplays()) + "\n";
    msg += "Is mirroring enabled: " + ofToString(detectDisplays.isMirroringEnabled()) + "\n";
	msg += "Left-click on a display button to move to it, right-click to go fullscreen.";
    
    ofSetColor(0);
    ofDrawBitmapString(msg, 10, 20);
    
    int xOffset = 10;
    int yOffset = 70;
    
	for (int i=0; i<detectDisplays.getDisplays().size(); i++) {
        msg = "";
        
        ofRectangle displayBounds = detectDisplays.getDisplayBounds(i);
        ofRectangle displayDebugFrame = ofRectangle(xOffset, yOffset, displayBounds.width/10, displayBounds.height/10);
        
        
        if (detectDisplays.isDisplayPrimary(i)) {
            ofSetColor(255, 0, 0);
            ofRect(displayDebugFrame.x - 3, displayDebugFrame.y - 3, displayDebugFrame.width + 6, displayDebugFrame.height + 6);
        }
        
        ofSetColor(0);
        ofRect(displayDebugFrame);
        
        msg += "Display " + ofToString(i) + "\n";
        msg += "Primary :" + ofToString(detectDisplays.isDisplayPrimary(i)) + "\n";
        msg += "Origin: " + ofToString(displayBounds.x) + " - " + ofToString(displayBounds.y) + "\n";
        msg += "Size: " + ofToString(displayBounds.width) + " - " + ofToString(displayBounds.height) + "\n\n";
        
        ofSetColor(0);
        ofDrawBitmapString(msg, xOffset, yOffset + displayBounds.height/10 + 20);
        
        xOffset += 20 + displayBounds.width/10;
        
        if (displayDebugFrame.inside(mouseX, mouseY)) {
            if (_bMouseLeftButtonPressed) {
                _bMouseLeftButtonPressed = false;
                detectDisplays.placeWindowOnDisplay(i);
            }
            else if (_bMouseRightButtonPressed) {
                _bMouseRightButtonPressed = false;
                detectDisplays.fullscreenWindowOnDisplay(i);                
            }
        }

	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    if (button == 0) {
        _bMouseLeftButtonPressed = true;
    }
    else if (button == 2) {
        _bMouseRightButtonPressed = true;
    }
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{

}

//--------------------------------------------------------------
void ofApp::displayConfigurationChanged()
{
    ofLog() << "Display configuration changed!";
}
