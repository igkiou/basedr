/*
 * renderDerivImage_mex.cpp
 *
 *  Created on: Sep 3, 2012
 *      Author: igkiou
 */

#include "renderer.h"
#include "mex_wrapper.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 26) {
		mexErrMsgTxt("Twenty-six input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input scattering parameters. */
	const double *rv = (double *) mxGetPr(prhs[0]);
	const double *pdfpf = (double *) mxGetPr(prhs[1]);
	const double sigmaTd = (double) mxGetScalar(prhs[2]);
	const double sigmaSd = (double) mxGetScalar(prhs[3]);

	const double *rvSingle = (double *) mxGetPr(prhs[4]);
	const double *pdfpfSingle = (double *) mxGetPr(prhs[5]);
	const double sigmaTSingled = (double) mxGetScalar(prhs[6]);
	const double sigmaSSingled = (double) mxGetScalar(prhs[7]);

	/* Input medium parameters. */
	const double iorMediumd = (double) mxGetScalar(prhs[8]);
	const double *mediumDimensionsd = (double *) mxGetPr(prhs[9]);

	/* Input source parameters. */
	const double *rayOrigind = (double *) mxGetPr(prhs[12]);
	const double *rayDird = (double *) mxGetPr(prhs[13]);
	const double rayRadiusd = (double) mxGetScalar(prhs[14]);
	const double Lid = (double) mxGetScalar(prhs[15]);

	/* Input camera parameters. */
	const double *viewOrigind = (double *) mxGetPr(prhs[16]);
	const double *viewDird = (double *) mxGetPr(prhs[17]);
	const double *viewYd = (double *) mxGetPr(prhs[18]);
	const double *viewPlaned = (double *) mxGetPr(prhs[19]);
	const double *viewResod = (double *) mxGetPr(prhs[20]);

	/* Input rendering parameters. */
	const double hstepd = (double) mxGetScalar(prhs[21]);
	const double numPhotonsd = (double) mxGetScalar(prhs[22]);
	const double numPhotonsLFd = (double) mxGetScalar(prhs[23]);
	const double useContinuousd = (double) mxGetScalar(prhs[24]);
//	const double useLFd = (double) mxGetScalar(prhs[24]);
	const double useHybridd = (double) mxGetScalar(prhs[25]);

	const int size = (int) mxGetNumberOfElements(prhs[0]);
	if(mxGetNumberOfElements(prhs[1]) != (unsigned int) size) {
		mexErrMsgIdAndTxt(wrapper::ERROR_ID, "PDF array must have the same number of elements as random variable array.\n");
	}

	const int sizeSingle = (int) mxGetNumberOfElements(prhs[4]);
	if(mxGetNumberOfElements(prhs[5]) != (unsigned int) sizeSingle) {
		mexErrMsgIdAndTxt(wrapper::ERROR_ID, "PDF array must have the same number of elements as random variable array.\n");
	}

//	/*
//	 * Initialize scattering parameters.
//	 */
//	const std::string phaseFuncFileName("../samples/hg0p8.txt");
//	pfunc::PhaseFunction *phase = new pfunc::TabulatedPhaseFunction(phaseFuncFileName);
//	const Float g = FPCONST(0.8);
//	pfunc::PhaseFunction *phase = new pfunc::HGPhaseFunction(g);
	pfunc::HenyeyGreenstein *phase = new pfunc::TabulatedPhaseFunction(rv, pdfpf, size);
	const Float sigmaT = (Float) sigmaTd;
	const Float sigmaS = (Float) sigmaSd;
	pfunc::HenyeyGreenstein *phaseSingle = new pfunc::TabulatedPhaseFunction(rvSingle, pdfpfSingle, sizeSingle);
	const Float sigmaTSingle = (Float) sigmaTSingled;
	const Float sigmaSSingle = (Float) sigmaSSingled;

	/*
	 * Initialize scene parameters.
	 */
	const Float iorMedium = (Float) iorMediumd;
	const tvec::Vec3f mediumL(- (Float) mediumDimensionsd[0] * FPCONST(0.5), \
							- (Float) mediumDimensionsd[1] * FPCONST(0.5), \
							- (Float) mediumDimensionsd[2] * FPCONST(0.5));
	const tvec::Vec3f mediumR((Float) mediumDimensionsd[0] * FPCONST(0.5), \
							(Float) mediumDimensionsd[1] * FPCONST(0.5), \
							(Float) mediumDimensionsd[2] * FPCONST(0.5));

	/*
	 * Initialize source parameters.
	 */
	const tvec::Vec3f rayOrigin((Float) rayOrigind[0], (Float) rayOrigind[1], (Float) rayOrigind[2]);
	const tvec::Vec3f rayDir((Float) rayDird[0], (Float) rayDird[1], (Float) rayDird[2]);
	const Float rayRadius = (Float) rayRadiusd;
	const Float Li = (Float) Lid;

	/*
	 * Initialize camera parameters.
	 */
	const tvec::Vec2f viewOrigin((Float) viewOrigind[0], (Float) viewOrigind[1]);
	const tvec::Vec3f viewDir((Float) viewDird[0], (Float) viewDird[1], (Float) viewDird[2]);
	const tvec::Vec3f viewY((Float) viewYd[0], (Float) viewYd[1], (Float) viewYd[2]);
	const tvec::Vec2f viewPlane((Float) viewPlaned[0], (Float) viewPlaned[1]);
	const tvec::Vec2i viewReso((int) viewResod[0], (int) viewResod[1]);

	/*
	 * Initialize rendering parameters.
	 */
	const Float hstep = (Float) hstepd;
	const int64 numPhotons = (int64) numPhotonsd;
	const int64 numPhotonsLF = (int64) numPhotonsLFd;
	const int maxDepth = -1;
	const bool useContinuous = (useContinuousd > 0);
//	const bool useLF = (useLFd > 0);
	const bool useLF = true;
	const bool useHybrid = (useHybridd > 0);
	const bool useDirect = false;

	const med::Medium medium(sigmaT, sigmaS, phase);
	const med::Medium mediumSingle(sigmaTSingle, sigmaSSingle, phaseSingle);
	const scn::SampleScene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);
	image::SmallImage img0(viewReso.x, viewReso.y);
	if (useContinuous) {
		photon::ContinuousRenderer renderer(maxDepth, useDirect);
		if (!useLF) {
			Float foo = FPCONST(0.1);
			renderer.renderDerivImage(img0, medium, mediumSingle, scene, numPhotons, foo);
#ifndef NDEBUG
			std::cout << "Used scatter2 continuous." << std::endl;
#endif
		} else {
			renderer.renderLFDerivImage(img0, medium, mediumSingle, scene, numPhotons, numPhotonsLF);
#ifndef NDEBUG
			std::cout << "Used LF continuous." << std::endl;
#endif
		}
	} else {
		if (!useHybrid) {
			photon::DiscreteRenderer renderer(maxDepth, useDirect, hstep);
			if (!useLF) {
				Float foo = FPCONST(0.1);
				renderer.renderDerivImage(img0, medium, mediumSingle, scene, numPhotons, foo);
#ifndef NDEBUG
				std::cout << "Used scatter2 discrete." << std::endl;
#endif
			} else {
				renderer.renderLFDerivImage(img0, medium, mediumSingle, scene, numPhotons, numPhotonsLF);
#ifndef NDEBUG
				std::cout << "Used LF discrete." << std::endl;
#endif
			}
		} else {
			photon::HybridRenderer renderer(maxDepth, useDirect, hstep);
			if (!useLF) {
				Float foo = FPCONST(0.1);
				renderer.renderDerivImage(img0, medium, mediumSingle, scene, numPhotons, foo);
#ifndef NDEBUG
				std::cout << "Used scatter2 hybrid." << std::endl;
#endif
			} else {
				renderer.renderLFDerivImage(img0, medium, mediumSingle, scene, numPhotons, numPhotonsLF);
#ifndef NDEBUG
				std::cout << "Used LF hybrid." << std::endl;
#endif
			}
		}
	}

	/* Be sure to check for x and y here. */
	plhs[0] = mxCreateNumericMatrix(viewReso.y, viewReso.x, mxSINGLE_CLASS, mxREAL); /* x */
	float *matImg = (float *) mxGetData(plhs[0]);
	img0.copyImage(matImg, viewReso.y * viewReso.x);

	delete phase;
}
