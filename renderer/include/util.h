/*
 * util.h
 *
 *  Created on: Aug 15, 2012
 *      Author: igkiou
 */

#ifndef UTIL_H_
#define UTIL_H_

#include "tvector.h"
#include "constants.h"
#include <vector>
#include <iterator>
#include <algorithm>

namespace util {

template <typename T>
inline void coordinateSystem(const tvec::TVector3<T> &a, tvec::TVector3<T> &b, tvec::TVector3<T> &c) {

	if (std::abs(a.x) > std::abs(a.y)) {
		Float invLen = FPCONST(1.0) / std::sqrt(a.x * a.x + a.z *a.z);
		c = tvec::TVector3<T>(a.z * invLen, FPCONST(0.0), -a.x * invLen);
	} else {
		Float invLen = FPCONST(1.0) / std::sqrt(a.y * a.y + a.z * a.z);
		c = tvec::TVector3<T>(FPCONST(0.0), a.z * invLen, -a.y * invLen);
	}
	b = tvec::cross(c, a);
}

//void coordinateSystem(const Vector &a, Vector &b, Vector &c) {
//	if (std::abs(a.x) > std::abs(a.y)) {
//		Float invLen = FPCONST(1.0) / std::sqrt(a.x * a.x + a.z * a.z);
//		c = Vector(a.z * invLen, FPCONST(0.0), -a.x * invLen);
//	} else {
//		Float invLen = FPCONST(1.0) / std::sqrt(a.y * a.y + a.z * a.z);
//		c = Vector(FPCONST(0.0), a.z * invLen, -a.y * invLen);
//	}
//	b = cross(c, a);
//}

template <typename T>
inline Float unitAngle(const tvec::TVector3<T> &u, const tvec::TVector3<T> &v) {
	if (tvec::dot(u, v) < FPCONST(0.0)) {
		return static_cast<Float>(M_PI - FPCONST(2.0) * std::asin((v+u).length() / FPCONST(2.0)));
	} else {
		return static_cast<Float>(FPCONST(2.0) * std::asin((v-u).length() / FPCONST(2.0)));
	}
}

template <typename T>
inline bool isSorted(const std::vector<T> &vec) {

	typename std::vector<T>::const_iterator iter = \
				std::adjacent_find(vec.begin(), vec.end(), std::greater<T>());
	return iter == vec.end();
//	return true;
}

template <typename T>
inline Float fresnelDielectric(T cosThetaI, T cosThetaT, T eta)
{
	if (std::abs(eta - FPCONST(1.0)) < M_EPSILON * std::max(FPCONST(1.0), std::abs(eta))) {
		return FPCONST(0.0);
	} else {
		T Rs = (cosThetaI - eta * cosThetaT) / (cosThetaI + eta * cosThetaT);
		T Rp = (cosThetaT - eta * cosThetaI) / (cosThetaT + eta * cosThetaI);

		/* No polarization -- return the unpolarized reflectance */
		/*
		 * TODO: May need to fix for polarization.
		 */
		return FPCONST(0.5) * static_cast<Float>(Rs * Rs + Rp * Rp);
	}
}


template <typename T>
inline T safeSqrt(T x) {
    return x > static_cast<T>(0.0) ? std::sqrt(x) : static_cast<T>(0.0);
}

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

//Vector sphericalDirection(Float theta, Float phi) {
//	Float sinTheta, cosTheta, sinPhi, cosPhi;
//
//	std::sincos(theta, &sinTheta, &cosTheta);
//	std::sincos(phi, &sinPhi, &cosPhi);
//
//	return Vector(
//		sinTheta * cosPhi,
//		sinTheta * sinPhi,
//		cosTheta
//	);
//}
//
//
//Point2 squareToDisk(const Point2 &sample) {
//	Float r = std::sqrt(sample.x);
//	Float sinPhi, cosPhi;
//	std::sincos(FPCONST(2.0) * M_PI * sample.y, &sinPhi, &cosPhi);
//
//	return Point2(
//		cosPhi * r,
//		sinPhi * r
//	);
//}
//
//
//Point2 toSphericalCoordinates(const Vector &v) {
//	Point2 result(
//		std::acos(v.z),
//		std::atan2(v.y, v.x)
//	);
//	if (result.y < FPCONST(0.0))
//		result.y += FPCONST(2.0)*M_PI;
//	return result;
//}

//Point2 squareToStdNormal(const Point2 &sample) {
//	Float r   = std::sqrt(-FPCONST(2.0) * std::fastlog(FPCONST(1.0)-sample.x)),
//		  phi = FPCONST(2.0) * M_PI * sample.y;
//	Point2 result;
//	std::sincos(phi, &result.y, &result.x);
//	return result * r;
//}


}	/* namespace util */

#endif /* UTIL_H_ */
