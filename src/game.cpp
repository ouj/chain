#include "game.h"
#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32
#include "kinect.h"
#include "physics.h"

bool initGame() {
    return true;
}

void gameLogic() {
    
}


void drawUser(KinectUser &user) {
    
}

void drawGameBackground() {
    glLineWidth(2);
    
    glColor3f(0,0,0);
    glBegin(GL_QUADS);
    glColor3f(0.8, 0.8, 0.8);
    glVertex2f(-1, -0.6);
    glVertex2f( 1, -0.6);
    glColor3f(0.0, 0.0, 0.0);
    glVertex2f( 1, -1);
    glVertex2f(-1, -1);
    glEnd();
}

void drawGame() {
    drawGameBackground();
    
}