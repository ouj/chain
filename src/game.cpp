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

const int kwidth = XN_VGA_X_RES;
const int kheight = XN_VGA_Y_RES;

bool initGame() {
    return true;
}

void gameLogic() {
    
}


void drawUser(KinectUser &user) {
//    glPushMatrix();
//    
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(0, kwidth, kheight, 0, -1.0, 1.0);
//    
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glScalef(0.2, 0.2, 0.2);
//    glTranslated(user.center3D.X, user.center3D.Y, 0);
//    
//    getKinectUser().glDraw();
//    
//    glPopMatrix();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, kwidth, kheight, 0, -1.0, 1.0);
    user.glDraw();
    glPopMatrix();
}

void drawGameBackground() {
    glPushMatrix();
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
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
    
    glPopMatrix();
}

void drawGame() {
    drawGameBackground();
    drawUser(getKinectUser());
}