#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    int baud = 9600;
    ofSetVerticalSync(true);
    ofBackground(0, 0, 0);
    
    serial.setup("/dev/tty.usbmodem1411", baud);
    serial.startContinuousRead();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    
    message = "";
    
    ofSetVerticalSync(true);
    ofBackgroundHex(0xfdefc2);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createBounds();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    force = 100;
    //first test circle
    /*
    float r = 50;
    testCircle.setPhysics(3.0, 0.7, 0.1);
    testCircle.setup(box2d.getWorld(), ofGetWidth()/2, ofGetHeight()-100, r);
    
    
    */
    //testCircle.addRepulsionForce(ofVec2f(-30, 10), 250);
    force_prev = 100, force_now = 100;
}


void ofApp::onNewMessage(string & message)
{
    cout << "onNewMessage, message: " << message << "\n";
    
    vector<string> input = ofSplitString(message, ",");
    /*
    if(input.size() >= 3)
    {
        red =    input.at(0) == "r";
        green =    input.at(1) == "g";
        blue =    input.at(2) == "b";
    }*/
    
    
    if(input.size() >= 4 && delay_serial >= 1)
    {
        force_prev = force_now;
        acceX = std::stof(input.at(0));
        acceY = std::stof(input.at(1));
        acceZ = std::stof(input.at(2));
        force = std::stof(input.at(3));
        cout<<"test seperate:"<< acceX <<"," <<acceY<<"," <<acceZ<<"," <<force<<"\n";
        
        cout << "time: "<< delay_serial << endl;
        force_directionY = ofMap(acceZ, 12, 38, -200, 200);
        if(acceZ <= 25){
            force_directionX = ofMap(acceZ, 12, 25, 0, 200);
        }
        else {
            force_directionX = ofMap(acceZ, 25, 38, 200, 0);
        }
        
        force_now = force;
        cout<<"force_now: " <<force_now <<", prev: "<<force_prev << endl;
        
        if(force_prev - force_now >= 8) {
            shot_ball = 1;
            cout<< "shot!!!!!!!!!!"<< endl;
        }
        
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    if(requestRead)
    {
        cout << "sendRequest\n";
        serial.sendRequest();
        requestRead = false;
    }
    
    if(force<=5){
        
        
    }
    box2d.update();
    delay_serial = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofFill();
    ofSetHexColor(0xf6c738);
    for(auto circle: circles) {
        circle->draw();
    }
    
    
    
    if(shot_ball == 1){
        float r = 50;
        angrybirds.push_back(std::make_shared<ofxBox2dCircle>());
        angrybirds.back()->setPhysics(2.0, 0.53, 0.1);
        angrybirds.back()->setup(box2d.getWorld(), mouseX, mouseY, r);
        //angrybirds.back()->addForce( ofVec2f(30,-10), 20);
        
        angrybirds.back()->addForce( ofVec2f(force_directionX, force_directionY), 500);
        
        //shot_ball = 1;
        
        shot_ball = 0;
    }
    
    //if(shot_ball == 1) {
        for(auto angrybird: angrybirds) {
            angrybird->draw();
        }
    //}
    
    ofFill();
    ofSetHexColor(0xBF2545);
    for(auto box: boxes) {
        box->draw();
    }
    
    // draw the ground
    box2d.drawGround();
    
    string info = "";
    info += "Press [c] for circles\n";
    info += "Press [b] for blocks\n";
    info += "Total Bodies: "+ofToString(box2d.getBodyCount())+"\n";
    info += "Total Joints: "+ofToString(box2d.getJointCount())+"\n\n";
    info += "FPS: "+ofToString(ofGetFrameRate(), 1)+"\n";
    ofSetHexColor(0x444342);
    ofDrawBitmapString(info, 30, 30);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == 'c') {
        float r = ofRandom(4, 20);
        circles.push_back(std::make_shared<ofxBox2dCircle>());
        circles.back()->setPhysics(3.0, 0.53, 0.1);
        circles.back()->setup(box2d.getWorld(), mouseX, mouseY, r);
        circles.back()->addForce( ofVec2f(30,-10), 20);
    }
    
    if(key == 'b') {
        float w = ofRandom(4, 20);
        float h = ofRandom(4, 20);
        boxes.push_back(std::make_shared<ofxBox2dRect>());
        boxes.back()->setPhysics(3.0, 0.53, 0.1);
        boxes.back()->setup(box2d.getWorld(), mouseX, mouseY, w, h);
    }
    
    if(key == 't') ofToggleFullscreen();
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
requestRead = true;
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
