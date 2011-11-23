#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include <XnCppWrapper.h>
#include <XnVNite.h>
#pragma clang diagnostic pop
#pragma GCC diagnostic pop

#include "debug.h"
#include "driver.h"
#include <Box2D/Box2D.h>

xn::Context context;
xn::UserGenerator userGenerator;
#define POSE_TO_USE "Psi"

void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator,
                                   XnUserID nId, void* pCookie) {
    message_va("New User: %d", nId); 
    userGenerator.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE, nId);
}


void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId,
                                    void* pCookie) { }

void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, 
                                    const XnChar* strPose, XnUserID nId, 
                                    void* pCookie) {
    message_va("Pose %s for user %d", strPose, nId); 
    userGenerator.GetPoseDetectionCap().StopPoseDetection(nId); 
    userGenerator.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId,
                                        void* pCookie) {
    message_va("Starting calibration for user %d", nId);
}

void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId, 
                                      XnBool bSuccess, void* pCookie) {
    if (bSuccess) {
        message("User calibrated");
        userGenerator.GetSkeletonCap().StartTracking(nId); 
    } else {
        message_va("Failed to calibrate user %d", nId);
        userGenerator.GetPoseDetectionCap().StartPoseDetection( POSE_TO_USE,
                                                                 nId);
    }
}

KinectDriver driver;

void nodKinect(KinectDriver &driver) {
    driver.setTiltAngle(30);
    sleep(2);
    driver.setTiltAngle(-30);
    sleep(2);
    driver.setTiltAngle(0);
}

bool initDepthGenerator(xn::Context &context, xn::DepthGenerator &depth) {
    XnStatus rc = XN_STATUS_OK;	
	XnMapOutputMode map_mode; 
	
    rc = context.FindExistingNode(XN_NODE_TYPE_DEPTH, depth);
    if (rc == XN_STATUS_OK) {
        rc = depth.GetMapOutputMode(map_mode);
        error_if_not(rc == XN_STATUS_OK, "failed to get map output mode");
    } else {
        rc = depth.Create(context);
		error_if_not(rc == XN_STATUS_OK, "error creating depth generator");
		if (rc != XN_STATUS_OK) return false;
		
		// make new map mode -> default to 640 x 480 @ 30fps
		map_mode.nXRes = XN_VGA_X_RES;
		map_mode.nYRes = XN_VGA_Y_RES;
		map_mode.nFPS  = 30;
		
		rc = depth.SetMapOutputMode(map_mode);
        error_if_not(rc == XN_STATUS_OK, "failed to set map output mode");
    }
    return true;
}

int main(int argc, char** argv) {
    driver.setup();
    //nodKinect(driver);
    driver.setLedOption(LED_YELLOW);
    
    XnStatus ret = context.Init();
    if(!error_if_not(ret == XN_STATUS_OK, "failed to init openni context"))
        return -1;
    
	xn::DepthGenerator	depthGenerator;
	xn::DepthMetaData	dmd;
    
    if (!initDepthGenerator(context, depthGenerator)) {
        error("failed to init kinect");
        exit(-1);
    }


    context.Shutdown();
    message("finish");
    return 0;
}