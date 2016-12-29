/*
 * renderImage_mex.cpp
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
	if (nlhs > 1) {
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
	const double *pathlengthRanged = (double *) mxGetPr(prhs[13]);
	const double *viewResod = (double *) mxGetPr(prhs[14]);

	/* Input rendering parameters. */
	const double numPhotonsd = (double) mxGetScalar(prhs[15]);
	const double maxDepthd = (double) mxGetScalar(prhs[16]);
	const double maxPathlengthd = (double) mxGetScalar(prhs[17]);
	const double useDirectd = (double) mxGetScalar(prhs[18]);

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
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius,
						Li, viewOrigin, viewDir, viewY, viewPlane, pathlengthRange);
	image::SmallImage img0(viewReso.x, viewReso.y, viewReso.z);

	photon::Renderer renderer(maxDepth, maxPathlength, useDirect);
	renderer.renderImage(img0, medium, scene, numPhotons);

	/* Be sure to check for x and y here. */
#ifdef USE_DOUBLE_PRECISION
	mxClassID matClassID = mxDOUBLE_CLASS;
#else
	mxClassID matClassID = mxSINGLE_CLASS;
#endif
	plhs[0] = mxCreateNumericMatrix(viewReso.y, viewReso.x, matClassID, mxREAL); /* x */
	Float *matImg = (Float *) mxGetData(plhs[0]);
	img0.copyImage(matImg, viewReso.y * viewReso.x);
	delete phase;
}
