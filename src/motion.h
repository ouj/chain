#ifndef _MOTION_H_
#define _MOTION_H_

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#include <XnCppWrapper.h>
#include <XnVNite.h>
#pragma clang diagnostic pop


#define POSE_TO_USE "Psi"

bool setupKinect();
void cleanupKinect();
bool runKinect();
void updateKinect();
const XnDepthPixel* getKinectDepthImage();
const XnRGB24Pixel* getKinectColorImage();
const xn::SkeletonCapability getSkeleton();

bool initDepthGenerator(xn::Context &context, xn::DepthGenerator &depth);
bool initImageGenerator(xn::Context &context, xn::ImageGenerator &image);
bool initUserGenerator(xn::Context &context, xn::UserGenerator &user);
//
//void XN_CALLBACK_TYPE User_NewUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
//void XN_CALLBACK_TYPE User_LostUser(xn::UserGenerator& generator, XnUserID nId, void* pCookie);
//void XN_CALLBACK_TYPE Pose_Detected(xn::PoseDetectionCapability& pose, const XnChar* strPose, XnUserID nId, void* pCookie);
//void XN_CALLBACK_TYPE Calibration_Start(xn::SkeletonCapability& capability, XnUserID nId, void* pCookie);
//void XN_CALLBACK_TYPE Calibration_End(xn::SkeletonCapability& capability, XnUserID nId, XnBool bSuccess, void* pCookie);

#endif 
