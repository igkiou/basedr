/*
 * photon.cpp
 *
 *  Created on: Nov 21, 2015
 *      Author: igkiou
 */

#include "photon.h"
#include "util.h"
#include <iterator>
#include <iostream>

namespace photon {

bool Renderer::scatterOnce(tvec::Vec3f &p, tvec::Vec3f &d, Float &dist, Float &cosTheta,
						const scn::Scene &scene, const med::Medium &medium,
						smp::Sampler &sampler) const {

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f d1;
		medium.getPhaseFunction()->sample(d, sampler, d1);
		cosTheta = tvec::dot(d, d1);
		d = d1;
		dist = getMoveStep(medium, sampler);
		return scene.movePhoton(p, d, dist, sampler);
	} else {
		dist = FPCONST(0.0);
		return false;
	}
}

void Renderer::scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
					const scn::Scene &scene, const med::Medium &medium,
					smp::Sampler &sampler, image::SmallImage &img, Float weight) const {

	Assert(scene.getMediumBlock().inside(p));

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f pos(p), dir(d);

		Float cosTheta = FPCONST(0.0);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}

		int depth = 1;
		Float totalDist = dist;
		while ((m_maxDepth < 0 || depth <= m_maxDepth) &&
				(m_maxPathlength < 0 || totalDist <= m_maxPathlength)) {
			scene.addEnergy(img, pos, dir, totalDist, weight, medium);

			if (!scatterOnce(pos, dir, dist, cosTheta, scene, medium, sampler)) {
				break;
			}
			totalDist += dist;
			++depth;
		}
	}
}

void Renderer::scatterDeriv(const tvec::Vec3f &p, const tvec::Vec3f &d,
							const scn::Scene &scene, const med::Medium &medium,
							smp::Sampler &sampler, image::SmallImage &img,
							image::SmallImage &dSigmaT, image::SmallImage &dAlbedo,
							image::SmallImage &dGVal, Float weight) const {

	Assert(scene.getMediumBlock().inside(p));

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f pos(p), dir(d);


		Float cosTheta = FPCONST(0.0);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}

		int depth = 1;
		Float totalDist = dist;
		Float sumScoreSigmaT = (FPCONST(1.0) - medium.getSigmaT() * dist);
		Float sumScoreAlbedo = FPCONST(1.0) / medium.getAlbedo();
		Float sumScoreGVal = FPCONST(0.0);
		while ((m_maxDepth < 0 || depth <= m_maxDepth) &&
				(m_maxPathlength < 0 || totalDist <= m_maxPathlength)) {
			scene.addEnergyDeriv(img, dSigmaT, dAlbedo, dGVal, pos, dir,
							totalDist, weight, sumScoreSigmaT, sumScoreAlbedo,
							sumScoreGVal, medium);

			if (!scatterOnce(pos, dir, dist, cosTheta, scene, medium, sampler)) {
				break;
			}
			totalDist += dist;
			++depth;
			sumScoreAlbedo += FPCONST(1.0) / medium.getAlbedo();
			sumScoreSigmaT += (FPCONST(1.0) - medium.getSigmaT() * dist);
			sumScoreGVal += medium.getPhaseFunction()->score(cosTheta);
		}
	}
}

bool Renderer::scatterOnceWeight(tvec::Vec3f &p, tvec::Vec3f &d, Float &weight,
						Float &dist, Float &cosTheta, const scn::Scene &scene,
						const med::Medium &medium, const med::Medium &samplingMedium,
						smp::Sampler &sampler) const {

	if ((samplingMedium.getAlbedo() > FPCONST(0.0)) && ((samplingMedium.getAlbedo() >= FPCONST(1.0))
			|| (sampler() < samplingMedium.getAlbedo()))) {
		tvec::Vec3f d1;
		samplingMedium.getPhaseFunction()->sample(d, sampler, d1);
		cosTheta = tvec::dot(d, d1);
		d = d1;
		dist = getMoveStep(samplingMedium, sampler);

		weight *= (medium.getAlbedo() / samplingMedium.getAlbedo()) *
				((medium.getSigmaT() * std::exp(-medium.getSigmaT() * dist)) /
						(samplingMedium.getSigmaT() * std::exp(-samplingMedium.getSigmaT() * dist))) *
				(medium.getPhaseFunction()->f(cosTheta) / samplingMedium.getPhaseFunction()->f(cosTheta));
		return scene.movePhoton(p, d, dist, sampler);
	} else {
		dist = FPCONST(0.0);
		return false;
	}
}

