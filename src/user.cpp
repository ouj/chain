#include "user.h"

KinectLimb::KinectLimb(XnSkeletonJoint nStartJoint, XnSkeletonJoint nEndJoint) 
: startJoint(nStartJoint), endJoint(nEndJoint) ,found(false) {
    position[0].X = position[1].X = 0;
    position[0].Y = position[1].Y = 0;
    position[0].Z = position[1].Z = 0;
}

void KinectLimb::glDraw() {
    if(!found)
        return;
    glPushMatrix();
    glLineWidth(5);
    glColor3f(1,0,0);
    glBegin(GL_LINES);
    glVertex2i(position[0].X, position[0].Y);
    glVertex2i(position[1].X, position[1].Y);
    glEnd();
    glPopMatrix();
}


KinectUser::KinectUser() : 
neck(XN_SKEL_HEAD, XN_SKEL_NECK),
leftShoulder(XN_SKEL_NECK, XN_SKEL_LEFT_SHOULDER),
leftUpperArm(XN_SKEL_LEFT_SHOULDER, XN_SKEL_LEFT_ELBOW),
leftLowerArm(XN_SKEL_LEFT_ELBOW, XN_SKEL_LEFT_HAND),
rightShoulder(XN_SKEL_NECK, XN_SKEL_RIGHT_SHOULDER),
rightUpperArm(XN_SKEL_RIGHT_SHOULDER, XN_SKEL_RIGHT_ELBOW),
rightLowerArm(XN_SKEL_RIGHT_ELBOW, XN_SKEL_RIGHT_HAND),
leftUpperTorso(XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO),
rightUpperTorso(XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO),
leftLowerTorso(XN_SKEL_TORSO, XN_SKEL_LEFT_HIP),
leftUpperLeg(XN_SKEL_LEFT_HIP, XN_SKEL_LEFT_KNEE),
leftLowerLeg(XN_SKEL_LEFT_KNEE, XN_SKEL_LEFT_FOOT),
rightLowerTorso(XN_SKEL_TORSO, XN_SKEL_RIGHT_HIP),
rightUpperLeg(XN_SKEL_RIGHT_HIP, XN_SKEL_RIGHT_KNEE),
rightLowerLeg(XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT), tracking(false) {
    
}

void KinectUser::glDraw() {
    neck.glDraw();
	
	// left arm + shoulder
	leftShoulder.glDraw();
	leftUpperArm.glDraw();
	leftLowerArm.glDraw();
	
	// right arm + shoulder
	rightShoulder.glDraw();
	rightUpperArm.glDraw();
	rightLowerArm.glDraw();
	
	// upper torso
	leftUpperTorso.glDraw();
	rightUpperTorso.glDraw();
	
	// left lower torso + leg
	leftLowerTorso.glDraw();
	leftUpperLeg.glDraw();
	leftLowerLeg.glDraw();
    
	// right lower torso + leg
	rightLowerTorso.glDraw();
	rightUpperLeg.glDraw();
	rightLowerLeg.glDraw();
	
	hip.glDraw();
}

void KinectUser::update(xn::UserGenerator &user, xn::DepthGenerator &depth, XnUserID id) {
    this->id = id;
    
    updateLimb(neck, user, depth);
	
	// left arm + shoulder
	updateLimb(leftShoulder, user, depth);
	updateLimb(leftUpperArm, user, depth);
	updateLimb(leftLowerArm, user, depth);
	
	// right arm + shoulder
	updateLimb(rightShoulder, user, depth);
	updateLimb(rightUpperArm, user, depth);
	updateLimb(rightLowerArm, user, depth);
	
	// upper torso
	updateLimb(leftUpperTorso, user, depth);
	updateLimb(rightUpperTorso, user, depth);
	
	// left lower torso + leg
	updateLimb(leftLowerTorso, user, depth);
	updateLimb(leftUpperLeg, user, depth);
	updateLimb(leftLowerLeg, user, depth);
	
	// right lower torso + leg
	updateLimb(rightLowerTorso, user, depth);
	updateLimb(rightUpperLeg, user, depth);
	updateLimb(rightLowerLeg, user, depth);
    
	updateLimb(hip, user, depth);	
}

void KinectUser::updateLimb(KinectLimb& limb, xn::UserGenerator &user, xn::DepthGenerator &depth) {
    if(!user.GetSkeletonCap().IsTracking(id)) {
		return;
	}
	
	XnSkeletonJointPosition a,b;
	user.GetSkeletonCap().GetSkeletonJointPosition(id, limb.startJoint, a);
	user.GetSkeletonCap().GetSkeletonJointPosition(id, limb.endJoint, b);
	if(a.fConfidence < 0.3f || b.fConfidence < 0.3f) {
		limb.found = false; 
		return;
	}
	
	limb.found = true;
	limb.position[0] = a.position;
	limb.position[1] = b.position;
	
	depth.ConvertRealWorldToProjective(2, limb.position, limb.position);
}