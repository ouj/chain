#include "motion.h"
#include "debug.h"

static xn::Context          _context;
static xn::UserGenerator    _user;
static xn::DepthGenerator	_depth;
static xn::ImageGenerator   _image;
static xn::DepthMetaData	_dmd;

bool setupKinect() {
    XnStatus ret = _context.Init();
    if(!error_if_not(ret == XN_STATUS_OK, "failed to init openni context"))
        return -1;
    
    if (!initDepthGenerator(_context, _depth)) {
        error("failed to init depth genreator");
        return false;
    }
    
    if (!initImageGenerator(_context, _image)) {
        error("failed to init image generator");
        return false;
    }
    
    if (!initUserGenerator(_context, _user)) {
        error("failed to init user generator");
        return false;
    }
    
    return true;
}

const XnRGB24Pixel* getKinectColorImage() {
    if (_image.IsValid()) 
        return _image.GetRGB24ImageMap();
    return 0;
}

const XnDepthPixel* getKinectDepthImage() {
    if (_depth.IsValid()) 
        return _depth.GetDepthMap();
    return 0;
}
                                     
const xn::SkeletonCapability getSkeleton() {
    if (!_user.IsValid())
        error("invalid user node");
    return _user.GetSkeletonCap();
}

void updateKinect() {
    _context.WaitAndUpdateAll();
}

void cleanupKinect() {
    _context.Shutdown();
}

bool runKinect() {
    XnStatus rc = _context.StartGeneratingAll();
    if(!error_if_not_va(rc == XN_STATUS_OK, "failed to start generating data"))
       return false;
    return true;
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

bool initImageGenerator(xn::Context &context, xn::ImageGenerator &image) {
    XnStatus rc = XN_STATUS_OK;	
	XnMapOutputMode map_mode; 
	
    rc = context.FindExistingNode(XN_NODE_TYPE_IMAGE, image);
    if (rc == XN_STATUS_OK) {
        rc = image.GetMapOutputMode(map_mode);
        error_if_not(rc == XN_STATUS_OK, "failed to get map output mode");
    } else {
        rc = image.Create(context);
		error_if_not(rc == XN_STATUS_OK, "error creating depth generator");
		if (rc != XN_STATUS_OK) return false;
		
		// make new map mode -> default to 640 x 480 @ 30fps
		map_mode.nXRes = XN_VGA_X_RES;
		map_mode.nYRes = XN_VGA_Y_RES;
		map_mode.nFPS  = 30;
		
		rc = image.SetMapOutputMode(map_mode);
        error_if_not(rc == XN_STATUS_OK, "failed to set map output mode");
    }
    return true;
}


void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator,
                                   XnUserID nId, void* pCookie) {
    message_va("New User: %d", nId); 
    _user.GetPoseDetectionCap().StartPoseDetection(POSE_TO_USE, nId);
}


void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId,
                                    void* pCookie) {
    message_va("Lost user %d\n", nId);
	generator.GetSkeletonCap().Reset(nId);
}

void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, 
                                    const XnChar* strPose, XnUserID nId, 
                                    void* pCookie) {
    message_va("Pose %s for user %d", strPose, nId); 
    _user.GetPoseDetectionCap().StopPoseDetection(nId); 
    _user.GetSkeletonCap().RequestCalibration(nId, TRUE);
}

void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId,
                                        void* pCookie) {
    message_va("Starting calibration for user %d", nId);
}

void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId, 
                                      XnBool bSuccess, void* pCookie) {
    if (bSuccess) {
        message("User calibrated");
        _user.GetSkeletonCap().StartTracking(nId); 
    } else {
        message_va("Failed to calibrate user %d", nId);
        _user.GetPoseDetectionCap().StartPoseDetection( POSE_TO_USE,
                                                               nId);
    }
}


bool initUserGenerator(xn::Context &context, xn::UserGenerator &user) {
    XnStatus rc = XN_STATUS_OK;	

    rc = context.FindExistingNode(XN_NODE_TYPE_USER, user);
    if (rc != XN_STATUS_OK) {
        rc = _user.Create(context);
		error_if_not(rc == XN_STATUS_OK, "error creating user generator");
		if (rc != XN_STATUS_OK) return false;
    }
    XnCallbackHandle h1, h2, h3; 
    user.RegisterUserCallbacks(User_NewUser, User_LostUser, 0, h1); 
    user.GetPoseDetectionCap().RegisterToPoseCallbacks(Pose_Detected, 0, 0, h2); 
    user.GetSkeletonCap().RegisterCalibrationCallbacks(Calibration_Start, Calibration_End, 0, h3);
    return true; 
}