void Renderer::scatterDerivWeight(const tvec::Vec3f &p, const tvec::Vec3f &d,
							const scn::Scene &scene, const med::Medium &medium,
							const med::Medium &samplingMedium,
							smp::Sampler &sampler, image::SmallImage &img,
							image::SmallImage &dSigmaT, image::SmallImage &dAlbedo,
							image::SmallImage &dGVal, Float weight) const {

	Assert(scene.getMediumBlock().inside(p));

	if ((samplingMedium.getAlbedo() > FPCONST(0.0)) && ((samplingMedium.getAlbedo() >= FPCONST(1.0)) ||
		(sampler() < samplingMedium.getAlbedo()))) {
		tvec::Vec3f pos(p), dir(d);


		Float cosTheta = FPCONST(0.0);
		Float dist = getMoveStep(samplingMedium, sampler);
#ifdef USE_PRINTING
		std::cout << "sampled first = " << dist << std::endl;
#endif
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}

		int depth = 1;
		Float totalDist = dist;
		weight *= (medium.getAlbedo() / samplingMedium.getAlbedo()) *
				((medium.getSigmaT() * std::exp(-medium.getSigmaT() * dist)) /
					(samplingMedium.getSigmaT() * std::exp(-samplingMedium.getSigmaT() * dist)));
		Float sumScoreSigmaT = (FPCONST(1.0) - medium.getSigmaT() * dist);
		Float sumScoreAlbedo = FPCONST(1.0) / medium.getAlbedo();
		Float sumScoreGVal = FPCONST(0.0);
		while ((m_maxDepth < 0 || depth <= m_maxDepth) &&
				(m_maxPathlength < 0 || totalDist <= m_maxPathlength)) {
#ifdef USE_PRINTING
		std::cout << "total to add = " << totalDist << std::endl;
#endif
//			if (depth == 2) {
			scene.addEnergyDeriv(img, dSigmaT, dAlbedo, dGVal, pos, dir,
							totalDist, weight, sumScoreSigmaT, sumScoreAlbedo,
							sumScoreGVal, medium);
//			}

			if (!scatterOnceWeight(pos, dir, weight, dist, cosTheta, scene,
								medium, samplingMedium, sampler)) {
				break;
			}
#ifdef USE_PRINTING
			std::cout << "sampled = " << dist << std::endl;
#endif
			totalDist += dist;
			++depth;
			sumScoreAlbedo += FPCONST(1.0) / medium.getAlbedo();
			sumScoreSigmaT += (FPCONST(1.0) - medium.getSigmaT() * dist);
			sumScoreGVal += medium.getPhaseFunction()->score(cosTheta);
		}
	}
}

/**
 * Render an image.
 **/
void Renderer::renderImage(image::SmallImage &img0,
				const med::Medium &medium, const scn::Scene &scene,
				const int64 numPhotons) const {

#ifdef USE_THREADED
	int numThreads = omp_get_num_procs();
	omp_set_num_threads(numThreads);
#else
	int numThreads = 1;
#endif
#ifndef NDEBUG
	std::cout << "numthreads = " << numThreads << std::endl;
	std::cout << "numphotons = " << numPhotons << std::endl;
#endif

	smp::SamplerSet sampler(numThreads);

	image::SmallImageSet img(img0.getXRes(), img0.getYRes(), img0.getZRes(), numThreads);
	img.zero();

	Float weight = getWeight(medium, scene, numPhotons);
	Float Li = scene.getRaySource().getLi();
#ifdef USE_PRINTING
	std::cout << "weight " << weight << " Li " << Li << std::endl;
#endif

#ifdef USE_THREADED
	#pragma omp parallel for
#endif
	for (int64 omp_i = 0; omp_i < numPhotons; ++omp_i) {

#ifdef USE_THREADED
		const int id = omp_get_thread_num();
#else
		const int id = 0;
#endif
		tvec::Vec3f pos, dir;
		if (scene.genRay(pos, dir, sampler[id])) {

			/*
			 * TODO: Direct energy calculation. Only works right if only one contribution is made from each source location.
			 */
			if (m_useDirect) {
				scene.addEnergyDirect(img[id], pos, dir, Li * scene.getFresnelTrans(), medium);
			}
			scatter(pos, dir, scene, medium, sampler[id], img[id], weight);
		}
	}

	img.mergeImages(img0);
}

