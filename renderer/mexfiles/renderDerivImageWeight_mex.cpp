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
	if (nrhs != 19) {
		mexErrMsgTxt("Nineteen input arguments are required.");
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
	const double *viewResod = (double *) mxGetPr(prhs[16]);

	/* Input rendering parameters. */
	const double numPhotonsd = (double) mxGetScalar(prhs[17]);
	const double useDirectd = (double) mxGetScalar(prhs[18]);

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
	const tvec::Vec2i viewReso((int) viewResod[0], (int) viewResod[1]);

	/*
	 * Initialize rendering parameters.
	 */
	const int64 numPhotons = (int64) numPhotonsd;
	const bool useDirect = (useDirectd > 0);
	const int maxDepth = -1;

	const med::Medium medium(sigmaT, albedo, phase);
	const med::Medium samplingMedium(samplingSigmaT, samplingAlbedo, samplingPhase);
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);
	image::SmallImage img0(viewReso.x, viewReso.y);
	image::SmallImage dSigmaT0(viewReso.x, viewReso.y);
	image::SmallImage dAlbedo0(viewReso.x, viewReso.y);
	image::SmallImage dGVal0(viewReso.x, viewReso.y);

	photon::Renderer renderer(maxDepth, useDirect);
	renderer.renderDerivImageWeight(img0, dSigmaT0, dAlbedo0, dGVal0, medium, samplingMedium, scene, numPhotons);

	/* Be sure to check for x and y here. */
	plhs[0] = mxCreateNumericMatrix(viewReso.y, viewReso.x, mxSINGLE_CLASS, mxREAL); /* x */
	float *matImg = (float *) mxGetData(plhs[0]);
	img0.copyImage(matImg, viewReso.y * viewReso.x);
	plhs[1] = mxCreateNumericMatrix(viewReso.y, viewReso.x, mxSINGLE_CLASS, mxREAL); /* x */
	float *matDSigmaT = (float *) mxGetData(plhs[1]);
	dSigmaT0.copyImage(matDSigmaT, viewReso.y * viewReso.x);
	plhs[2] = mxCreateNumericMatrix(viewReso.y, viewReso.x, mxSINGLE_CLASS, mxREAL); /* x */
	float *matDAlbedo = (float *) mxGetData(plhs[2]);
	dAlbedo0.copyImage(matDAlbedo, viewReso.y * viewReso.x);
	plhs[3] = mxCreateNumericMatrix(viewReso.y, viewReso.x, mxSINGLE_CLASS, mxREAL); /* x */
	float *matDGVal = (float *) mxGetData(plhs[3]);
	dGVal0.copyImage(matDGVal, viewReso.y * viewReso.x);
	delete phase;
}
