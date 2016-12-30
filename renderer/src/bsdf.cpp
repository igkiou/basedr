/*
 * bsdf.cpp
 *
 *  Created on: Nov 29, 2015
 *      Author: igkiou
 */

#include "bsdf.h"
#include "util.h"
#include "constants.h"

namespace bsdf {

/*
Float BSDF::eval(const tvec::Vec3f &in, const tvec::Vec3f &n, const tvec::Vec3f &out) const {
    return 0.0f;
}
*/

void SmoothDielectric::sample(const tvec::Vec3f &in, const tvec::Vec3f &n,
                              smp::Sampler &sampler, tvec::Vec3f &out) const {

	if (std::abs(m_ior1 - m_ior2) < M_EPSILON) {
		// index matched
		out = in;
	} else {
		Float eta;
		if (dot(in, n) < -M_EPSILON) {
			// entering ior2 from ior1
			eta = m_ior2/m_ior1;
		}
		else {
			// entering ior1 from ior2
			eta = m_ior1/m_ior2;
		}

		tvec::Vec3f outT;
		if (!util::refract(in, n, eta, outT)) {
			// TIR
			out = outT;
		} else {
			tvec::Vec3f outR;
			util::reflect(in, n, outR);

			Float cosI = tvec::absDot(n, in), cosT = tvec::absDot(n, outT);
			Float fresnelR = util::fresnelDielectric(cosI, cosT, eta);

			// return either refracted or reflected direction based on the Fresnel term
			out = (sampler() < fresnelR ? outR : outT);
		}
	}
}

} //namespace bsdf
