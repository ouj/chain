#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32
#include <XnCppWrapper.h>
#include <XnVNite.h>

#include "debug.h"
#include <Box2D/Box2D.h>

xn::Context context;

int main(int argc, char** argv) {
    XnStatus ret = context.Init();
    if(!error_if_not(ret == XN_STATUS_OK, "failed to init openni context"))
        return -1;
    
//    xn::DepthGenerator depth;
//    ret = depth.Create(context);
//    if(!error_if_not(ret == XN_STATUS_OK, "failed to create depth node"))
//        return -1;
//    
    
    xn::ImageGenerator image;
    ret = image.Create(context);
    if(!error_if_not(ret == XN_STATUS_OK, "failed to create image node"))
        return -1;
    
    
    context.Shutdown();
    message("finish");
    return 0;
}