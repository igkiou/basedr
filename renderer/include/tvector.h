/*
 * vector.h
 *
 *  Created on: Nov 24, 2015
 *      Author: igkiou
 */

#ifndef TVECTOR_H_
#define TVECTOR_H_

#include <cmath>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include "constants.h"
#include "debug_util.h"

namespace tvec {

/*
 * TODO: Add inlines here.
 */

template <typename T> struct TVector2 {
	typedef T          value_type;
//	typedef TPoint2<T> point_type;

	T x, y;

	/// Number of dimensions
	const static int dim = 2;

	// Default constructor
	TVector2() : x(0), y(0) {  }

	/// Initialize the vector with the specified X and Z components
	TVector2(T x, T y) : x(x), y(y) {  }

	/// Initialize all components of the the vector with the specified value
	explicit TVector2(T val) : x(val), y(val) { }

	/// Initialize the vector with the components of another vector data structure
	template <typename T2> explicit TVector2(const TVector2<T2> &v)
		: x((T) v.x), y((T) v.y) { }

	/// Initialize from a C-style array. For use with MATLAB mxArrays with
	/// double data fields.
	// Not safe if incorrectly sized array is passed. size argument is used for
	// some weak safety checking.
	template <typename T2> explicit TVector2(const T2 *v, const int size) {
		Assert(size == dim);
		x = static_cast<T>(v[0]);
		y = static_cast<T>(v[1]);
	}

	/// Add two vectors and return the result
	TVector2 operator+(const TVector2 &v) const {
		return TVector2(x + v.x, y + v.y);
	}

	/// Subtract two vectors and return the result
	TVector2 operator-(const TVector2 &v) const {
		return TVector2(x - v.x, y - v.y);
	}

	/// Add another vector to the current one
	TVector2& operator+=(const TVector2 &v) {
		x += v.x; y += v.y;
		return *this;
	}

	/// Subtract another vector from the current one
	TVector2& operator-=(const TVector2 &v) {
		x -= v.x; y -= v.y;
		return *this;
	}

	/// Multiply the vector by the given scalar and return the result
	TVector2 operator*(T f) const {
		return TVector2(x*f, y*f);
	}

	/// Multiply the vector by the given scalar
	TVector2 &operator*=(T f) {
		x *= f; y *= f;
		return *this;
	}

	/// Return a negated version of the vector
	TVector2 operator-() const {
		return TVector2(-x, -y);
	}

	/// Divide the vector by the given scalar and return the result
	TVector2 operator/(T f) const {
		Assert(std::abs(static_cast<Float>(f)) > M_EPSILON);
		T recip = (T) 1 / f;
		return TVector2(x * recip, y * recip);
	}

	/// Divide the vector by the given scalar
	TVector2 &operator/=(T f) {
		Assert(std::abs(static_cast<Float>(f)) > M_EPSILON);
		T recip = (T) 1 / f;
		x *= recip; y *= recip;
		return *this;
	}

//	T dot(const TVector2<T> &v) {
//		return v.x * x + v.y * y;
//	}

	/// Index into the vector's components
	T &operator[](int i) {
		return (&x)[i];
	}

	/// Index into the vector's components (const version)
	T operator[](int i) const {
		return (&x)[i];
	}

	/// Return the squared 2-norm of this vector
	T lengthSquared() const {
		return x*x + y*y;
	}

	/// Return the 2-norm of this vector
	T length() const {
		return std::sqrt(lengthSquared());
	}

	void normalize() {
		if (!isZero()) {
			T tmp = length();
			x = x / tmp;
			y = y / tmp;
		}
	}

	void zero() {
		x = 0;
		y = 0;
	}

	/// Return whether or not this vector is identically zero
	bool isZero() const {
		return x == 0 && y == 0;
	}

	/// Equality test
	bool operator==(const TVector2 &v) const {
		return (v.x == x && v.y == y);
	}

	/// Approximate equality test
	bool aproxEqual(const TVector2 &v) const {
		return (std::abs(v.x - x) < M_EPSILON * std::max((T) 1, std::max(std::abs(v.x), std::abs(x)))) && \
				(std::abs(v.y - y) < M_EPSILON * std::max((T) 1, std::max(std::abs(v.y), std::abs(y))));
	}

	/// Inequality test
	bool operator!=(const TVector2 &v) const {
		return v.x != x || v.y != y;
	}

};

template <typename T> inline TVector2<T> operator*(T f, const TVector2<T> &v) {
	return v*f;
}

template <typename T> inline T dot(const TVector2<T> &v1, const TVector2<T> &v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

template <typename T> inline T absDot(const TVector2<T> &v1, const TVector2<T> &v2) {
	return std::abs(dot(v1, v2));
}

template <typename T> inline TVector2<T> normalize(const TVector2<T> &v) {
	return v / v.length();
}

template <> inline TVector2<int> TVector2<int>::operator/(int s) const {
	Assert(std::abs(static_cast<Float>(s)) > M_EPSILON);
	return TVector2(x/s, y/s);
}

template <> inline TVector2<int> &TVector2<int>::operator/=(int s) {
	Assert(std::abs(static_cast<Float>(s)) > M_EPSILON);
	x /= s;
	y /= s;
	return *this;
}

/**
 * \headerfile mitsuba/core/vector.h mitsuba/mitsuba.h
 * \brief Parameterizable three-dimensional vector data structure
 * \ingroup libcore
 */
template <typename T> struct TVector3 {
	typedef T          value_type;
//	typedef TPoint3<T> point_type;

	T x, y, z;

	/// Number of dimensions
	const static int dim = 3;

	/// default constructor
	TVector3() : x(0), y(0), z(0) {  }

	/// Initialize the vector with the specified X, Y and Z components
	TVector3(T x, T y, T z) : x(x), y(y), z(z) {  }

	/// Initialize all components of the the vector with the specified value
	explicit TVector3(T val) : x(val), y(val), z(val) { }

	/// Initialize the vector with the components of another vector data structure
	template <typename T2> explicit TVector3(const TVector3<T2> &v)
		: x((T) v.x), y((T) v.y), z((T) v.z) { }

	/// Initialize from a C-style array. For use with MATLAB mxArrays with
	/// double data fields.
	// Not safe if incorrectly sized array is passed. size argument is used for
	// some weak safety checking.
	template <typename T2> explicit TVector3(const T2 *v, const int size) {
		Assert(size == dim);
		x = static_cast<T>(v[0]);
		y = static_cast<T>(v[1]);
		z = static_cast<T>(v[2]);
	}

	/// Add two vectors and return the result
	TVector3 operator+(const TVector3 &v) const {
		return TVector3(x + v.x, y + v.y, z + v.z);
	}

	/// Subtract two vectors and return the result
	TVector3 operator-(const TVector3 &v) const {
		return TVector3(x - v.x, y - v.y, z - v.z);
	}

	/// Add another vector to the current one
	TVector3& operator+=(const TVector3 &v) {
		x += v.x; y += v.y; z += v.z;
		return *this;
	}

	/// Subtract another vector from the current one
	TVector3& operator-=(const TVector3 &v) {
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}

	/// Multiply the vector by the given scalar and return the result
	TVector3 operator*(T f) const {
		return TVector3(x*f, y*f, z*f);
	}

	/// Multiply the vector by the given scalar
	TVector3 &operator*=(T f) {
		x *= f; y *= f; z *= f;
		return *this;
	}

	/// Return a negated version of the vector
	TVector3 operator-() const {
		return TVector3(-x, -y, -z);
	}

	/// Divide the vector by the given scalar and return the result
	TVector3 operator/(T f) const {
		Assert(std::abs(static_cast<Float>(f)) > M_EPSILON);
		T recip = (T) 1 / f;
		return TVector3(x * recip, y * recip, z * recip);
	}

	/// Divide the vector by the given scalar
	TVector3 &operator/=(T f) {
		Assert(std::abs(static_cast<Float>(f)) > M_EPSILON);
		T recip = (T) 1 / f;
		x *= recip; y *= recip; z *= recip;
		return *this;
	}

//	void cross(const TVector3 &v1, const TVector3 &v2) {
//		x =  v1.y*v2.z - v1.z*v2.y;
//		y = -v1.x*v2.z + v1.z*v2.x;
//		z =  v1.x*v2.y - v1.y*v2.x;
//	}
//
//	T dot(const TVector3<T> &v) {
//		return v.x * x + v.y * y + v.z * z;
//	}

	/// Index into the vector's components
	T &operator[](int i) {
		return (&x)[i];
	}

	/// Index into the vector's components (const version)
	T operator[](int i) const {
		return (&x)[i];
	}

	/// Return the squared 2-norm of this vector
	T lengthSquared() const {
		return x*x + y*y + z*z;
	}

	/// Return the 2-norm of this vector
	T length() const {
		return std::sqrt(lengthSquared());
	}

	void normalize() {
		if (!isZero()) {
			T tmp = length();
			x = x / tmp;
			y = y / tmp;
			z = z / tmp;
		}
	}

	void zero() {
		x = 0;
		y = 0;
		z = 0;
	}

	/// Return whether or not this vector is identically zero
	bool isZero() const {
		return x == 0 && y == 0 && z == 0;
	}

	/// Equality test
	bool operator==(const TVector3 &v) const {
		return (v.x == x && v.y == y && v.z == z);
	}

	/// Approximate equality test
	bool aproxEqual(const TVector3 &v) const {
		return (std::abs(v.x - x) < M_EPSILON * std::max((T) 1, std::max(std::abs(v.x), std::abs(x)))) && \
				(std::abs(v.y - y) < M_EPSILON * std::max((T) 1, std::max(std::abs(v.y), std::abs(y)))) && \
				(std::abs(v.z - z) < M_EPSILON * std::max((T) 1, std::max(std::abs(v.z), std::abs(z))));
	}

	/// Inequality test
	bool operator!=(const TVector3 &v) const {
		return v.x != x || v.y != y || v.z != z;
	}

};

template <typename T> inline TVector3<T> operator*(T f, const TVector3<T> &v) {
	return v*f;
}

template <typename T> inline T dot(const TVector3<T> &v1, const TVector3<T> &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template <typename T> inline T absDot(const TVector3<T> &v1, const TVector3<T> &v2) {
	return std::abs(dot(v1, v2));
}

template <typename T> inline TVector3<T> cross(const TVector3<T> &v1, const TVector3<T> &v2) {
	/* Left-handed vector cross product */
	return TVector3<T>(
		(v1.y * v2.z) - (v1.z * v2.y),
		(v1.z * v2.x) - (v1.x * v2.z),
		(v1.x * v2.y) - (v1.y * v2.x)
	);
}

template <typename T> inline TVector3<T> normalize(const TVector3<T> &v) {
	return v / v.length();
}

template <> inline TVector3<int> TVector3<int>::operator/(int s) const {
	Assert(std::abs(static_cast<Float>(s)) > M_EPSILON);
	return TVector3(x/s, y/s, z/s);
}

template <> inline TVector3<int> &TVector3<int>::operator/=(int s) {
	Assert(std::abs(static_cast<Float>(s)) > M_EPSILON);
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

typedef TVector2<int> Vec2i;
typedef TVector2<Float> Vec2f;
typedef TVector2<double> Vec2d;
//typedef TVector2<Float> Vec2F;

typedef TVector3<int> Vec3i;
typedef TVector3<Float> Vec3f;
typedef TVector3<double> Vec3d;
//typedef TVector3<Float> Vec3F;

}	/* namespace tvec */

#endif /* TVECTOR_H_ */
