#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32

#include "debug.h"
#include "driver.h"
#include "motion.h"
#include <Box2D/Box2D.h>

const int kwidth = 512;
const int kheight = 512;

KinectDriver driver;
void nodKinect(KinectDriver &driver) {
    driver.setTiltAngle(30);
    sleep(2);
    driver.setTiltAngle(-30);
    sleep(2);
    driver.setTiltAngle(0);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
}

void display() {
    updateKinect();
    const XnDepthPixel* dImg = getKinectDepthImage();
    const XnRGB24Pixel* cImg = getKinectColorImage();
    glClearColor(0.5,0.5,0.5,0);
    glClear(GL_COLOR_BUFFER_BIT);
    //glDrawPixels(XN_VGA_X_RES, XN_VGA_Y_RES, GL_LUMINANCE, GL_UNSIGNED_SHORT, dImg);
    glDrawPixels(XN_VGA_X_RES, XN_VGA_Y_RES, GL_RGB, GL_UNSIGNED_BYTE, cImg);
    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

void initGlut(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowPosition(256, 128);
    glutInitWindowSize(XN_VGA_X_RES, XN_VGA_Y_RES);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("angry man");
    
    glutIgnoreKeyRepeat(1);
    
    glutDisplayFunc(display);
//    glutKeyboardFunc(keyboardDown);
//    glutKeyboardUpFunc(keyboardUp);
    //glutMouseFunc(mouse);
    //glutMotionFunc(motion);
    glutIdleFunc(idle);
}

void run() {
    runKinect();
    glutMainLoop();
}

int main(int argc, char** argv) {
//    driver.setup();
//    nodKinect(driver);
//    driver.setLedOption(LED_YELLOW);
    
    if(!setupKinect()) {
        error("failed to initialize kinect");
        return 0;
    }
    
    initGlut(argc,argv);
    run();
    
//    XnUInt32 middle = XN_VGA_X_RES * XN_VGA_Y_RES/2 + XN_VGA_X_RES/2;    
//    XnStatus rc = context.StartGeneratingAll();
//    while (true) {
//        // Update to next frame
//        rc = context.WaitOneUpdateAll(depth); // TODO: check error code
//        error_if_not_va(rc == XN_STATUS_OK, "failed to update");
//        const XnDepthPixel* pDepthMap = depth.GetDepthMap(); 
//        message_va("Middle pixel is %u millimeters away\n", pDepthMap[middle]);
//    }

    cleanupKinect();
    message("finish");
    return 0;
}