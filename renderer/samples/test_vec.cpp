/*
 * test_vec.cpp
 *
 *  Created on: Jul 25, 2012
 *      Author: igkiou
 */

#include <iostream>
#include <iterator>
#include <vector>
#include <deque>
//#include <Eigen/Dense>
#include "tvector.h"
//using Eigen::Vector4d;

#define NTIMES 1000000000

int main() {

	tvec::Vec3f a(0,0,0);
	tvec::Vec3f b(2,5,1);
	tvec::Vec3f c(2,5,1);
	Float dist = 1.5;
	for (int iter = 0; iter < NTIMES; ++iter) {
		a = dist * b + c;
	}
	std::cout << "a " << a.x << " " << a.y << " " << a.z << std::endl;

//	Vector4d aE(0,0,0,0);
//	Vector4d bE(2,5,1,0);
//	Vector4d cE(2,5,1,0);
//	Float distE = 1.5;
//	for (int iter = 0; iter < NTIMES; ++iter) {
//		aE = cE + distE * bE;
//	}
//	std::cout << "aE " << aE[0] << " " << aE[1] << " " << aE[2] << std::endl;

//	tvec::Vec3f c;
//	c = tvec::cross(a,b);
//	std::cout << "c = a x b " << c.x << " " << c.y << " " << c.z << std::endl;
//	c = tvec::cross(b,a);
//	std::cout << "c = b x a " << c.x << " " << c.y << " " << c.z << std::endl;
////	std::cout << tvec::dot(a, b) << std::endl;
////	std::cout << c.x << " " << c.y << " " << c.z << std::endl;
////	a = tvec::cross(b, c);
////	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
////	a.cross(b, c);
////	std::cout << a.x << " " << a.y << " " << a.z << std::endl;
////	std::cout << b.x << " " << b.y << " " << b.z << std::endl;
////	c = FPCONST(2.0) * a - b * FPCONST(3.0);
////	std::cout << c.x << " " << c.y << " " << c.z << std::endl;
////	c.normalize();
//	std::cout << c.x << " " << c.y << " " << c.z << std::endl;
//
//	std::vector<int> firstvector;
//	std::deque<int> secondvector;
//	for (int i=1; i<=5; i++) {
//		firstvector.push_back(i);
//		secondvector.push_back(i*10);
//	}
//	copy(firstvector.begin(),firstvector.end(),back_inserter(secondvector));
//
//	std::cout << "first vec ";
//	for (unsigned int it = 0; it < firstvector.size(); ++it) {
//		std::cout << firstvector[it] << " ";
//	}
//	std::cout << std::endl;
//
//	std::cout << "second vec ";
//	for (unsigned int it = 0; it < secondvector.size(); ++it) {
//		std::cout << secondvector[it] << " ";
//	}
//	std::cout << std::endl;

	return 0;
}
