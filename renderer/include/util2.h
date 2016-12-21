/*
 * util2.h
 *
 *  Created on: Feb 14, 2013
 *      Author: szhao
 */

#ifndef UTIL2_H_
#define UTIL2_H_

#include "util.h"
#include <algorithm>


namespace util {


template <typename T>
inline void reflect(const tvec::TVector3<T> &a, const tvec::TVector3<T> &n, tvec::TVector3<T> &b) {
    b = -static_cast<T>(2.0)*tvec::dot(a, n)*n + a;
}


template <typename T>
inline bool refract(const tvec::TVector3<T> &a, const tvec::TVector3<T> &n, T eta, tvec::TVector3<T> &b) {
    tvec::TVector3<T> q = tvec::dot(a, n)*n;
    tvec::TVector3<T> p = (a - q)/eta;
    if ( p.length() > static_cast<T>(1.0) ) {
        reflect(a, n, b);
        return false;
    } else {
        q.normalize();
        q *= util::safeSqrt(static_cast<T>(1.0) - p.lengthSquared());
        b = p + q;
        return true;
   }
}


template <typename T>
inline bool rayIntersectBox(const tvec::TVector3<T> &p, const tvec::TVector3<T> &d,
                             const tvec::TVector3<T> &min, const tvec::TVector3<T> &max,
                             T &t1, T &t2) {
    t1 = M_MIN; t2 = M_MAX;
    for ( int i = 0; i < 3; ++i )
        if ( std::abs(d[i]) > M_EPSILON ) {
            T v1, v2;
            if ( d[i] > static_cast<T>(0.0) ) {
                v1 = (min[i] - p[i])/d[i];
                v2 = (max[i] - p[i])/d[i];
            } else {
                v1 = (max[i] - p[i])/d[i];
                v2 = (min[i] - p[i])/d[i];
            }

            if ( v1 > t1 ) t1 = v1;
            if ( v2 < t2 ) t2 = v2;
        }
        else if ( min[i] - p[i] > M_EPSILON || p[i] - max[i] > M_EPSILON )
            return false;

    return t2 > t1;
}


}   /* namespace util */

#endif /* UTIL2_H_ */
