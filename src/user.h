#ifndef _USER_H_
#define _USER_H_

#include <XnCppWrapper.h>
#include <XnVNite.h>

#ifdef WIN32
#include <gl/glew.h>
#include <gl/glut.h>
#else
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#endif // WIN32

struct KinectLimb {
    KinectLimb(XnSkeletonJoint nStartJoint, XnSkeletonJoint nEndJoint);
    
	KinectLimb() {};
	~KinectLimb() {};
    
	XnSkeletonJoint startJoint;
	XnSkeletonJoint endJoint;
	XnPoint3D position[2];
	bool found;
	void glDraw();
};

struct KinectUser {
    KinectUser();
    void update(xn::UserGenerator &user, xn::DepthGenerator &depth, XnUserID id);
    void updateLimb(KinectLimb& limb, xn::UserGenerator &user, xn::DepthGenerator &depth);
    void glDraw();
    KinectLimb neck;
    
	// left arm + shoulder
	KinectLimb leftShoulder;
	KinectLimb leftUpperArm;
	KinectLimb leftLowerArm;
    
	// right arm + shoulder
	KinectLimb rightShoulder;
	KinectLimb rightUpperArm;
	KinectLimb rightLowerArm;
    
	// torso
	KinectLimb leftUpperTorso;
	KinectLimb rightUpperTorso;
    
	// left lower torso + leg
	KinectLimb leftLowerTorso;
	KinectLimb leftUpperLeg;
	KinectLimb leftLowerLeg;
    
	// right lower torso + leg
	KinectLimb rightLowerTorso;
	KinectLimb rightUpperLeg;
	KinectLimb rightLowerLeg;
    
	KinectLimb  hip;
	XnUserID    id;
    
    XnPoint3D	center;
    bool        tracking;
};

#endif