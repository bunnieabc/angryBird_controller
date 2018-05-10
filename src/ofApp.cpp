#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    
    ofSetVerticalSync(true);
    ofBackgroundHex(0xfdefc2);
    ofSetLogLevel(OF_LOG_NOTICE);
    
    box2d.init();
    box2d.enableEvents();
    box2d.setGravity(0, 10);
    box2d.createBounds();
    box2d.setFPS(60.0);
    box2d.registerGrabbing();
    
    
    int baud = 9600;
    serial.setup("/dev/tty.usbmodem1411", baud);
    serial.startContinuousRead();
    ofAddListener(serial.NEW_MESSAGE,this,&ofApp::onNewMessage);
    
    message = "";
    //load functions
    loadBirdImage();
    setGround();
    loadWoods();
    
    // initial listener and contact
    // register the listener so that we get the events
    ofAddListener(box2d.contactStartEvents, this, &ofApp::contactStart);
    ofAddListener(box2d.contactEndEvents, this, &ofApp::contactEnd);
    
    // load the 8 sfx soundfile
    for (int i=0; i<N_SOUNDS; i++) {
        sound[i].loadSound("sfx/"+ofToString(i)+".mp3");
        sound[i].setMultiPlay(true);
        sound[i].setLoop(false);
    }

}

