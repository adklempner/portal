/* vim: set tabstop=4 shiftwidth=4 expandtab: */
#include "ofApp.h"

void ofApp::setup(){

    ofSetFrameRate( 60 );
    ofSetWindowTitle( "Mars Portal" );
    ofSetVerticalSync( true );
    ofEnableAntiAliasing();

    ofSetLogLevel( OF_LOG_VERBOSE );

/*
#if OF_VERSION_MINOR >= 11
    if ( ofGetScreenWidth() >= 2560 ) {
        ofxGuiEnableHiResDisplay();
    }
#endif
*/

    osc.setup(OSC_PORT);
    
    if( (ofGetEnv("PORTAL").length() != 0) ){

        translateX = ofToInt( ofGetEnv("TRANSLATE_X") );
        translateY = ofToInt( ofGetEnv("TRANSLATE_Y") );

        sizeX = ofToInt( ofGetEnv("SIZE_X") );
        sizeY = ofToInt( ofGetEnv("SIZE_Y") );

        totalX = ofToInt( ofGetEnv("TOTAL_X") );
        totalY = ofToInt( ofGetEnv("TOTAL_Y") );

        shiftX = ofToInt( ofGetEnv("SHIFT_X") );
        borderX = ofToInt( ofGetEnv("BORDER_X") );
        translateX += shiftX + borderX;

        portalId = ofGetEnv("PORTAL_ID");
        mapId = ofGetEnv("MAP_ID");

        ofLogNotice() << "My location, x: " << translateX << ", y: " << translateY;

        ofSetWindowShape( sizeX, sizeY );
        ofSetWindowPosition( shiftX, 0 );

    } else {

        translateX = 0;
        translateY = 0;

        sizeX = ofGetWindowWidth();
        sizeY = ofGetWindowHeight();

        totalX = ofGetWindowWidth();
        totalY = ofGetWindowHeight();

        shiftX = 0;
        borderX = 0;

        portalId = "n/a";
        mapId = "n/a";

        ofLogNotice() << "No portal info received from env";
    }

    bgColor = ofColor::black;
    ofSetBackgroundColor(bgColor);
 
    clock.setup();

    ball.setup( translateX, translateY, sizeX, sizeY, totalX, totalY );
    
    // Pixel Painter
    defaultPixelColor = ofColor::white;
    gridColors.resize(totalX / pixelSize.x);
    for(int i = 0 ; i < gridColors.size(); i++)
    {
//        //Grow Columns by n
//        gridColors[i].resize(totalY / pixelSize.y);
        for(int j = 0; j < totalY / pixelSize.y; j++) {
            gridColors[i].push_back(defaultPixelColor);
        }
    }
    
    Player *firstPlayer = new Player();
    ofPoint pos = ofPoint(0, 0);
    firstPlayer->initialize(playerSize, pos, ofColor::red);
    players.push_back(firstPlayer);

}

void ofApp::update(){
    
    while (osc.hasWaitingMessages()) {
      ofxOscMessage m;
      osc.getNextMessage(m);
        
        if(m.getAddress() == BG_COLOR_ADDRESS) {
            if( m.getNumArgs() == 3) {
                bgColor = ofColor(m.getArgAsInt(0), m.getArgAsInt(1), m.getArgAsInt(2) );
            }
        }
        
        if(m.getAddress() == PLAYER_MOVE_ADDRESS) {
            if(m.getNumArgs() == 3) {
                int playerId = m.getArgAsInt(0);
                players[playerId]->updatePosition(m.getArgAsFloat(1), m.getArgAsFloat(2));
            }
        }
        
        if(m.getAddress() == PLAYER_DRAW_ADDRESS) {
            int playerId = m.getArgAsInt(0);
            ofPoint playerPos = players[playerId]->position;
            gridColors[playerPos.x / playerSize.x][playerPos.y / playerSize.y] = players[playerId]->playerColor;
        }
    }

    clock.update();

    ball.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor( bgColor );
    ofSetColor( bgColor.getInverted() );
        

    ofSetColor( ofColor::pink );
    ofSetCircleResolution( 64 );

    // general info
    ofDrawBitmapString( "translate x, y: " + ofToString(translateX) + ", " + ofToString(translateY), 10, 10 );
    ofDrawBitmapString( "screen width, height: " + ofToString(ofGetWindowWidth()) + ", " + ofToString(ofGetWindowHeight()), 10, 30 );
    ofDrawBitmapString( "total width, total height: " + ofToString(totalX) + ", " + ofToString(totalY), 10, 50 );
    ofDrawBitmapString( "portalId (mapId): " + portalId + " (" + mapId + ")", 10, 70 );

    ofPushMatrix();
        ofTranslate( -translateX, -translateY );
            ball.draw();
    ofPopMatrix();

    clock.draw();
    
    // Layer 0 - grid
    for(unsigned int i = 0; i < gridColors.size(); i++) {
        for(unsigned int j = 0; j < gridColors[i].size(); j++) {
            ofPushStyle();
//            ofPushMatrix();
            ofSetColor(gridColors[i][j]);
            ofDrawRectangle(pixelSize.x * i, pixelSize.y * j, pixelSize.x, pixelSize.y);
//            ofPopMatrix();
            ofPopStyle();
        }
    }
    
    // Layer 1 - player cursors
    for (unsigned int i = 0; i < players.size(); ++i) {
        players[i]->draw();
    }


}

void ofApp::keyPressed(int key){

    switch ( key ) {
        case 'w':
            players[0]->updatePosition(0, -playerSize.y);
            break;
        case 's':
            players[0]->updatePosition(0, playerSize.y);
            break;
        case 'd':
            players[0]->updatePosition(playerSize.x, 0);
            break;
        case 'a':
            players[0]->updatePosition(-playerSize.x, 0);
            break;
        case OF_KEY_SPACE:
//            ofPoint playerPos = players[0]->position;
            ofLog() << "Player X: " << players[0]->position.x << " Player Y: " << players[0]->position.y;
            ofLog() << "Grid index X: " << (players[0]->position.x / playerSize.x) << " Y: " << (players[0]->position.y / playerSize.y);
            gridColors[players[0]->position.x / playerSize.x][players[0]->position.y / playerSize.y] = ofColor::red;
            break;
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
        case 'p':
        case 'P':
        case OF_KEY_RETURN:
            ofSaveScreen( "screenshot-" + ofToString( ofGetUnixTime() ) + ".png");
            break;
        case 'q':
        case 'Q':
            ofExit();
            break;
        case '?':
            ofLogNotice() << "OF version " << ofGetVersionMajor() << "." << ofGetVersionMinor() << "." << ofGetVersionPatch() << " " << ofGetVersionPreRelease();
            break;
    }
}

void ofApp::keyReleased(int key){

}

void ofApp::mouseMoved(int x, int y ){

}

void ofApp::mouseDragged(int x, int y, int button){

}

void ofApp::mousePressed(int x, int y, int button){

}

void ofApp::mouseReleased(int x, int y, int button){

}

void ofApp::mouseEntered(int x, int y){

}

void ofApp::mouseExited(int x, int y){

}

void ofApp::windowResized(int w, int h){

}

void ofApp::gotMessage(ofMessage msg){

}

void ofApp::dragEvent(ofDragInfo dragInfo){ 
}
