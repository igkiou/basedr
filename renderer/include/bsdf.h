/*
 * bsdf.h
 *
 *  Created on: Nov 29, 2015
 *      Author: igkiou
 */

#ifndef BSDF_H_
#define BSDF_H_

#include <string>
#include <vector>
#include "sampler.h"
#include "tvector.h"
#include "constants.h"

namespace bsdf {
 
class SmoothDielectric {
public:
	SmoothDielectric(Float ior1, Float ior2) :
		m_ior1(ior1),
		m_ior2(ior2) { }

	SmoothDielectric(const SmoothDielectric &in) {
		m_ior1 = in.m_ior1; m_ior2 = in.m_ior2;
	}

    //virtual Float eval(const tvec::Vec3f &in, const tvec::Vec3f &n, const tvec::Vec3f &out) const;

	void sample(const tvec::Vec3f &in, const tvec::Vec3f &n,
				smp::Sampler &sampler, tvec::Vec3f &out) const;

	inline Float getIor1() const {
		return m_ior1;
	}

	inline Float getIor2() const {
		return m_ior2;
	}

private:
	Float m_ior1;
	Float m_ior2;
};

} //namespace bsdf

#endif /* BSDF_H_ */
