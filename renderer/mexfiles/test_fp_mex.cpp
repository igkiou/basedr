/*
 * test_fp_mex.cpp
 *
 *  Created on: Dec 4, 2015
 *      Author: igkiou
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "renderer.h"
#include "mex_wrapper.h"

//void asm_sincos(double x, double *sinx, double *cosx) {
//	__asm
//	{
//	fld QWORD PTR [x]
//	fsincos
//	fstp QWORD PTR [cosx]
//	fstp QWORD PTR [sinx]
//	}
//}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	/* Check number of input arguments */
	if (nrhs != 3) {
		mexErrMsgTxt("Three input arguments required.");
	}

	/* Check number of output arguments */
	if (nlhs > 1) {
		mexErrMsgTxt("Too many output arguments.");
	}

	/* Input medium parameters. */
	const double *x = (double *) mxGetPr(prhs[0]);
	const int size = mxGetNumberOfElements(prhs[0]);
	double *sinx = (double *) mxGetPr(prhs[1]);
	double *cosx = (double *) mxGetPr(prhs[2]);

	for (int iter = 0; iter < size; ++iter) {
		/* Listed in ascending efficiency (descending computation time). */
//		asm_sincos(x[iter], &sinx[iter], &cosx[iter]);
		sincos(x[iter], &sinx[iter], &cosx[iter]);
		sinx[iter] = sin(x[iter]); cosx[iter] = cos(x[iter]);
		sinx[iter] = std::sin(x[iter]); cosx[iter] = std::cos(x[iter]);
	}
	plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL); /* x */
	double *out = (double *) mxGetData(plhs[0]);
	out[0] = 1;
}