void Renderer::renderDerivImage(image::SmallImage &img0, image::SmallImage &dSigmaT0,
					image::SmallImage &dAlbedo0, image::SmallImage &dGVal0,
					const med::Medium &medium, const scn::Scene &scene,
					const int64 numPhotons) const {

#ifdef USE_THREADED
	int numThreads = omp_get_num_procs();
	omp_set_num_threads(numThreads);
#else
	int numThreads = 1;
#endif
#ifndef NDEBUG
	std::cout << "numthreads = " << numThreads << std::endl;
	std::cout << "numphotons = " << numPhotons << std::endl;
#endif

	smp::SamplerSet sampler(numThreads);

	image::SmallImageSet img(img0.getXRes(), img0.getYRes(), img0.getZRes(), numThreads);
	img.zero();

	image::SmallImageSet dSigmaT(dSigmaT0.getXRes(), dSigmaT0.getYRes(), dSigmaT0.getZRes(), numThreads);
	dSigmaT.zero();

	image::SmallImageSet dAlbedo(dAlbedo0.getXRes(), dAlbedo0.getYRes(), dAlbedo0.getZRes(), numThreads);
	dAlbedo.zero();

	image::SmallImageSet dGVal(dGVal0.getXRes(), dGVal0.getYRes(), dGVal0.getZRes(), numThreads);
	dGVal.zero();

	Float weight = getWeight(medium, scene, numPhotons);
	Float Li = scene.getRaySource().getLi();
#ifdef USE_PRINTING
	std::cout << "weight " << weight << " Li " << Li << std::endl;
#endif

#ifdef USE_THREADED
	#pragma omp parallel for
#endif
	for (int64 omp_i = 0; omp_i < numPhotons; ++omp_i) {

#ifdef USE_THREADED
		const int id = omp_get_thread_num();
#else
		const int id = 0;
#endif
		tvec::Vec3f pos, dir;
		if (scene.genRay(pos, dir, sampler[id])) {

			/*
			 * TODO: Direct energy calculation. Only works right if only one contribution is made from each source location.
			 */
			if (m_useDirect) {
				scene.addEnergyDirect(img[id], pos, dir, Li * scene.getFresnelTrans(), medium);
			}
			scatterDeriv(pos, dir, scene, medium, sampler[id], img[id],
						dSigmaT[id], dAlbedo[id], dGVal[id], weight);
		}
	}

	img.mergeImages(img0);
	dSigmaT.mergeImages(dSigmaT0);
	dAlbedo.mergeImages(dAlbedo0);
	dGVal.mergeImages(dGVal0);
}

void Renderer::renderDerivImageWeight(image::SmallImage &img0, image::SmallImage &dSigmaT0,
					image::SmallImage &dAlbedo0, image::SmallImage &dGVal0,
					const med::Medium &medium, const med::Medium &samplingMedium,
					const scn::Scene &scene, const int64 numPhotons) const {

#ifdef USE_THREADED
	int numThreads = omp_get_num_procs();
	omp_set_num_threads(numThreads);
#else
	int numThreads = 1;
#endif
#ifndef NDEBUG
	std::cout << "numthreads = " << numThreads << std::endl;
	std::cout << "numphotons = " << numPhotons << std::endl;
#endif

	smp::SamplerSet sampler(numThreads);

	image::SmallImageSet img(img0.getXRes(), img0.getYRes(), img0.getZRes(), numThreads);
	img.zero();

	image::SmallImageSet dSigmaT(dSigmaT0.getXRes(), dSigmaT0.getYRes(), dSigmaT0.getZRes(), numThreads);
	dSigmaT.zero();

	image::SmallImageSet dAlbedo(dAlbedo0.getXRes(), dAlbedo0.getYRes(), dAlbedo0.getZRes(), numThreads);
	dAlbedo.zero();

	image::SmallImageSet dGVal(dGVal0.getXRes(), dGVal0.getYRes(), dGVal0.getZRes(), numThreads);
	dGVal.zero();

	Float weight = getWeight(medium, scene, numPhotons);
	Float Li = scene.getRaySource().getLi();
#ifdef USE_PRINTING
	std::cout << "weight " << weight << " Li " << Li << std::endl;
#endif

#ifdef USE_THREADED
	#pragma omp parallel for
#endif
	for (int64 omp_i = 0; omp_i < numPhotons; ++omp_i) {

#ifdef USE_THREADED
		const int id = omp_get_thread_num();
#else
		const int id = 0;
#endif
		tvec::Vec3f pos, dir;
		if (scene.genRay(pos, dir, sampler[id])) {

			/*
			 * TODO: Direct energy calculation. Only works right if only one contribution is made from each source location.
			 */
			if (m_useDirect) {
				scene.addEnergyDirect(img[id], pos, dir, Li * scene.getFresnelTrans(), medium);
			}
			scatterDerivWeight(pos, dir, scene, medium, samplingMedium, sampler[id], img[id],
						dSigmaT[id], dAlbedo[id], dGVal[id], weight);
		}
	}

	img.mergeImages(img0);
	dSigmaT.mergeImages(dSigmaT0);
	dAlbedo.mergeImages(dAlbedo0);
	dGVal.mergeImages(dGVal0);
}

}	/* namespace photon */
