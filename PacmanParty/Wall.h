#ifndef PacmanLove_Wall_h
#define PacmanLove_Wall_h

#include "Brick.h"
#include "Constraints.h"


class Wall {
    
public:
    Wall();
    ~Wall();
    
    void resize(float width, float height);
    void intoPlace(float posx, float posy, float posz);
	void draw();
    
    
    
private:
    
    float _width;
    float _height;
    float _posX;
    float _posY;
    float _posZ;
    Brick *_brick;
    
    
    
};




#endif
