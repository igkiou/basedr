/*
 * test_source_mex.cpp
 *
 *  Created on: Dec 27, 2012
 *      Author: igkiou
 */

#include "renderer.h"
#include "mex_wrapper.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 16) {
		mexErrMsgTxt("Twenty input arguments are required.");
	}

	/* Check number of output arguments */
	if (nlhs > 5) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input medium parameters. */
	const double iorMediumd = (double) mxGetScalar(prhs[0]);
	const double *mediumDimensionsd = (double *) mxGetPr(prhs[1]);

	/* Input source parameters. */
	const double *rayOrigind = (double *) mxGetPr(prhs[4]);
	const double *rayDird = (double *) mxGetPr(prhs[5]);
	const double rayRadiusd = (double) mxGetScalar(prhs[6]);
	const double Lid = (double) mxGetScalar(prhs[7]);

	/* Input camera parameters. */
	const double *viewOrigind = (double *) mxGetPr(prhs[8]);
	const double *viewDird = (double *) mxGetPr(prhs[9]);
	const double *viewYd = (double *) mxGetPr(prhs[10]);
	const double *viewPlaned = (double *) mxGetPr(prhs[11]);
	const double *viewResod = (double *) mxGetPr(prhs[12]);

	/* Input pos dir and dist. */
	const double *iPos = (double *) mxGetPr(prhs[13]);
	const double *iDir = (double *) mxGetPr(prhs[14]);
	const double distd = (double) mxGetScalar(prhs[15]);

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
	const tvec::Vec3f rayOrigin(mediumL.x, (Float) rayOrigind[0], (Float) rayOrigind[1]);
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
	tvec::Vec3f p((Float) iPos[0], (Float) iPos[1], (Float) iPos[2]);
	tvec::Vec3f d((Float) iDir[0], (Float) iDir[1], (Float) iDir[2]);
	const Float dist = (Float) distd;

	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);

	tvec::Vec3fBuffer pbuff(0);
	tvec::Vec3fBuffer dbuff(0);
	bool ref = scene.movePhoton(p, d, dist, pbuff, dbuff);
	plhs[0] = mxCreateNumericMatrix(1, 1, mxINT8_CLASS, mxREAL); /* x */
	int *reflected = (int *) mxGetData(plhs[0]);
	reflected[0] = static_cast<int>(ref);
	plhs[1] = mxCreateNumericMatrix(3, 1, mxSINGLE_CLASS, mxREAL); /* x */
	Float *fPos = (Float *) mxGetData(plhs[1]);
	plhs[2] = mxCreateNumericMatrix(3, 1, mxSINGLE_CLASS, mxREAL); /* x */
	Float *fDir = (Float *) mxGetData(plhs[2]);
	fPos[0] = p.x;
	fPos[1] = p.y;
	fPos[2] = p.z;
	fDir[0] = d.x;
	fDir[1] = d.y;
	fDir[2] = d.z;
	plhs[3] = mxCreateNumericMatrix(3, pbuff.size(), mxSINGLE_CLASS, mxREAL); /* x */
	Float *posBuf = (Float *) mxGetData(plhs[3]);
	plhs[4] = mxCreateNumericMatrix(3, dbuff.size(), mxSINGLE_CLASS, mxREAL); /* x */
	Float *dirBuf = (Float *) mxGetData(plhs[4]);

	for (int iter = 0; iter < static_cast<int>(pbuff.size()); ++iter) {
		posBuf[iter * 3 + 0] = pbuff[iter].x;
		posBuf[iter * 3 + 1] = pbuff[iter].y;
		posBuf[iter * 3 + 2] = pbuff[iter].z;
		dirBuf[iter * 3 + 0] = dbuff[iter].x;
		dirBuf[iter * 3 + 1] = dbuff[iter].y;
		dirBuf[iter * 3 + 2] = dbuff[iter].z;
	}
}
