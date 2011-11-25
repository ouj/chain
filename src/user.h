#ifndef _USER_H_
#define _USER_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <XnCppWrapper.h>
#include <XnVNite.h>
#pragma GCC diagnostic pop

struct KinectUser {
    void update(xn::UserGenerator &generator, XnUserID id);
};

#endif