//--------------------------------------------------------------
void ofApp::contactStart(ofxBox2dContactArgs &e) {
    if(e.a != NULL && e.b != NULL) {
        
        // if we collide with the ground we do not
        // want to play a sound. this is how you do that
        if(e.a->GetType() == b2Shape::e_circle && e.b->GetType() == b2Shape::e_circle) {
            
            SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
            SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
            
            if(aData) {
                aData->bHit = true;
                sound[aData->soundID].play();
            }
            
            if(bData) {
                bData->bHit = true;
                sound[bData->soundID].play();
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::contactEnd(ofxBox2dContactArgs &e) {
    if(e.a != NULL && e.b != NULL) {
        
        SoundData * aData = (SoundData*)e.a->GetBody()->GetUserData();
        SoundData * bData = (SoundData*)e.b->GetBody()->GetUserData();
        
        if(aData) {
            aData->bHit = false;
        }
        
        if(bData) {
            bData->bHit = false;
        }
    }
}

//--------------------------------------------------------------
vector <ofVec3f> ofApp::loadPoints(string file) {
    vector <ofVec3f> pts;
    vector <string>  ptsStr = ofSplitString(ofBufferFromFile(file).getText(), ",");
    for (int i=0; i<ptsStr.size(); i+=3) {
        float x = ofToFloat(ptsStr[i]);
        float y = ofToFloat(ptsStr[i+1]);
        float rotation = ofToFloat(ptsStr[i+2]);
        pts.push_back(ofVec3f(x, y, rotation));
        cout<<"--------rotation--------"<< endl << rotation << endl <<"----------------------" << endl;
    }
    return pts;
}

///////////////////set up ground ////////////////////
void ofApp::setGround(){
    ground.setPhysics(100.0, 0, 0.1);
    ground.setup(box2d.getWorld(),  ofGetWidth()/2, ofGetHeight()-60, ofGetWidth(), 120);
    angryBirdBg.load("bg/angry_bird_world.png");
    
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
//////////////////load bird images ///////////////////
void ofApp::loadWoods(){
    int woodHeight = 180;
    int woodWidth = 40;
    vector <ofVec3f> pts = loadPoints("woods.dat");
    
    
    ofDirectory dir;
    ofDisableArbTex();
    
    
    int n = dir.listDir("woods");
    for (int i=0; i<n; i++) {
        woodPics.push_back(ofImage(dir.getPath(i)));
    }
    
    for (int i=0; i<pts.size(); i++) {
        woods.push_back(std::make_shared<ofxBox2dRect>());
        woods.back()->setPhysics(3, 0, 0.1);
        woods.back()->setup(box2d.getWorld(),  pts[i].x, pts[i].y, woodWidth, woodHeight);
        woods.back()->setRotation(pts[i].z);
        dynamicWoodPics.push_back(woodPics[ofRandom(0,woodPics.size())]);
        
    }
    /*
    woods.push_back(std::make_shared<ofxBox2dRect>());
    woods.back()->setPhysics(3, 0, 0.1);
    woods.back()->setup(box2d.getWorld(),  ofGetWidth()/4*3, ofGetHeight() - 60 - woodHeight/2, woodWidth, woodHeight);
    dynamicWoodPics.push_back(woodPics[ofRandom(0,woodPics.size())]);
     
    woods.push_back(std::make_shared<ofxBox2dRect>());
    woods.back()->setPhysics(3, 0, 0.1);
    woods.back()->setup(box2d.getWorld(),  ofGetWidth()/4*3 - woodHeight + woodWidth, ofGetHeight() - 60 - woodHeight/2, woodWidth, woodHeight);
    dynamicWoodPics.push_back(woodPics[ofRandom(0,woodPics.size())]);
    
    woods.push_back(std::make_shared<ofxBox2dRect>());
    woods.back()->setPhysics(3, 0, 0.1);
    woods.back()->setup(box2d.getWorld(),  ofGetWidth()/4*3 - woodHeight/2 + woodWidth/2, ofGetHeight() - 60 - woodHeight, woodWidth, woodHeight);
    woods.back()->setRotation(90);
    dynamicWoodPics.push_back(woodPics[ofRandom(0,woodPics.size())]);
    /*
    */
    
}


/////////////// migrate from final serial
void ofApp::onNewMessage(string & message)
{
    cout << "onNewMessage, message: " << message << "\n";
    
    vector<string> input = ofSplitString(message, ",");
    
    
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
    
    
    box2d.update();
    
    // get the delay time so you can read the value
    delay_serial = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::draw() {
    
    
    
    ofFill();
    ofSetHexColor(0xffffff);
    
    // set background
    angryBirdBg.draw(0, -20);
    
    
    if(shot_ball == 1){
        
        angrybirds.push_back(std::make_shared<ofxBox2dCircle>());
        angrybirds.back()->setPhysics(2.0, 0.53, 0.1);
        angrybirds.back()->setup(box2d.getWorld(), mouseX, mouseY, r);
        angrybirds.back()->setData(new SoundData());
        SoundData * sd = (SoundData*)angrybirds.back()->getData();
        sd->soundID = ofRandom(0, N_SOUNDS);
        sd->bHit = false;
        
        angrybirds.back()->addForce( ofVec2f(force_directionX, force_directionY), 300);
        dynamicBirdPics.push_back(birdPics[ofRandom(0,birdPics.size())]);
        
        shot_ball = 0;
    }
    
    
    /*for(auto angrybird: angrybirds) {
        angrybird->draw();
    }*/
    
    // draw birds
    for(auto i=0; i<angrybirds.size(); i++){
        ofPushMatrix();
        //int random_bird = ofRandom(0,birdPics.size());
        float scalePic = (r * 2 + 10) / dynamicBirdPics[i].getWidth();
        ofTranslate(angrybirds[i]->getPosition().x, angrybirds[i]->getPosition().y, 0);
        ofRotate(angrybirds[i]->getRotation());
        ofScale(scalePic, scalePic);
        dynamicBirdPics[i].setAnchorPercent(0.5, 0.5);
        dynamicBirdPics[i].draw(0, 0);
        ofPopMatrix();
        SoundData * data = (SoundData*)angrybirds[i].get()->getData();
    }
    
    //draw woods
    
    for(auto i=0; i<woods.size(); i++){
        ofPushMatrix();
        //int random_bird = ofRandom(0,birdPics.size());
        ofTranslate(woods[i]->getPosition().x, woods[i]->getPosition().y, 0);
        ofRotate(woods[i]->getRotation());
        dynamicWoodPics[i].setAnchorPercent(0.5, 0.5);
        dynamicWoodPics[i].draw(0, 0);
        ofPopMatrix();
        
    }
    
    
    // draw woods
    ofPushStyle();
    ofFill();
    ofSetHexColor(0xffffff);
    
    ofPopStyle();
    
    
    ofFill();
    
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
    

    if(drawShape == 1){
        ground.draw();
        for(auto angrybird: angrybirds) {
            angrybird->draw();
        }
        
        for(auto wood: woods) {
            wood->draw();
        }
    }
    

}


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if(key == 'c') {
        //shapes.clear();
        
        if(drawShape == 1){
            drawShape = 0;
        }
        else{
            drawShape = 1;
        }
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

