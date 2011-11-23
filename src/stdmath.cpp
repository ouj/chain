#include "stdmath.h"
#define PIf 3.14159265f
#define PI 3.1415926535897932384626433832795

template<> const float consts<float>::pi = PIf;
template<> const float consts<float>::pi2 = PIf*PIf;
template<> const float consts<float>::pi3 = PIf*PIf*PIf;
template<> const float consts<float>::twopi = 2.0f * PIf;
template<> const float consts<float>::halfpi = PIf / 2.0f;
template<> const float consts<float>::fourpi = 4.0f * PIf;
template<> const float consts<float>::inv_pi = 1.0f / PIf;
template<> const float consts<float>::inv_twopi = 1.0f / (2.0f * PIf);
template<> const float consts<float>::inv_fourpi = 1.0f / (4.0f * PIf);

template<> const double consts<double>::pi = PI;
template<> const double consts<double>::pi2 = PI*PI;
template<> const double consts<double>::pi3 = PI*PI*PI;
template<> const double consts<double>::twopi = 2.0 * PI;
template<> const double consts<double>::halfpi = PI / 2.0;
template<> const double consts<double>::fourpi = 4.0 * PI;
template<> const double consts<double>::inv_pi = 1.0 / PI;
template<> const double consts<double>::inv_twopi = 1.0 / (2.0 * PI);
template<> const double consts<double>::inv_fourpi = 1.0 / (4.0 * PI);

template<> const float consts<float>::epsilon = 1e-7f;
template<> const double consts<double>::epsilon = 1e-15;
