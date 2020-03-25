#include "Player.h"

Player::Player() {
    size.x = 15;
    size.y = 10;
    playerColor = ofColor::red;
    cursorColor = ofColor(playerColor);
    cursorColor.setBrightness(150);

    reinitialize();
}

Player::~Player() {
    
}

void Player::initialize(ofPoint size, ofPoint position, ofColor color) {
    this->size = size;
    this->position = position;
    this->setColor(color);
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
    
    ofSetColor(cursorColor);
    ofTranslate(position.x, position.y, 0);
//    ofDrawRectangle(position.x, position.y, size.x, size.y);
    ofDrawRectangle(0, 0, size.x, 2);
    ofDrawRectangle(0, 0, 2, size.y);
    ofDrawRectangle(size.x, 0, 2, size.y + 2);
    ofDrawRectangle(0, size.y, size.x + 1, 2 );
//    ofLine(size.x, size.y / 2, 0, size.y);
//    ofLine(size.x * 0.1, size.y * 0.1, size.x * 0.1, size.y - size.y * 0.1);

    ofPopMatrix();
    ofPopStyle();
}

void Player::setColor(ofColor color) {
    this->playerColor.set(color);
    this->cursorColor.set(color);
    this->cursorColor.setBrightness(150);
}

void Player::setColor(float r, float g, float b) {
    this->playerColor.set(r, g, b);
    this->cursorColor.set(r, g, b);
    this->cursorColor.setBrightness(150);
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



