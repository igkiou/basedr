/*
 * test_img.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: igkiou
 */

#include <iostream>
#include <vector>
#include <ctime>
#include <string>

#include "image.h"
#include "sampler.h"

int nworker = 3;

int main() {

	tvec::Vec2i viewReso(500, 250);
	std::string fileName1("zero.pfm");
	std::string fileName2("inds.pfm");
	std::string fileName3("data.pfm");
	image::SmallImage *im;
	im = new image::SmallImage[nworker];
	for (int iterIm = 0; iterIm < nworker; ++iterIm) {
		im[iterIm].setSize(viewReso.x, viewReso.y);
	}
	im[0].zero();
	im[0].writeToFile(fileName1);
	Float val = 0;
	for (int iterX = 0; iterX < viewReso.x; ++iterX) {
		for (int iterY = 0; iterY < viewReso.y; ++iterY) {
			im[1].setPixel(iterX, iterY, static_cast<float>(val));
			val += 1;
		}
	}
	im[1].writeToFile(fileName2);

	smp::Sampler sampler(0);
	for (int iterX = 0; iterX < viewReso.x; ++iterX) {
		for (int iterY = 0; iterY < viewReso.y; ++iterY) {
			val = sampler();
			im[2].setPixel(iterX, iterY, static_cast<float>(val));
		}
	}
	im[2].writeToFile(fileName3);
}
