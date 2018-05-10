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