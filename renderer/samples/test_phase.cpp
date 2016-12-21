/*
 * test_phase.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: igkiou
 */

#include "phase.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <limits>
#include <string>

int main() {

	smp::Sampler sampler_hg(0);
	Float gVal = FPCONST(0.5);
	pfunc::HenyeyGreenstein hg(gVal);

	Float theta = FPCONST(0.0);
	Float step = FPCONST(0.01);
	tvec::Vec3f in(0, 0, 1);
	while (theta < M_PI) {
		tvec::Vec3f out(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
		std::cout << "theta: " << theta << " hg: " << hg.f(in, out) << std::endl;
		theta += step;
	}
	theta = M_PI;
	tvec::Vec3f out(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
	std::cout << "theta: " << theta << " hg: " << hg.f(in, out) << std::endl;
	std::cout << std::endl;

	theta = FPCONST(0.0);
	tvec::Vec3f probe0(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
	int numSamples = 1000;
	sampler_hg.seed(0);
	std::cout << "num\tsample" << std::endl;
	for (int iterSample = 0; iterSample < numSamples; ++iterSample) {
//		tvec::Vec3f probe(0, 0, 1);
		tvec::Vec3f out_hg;
		Float val = hg.sample(probe0, sampler_hg, out_hg);
		std::cout
		<< iterSample << "\t" << val
		<< std::endl;
	}
	std::cout << std::endl;

	theta = FPCONST(0.0);
	step = FPCONST(0.1);
	sampler_hg.seed(0);
	while (theta < M_PI) {
		tvec::Vec3f probe(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
//		tvec::Vec3f probe(0, 0, 1);
		tvec::Vec3f out_hg;
		hg.sample(probe, sampler_hg, out_hg);
		std::cout
		<< "theta: " << theta
		<< " hg: [" << out_hg.x << ", "  << out_hg.y << ", "  << out_hg.z << "]"
		<< std::endl;
		theta += step;
	}
	theta = M_PI;
	tvec::Vec3f probe(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
	tvec::Vec3f out_tab;
	tvec::Vec3f out_hg;
	hg.sample(probe, sampler_hg, out_hg);
	std::cout
	<< "theta: " << theta
	<< " hg: [" << out_hg.x << ", "  << out_hg.y << ", "  << out_hg.z << "]"
	<< std::endl;
	std::cout << std::endl;

	pfunc::HenyeyGreenstein *phaseFunc = NULL;
	phaseFunc = new pfunc::HenyeyGreenstein(gVal);

    theta = FPCONST(0.0);
	step = FPCONST(0.1);
	sampler_hg.seed(0);
	while (theta < M_PI) {
		tvec::Vec3f probe(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
		tvec::Vec3f out_hg;
		phaseFunc->sample(probe, sampler_hg, out_hg);
		std::cout
		<< "theta: " << theta
		<< " hg: [" << out_hg.x << ", "  << out_hg.y << ", "  << out_hg.z << "]"
		<< std::endl;
		theta += step;
	}
	theta = M_PI;
	tvec::Vec3f probe1(std::cos(FPCONST(0.0)) * std::sin(theta), std::sin(FPCONST(0.0)) * std::sin(theta), std::cos(theta));
	tvec::Vec3f out_tab1;
	tvec::Vec3f out_hg1;
	phaseFunc->sample(probe1, sampler_hg, out_hg1);
	std::cout
	<< "theta: " << theta
	<< " hg: [" << out_hg1.x << ", "  << out_hg1.y << ", "  << out_hg1.z << "]"
	<< std::endl;

	return 0;
}
