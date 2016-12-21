/*
 * photon.cpp
 *
 *  Created on: July 21, 2012
 *      Author: igkiou
 */

#include "photon.h"
#include "util.h"
#include <iterator>
#include <iostream>

namespace photon {

bool Renderer::scatterOnce(tvec::Vec3f &p, tvec::Vec3f &d,
						const scn::Scene &scene, const med::Medium &medium,
						smp::Sampler &sampler) const {

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f d1;
		medium.getPhaseFunction()->sample(d, sampler, d1);
		d = d1;
		Float dist = getMoveStep(medium, sampler);
		return scene.movePhoton(p, d, dist, sampler);
	} else {
		return false;
	}
}

void Renderer::scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
					const scn::Scene &scene, const med::Medium &medium,
					smp::Sampler &sampler,
					tvec::Vec3fBuffer &buf_pos, tvec::Vec3fBuffer &buf_dir, int &depth) const {

	Assert(scene.getMediumBlock().inside(p));
	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
//	if (medium.getAlbedo() > FPCONST(0.0)) {
		tvec::Vec3f pos(p), dir(d);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}
		int bufSize = buf_pos.size();
		Assert(bufSize == static_cast<int>(buf_dir.size()));
		depth = 0;
		while (m_maxDepth < 0 || depth < m_maxDepth) {
			if (depth < bufSize) {
				buf_pos[depth] = pos;
				buf_dir[depth] = dir;
			} else {
				buf_pos.push_back(pos);
				buf_dir.push_back(dir);
			}
			++depth;

			if (!scatterOnce(pos, dir, scene, medium, sampler)) {
				break;
			}
		}
	}
	Assert(depth <= static_cast<int>(buf_pos.size()));
}

void Renderer::scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
					const scn::Scene &scene, const med::Medium &medium,
					smp::Sampler &sampler,
					tvec::Vec3fBuffer &buf_pos, tvec::Vec3fBuffer &buf_dir) const {

	Assert(scene.getMediumBlock().inside(p));

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
//	if (medium.getAlbedo() > FPCONST(0.0)) {
		tvec::Vec3f pos(p), dir(d);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}
		int depth = 0;
		while (m_maxDepth < 0 || depth < m_maxDepth) {
			buf_pos.push_back(pos);
			buf_dir.push_back(dir);
			++depth;

			if (!scatterOnce(pos, dir, scene, medium, sampler)) {
				break;
			}
		}
	}
}

//void Renderer::scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
//					const scn::Scene &scene, const med::Medium &medium,
//					smp::Sampler &sampler,
//					tvec::Vec3fBuffer &buf_pos, tvec::Vec3fBuffer &buf_dir,
//					image::SmallImage &img, Float weight) const {
//
//	Assert(scene.getMediumBlock().inside(p));
//
//	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
////	if (medium.getAlbedo() > FPCONST(0.0)) {
//		tvec::Vec3f pos(p), dir(d);
//		Float dist = getMoveStep(medium, sampler);
//		if (!scene.movePhoton(pos, dir, dist)) {
//			return;
//		}
//		int depth = 0;
//		while (m_maxDepth < 0 || depth < m_maxDepth) {
//			buf_pos.push_back(pos);
//			buf_dir.push_back(dir);
//			++depth;
//			scene.addEnergy(img, pos, dir, weight, medium);
//
//			if (!scatterOnce(pos, dir, scene, medium, sampler)) {
//				break;
//			}
//		}
//	}
//}

void Renderer::scatter(const tvec::Vec3f &p, const tvec::Vec3f &d,
					const scn::Scene &scene, const med::Medium &medium,
					smp::Sampler &sampler, image::SmallImage &img, Float weight) const {

	Assert(scene.getMediumBlock().inside(p));

	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f pos(p), dir(d);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}
		int depth = 0;
		while (m_maxDepth < 0 || depth < m_maxDepth) {
			++depth;
			scene.addEnergy(img, pos, dir, weight, medium);

			if (!scatterOnce(pos, dir, scene, medium, sampler)) {
				break;
			}
		}
	}
}

