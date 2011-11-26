#include "user.h"
#include "debug.h"
#include <stdmath.h>
#include <float.h>

static KinectUser kinectUser;
KinectUser& getKinectUser() {
    return kinectUser;
}

KinectLimb::KinectLimb(XnSkeletonJoint nStartJoint, XnSkeletonJoint nEndJoint) 
: startJoint(nStartJoint), endJoint(nEndJoint) ,found(false) {
    posSrn[0].X = posSrn[1].X = 0;
    posSrn[0].Y = posSrn[1].Y = 0;
    posSrn[0].Z = posSrn[1].Z = 0;
    pos3D[0].X = pos3D[1].X = 0;
    pos3D[0].Y = pos3D[1].Y = 0;
    pos3D[0].Z = pos3D[1].Z = 0;
}

void KinectLimb::glDraw() {
    if(!found) return;
    glLineWidth(5);
    glColor3f(0,0,1);
    glBegin(GL_LINES);
    glVertex2f(posSrn[0].X, posSrn[0].Y);
    glVertex2f(posSrn[1].X, posSrn[1].Y);
    glEnd();
    glPointSize(10);
    glBegin(GL_POINTS);
    glColor3f(1,0,0);
    glVertex2f(posSrn[0].X, posSrn[0].Y);
    glVertex2f(posSrn[1].X, posSrn[1].Y);
    glEnd();
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
rightLowerLeg(XN_SKEL_RIGHT_KNEE, XN_SKEL_RIGHT_FOOT), 
hip(XN_SKEL_LEFT_HIP, XN_SKEL_RIGHT_HIP), tracking(false) {
}

void KinectUser::glDraw() {
    if (tracking) {
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
    
    //draw bounding box
    glLineWidth(1);
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex2f(bboxSrn.LeftBottomNear.X, bboxSrn.LeftBottomNear.Y);
    glVertex2f(bboxSrn.RightTopFar.X, bboxSrn.LeftBottomNear.Y);
    
    glVertex2f(bboxSrn.LeftBottomNear.X, bboxSrn.LeftBottomNear.Y);
    glVertex2f(bboxSrn.LeftBottomNear.X, bboxSrn.RightTopFar.Y);
    
    glVertex2f(bboxSrn.RightTopFar.X, bboxSrn.RightTopFar.Y);
    glVertex2f(bboxSrn.LeftBottomNear.X, bboxSrn.RightTopFar.Y);
    
    glVertex2f(bboxSrn.RightTopFar.X, bboxSrn.LeftBottomNear.Y);
    glVertex2f(bboxSrn.RightTopFar.X, bboxSrn.RightTopFar.Y);
    glEnd();
    
    //draw center
    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(1,1,0);
    glVertex2f(centerSrn.X, centerSrn.Y);
    glEnd();
}

static void grow(XnBoundingBox3D& bbox, const XnPoint3D &pt) {
    bbox.LeftBottomNear.X = std::min<XnFloat>(bbox.LeftBottomNear.X, pt.X);
    bbox.LeftBottomNear.Y = std::min<XnFloat>(bbox.LeftBottomNear.Y, pt.Y);
    bbox.LeftBottomNear.Z = std::min<XnFloat>(bbox.LeftBottomNear.Z, pt.Z);
    bbox.RightTopFar.X = std::max<XnFloat>(bbox.RightTopFar.X, pt.X);
    bbox.RightTopFar.Y = std::max<XnFloat>(bbox.RightTopFar.Y, pt.Y);
    bbox.RightTopFar.Z = std::max<XnFloat>(bbox.RightTopFar.Z, pt.Z);
}

void KinectUser::computeBound(xn::UserGenerator &user, xn::DepthGenerator &depth) {
    bbox3D.LeftBottomNear.X = bbox3D.LeftBottomNear.Y = bbox3D.LeftBottomNear.Z = FLT_MAX;
    bbox3D.RightTopFar.X = bbox3D.RightTopFar.Y = bbox3D.RightTopFar.Z = -FLT_MAX;
    
    XnSkeletonJointPosition a;
    xn::SkeletonCapability cap = user.GetSkeletonCap();
	cap.GetSkeletonJointPosition(id, XN_SKEL_HEAD, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_NECK, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_TORSO, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_LEFT_SHOULDER, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_RIGHT_SHOULDER, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_LEFT_HIP, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    cap.GetSkeletonJointPosition(id, XN_SKEL_RIGHT_HIP, a);
	if(a.fConfidence >= 0.3f) grow(bbox3D, a.position);
    
    
    depth.ConvertRealWorldToProjective(2, (XnPoint3D*)&bbox3D, (XnPoint3D*)&bboxSrn);
    
    center3D.X = (0.5) * (bbox3D.LeftBottomNear.X + bbox3D.RightTopFar.X);
    center3D.Y = (0.5) * (bbox3D.LeftBottomNear.Y + bbox3D.RightTopFar.Y);
    center3D.Z = (0.5) * (bbox3D.LeftBottomNear.Z + bbox3D.RightTopFar.Z);
   
    centerSrn.X = (0.5) * (bboxSrn.LeftBottomNear.X + bboxSrn.RightTopFar.X);
    centerSrn.Y = (0.5) * (bboxSrn.LeftBottomNear.Y + bboxSrn.RightTopFar.Y);
    centerSrn.Z = (0.5) * (bboxSrn.LeftBottomNear.Z + bboxSrn.RightTopFar.Z);
}

void KinectUser::update(xn::UserGenerator &user, xn::DepthGenerator &depth, XnUserID id) {
    if (user.GetSkeletonCap().IsTracking(id)) {
        this->id = id;
        tracking = true;
        
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
        
        computeBound(user, depth);
    } else tracking = false;
}

void KinectUser::updateLimb(KinectLimb& limb, xn::UserGenerator &user, xn::DepthGenerator &depth) {	
	XnSkeletonJointPosition a,b;
	user.GetSkeletonCap().GetSkeletonJointPosition(id, limb.startJoint, a);
	user.GetSkeletonCap().GetSkeletonJointPosition(id, limb.endJoint, b);
	if(a.fConfidence < 0.3f || b.fConfidence < 0.3f) {
		limb.found = false; 
		return;
	}
	
	limb.found = true;
	limb.pos3D[0] = a.position;
	limb.pos3D[1] = b.position;
	depth.ConvertRealWorldToProjective(2, limb.pos3D, limb.posSrn);
}