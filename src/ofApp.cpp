#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetVerticalSync(true);
    ofBackgroundHex(0xfdefc2);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    box2d.init();
    box2d.setGravity(0, 10);
    box2d.createBounds();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    int baud = 9600;
    serial.setup("/dev/tty.usbmodem1411", baud);
    serial.startContinuousRead();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    
    message = "";
    loadBirdImage();
}

//////////////////load bird images ///////////////////
void ofApp::loadBirdImage(){
    ofDirectory dir;
    ofDisableArbTex();
    int n = dir.listDir("birds");
    for (int i=0; i<n; i++) {
        birdPics.push_back(ofImage(dir.getPath(i)));
    }
    
}

/////////////// migrate from final serial
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
        cout<<"texture_size: " << textures.size() << endl;
        
        
        if(force_prev - force_now >= 8) {
            shot_ball = 1;
            cout<< "shot!!!!!!!!!!"<< endl;
        }
        
    }
}


//--------------------------------------------------------------
void ofApp::update() {
    
    
    if(requestRead)
    {
        cout << "sendRequest\n";
        serial.sendRequest();
        requestRead = false;
    }
    
    
    // add some circles every so often
    /*if((int)ofRandom(0, 10) == 0) {
     shapes.push_back(std::make_shared<TextureShape>());
     shapes.back()->setTexture(&textures[(int)ofRandom(textures.size())]);
     shapes.back()->setup(box2d, (ofGetWidth()/2)+ofRandom(-20, 20), -20, ofRandom(10, 50));
     }*/
    
    box2d.update();
    delay_serial = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
    
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
        
        angrybirds.back()->addForce( ofVec2f(force_directionX, force_directionY), 300);
        dynamicBirdPics.push_back(birdPics[ofRandom(0,birdPics.size())]);
        
        shot_ball = 0;
    }
    
    
    /*for(auto angrybird: angrybirds) {
        angrybird->draw();
    }*/
    
    for(auto i=0; i<angrybirds.size(); i++){
        ofPushMatrix();
        //int random_bird = ofRandom(0,birdPics.size());
        ofTranslate(angrybirds[i]->getPosition().x, angrybirds[i]->getPosition().y, 0);
        ofRotate(angrybirds[i]->getRotation());
        dynamicBirdPics[i].setAnchorPercent(0.5, 0.5);
        dynamicBirdPics[i].draw(0, 0);
        ofPopMatrix();
        
    }
    
    /*for(auto i=0; i<shapes.size(); i++) {
     shapes[i]->draw();
     }*/
    //}
    
    ofFill();
    ofSetHexColor(0xBF2545);
    for(auto box: boxes) {
        box->draw();
    }
    
    //birdPic.draw(0, 0);
    
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
    

    
    
    /*
     // some debug information
     string info = "Textures from subtlepatterns.com\n";
     info += "Press c to clear everything\n";
     */
    //ofSetHexColor(0x444342);
    //ofDrawBitmapString(info, 10, 15);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 'c') {
        //shapes.clear();
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
    
}

