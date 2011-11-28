#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32
#include "stdmath.h"
#include "debug.h"
#include "array.h"
#include "kinect.h"
#include "game.h"
#include "physics.h"

#include "dims.h"

int window_width = WINDOW_WIDTH;
int window_height = WINDOW_HEIGHT;
int window_margin = 0;

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
    window_height = h;
    window_width = ((float)WINDOW_WIDTH / WINDOW_HEIGHT * window_height);
    window_margin = std::max(0.0f,(w - window_width) / 2.0f);
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
    for (int j = 0; j < KINECT_HEIGHT; j++) {
        for (int i = 0; i < KINECT_WIDTH; i++) {
            XnRGB24Pixel p = cImg[(KINECT_HEIGHT - j - 1) * KINECT_WIDTH + i];
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
    
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glColor4f(1,1,1,1);
    drawQuad();
    glPopMatrix();
    
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
        for (int i =0 ; i < KINECT_HEIGHT * KINECT_WIDTH; i++) {
            if (userPix[i] == 0) {
                mask[i] = false;
            } else mask[i] = true;
        }
    }
    for (int j = 0; j < KINECT_HEIGHT; j++) {
        for (int i = 0; i < KINECT_WIDTH; i++) {
            if (hasMask && mask[(KINECT_HEIGHT - j - 1) * KINECT_WIDTH + i]) {
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
    
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glColor4f(1,1,1,1);
    drawQuad();
    glPopMatrix();
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void renderSkeleton(XnUserID userId) {
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, KINECT_WIDTH, KINECT_HEIGHT, 0, -1.0, 1.0);
    getKinectUser().glDraw();
    glPopMatrix();
}



void renderText(int x, int y, const char* text) {    
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
    
	glColor3f(0.9f, 0.6f, 0.6f);
	glRasterPos2i(x, y);
	int32 length = (int32)strlen(text);
	for (int32 i = 0; i < length; ++i) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
    
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void renderScore(int x, int y) {
    char buffer[128];
    sprintf(buffer, "Score: %d", getScore());
    renderText(x, y, buffer);
}

void display() {
    updateKinect();
    simulate();   
    glClearColor(0.0,0.0,0.0,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(window_margin, 0, window_width * 0.6f, window_height);
    drawWorld();
    glPopAttrib();
    
    XnUserID userId;
    if (getUserId(userId)) {
        getKinectUser().update(getUserGenerator(), getDepthGenerator(), userId);
        glPushAttrib(GL_VIEWPORT_BIT);
        glViewport(window_margin + window_width * 0.6f, window_height / 3.0f, window_width * 0.4f, window_height / 3.0f);
        renderBackground(userId);
        renderSkeleton(userId);
        glPopAttrib();
    }
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(window_margin + window_width * 0.6f, window_height * 2.0f / 3.0f, window_width * 0.4f, window_height / 3.0f);
    renderBackground();
    glPopAttrib();
    
    glPushAttrib(GL_VIEWPORT_BIT);
    glViewport(window_margin + window_width * 0.6f, 0, window_width * 0.4f, window_height / 3.0f);
    renderScore(window_width * 0.3f, window_height / 3.0f);
    
    if (isLostHand()) {
        renderText(window_width * 0.3f, window_height / 4.0f, "Lost Track of Hands");
    }
    glPopAttrib();
    
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
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
}

void run() {
    gameState() = GS_CALI;
    runKinect();
    glutMainLoop();
}

void exit() {
    gameState() = GS_STOP;
    cleanupKinect();
}

int main(int argc, char** argv) {
    gameState() = GS_INIT;
    if(!setupKinect()) {
        error("failed to initialize kinect");
        return 0;
    }
  
    if (!setupPhysics()) {
        error("failed to setup game");
        return 0;
    }
    atexit(exit);
    
    
    initGlut(argc,argv);
    image.resize(KINECT_WIDTH, KINECT_HEIGHT);
    mask.resize(KINECT_WIDTH, KINECT_HEIGHT);
    run();

    message("finish");
    return 0;
}