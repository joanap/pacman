#ifndef __GHOST_H__
#define __GHOST_H__

#if defined (__APPLE__) || defined (MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
#include <cmath>
#include "Character.h"
#include "Constraints.h"

class Ghost : public Character {

public:
    Ghost(); //SO DEVE SER USADO PARA O HUD
    Ghost(int posx, int posy, int id, float color[3]);
    
    bool getTrouble();
    bool getHidden();
    
    void setSpeed(float speed);
    void setTrouble(bool trouble);
    void setHidden(bool value);
    
    void drawCircle(float cx, float cy, float r, int num_segments);

    void draw();
    void update(float dt);
    void move(float dist);
    void eat(float x, float y, char symbol);
    int shoot(int i);

    void backAgain();
    
    
private:
    int _ghostId;
    bool _trouble;
    bool _hidden;

};

#endif
