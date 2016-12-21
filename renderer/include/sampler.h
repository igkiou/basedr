/*
 * sampler.h
 *
 *  Created on: Jul 24, 2012
 *      Author: igkiou
 */

#ifndef SAMPLER_H_
#define SAMPLER_H_

#include "constants.h"

#ifdef USE_SFMT
#include "rng_sse.h"
#else
#include "rng_boost.h"
#endif

namespace smp {

#ifdef USE_SFMT
typedef rng::SSEEngineSeedType SeedType;
typedef rng::SSEEngine Sampler;
#else
typedef rng::BoostEngineSeedType SeedType;
typedef rng::BoostEngine Sampler;
#endif

//class Sampler {
//public:
//	Sampler()
//		: m_rng(),
//		  m_distrib(),
//		  m_sampler(m_rng, m_distrib) {	}
//
//	Sampler(const unsigned int seedval)
//		: m_rng(),
//		  m_distrib(),
//		  m_sampler(m_rng, m_distrib) {
//		m_rng.seed(seedval);
//	}
//
//	inline Float operator()() {
////		return static_cast<Float>(m_sampler());
//		return m_sampler();
//	}
//
//	inline void seed(const unsigned int seedval) {
//		m_rng.seed(seedval);
//	}
//
//private:
//	Engine m_rng;
//	RealDistribution m_distrib;
//	Generator m_sampler;
//};

class SamplerSet {
public:
	explicit SamplerSet(const int numSamplers);

	SamplerSet(const int numSamplers, const unsigned int seedValue);

	explicit SamplerSet(const std::vector<unsigned int>& seedVector);

	inline Sampler &operator[](int i) {
		return m_samplers[i];
	}

	inline const Sampler &operator[](int i) const {
		return m_samplers[i];
	}

	~SamplerSet();

protected:
	int m_numSamplers;
	Sampler* m_samplers;
};

}	/* namespace smp */

#endif /* SAMPLER_H_ */
