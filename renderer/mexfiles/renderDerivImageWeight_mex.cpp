/*
 * renderDerivImageWeight_mex.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: igkiou
 */

#include "renderer.h"
#include "mex_wrapper.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 22) {
		mexErrMsgTxt("Twenty-two input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 4) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input scattering parameters. */
	const double sigmaTd = (double) mxGetScalar(prhs[0]);
	const double albedod = (double) mxGetScalar(prhs[1]);
	const double gVald = (double) mxGetScalar(prhs[2]);

	const double samplingSigmaTd = (double) mxGetScalar(prhs[3]);
	const double samplingAlbedod = (double) mxGetScalar(prhs[4]);
	const double samplingGVald = (double) mxGetScalar(prhs[5]);

	/* Input medium parameters. */
	const double iorMediumd = (double) mxGetScalar(prhs[6]);
	const double *mediumDimensionsd = (double *) mxGetPr(prhs[7]);

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
	const double *pathlengthRanged = (double *) mxGetPr(prhs[16]);
	const double *viewResod = (double *) mxGetPr(prhs[17]);

	/* Input rendering parameters. */
	const double numPhotonsd = (double) mxGetScalar(prhs[18]);
	const double maxDepthd = (double) mxGetScalar(prhs[19]);
	const double maxPathlengthd = (double) mxGetScalar(prhs[20]);
	const double useDirectd = (double) mxGetScalar(prhs[21]);

	/*
	 * Initialize scattering parameters.
	 */
	pfunc::HenyeyGreenstein *phase = new pfunc::HenyeyGreenstein((Float) gVald);
	const Float sigmaT = (Float) sigmaTd;
	const Float albedo = (Float) albedod;
	/*
	 * Initialize sampling scattering parameters.
	 */
	pfunc::HenyeyGreenstein *samplingPhase = new pfunc::HenyeyGreenstein((Float) samplingGVald);
	const Float samplingSigmaT = (Float) samplingSigmaTd;
	const Float samplingAlbedo = (Float) samplingAlbedod;

	/*
	 * Initialize scene parameters.
	 */
	const Float iorMedium = (Float) iorMediumd;
	const tvec::Vec3f mediumL(- (Float) mediumDimensionsd[0] * FPCONST(0.5),
							- (Float) mediumDimensionsd[1] * FPCONST(0.5),
							- (Float) mediumDimensionsd[2] * FPCONST(0.5));
	const tvec::Vec3f mediumR((Float) mediumDimensionsd[0] * FPCONST(0.5),
							(Float) mediumDimensionsd[1] * FPCONST(0.5),
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
	const tvec::Vec2f pathlengthRange((Float) pathlengthRanged[0], (Float) pathlengthRanged[1]);
	const tvec::Vec3i viewReso((int) viewResod[0], (int) viewResod[1], (int) viewResod[2]);

	/*
	 * Initialize rendering parameters.
	 */
	const int64 numPhotons = (int64) numPhotonsd;
	const int maxDepth = (int) maxDepthd;
	const Float maxPathlength = (Float) maxPathlengthd;
	const bool useDirect = (useDirectd > 0);

	const med::Medium medium(sigmaT, albedo, phase);
	const med::Medium samplingMedium(samplingSigmaT, samplingAlbedo, samplingPhase);
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius,
						Li, viewOrigin, viewDir, viewY, viewPlane, pathlengthRange);
	image::SmallImage img0(viewReso.x, viewReso.y, viewReso.z);
	image::SmallImage dSigmaT0(viewReso.x, viewReso.y, viewReso.z);
	image::SmallImage dAlbedo0(viewReso.x, viewReso.y, viewReso.z);
	image::SmallImage dGVal0(viewReso.x, viewReso.y, viewReso.z);

	photon::Renderer renderer(maxDepth, maxPathlength, useDirect);
	renderer.renderDerivImageWeight(img0, dSigmaT0, dAlbedo0, dGVal0, medium, samplingMedium, scene, numPhotons);

	/* Be sure to check for x and y here. */
#ifdef USE_DOUBLE_PRECISION
	mxClassID matClassID = mxDOUBLE_CLASS;
#else
	mxClassID matClassID = mxSINGLE_CLASS;
#endif
	if (viewReso.z == 1) {
		plhs[0] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL);
		plhs[1] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL);
		plhs[2] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL);
		plhs[3] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL);
	} else {
		mwSize dims[3];
		dims[0] = viewReso.y;
		dims[1] = viewReso.x;
		dims[2] = viewReso.z;
		plhs[0] = mxCreateNumericArray(static_cast<mwSize>(3), dims, matClassID, mxREAL);
		plhs[1] = mxCreateNumericArray(static_cast<mwSize>(3), dims, matClassID, mxREAL);
		plhs[2] = mxCreateNumericArray(static_cast<mwSize>(3), dims, matClassID, mxREAL);
		plhs[3] = mxCreateNumericArray(static_cast<mwSize>(3), dims, matClassID, mxREAL);
	}
	Float *matImg = (Float *) mxGetData(plhs[0]);
	img0.copyImage(matImg, viewReso.y * viewReso.x * viewReso.z);
	Float *matDSigmaT = (Float *) mxGetData(plhs[1]);
	dSigmaT0.copyImage(matDSigmaT, viewReso.y * viewReso.x * viewReso.z);
	Float *matDAlbedo = (Float *) mxGetData(plhs[2]);
	dAlbedo0.copyImage(matDAlbedo, viewReso.y * viewReso.x * viewReso.z);
	Float *matDGVal = (Float *) mxGetData(plhs[3]);
	dGVal0.copyImage(matDGVal, viewReso.y * viewReso.x * viewReso.z);
	delete phase;
}