void Renderer::scatter2(const tvec::Vec3f &p, const tvec::Vec3f &d,
		const scn::Scene &scene, const med::Medium &medium, const med::Medium &mediumSingle,\
		smp::Sampler &sampler, image::SmallImage &img, Float weight, const Float pfSampleLF) const {

	Assert(scene.getMediumBlock().inside(p));
	if ((medium.getAlbedo() > FPCONST(0.0)) && ((medium.getAlbedo() >= FPCONST(1.0)) || (sampler() < medium.getAlbedo()))) {
		tvec::Vec3f pos(p), dir(d);
		Float dist = getMoveStep(medium, sampler);
		if (!scene.movePhoton(pos, dir, dist, sampler)) {
			return;
		}
		int depth = 0;
		while (m_maxDepth < 0 || depth < m_maxDepth) {
			++depth;
			if (sampler() < pfSampleLF) {
				tvec::Vec3f pos2(pos), dir2(dir);
				if (scatterOnce(pos2, dir2, scene, mediumSingle, sampler)) {
					scatter(pos2, dir2, scene, medium, sampler, img, weight);
				}
			}
			if (!scatterOnce(pos, dir, scene, medium, sampler)) {
				break;
			}
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
#ifndef NDEBUG
	std::cout << "numthreads = " << numThreads << std::endl;
#endif
#else
	int numThreads = 1;
#endif

	smp::SamplerSet sampler(numThreads);

	image::SmallImageSet img(img0.getXRes(), img0.getYRes(), numThreads);
	img.zero();

	Float weight = getWeight(medium, scene, numPhotons);
	Float Li = scene.getRaySource().getLi();
#ifndef NDEBUG
	std::cout << "weight " << weight << " Li " << Li << std::endl;
#endif
#ifdef USE_LIST
	std::vector<tvec::Vec3fBuffer> buf_pos(numThreads);
	std::vector<tvec::Vec3fBuffer> buf_dir(numThreads);
	printf("using list\n");
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

#ifdef USE_LIST
			buf_pos[id].resize(0);
			buf_dir[id].resize(0);
#endif
			/*
			 * TODO: Direct energy calculation. Only works right if only one contribution is made from each source location.
			 */
			if (m_useDirect) {
				scene.addEnergyDirect(img[id], pos, dir, Li * scene.getFresnelTrans(), medium);
			}
//			pos += getMoveStep(medium, sampler[id]) * dir;
////			if (useContinuous) {
////				pos += -medium.getMfp() * std::log(sampler[id]()) * dir;
////			} else {
////				pos += hstep * dir;
////			}
#ifdef USE_LIST
			scatter(pos, dir, scene, medium, sampler[id], buf_pos[id], buf_dir[id]);
#else
			scatter(pos, dir, scene, medium, sampler[id], img[id], weight);
#endif
////			if (useContinuous) {
////				photon::scatterContinuous(pos, dir, scene, medium, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
////			} else {
////				photon::scatterDiscrete(pos, dir, scene, medium, hstep, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
////			}

#ifdef USE_LIST
			for (size_t j = 0; j < buf_pos[id].size(); ++j) {
				scene.addEnergy(img[id], buf_pos[id][j], buf_dir[id][j], weight, medium);
////				if (useContinuous) {
////					scene.addEnergy(img[id], buf_pos[id][j], buf_dir[id][j], weight, medium);
////				} else {
////					scene.addEnergy(img[id], buf_pos[id][j], buf_dir[id][j], weight * hstep * medium.getSigmaT(), medium);
////				}
			}
#endif
		}
	}

	img.mergeImages(img0);
}

///**
// * Render a deriv image without LF.
// **/
//void Renderer::renderDerivImage(image::SmallImage &img0,
//				const med::Medium &medium, const med::Medium &mediumSingle, const scn::Scene &scene,
//				const int64 numPhotons, const Float pfSampleLF) const {
//
//#ifdef USE_THREADED
//	int numThreads = omp_get_num_procs();
//	omp_set_num_threads(numThreads);
//#else
//	int numThreads = 1;
//#endif
//
//	smp::SamplerSet sampler(numThreads);
//
//	image::SmallImageSet img(img0.getXRes(), img0.getYRes(), numThreads);
//	img.zero();
//	Float weight = getWeight(medium, scene, numPhotons);
//
//#ifdef USE_LIST_DERIV
//	/*
//	 * Use proper reserve here.
//	 */
//	std::vector<tvec::Vec3fBuffer> buf_pos(numThreads);
//	std::vector<tvec::Vec3fBuffer> buf_dir(numThreads);
//	printf("using list\n");
//#endif
//
//#ifdef USE_THREADED
//	#pragma omp parallel for
//#endif
//	for (int64 omp_i = 0; omp_i < numPhotons; ++omp_i) {
//
//#ifdef USE_THREADED
//		const int id = omp_get_thread_num();
//#else
//		const int id = 0;
//#endif
//		tvec::Vec3f pos, dir;
//		if (scene.genRay(pos, dir, sampler[id])) {
//
//#ifdef USE_LIST_DERIV
//			buf_pos[id].resize(0);
//			buf_dir[id].resize(0);
//			scatter(pos, dir, scene, medium, sampler[id], buf_pos[id], buf_dir[id]);
//			if (buf_pos[id].size()) {
//				int numSamples = static_cast<int>(std::floor(pfSampleLF * static_cast<Float>(buf_pos[id].size())));
//				for (int iter = 0; iter < numSamples; ++iter) {
//					int loc = static_cast<int>(std::floor(sampler[id]() * static_cast<Float>(buf_pos[id].size())));
//					if (loc >= buf_pos[id].size()) {
//						loc = buf_pos.size() - 1;
//					}
//					pos = buf_pos[id][loc];
//					dir = buf_dir[id][loc];
//					if (scatterOnce(pos, dir, scene, mediumSingle, sampler[id])) {
//						scatter(pos, dir, scene, medium, sampler[id], img[id], weight);
//					}
//				}
//			}
//#else
//			scatter2(pos, dir, scene, medium, mediumSingle, sampler[id], img[id], weight, pfSampleLF);
//#endif
//
//		}
//	}
//
//	img.mergeImages(img0);
//}

}	/* namespace photon */