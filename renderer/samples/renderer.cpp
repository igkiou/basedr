/*
 * renderer.cpp
 *
 *  Created on: Jul 20, 2012
 *      Author: igkiou
 */

#include "renderer.h"

#define USE_THREADED
//#undef USE_THREADED

/////////////////////////
// Orthographic camera
/*
 * viewDir and viewY are the -Z and Y axes, respectively, of the camera
 * coordinate system.
 */

// side-view 1
//tvec::Vec3f viewDir(1.0f, 1.0f, -0.5f);
//tvec::Vec3f viewY(0.5f, 0.5f, 2.0f);

// side-view 2
//tvec::Vec3f viewDir(-1.0f, 1.0f, -0.5f);
//tvec::Vec3f viewY(-0.5f, 0.5f, 2.0f);

// top-view
//tvec::Vec3f viewDir(0.0f, 0.0f, -1.0f);
//tvec::Vec3f viewY(0.0f, 1.0f, 0.0f);

// front-view
//tvec::Vec3f viewDir(1.0f, 0.0f, 0.0f);
//tvec::Vec3f viewY(0.0f, 0.0f, 1.0f);

//tvec::Vec3f refX, refY, refZ;

#ifdef USE_THREADED
	int nworker = omp_get_num_procs();
#else
	int nworker = 1;
#endif

/*
 * TODO: Make use of u/int/64/ consistent.
 */
