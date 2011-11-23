#ifndef FUNC_H
#define FUNC_H

#include "vec.h"
#include <stdint.h>
#include "stdmath.h"

template<typename R>
inline R radian(R deg) {
    return deg / 180.0 * consts<R>::pi;
}

template<typename R>
inline R degree(R rad) {
    return rad / consts<R>::pi * 180.0;
}

template<typename R>
inline R mean(const vec3<R>& v) {
    return (v.x + v.y + v.z) / 3;
}

template<typename T, typename R>
inline T lerp(const T& a, const T& b, R t) {
	return a * (1-t) + b * t;
}

template<typename T, typename R>
inline T lerp(const T& a, const T& b, const T& c, R u, R v) {
	return a * (1-u-v) + b * u + c * v;
}

template<typename T, typename R>
inline T lerp(const T& a, const T& b, const T& c, vec2<R> uv) {
	return a * (1-uv.x-uv.y) + b * uv.x + c * uv.y;
}

template<typename T>
inline T clamp(const T& x, const T& min, const T& max) {
    if(x < min) return min;
    else if(x > max) return max;
    else return x;
}

// power of 2
template<typename R>
inline R pow2(R x) {
    return x * x;
}

template<typename R>
inline int sign(R v){
    return v < R(0) ? -1 : 1;
}

// cubic root
template<typename R>
inline R curt(R v) {
    return (v < R(0) ? -1 : 1) * pow((R)fabs(v), R(1)/R(3));
}
    
// from pbrt
inline bool ispow2(int v) {
    return (v & (v - 1)) == 0;
}

// from pbrt
inline unsigned int topow2(unsigned int v) {
    v--;
    v |= v >> 1;    v |= v >> 2;
    v |= v >> 4;    v |= v >> 8;
    v |= v >> 16;
    return v+1;
}

// from pbrt
inline int posmod(int a, int b) {
    int n = int(a/b);
    a -= n*b;
    if (a < 0) a += b;
    return a;
}

inline vec3<unsigned char> convertcolor_fto8(const vec3f& vf) {
    return makevec3<unsigned char>(
        (unsigned char)clamp(vf.x*256, 0.0f, 255.9999f),
        (unsigned char)clamp(vf.y*256, 0.0f, 255.9999f),
        (unsigned char)clamp(vf.z*256, 0.0f, 255.9999f)
    );
}

// hashing ------------------------------------------------------

// move hash function to boost?

inline uint32_t hash32ui_knuth(uint32_t v) {
    return 2654435761 * v;
}

// http://www.concentric.net/~ttwang/tech/inthash.htm
inline uint32_t hash32ui_wang1(uint32_t key) {
    key = ~key + (key << 15); // key = (key << 15) - key - 1;
    key = key ^ (key >> 12);
    key = key + (key << 2);
    key = key ^ (key >> 4);
    key = key * 2057; // key = (key + (key << 3)) + (key << 11);
    key = key ^ (key >> 16);
    return key;
}

inline uint32_t hash32ui_wang2(uint32_t key) {
    uint32_t c2=0x27d4eb2d; // a prime or an odd constant
    key = (key ^ 61) ^ (key >> 16);
    key = key + (key << 3);
    key = key ^ (key >> 4);
    key = key * c2;
    key = key ^ (key >> 15);
    return key;
}

// http://isthe.com/chongo/tech/comp/fnv/
inline uint32_t hash32ui_fnv1a(const unsigned char* data, int l) {
    uint32_t hash = 2166136261;
    for(int i = 0; i < l; i ++) {
        hash ^= (uint32_t)data[i];
        hash *= 16777619;
    }
    return hash;
}

template<typename T>
inline uint32_t hash32ui_fnv1a(const T& key) {
    return hash32ui_fnv1a((unsigned char*)&key, sizeof(T));
}

// Gaussian functions
template<typename R>
inline R normalizedGaussian(R x, R mu, R sigma) {
    R a = x - mu;
    R b = 2*sigma*sigma;
    return exp(-a*a / b) / sqrt(b * consts<R>::pi);
}

template<typename R>
inline R gaussian(R x, R mu, R sigma) {
    R a = x - mu;
    R b = 2*sigma*sigma;
    return exp(-a*a / b);
}

// polynomial solving.
// sovle linear equation
template<typename R>
inline int solveLinear(R a, R b, R *root) {
    if (fabs(a) > consts<R>::epsilon) {
        *root = -b / a;
        return 1;
    } else if (fabs(b) > consts<R>::epsilon) {
        return 0;
    }
    return -1;
}

// solve quadratic equation
template<typename R>
inline int solveQuadratic(R a, R b, R c, R roots[2]) {
    if (fabs(a) <= consts<R>::epsilon) 
        return solveLinear(b, c, roots);
    R d = b*b - 4*a*c;
    if (fabs(d) <= consts<R>::epsilon) {
        roots[0] = (-b) / (2*a);
        return 1;
    } else if (d < 0.0f) {
        return 0;
    } else {
        roots[0] = (-b - sqrt(d)) / (2*a);
        roots[1] = (-b + sqrt(d)) / (2*a);
        return 2;
    }
    return -1;
}

template<typename R>
inline int solveNormalizedCubic(R a, R b, R c, R roots[3]) {
    R q = (3*b - a*a)/9;
    R r = (9*a*b - 27*c - 2*a*a*a)/54;
    R q3 = q*q*q;
    R d = q3 + r*r;  // polynomial discriminant
    if (d >= R(0)) {
        R sqrtd = sqrt(d);
        R s = curt(r + sqrtd);
        R t = curt(r - sqrtd);
        roots[0] = (-a/3 + (s + t));   // one real root
        return 1;
    } else {
        float th = acos( r/sqrt(-(q3)) );
        float sqrtq = sqrt(-q);
        roots[0] = (2*sqrtq*cos(th/3) - a/3);
        roots[1] = (2*sqrtq*cos((th + consts<R>::twopi)/3) - a/3);
        roots[2] = (2*sqrtq*cos((th + consts<R>::fourpi)/3) - a/3);
        return 3;
    }
    return -1;
}

// solve cubic equatioin
template<typename R>
inline int solveCubic(R a, R b, R c, R d, R roots[3]) {
    if (fabs(a) <= consts<R>::epsilon) 
        return solveQuadratic(b, c, d, roots);
    else
        return solveNormalizedCubic(b/a, c/a, d/a, roots);
}
    
template<typename R>
inline R smoothstep(const R& e1, const R& e0, const R& x)
{
    R t = clamp((x - e0)/(e1 - e0), (R)0, (R)1); 
    return t*t*(3 - 2*t);
}

#endif
