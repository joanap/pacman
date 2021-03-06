#include "Camera.h"

Camera::Camera(Pacman* pacman) {
    _pacman = pacman;
    _mode = DEFAULT;
}

Camera::~Camera() {

}

int Camera::getMode() {
    return _mode;
}

void Camera::setMode(int mode) {
    _mode = mode;
}

void Camera::nextMode() {
    _mode = (++_mode) % 3;
}

void Camera::setProjection(double aspect_ratio) {

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    switch(_mode){
        case CAM_MODE_1:
            if (aspect_ratio >= 1.0) {
                glOrtho(-35.0f * aspect_ratio, 35.0f * aspect_ratio, -35.0f, 35.0f, -35.0f, 35.0f);
            } else {
                glOrtho(-35.0f, 35.0f, -35.0f / aspect_ratio, 35.0f / aspect_ratio, -35.0f, 35.0f);
            }
            break;
            
        case CAM_MODE_2:
            glOrtho(-35.0f * aspect_ratio, 35.0f * aspect_ratio, -35.0f, 35.0f, -35.0f, 35.0f);
            break;
            
        case CAM_MODE_3: 
            gluPerspective(80, aspect_ratio, 1, 200);
            break;

        default:
            glOrtho(-35.0f, 35.0f, -35.0f / aspect_ratio, 35.0f / aspect_ratio, -35.0f, 35.0f);
            break;
    }
    
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    
}

void Camera::setLookAt() {
    
    switch(_mode){
        case CAM_MODE_1:
            gluLookAt(0, 0, 1,
                      0, 0, -1,
                      0, 1, 0);
            break;
            
        case CAM_MODE_2:
            gluLookAt(_pacman->getX(), _pacman->getY(), 1,
                      _pacman->getX(), _pacman->getY(), -100,
                      0, 1, 0);
            break;
            
        case CAM_MODE_3:
            gluLookAt(_pacman->getX(), _pacman->getY()-15, 15,
                      _pacman->getX(), _pacman->getY(), 0,
                      0, 0, 1);
            break;
    }
}
