/*
 * phase.h
 *
 *  Created on: Jul 24, 2012
 *      Author: igkiou
 */

#ifndef PHASE_H_
#define PHASE_H_

#include <string>
#include <vector>
#include "sampler.h"
#include "tvector.h"
#include "constants.h"

namespace pfunc {

class HenyeyGreenstein {
public:
	HenyeyGreenstein(const Float g)
					: m_g(g) {	}

	~HenyeyGreenstein() { }

	Float f(const tvec::Vec3f &in, const tvec::Vec3f &out) const;
	Float derivf(const tvec::Vec3f &in, const tvec::Vec3f &out) const;
	void fderivf(const tvec::Vec3f &in, const tvec::Vec3f &out,
				Float &f, Float &derivf) const;
	Float sample(const tvec::Vec3f &in, smp::Sampler &sampler, \
						tvec::Vec3f &out)  const;

protected:
	Float m_g;
};

}	/* namespace pfunc */

#endif /* PHASE_H_ */