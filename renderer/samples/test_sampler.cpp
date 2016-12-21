/*
 * test_phase.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: igkiou
 */

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>
#include <limits>
#include <omp.h>
#include "constants.h"
#include "sampler.h"

const int64 kNumSamples = 100000000000;

int main() {

#ifdef USE_THREADED
	int numThreads = omp_get_num_procs();
//	int numThreads = 8;
	omp_set_num_threads(numThreads);
#else
	int numThreads = 1;
#endif

#ifndef NDEBUG
	std::cout << "numthreads = " << numThreads << std::endl;
#endif
///	double sumVals[numThreads];
//	double sumSquareVals[numThreads];

#ifdef USE_THREADED
	#pragma omp parallel
	{
#endif
	smp::Sampler sampler;
#ifdef USE_THREADED
	#pragma omp for
#endif
	for (int64 omp_i = 0; omp_i < kNumSamples; ++omp_i) {

//#ifdef USE_THREADED
//		const int id = omp_get_thread_num();
//#else
//		const int id = 0;
//#endif
//		sampler.seed();
//		std::cout << "sampler " << nw << " " << sampler() << std::endl;
//		double val = sampler[id]();
		sampler();
//		sumVals[id] += val;
//		sumSquareVals[id] += val * val;
	}
#ifdef USE_THREADED
	}
#endif
//	const double kTotalSamples = static_cast<double>(kNumSamples);
//	double mean = 0;
//	double var = 0;
//	for (int id = 0; id < numThreads; ++id) {
//		mean += sumVals[id] / kTotalSamples;
//		var += sumSquareVals[id] / (kTotalSamples - 1);
//	}
//	var = var - (kTotalSamples) / (kTotalSamples - 1) * mean * mean;
//	std::cout << "mean " << mean << " var " << var << std::endl;
	return 0;
}
