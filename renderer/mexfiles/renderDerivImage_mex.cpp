/*
 * renderDerivImage_mex.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: igkiou
 */

#include "renderer.h"
#include "mex_wrapper.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 17) {
		mexErrMsgTxt("Seventeen input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 4) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input scattering parameters. */
	const double sigmaTd = (double) mxGetScalar(prhs[0]);
	const double albedod = (double) mxGetScalar(prhs[1]);
	const double gVald = (double) mxGetScalar(prhs[2]);

	/* Input medium parameters. */
	const double iorMediumd = (double) mxGetScalar(prhs[3]);
	const double *mediumDimensionsd = (double *) mxGetPr(prhs[4]);

	/* Input source parameters. */
	const double *rayOrigind = (double *) mxGetPr(prhs[5]);
	const double *rayDird = (double *) mxGetPr(prhs[6]);
	const double rayRadiusd = (double) mxGetScalar(prhs[7]);
	const double Lid = (double) mxGetScalar(prhs[8]);

	/* Input camera parameters. */
	const double *viewOrigind = (double *) mxGetPr(prhs[9]);
	const double *viewDird = (double *) mxGetPr(prhs[10]);
	const double *viewYd = (double *) mxGetPr(prhs[11]);
	const double *viewPlaned = (double *) mxGetPr(prhs[12]);
	const double *viewResod = (double *) mxGetPr(prhs[13]);

	/* Input rendering parameters. */
	const double numPhotonsd = (double) mxGetScalar(prhs[14]);
	const double maxDepthd = (double) mxGetScalar(prhs[15]);
	const double useDirectd = (double) mxGetScalar(prhs[16]);

	/*
	 * Initialize scattering parameters.
	 */
	pfunc::HenyeyGreenstein *phase = new pfunc::HenyeyGreenstein((Float) gVald);
	const Float sigmaT = (Float) sigmaTd;
	const Float albedo = (Float) albedod;

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
	const int maxDepth = (int) maxDepthd;
	const bool useDirect = (useDirectd > 0);

	const med::Medium medium(sigmaT, albedo, phase);
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);
	image::SmallImage img0(viewReso.x, viewReso.y);
	image::SmallImage dSigmaT0(viewReso.x, viewReso.y);
	image::SmallImage dAlbedo0(viewReso.x, viewReso.y);
	image::SmallImage dGVal0(viewReso.x, viewReso.y);

	photon::Renderer renderer(maxDepth, useDirect);
	renderer.renderDerivImage(img0, dSigmaT0, dAlbedo0, dGVal0, medium, scene, numPhotons);

	/* Be sure to check for x and y here. */
#ifdef USE_DOUBLE_PRECISION
	mxClassID matClassID = mxDOUBLE_CLASS;
#else
	mxClassID matClassID = mxSINGLE_CLASS;
#endif
	plhs[0] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL); /* x */
	Float *matImg = (Float *) mxGetData(plhs[0]);
	img0.copyImage(matImg, viewReso.y * viewReso.x);
	plhs[1] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL); /* x */
	Float *matDSigmaT = (Float *) mxGetData(plhs[1]);
	dSigmaT0.copyImage(matDSigmaT, viewReso.y * viewReso.x);
	plhs[2] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL); /* x */
	Float *matDAlbedo = (Float *) mxGetData(plhs[2]);
	dAlbedo0.copyImage(matDAlbedo, viewReso.y * viewReso.x);
	plhs[3] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL); /* x */
	Float *matDGVal = (Float *) mxGetData(plhs[3]);
	dGVal0.copyImage(matDGVal, viewReso.y * viewReso.x);
	delete phase;
}
