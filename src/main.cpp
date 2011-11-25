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
#include <Box2D/Box2D.h>

const int kwidth = XN_VGA_X_RES;
const int kheight = XN_VGA_Y_RES;

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

sarray2<Color>  image;
sarray2<bool>   mask;
int angle = 0;


void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void display() {
    updateKinect();
    XnUserID ausers[15];
    XnUInt16 nusers = 15; 
    getUserGenerator().GetUsers(ausers, nusers);
    
    // draw background image
    bool hasMask = false;
    if (nusers >= 1) {
        XnUserID userId = ausers[0];        
        xn::SceneMetaData smd;
        unsigned short *userPix;
        if (getUserGenerator().GetUserPixels(userId, smd) == XN_STATUS_OK) { 
            userPix = (unsigned short*)smd.Data();
            hasMask = true;
            for (int i =0 ; i < kwidth * kheight; i++) {
                if (userPix[i] == 0) {
                    mask[i] = false;
                } else mask[i] = true;
            }
        }
    }
    
    const XnRGB24Pixel* cImg = getKinectColorImage();
    const XnDepthPixel* dImg = getKinectDepthImage();
    for (int j = 0; j < kheight; j++) {
        for (int i = 0; i < kwidth; i++) {
            XnRGB24Pixel p = cImg[(kheight - j - 1) * kwidth + i];
            XnDepthPixel d = dImg[(kheight - j - 1) * kwidth + i];
            if (hasMask && mask[(kheight - j - 1) * kwidth + i]) {
                image.at(i, j) = red;
            } else {
                if (d > 0) image.at(i, j) = makecolor(p.nRed, p.nGreen, p.nBlue);
                else image.at(i, j) = black;
            }
        }
    }
    
    // draw user
    if (nusers >= 1) {
        XnUserID userId = ausers[0]; 
        if (getUserGenerator().GetSkeletonCap().IsTracking(userId)) {
            XnSkeletonJointPosition Head; 
            getUserGenerator().GetSkeletonCap().GetSkeletonJointPosition(userId, XN_SKEL_HEAD, Head); 
            message_va("%d: (%f,%f,%f) [%f]", userId,                                                                                                                                           Head.position.X, Head.position.Y, Head.position.Z, Head.fConfidence);
        }
        
    }
    
    glClearColor(0.5,0.5,0.5,0);
    glClear(GL_COLOR_BUFFER_BIT);
    if (cImg) glDrawPixels(kwidth, kheight, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);
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
    glutInitWindowSize(kwidth, kheight);
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
    runKinect();
    glutMainLoop();
}

void exit() {
    cleanupKinect();
}

int main(int argc, char** argv) {
    if(!setupKinect()) {
        error("failed to initialize kinect");
        return 0;
    }
    atexit(exit);
    
    initGlut(argc,argv);
    image.resize(kwidth, kheight);
    mask.resize(kwidth, kheight);
    run();

    message("finish");
    return 0;
}