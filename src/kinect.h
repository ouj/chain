#ifndef _MOTION_H_
#define _MOTION_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <XnCppWrapper.h>
#include <XnVNite.h>
#pragma GCC diagnostic pop

#include "driver.h"
#include "user.h"
#define POSE_TO_USE "Psi"

bool setupKinect();
void cleanupKinect();
bool runKinect();
void updateKinect();
const XnDepthPixel* getKinectDepthImage();
const XnRGB24Pixel* getKinectColorImage();
const xn::SkeletonCapability getSkeleton();
KinectDriver& getKinect();

bool initDepthGenerator(xn::Context &context, xn::DepthGenerator &depth);
bool initImageGenerator(xn::Context &context, xn::ImageGenerator &image);
bool initUserGenerator(xn::Context &context, xn::UserGenerator &user);

xn::UserGenerator& getUserGenerator();

#endif 
