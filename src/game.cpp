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
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, kwidth, kheight, 0, -1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    if (user.tracking) {
        user.neck.glDraw();
        
        // left arm + shoulder
        user.leftShoulder.glDraw();
        user.leftUpperArm.glDraw();
        user.leftLowerArm.glDraw();
        
        // right arm + shoulder
        user.rightShoulder.glDraw();
        user.rightUpperArm.glDraw();
        user.rightLowerArm.glDraw();
        
        // upper torso
        user.leftUpperTorso.glDraw();
        user.rightUpperTorso.glDraw();
        
        // left lower torso + leg
        user.leftLowerTorso.glDraw();
        user.leftUpperLeg.glDraw();
        user.leftLowerLeg.glDraw();
        
        // right lower torso + leg
        user.rightLowerTorso.glDraw();
        user.rightUpperLeg.glDraw();
        user.rightLowerLeg.glDraw();
        
        user.hip.glDraw();
    }
    
    glPopMatrix();
}

void drawGameBackground() {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
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
    //drawGameBackground();
    
}