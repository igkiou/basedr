/*
 * photon.h
 *
 *  Created on: July 21, 2012
 *      Author: igkiou
 */

#ifndef PHOTON_H_
#define PHOTON_H_

#include <stdio.h>
#include <vector>

#include "constants.h"
#include "image.h"
#include "phase.h"
#include "sampler.h"
#include "tvector.h"
#include "medium.h"
#include "scene.h"

#include <omp.h>

namespace photon {

class Renderer {
public:
	Renderer(const Float maxDepth, const bool useDirect)
			: m_maxDepth(maxDepth),
			  m_useDirect(useDirect) {	}

	void scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
				const scn::Scene &scene, const med::Medium &medium,
				smp::Sampler &sampler, image::SmallImage &img, Float weight) const;

	bool scatterOnce(tvec::Vec3f &p, tvec::Vec3f &d, const scn::Scene &scene,
				const med::Medium &medium, smp::Sampler &sampler) const;

	inline Float getMoveStep(const med::Medium &medium, smp::Sampler &sampler) const {
		return -medium.getMfp() * std::log(sampler());
	}

	inline Float getWeight(const med::Medium &, const scn::Scene &scene, const int64 numPhotons) const {
		return scene.getRaySource().getLi() * scene.getFresnelTrans() / static_cast<Float>(numPhotons);
	}

	void renderImage(image::SmallImage &img0,
					const med::Medium &medium, const scn::Scene &scene,
					const int64 numPhotons) const;

//	void renderDerivImage(image::SmallImage &img0,
//						const med::Medium &medium, const med::Medium &mediumSingle,
//						const scn::Scene &scene,
//						const int64 numPhotons, const Float pfSampleLF) const;

	inline Float getMaxDepth() const {
		return m_maxDepth;
	}

	inline bool getUseDirect() const {
		return m_useDirect;
	}

	~Renderer() { }

protected:
	Float m_maxDepth;
	bool m_useDirect;
};

}	/* namespace photon */

#endif /* PHOTON_H_ */
