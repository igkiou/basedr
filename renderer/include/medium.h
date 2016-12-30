/*
 * medium.h
 *
 *  Created on: Nov 3, 2015
 *      Author: igkiou
 */

#ifndef MEDIUM_H_
#define MEDIUM_H_

#include <stdio.h>
#include <iostream>
#include <vector>

#include "constants.h"
#include "image.h"
#include "phase.h"
#include "sampler.h"
#include "tvector.h"
#include "util.h"

namespace med {

struct Medium {

	Medium(const Float sigmaT, const Float albedo, pfunc::HenyeyGreenstein *phase)
		: m_sigmaT(sigmaT),
		  m_albedo(albedo),
		  m_sigmaS(albedo * sigmaT),
		  m_sigmaA((1 - albedo) * sigmaT),
		  m_mfp(FPCONST(1.0) / sigmaT),
		  m_phase(phase) {
		Assert(m_sigmaA >= 0);
		Assert(m_albedo <= 1);
		if (m_sigmaT <= M_EPSILON) {
			m_sigmaT = FPCONST(0.0);
			m_mfp = FPCONST(1.0);
			m_albedo = FPCONST(0.0);
		}
	}

	inline Float getSigmaT() const {
		return m_sigmaT;
	}

	inline Float getSigmaS() const {
		return m_sigmaS;
	}

	inline Float getSigmaA() const {
		return m_sigmaA;
	}

	inline Float getMfp() const {
		return m_mfp;
	}

	inline Float getAlbedo() const {
		return m_albedo;
	}

	inline const pfunc::HenyeyGreenstein *getPhaseFunction() const {
		return m_phase;
	}

	virtual ~Medium() { }

protected:
	Float m_sigmaT;
	Float m_albedo;
	Float m_sigmaS;
	Float m_sigmaA;
	Float m_mfp;
	pfunc::HenyeyGreenstein *m_phase;
};

}	/* namespace med */

#endif /* MEDIUM_H_ */