int main() {

#ifdef USE_THREADED
	omp_set_num_threads(nworker);
#endif

	const std::string ofname("temp86.exr");
	const tvec::Vec3f blockL(-0.25f, -2.75f, -2.75f);
	const tvec::Vec3f blockR(0.25f, 2.75f, 2.75f);

	const std::string phaseFuncFileName("hg0p8.txt");
	const std::string phaseFuncSingleFileName("hg0p8.txt");
	const Float g = 0.0f;
	const Float sigmaT = 0.1329f;
	const Float sigmaS = 0.1329f;
	const Float sigmaTSingle = 0.1329f;
	const Float sigmaSSingle = 0.1329f;
//	const Float epsilon = 0.5f;
	const Float hstep = 1.0f / sigmaT * 0.0001f;
	const int maxDepth = -1;
	const Float ior = 1.0f;
	const tvec::Vec3f rayOrigin(blockL.x, 0.0f, 0.0f);
	const Float rayAngle = 0.3927f;
//	const Float rayAngle = 0.0f;
	const Float rayRadius = 0.05f;
	const int mode = 0;	// 0 - normal mode, computing (I - K)^{-1} L_0
						// 1 - derivative mode, computing (I - K)^{-1} K2 (I - K)^{-1} L_0
	const bool useContinuous = false;
	const int64 nphoton = 5000000L;
//	const int64 nphoton = 20000000L;
	const Float Li = 75000.0f;

	// back-view
	tvec::Vec3f viewDir(-1.0f, 0.0f, 0.0f);
	tvec::Vec3f viewY(0.0f, 0.0f, 1.0f);

	tvec::Vec3f viewX;
	tvec::Vec2f viewPlane(10.0f, 10.0f);
	tvec::Vec2u viewReso(256, 256);
	Float viewRatio = 50.0f;

	printf("\nIoR = %.2f, ", ior);
	printf("\nMode = %d\n", mode);
	printf("Z0 = %.2f\n", rayOrigin.x);
	printf("ang = %.2f, ", rayAngle);
	printf("radius = %.2f\n", rayRadius);
	printf("# photons = %lld\n", nphoton);
	printf("max depth = %d\n", maxDepth);
	printf("Li = %.2f\n", Li);
	printf("file = %s\n\n", ofname.c_str());

	/* Initialization */
	pfunc::HenyeyGreenstein *phaseSingle = new pfunc::TabulatedPhaseFunction(phaseFuncFileName);
//	pfunc::PhaseFunction *phase = new pfunc::HGPhaseFunction(g);
//	pfunc::PhaseFunction *phaseSingle = new pfunc::TabulatedPhaseFunction(phaseFuncSingleFileName);
	pfunc::HenyeyGreenstein *phase = new pfunc::HenyeyGreenstein(g);


	const med::Medium medium(sigmaT, sigmaS, phase);
	const med::Medium mediumSingle(sigmaTSingle, sigmaSSingle, phaseSingle);
	const scn::SampleScene scene(ior, blockL, blockR, rayOrigin, rayAngle, rayRadius, viewDir, viewY, viewPlane, viewRatio);

	smp::Sampler *sampler = new smp::Sampler[nworker];
//	boost::mt19937 seed_engine(std::time(0));
	boost::mt19937 seed_engine(0);
	boost::uniform_int<unsigned int> seed_distr(0, UINT_MAX);
	boost::variate_generator<smp::Engine&, boost::uniform_int<unsigned int> > seed_generator(seed_engine, seed_distr);

	for (int nw = 0; nw < nworker; ++nw) {
		sampler[nw].seed(seed_generator());
	}

	image::SmallImage img0(viewReso.x, viewReso.y);
	image::SmallImage *img = new image::SmallImage[nworker];
	for (int nw = 0; nw < nworker; ++nw) {
		img[nw].setSize(img0.getXRes(), img0.getYRes());
		img[nw].zero();
	}

	Float weight = Li / static_cast<Float>(nphoton);

	/* This used to be rt::Buffer<rt::Vec3fBuffer<tvec::Vec3f> >. */
	std::vector<tvec::Vec3fBuffer> buf_pos(nworker);
	std::vector<tvec::Vec3fBuffer> buf_dir(nworker);
//	std::vector<tvec::Vec3fBuffer> buf_sample_pos(nworker);
//	std::vector<tvec::Vec3fBuffer> buf_sample_dir(nworker);

	/*
	 * Minimum number of scattering events that contribute a sample to
	 * derivative computation. The following value works for non-absorbing
	 * media: it is the shortest possible path. For absorbing media, we would
	 * need to set the number as a constant to be multiplied by length of path
	 * in terms of MFPs.
	 */
//	float minLength = std::abs(scene.getMediumBlock().getBlockR().x - scene.getMediumBlock().getBlockL().y) / medium.getMfp();

#ifdef USE_THREADED
	#pragma omp parallel for
#endif
	for (int64 omp_i = 0; omp_i < nphoton; ++omp_i) {

#ifdef USE_THREADED
		const int id = omp_get_thread_num();
#else
		const int id = 0;
#endif
		tvec::Vec3f pos, dir;
		if (scene.genRay(pos, dir, sampler[id])) {

			buf_pos[id].resize(0);
			buf_dir[id].resize(0);

			if (useContinuous) {
				pos += -medium.getMfp() * std::log(sampler[id]()) * dir;
			} else {
				pos += hstep * dir;
			}

			if (useContinuous) {
				photon::scatterContinuous(pos, dir, scene, medium, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
			} else {
				photon::scatterDiscrete(pos, dir, scene, medium, hstep, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
			}

			if ((mode == 1) && buf_pos[id].size()) {

				// computing a second pass
				size_t loc = static_cast<size_t>(static_cast<Float>(buf_pos[id].size()) * sampler[id]());
				if (loc >= buf_pos[id].size()) {
					loc = buf_pos.size() - 1;
				}

				pos = buf_pos[id][loc];
				dir = buf_dir[id][loc];

				bool dead = false;

				if (useContinuous) {
					if (!photon::scatterOnceContinuous(pos, dir, scene, mediumSingle, sampler[id])) {
						dead = true;
					}
					if (sampler[id]() > mediumSingle.getAlbedo()) {
						dead = true;
					}
				} else {
					bool flag;
					if (!photon::scatterOnceDiscrete(pos, dir, scene, mediumSingle, hstep, sampler[id], flag)) {
						dead = true;
					}
					if (flag && (sampler[id]() > mediumSingle.getAlbedo())) {
						dead = true;
					}
				}

				if (!dead) {
					buf_pos[id].resize(0);
					buf_dir[id].resize(0);
					if (useContinuous) {
						photon::scatterContinuous(pos, dir, scene, medium, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
					} else {
						photon::scatterDiscrete(pos, dir, scene, medium, hstep, maxDepth, sampler[id], buf_pos[id], buf_dir[id]);
					}
				}
			}

			for (size_t j = 0; j < buf_pos[id].size(); ++j) {
				if (useContinuous) {
					scene.addEnergy(img[id], buf_pos[id][j], buf_dir[id][j], weight, medium);
				} else {
					scene.addEnergy(img[id], buf_pos[id][j], buf_dir[id][j], weight * hstep * medium.getSigmaT(), medium);
				}
			}
    	}
	}

	for (unsigned int i = 0; i < img0.getYRes(); ++i) {
		for (unsigned int j = 0; j < img0.getXRes(); ++j) {
			Float val = 0;
			for (int k = 0; k < nworker; ++k) {
				val += img[k].getPixel(j, i);
			}
			img0.setPixel(j, i, val);
		}
	}
	img0.writeToFile(ofname);

	delete[] sampler;
	delete[] img;
	delete phase;
	delete phaseSingle;

	return 0;
}
