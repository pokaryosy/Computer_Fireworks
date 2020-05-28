#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    gui.setup();
    
    
    //Sliders
    // paremeters caption, default, value, min, max
    gui.add(bgColor.setup("BGColor", ofColor(ofRandom(255)), ofColor(0,0), ofColor(255, 255)));
    
    //Toggles
    gui.add(show.setup("drawObject", true));
    gui.add(fullScreen.setup("fullscreen", false));
    
    fullScreen.addListener(this, &ofApp::toggleFullscreenChanged);
    
    showGui = true;
    
    /*
    //initialize array pos
    for(int i=0; i<NUM; i++){
        pos[i].set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    }
    
    //initialize array vel
    for(int i=0; i<NUM; i++){
        vel[i].set(ofRandom(-3,3), ofRandom(-3,3));
    }
    */
    
    ofBackgroundHex(0x57554c);
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    
    
    // slow       = 1
    // faster     = 2
    // super fast = 3
    mode = 1;
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
    // add points all the time
    if(points.size() < 500000) {
        for (int i=0; i<30; i++) {
            addPoint(ofGetWidth()/2, ofGetHeight()/2);
        }
     // move all the points around
     for (unsigned int i=0; i<points.size(); i++) {
     
     speeds[i].y += 0.04; // some grav
     points[i]   += speeds[i];
     speeds[i]   *= 0.98;
     
     // move from the mouse
     ofVec2f mouseVec = ofVec2f(ofGetMouseX(), ofGetMouseY()) - points[i];
     if(mouseVec.length() < 100) {
     mouseVec.normalize();
     speeds[i] -= mouseVec;
     }
     
     // the screen
     if(points[i].x > ofGetWidth())    speeds[i].x = speeds[i].x * (-1.0);
     if(points[i].x < 0)               speeds[i].x = speeds[i].x * (-1.0);
     if(points[i].y > ofGetHeight())   speeds[i].y = speeds[i].y * (-1.0);
     if(points[i].y < 0)               speeds[i].y = speeds[i].y * (-1.0);
     }
    
    }
}


void ofApp::toggleFullscreenChanged(bool & val){
    ofToggleFullscreen();
    
    
    
    //update posiitons
    for(int i=0; i<NUM; i++){
        //pos[i] += vel[i];
        pos[i].x += vel[i].x + ofRandom(-1,1);
        pos[i].y += vel[i].y + ofRandom(-1,1);
    }
    
    // boundary condition
    for(int i=0; i<NUM; i++){
        if(pos[i].x < 0) pos[i].x = ofGetWidth();
        if(pos[i].x > ofGetWidth()) pos[i].x = 0;
        if(pos[i].y < 0) pos[i].y = ofGetHeight();
        if(pos[i].y > ofGetHeight()) pos[i].y = 0;
    }
}



//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(bgColor);
    
    
    if(showGui == true){
        gui.draw();
    }
    
    
    
    for(int i=0; i<NUM; i++){
        ofDrawCircle(pos[i], particleSize);
    }
    
    
    for(int i=0; i<NUM; i++){
        ofVec2f tmp = pos[i];
        
        for(int j=0; j<NUM; j++){
            ofVec2f other = pos[j];
            
            if(tmp.distance(other) < ofRandom(10,100)){
                ofDrawLine(tmp, other);
            }
        }
    }
    
    if(show == true){
     // draw the points the slow way
     if(mode == 1) {
     #ifdef TARGET_OPENGLES
     ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
     ofDrawBitmapString("OpenGL immediate mode not available in OpenGL ES. Press 2 or 3.",ofGetWidth() / 2.0f - 300 ,ofGetHeight() / 2.0f);
     #else
     ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
     glBegin(GL_POINTS);
     for (unsigned int i=0; i<points.size(); i++) {
     glVertex2f(points[i].x, points[i].y);
     }
     glEnd();
     #endif
     }
     
     // a bit faster
     else if(mode == 2) {
     ofSetColor(ofRandom(255), ofRandom(255), ofRandom(255));
     
     glEnableClientState(GL_VERTEX_ARRAY);
     glVertexPointer(2, GL_FLOAT, 0, &points[0].x);
     glDrawArrays(GL_POINTS, 0, (int)points.size());
     glDisableClientState(GL_VERTEX_ARRAY);
     }
     
     // super fast (vbo)
     else if(mode == 3) {
     ofSetColor(ofRandom(255));
     vbo.setVertexData(&points[0], (int)points.size(), GL_DYNAMIC_DRAW);
     vbo.draw(GL_POINTS, 0, (int)points.size());
     }
     
    }
     
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') showGui = !showGui;
    if(key == 'b' && particleSize < 50) particleSize ++;
    if(key == 's' && particleSize > 1) particleSize --;
    
    
     if(key == '1') mode = 1;
     if(key == '2') mode = 2;
     if(key == '3') mode = 3;
     
     
     // clear all the points
     if(key == 'c') {
     points.clear();
     speeds.clear();
     }
     
     // add crazy amount
     if(key == 'z') {
     for (int i=0; i<400000; i++) {
     addPoint(ofRandomWidth(), ofRandomHeight());
     }
     }
    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
     // add a bunch as you drag
     for (int i=0; i<400; i++) {
     addPoint(x, y);
     }
     
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
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
