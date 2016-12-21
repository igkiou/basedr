#include <algorithm>
#include <fstream>
#include <stdio.h>
#include "phase.h"
#include "util.h"

namespace pfunc {

Float HenyeyGreenstein::f(const tvec::Vec3f &in, const tvec::Vec3f &out) const {
	return static_cast<Float>(1.0 / (4.0 * M_PI)) * (FPCONST(1.0) - m_g * m_g) /
		std::pow(1.f + m_g * m_g - FPCONST(2.0) * m_g * tvec::dot(in, out), FPCONST(1.5));
}

Float HenyeyGreenstein::derivf(const tvec::Vec3f &in, const tvec::Vec3f &out) const {
	Float cosTheta = tvec::dot(in, out);
	return static_cast<Float>(1.0 / (4.0 * M_PI)) *
		(m_g * m_g * m_g + m_g * m_g * cosTheta - FPCONST(5.0) * m_g + FPCONST(3.0) * cosTheta) /
		std::pow(1.f + m_g * m_g - FPCONST(2.0) * m_g * cosTheta, FPCONST(2.5));
}

void HenyeyGreenstein::fderivf(const tvec::Vec3f &in, const tvec::Vec3f &out,
							Float &f, Float &derivf) const {
	Float cosTheta = tvec::dot(in, out);
	Float denominator = 1.f + m_g * m_g - FPCONST(2.0) * m_g * cosTheta;
	f = static_cast<Float>(1.0 / (4.0 * M_PI)) * (FPCONST(1.0) - m_g * m_g) /
			std::pow(denominator, FPCONST(1.5));
	derivf = static_cast<Float>(1.0 / (4.0 * M_PI)) *
			(m_g * m_g * m_g + m_g * m_g * cosTheta - FPCONST(5.0) * m_g + FPCONST(3.0) * cosTheta) /
			std::pow(denominator, FPCONST(2.5));
}

Float HenyeyGreenstein::sample(const tvec::Vec3f &in, smp::Sampler &sampler, tvec::Vec3f &out) const {

//	tvec::Vec2f sample;
//	sample.x = sampler();
//	sample.y = sampler();
	Float samplex = sampler();
	Float sampley = sampler();

    Float cosTheta;
    if (std::abs(m_g) < M_EPSILON) {
//		cosTheta = 1 - 2 * sample.x;
    	cosTheta = 1 - 2 * samplex;
	} else {
//		Float sqrTerm = (1 - m_g * m_g) / (1 - m_g + 2 * m_g * sample.x);
		Float sqrTerm = (1 - m_g * m_g) / (1 - m_g + 2 * m_g * samplex);
		cosTheta = (1 + m_g * m_g - sqrTerm * sqrTerm) / (2 * m_g);
	}

	Float sinTheta = std::sqrt(std::max(FPCONST(0.0), FPCONST(1.0) - cosTheta * cosTheta));
//	Float phi = static_cast<Float>(FPCONST(2.0) * M_PI) * sample.y;
	Float phi = static_cast<Float>(FPCONST(2.0) * M_PI) * sampley;
	Float sinPhi, cosPhi;
//#ifdef USE_GCC
//	std::sincos(phi, &sinPhi, &cosPhi);
//#else
	sinPhi = std::sin(phi);
	cosPhi = std::cos(phi);
//#endif
//	Float cosPhi = std::cos(phi);
//	Float sinPhi = std::sin(phi);

    tvec::Vec3f axisX, axisY;
    util::coordinateSystem(in, axisX, axisY);

    out = (sinTheta * cosPhi) * axisX + (sinTheta * sinPhi) * axisY + cosTheta * in;
    return cosTheta;
}

}	/* namespace pfunc */
