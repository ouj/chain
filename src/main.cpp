#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32

#include "debug.h"
#include "array.h"
#include "kinect.h"
#include "game.h"
#include "physics.h"

#include "dims.h"

struct Color {
    XnUInt8 R;
    XnUInt8 G;
    XnUInt8 B;
    XnUInt8 A;
};

Color makecolor(XnUInt8 R, XnUInt8 G, XnUInt8 B, XnUInt8 A = 255) {
    Color color;
    color.R = R;
    color.G = G;
    color.B = B;
    color.A = A;
    return color;
}

Color black = makecolor(0, 0, 0);
Color red = makecolor(255, 0, 0);
Color green = makecolor(0, 255, 0);
Color blue = makecolor(0, 0, 255);

sarray2<Color>  image;
sarray2<bool>   mask;
int angle = 0;


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}


bool getUserId(XnUserID& id) {
    XnUserID ausers[15];
    XnUInt16 nusers = 15; 
    getUserGenerator().GetUsers(ausers, nusers);
    if (nusers >= 1) {
        id = ausers[0];
        return true;
    } else {
        return false;
    }
}

void drawQuad() {
    glBegin(GL_QUADS);
    glTexCoord2i(0,0);
    glVertex2i(-1,-1);
    glTexCoord2i(1,0);
    glVertex2i(1,-1);
    glTexCoord2i(1,1);
    glVertex2i(1,1);
    glTexCoord2i(0,1);
    glVertex2i(-1,1);
    glEnd();
}

static GLuint textureId = 0;
void renderBackground() {
    const XnRGB24Pixel* cImg = getKinectColorImage();
    for (int j = 0; j < WINDOW_HEIGHT; j++) {
        for (int i = 0; i < WINDOW_WIDTH; i++) {
            XnRGB24Pixel p = cImg[(WINDOW_HEIGHT - j - 1) * WINDOW_WIDTH + i];
            image.at(i, j) = makecolor(p.nRed, p.nGreen, p.nBlue);
        }
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if(textureId == 0) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, &image.at(0));
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
                        GL_RGBA, GL_UNSIGNED_BYTE, &image.at(0));
    }
    
    glColor4f(1,1,1,1);
    drawQuad();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void renderBackground(XnUserID userId) {
    bool hasMask = false;
    xn::SceneMetaData smd;
    unsigned short *userPix;
    if (getUserGenerator().GetUserPixels(userId, smd) == XN_STATUS_OK) { 
        userPix = (unsigned short*)smd.Data();
        hasMask = true;
        for (int i =0 ; i < WINDOW_HEIGHT * WINDOW_WIDTH; i++) {
            if (userPix[i] == 0) {
                mask[i] = false;
            } else mask[i] = true;
        }
    }
    for (int j = 0; j < WINDOW_HEIGHT; j++) {
        for (int i = 0; i < WINDOW_WIDTH; i++) {
            if (hasMask && mask[(WINDOW_HEIGHT - j - 1) * WINDOW_WIDTH + i]) {
                image.at(i, j) = green;
            } else image.at(i, j) = black;
        }
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    if(textureId == 0) {
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(),
                     0, GL_RGBA, GL_UNSIGNED_BYTE, &image.at(0));
    } else {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width(), image.height(),
                        GL_RGBA, GL_UNSIGNED_BYTE, &image.at(0));
    }
    
    glColor4f(1,1,1,1);
    drawQuad();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
}

void renderSkeleton(XnUserID userId) {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1.0, 1.0);
    getKinectUser().glDraw();
    glPopMatrix();
}


void display() {
    //updateKinect();
    simulate();
    
    //gameLogic();
    glClearColor(1.0,1.0,1.0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    drawWorld();
    drawGame();
    
//    XnUserID userId;
//    if (getUserId(userId)) {
//        getKinectUser().update(getUserGenerator(), getDepthGenerator(), userId);
//        glPushAttrib(GL_VIEWPORT_BIT);
//        glViewport(kwidth - 160, kheight - 120, 160, 120);
//        renderBackground(userId);
//        renderSkeleton(userId);
//        glPopAttrib();
//    } else {
//        glPushAttrib(GL_VIEWPORT_BIT);
//        glViewport(kwidth - 160, kheight - 120, 160, 120);
//        renderBackground();
//        glPopAttrib();
//    }

    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case '-': 
            angle = max(angle-1, -30);
            getKinect().setTiltAngle(angle);
            break;
        case '=':
            angle = min(angle+1, 30);
            getKinect().setTiltAngle(angle);
            break;
        case '`':
            angle = 0;
            getKinect().setTiltAngle(angle);
        default: message_va("unknown key %c", key);
    }
}

void initGlut(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(256, 128);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("angry man");
    
    //glutIgnoreKeyRepeat(1);
    
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    glutIdleFunc(idle);
}

void run() {
    //runKinect();
    glutMainLoop();
}

void exit() {
    cleanupKinect();
}

int main(int argc, char** argv) {
//    if(!setupKinect()) {
//        error("failed to initialize kinect");
//        return 0;
//    }
  
    if (!setupPhysics()) {
        error("failed to setup game");
        return 0;
    }
    
    if (!initGame()) {
        error("failed to setup game");
        return 0;
    }
    atexit(exit);
    
    
    initGlut(argc,argv);
    image.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mask.resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    run();

    message("finish");
    return 0;
}