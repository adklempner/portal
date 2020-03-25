#pragma once

#include "Entity.h"

class Player : public Entity {
public:
    Player();
    ~Player();

    virtual void initialize(ofPoint size, ofPoint position, ofColor color);
    virtual void draw();
    virtual void updatePosition(float x, float y);
    virtual void reinitialize();
    virtual void checkBounds();
    virtual void setColor(ofColor color);
    virtual void setColor(float r, float g, float b);
    
    ofColor playerColor;
    ofColor cursorColor;
};


