/*
 * test_source_mex.cpp
 *
 *  Created on: Dec 15, 2015
 *      Author: igkiou
 */

#include "renderer.h"
#include "mex_wrapper.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 20) {
		mexErrMsgTxt("Twenty input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 4) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input scattering parameters. */
//	const double *rv = (double *) mxGetPr(prhs[0]);
//	const double *pdfpf = (double *) mxGetPr(prhs[1]);
//	const double sigmaTd = (double) mxGetScalar(prhs[2]);
//	const double sigmaSd = (double) mxGetScalar(prhs[3]);

	/* Input medium parameters. */
	const double iorMediumd = (double) mxGetScalar(prhs[4]);
	const double *mediumDimensionsd = (double *) mxGetPr(prhs[5]);

	/* Input source parameters. */
	const double *rayOrigind = (double *) mxGetPr(prhs[8]);
	const double *rayDird = (double *) mxGetPr(prhs[9]);
	const double rayRadiusd = (double) mxGetScalar(prhs[10]);
	const double Lid = (double) mxGetScalar(prhs[11]);

	/* Input camera parameters. */
	const double *viewOrigind = (double *) mxGetPr(prhs[12]);
	const double *viewDird = (double *) mxGetPr(prhs[13]);
	const double *viewYd = (double *) mxGetPr(prhs[14]);
	const double *viewPlaned = (double *) mxGetPr(prhs[15]);
//	const double *viewResod = (double *) mxGetPr(prhs[16]);

	/* Input rendering parameters. */
//	const double hstepd = (double) mxGetScalar(prhs[17]);
	const double numPhotonsd = (double) mxGetScalar(prhs[18]);
//	const double useContinuousd = (double) mxGetScalar(prhs[19]);

	const int size = (int) mxGetNumberOfElements(prhs[0]);
	if(mxGetNumberOfElements(prhs[1]) != (unsigned int) size) {
		mexErrMsgIdAndTxt(wrapper::ERROR_ID, "PDF array must have the same number of elements as random variable array.\n");
	}

//	/*
//	 * Initialize scattering parameters.
//	 */
//	const std::string phaseFuncFileName("../samples/hg0p8.txt");
//	pfunc::PhaseFunction *phase = new pfunc::TabulatedPhaseFunction(phaseFuncFileName);
//	const Float g = FPCONST(0.8);
//	pfunc::PhaseFunction *phase = new pfunc::HGPhaseFunction(g);

//	pfunc::PhaseFunction *phase = new pfunc::TabulatedPhaseFunction(rv, pdfpf, size);
//	const Float sigmaT = (Float) sigmaTd;
//	const Float sigmaS = (Float) sigmaSd;

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
//	const tvec::Vec2i viewReso((int) viewResod[0], (int) viewResod[1]);

	/*
	 * Initialize rendering parameters.
	 */
//	const Float hstep = (Float) hstepd;
	const int64 numPhotons = (int64) numPhotonsd;
//	const int maxDepth = -1;
//	const bool useContinuous = (useContinuousd > 0);

//	const med::Medium medium(sigmaT, sigmaS, phase);
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);
	plhs[0] = mxCreateNumericMatrix(3, numPhotons, mxDOUBLE_CLASS, mxREAL); /* x */
	Float *srcPos = (Float *) mxGetData(plhs[0]);
	plhs[1] = mxCreateNumericMatrix(3, numPhotons, mxDOUBLE_CLASS, mxREAL); /* x */
	Float *srcDir = (Float *) mxGetData(plhs[1]);
	plhs[2] = mxCreateNumericMatrix(3, numPhotons, mxDOUBLE_CLASS, mxREAL); /* x */
	Float *scnPos = (Float *) mxGetData(plhs[2]);
	plhs[3] = mxCreateNumericMatrix(3, numPhotons, mxDOUBLE_CLASS, mxREAL); /* x */
	Float *scnDir = (Float *) mxGetData(plhs[3]);

	smp::SamplerSet sampler(1);
	for (int64 omp_i = 0; omp_i < numPhotons; ++omp_i) {
		tvec::Vec3f pos;
		tvec::Vec3f dir;
		tvec::Vec3f possrc;
		tvec::Vec3f dirsrc;
		scene.genRay(pos, dir, sampler[0], possrc, dirsrc);
		srcPos[omp_i * 3 + 0] = possrc.x;
		srcPos[omp_i * 3 + 1] = possrc.y;
		srcPos[omp_i * 3 + 2] = possrc.z;
		srcDir[omp_i * 3 + 0] = dirsrc.x;
		srcDir[omp_i * 3 + 1] = dirsrc.y;
		srcDir[omp_i * 3 + 2] = dirsrc.z;
		scnPos[omp_i * 3 + 0] = pos.x;
		scnPos[omp_i * 3 + 1] = pos.y;
		scnPos[omp_i * 3 + 2] = pos.z;
		scnDir[omp_i * 3 + 0] = dir.x;
		scnDir[omp_i * 3 + 1] = dir.y;
		scnDir[omp_i * 3 + 2] = dir.z;
	}

//	delete phase;
}
