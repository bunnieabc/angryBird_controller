#pragma once
#include "ofMain.h"
#include "ofxSimpleSerial.h"
#include <vector>
#include "ofUtils.h"
#include "ofxBox2d.h"
#include "TextureShape.h"

// ------------------------------------------------- App
class ofApp: public ofBaseApp {
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void mouseMoved(int x, int y);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    
    // new function
    void loadBirdImage();
    void setGround();
    
    ofxBox2d box2d;
    
    ofxSimpleSerial    serial;
    string        message;
    bool        red;
    bool        green;
    bool        blue;
    float acceX, acceY, acceZ, force;
    bool        requestRead;
    void        onNewMessage(string & message);
    
    //ofxBox2d box2d;   // the box2d world
    std::vector<std::shared_ptr<ofxBox2dCircle>> circles; // default box2d circles
    std::vector<std::shared_ptr<ofxBox2dRect>> boxes; // defalut box2d rects
    std::vector<std::shared_ptr<ofxBox2dCircle>> angrybirds;
    ofxBox2dCircle testCircle;
    int shot_ball = 0;
    float delay_serial = 0;
    float force_directionX;
    float force_directionY;
    float force_prev, force_now;
    
    
    
    // a vector of all the texture images
    std::vector<ofImage> textures;
    
    // a vector of all the texture shapes
    //std::vector<std::shared_ptr<TextureShape>> shapes;
    
    std::vector<ofImage> birdPics;
    std::vector<ofImage> dynamicBirdPics;
    ofxBox2dRect ground;
};

