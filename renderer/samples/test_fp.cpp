/*
 * test_fp.cpp
 *
 *  Created on: Jan 4, 2013
 *      Author: igkiou
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "tvector.h"
#include "constants.h"

//void asm_sincos(double x, double *sinx, double *cosx) {
//	__asm
//	{
//	fld QWORD PTR [x]
//	fsincos
//	fstp QWORD PTR [cosx]
//	fstp QWORD PTR [sinx]
//	}
//}

int main() {

	double x, cosx, sinx;
//	x = M_PI / 3.0f;
	double step = 0.000000001;
	for (x = 0.0; x < M_PI; x += step) {
//		asm_sincos(x, &sinx, &cosx);
		sincos(x, &sinx, &cosx);
		sinx = sin(x); cosx = cos(x);
		sinx = std::sin(x); cosx = std::cos(x);
		printf("x = %e, sin = %e, cos = %e\n", x, sinx, cosx);
	}
	return 0;
}
