#ifndef STDMATH_H_
#define STDMATH_H_

#include <limits>
#include <algorithm>
#include <cmath>
#include <stdint.h>

using std::min;
using std::max;

using std::pow;
using std::cos;
using std::sin;
using std::abs;
using std::exp;
using std::log;
using std::modf;

using std::numeric_limits;

// check for nans and inf
#ifdef WIN32
#include <float.h>
#ifndef isnan
#define isnan(a) _isnan(a)
#endif
#ifndef isinf
#define isinf(a) (!_finite(a))
#endif
#else
using std::isnan;
using std::isinf;
#endif

// this is a gigantic hack to make it work now
// note that these are not consts, but there is no fix to it
template<typename R> 
struct consts {
    static const R halfpi;
    static const R pi;
    static const R pi2;
    static const R pi3;
    static const R twopi;
    static const R fourpi;
    static const R inv_pi;
    static const R inv_twopi;
    static const R inv_fourpi;

    static const R epsilon;
};

#endif
