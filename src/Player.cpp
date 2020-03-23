#include "Player.h"

Player::Player() {
    size.x = 15;
    size.y = 10;
    playerColor = ofColor::red;

    reinitialize();
}

Player::~Player() {
    
}

void Player::initialize(ofPoint size, ofPoint position, ofColor color) {
    this->size = size;
    this->position = position;
    this->playerColor = color;
}

void Player::reinitialize() {
    position.x = (ofGetWidth()  - size.x) / 2;
    position.y = (ofGetHeight() - size.y) / 2;
}

void Player::updatePosition(float x, float y) {
    position.x += x;
    position.y += y;
}


void Player::draw() {
    ofPushStyle();
    ofPushMatrix();

    ofSetColor(255, 0, 0);
    ofTranslate(position.x, position.y, 0);
//    ofDrawRectangle(position.x, position.y, size.x, size.y);
    ofLine(0, 0, size.x, 0);
    ofLine(0, 0, 0, size.y);
    ofLine(size.x, 0, size.x, size.y);
    ofLine(0, size.y,size.x, size.y );
//    ofLine(size.x, size.y / 2, 0, size.y);
//    ofLine(size.x * 0.1, size.y * 0.1, size.x * 0.1, size.y - size.y * 0.1);

    ofPopMatrix();
    ofPopStyle();
}

void Player::checkBounds() {
    if (position.x > (float) ofGetWidth()) {
        position.x = -size.x;
    } else if (position.x + size.x < 0) {
        position.x = ofGetWidth();
    }

    if (position.y > (float) ofGetHeight()) {
        position.y = -size.y;
    } else if (position.y + size.y < 0) {
        position.y = ofGetHeight();
    }
}



