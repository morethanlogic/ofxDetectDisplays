#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	detectDisplays.detectDisplays();
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    string mess = "";
    
    mess = "Number of displays: " + ofToString(detectDisplays.getNumDisplays()) + "\n";
    
    mess += "Is mirroring enabled: " + ofToString(detectDisplays.isMirroringEnabled()) + "\n\n";
    
	for (int i=0; i<detectDisplays.getDisplays().size(); i++) {
        ofRectangle displayBounds = detectDisplays.getDisplayBounds(i);
        
        mess += "Display " + ofToString(i) + "\n";
        mess += "Primary :" + ofToString(detectDisplays.isDisplayPrimary(i)) + "\n";
        mess += "Origin: " + ofToString(displayBounds.x) + " - " + ofToString(displayBounds.y) + "\n";
        mess += "Size: " + ofToString(displayBounds.width) + " - " + ofToString(displayBounds.height) + "\n\n";
	}
    
    ofSetColor(0);
    ofDrawBitmapString(mess, 10, 20);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
