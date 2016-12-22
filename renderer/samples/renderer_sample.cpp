/*
 * renderer.cpp
 *
 *  Created on: Jul 20, 2012
 *      Author: igkiou
 */

#include "renderer.h"

int main() {

	/*
	 * Initialize scattering parameters.
	 */
	const Float sigmaT = FPCONST(10.0);
	const Float albedo = FPCONST(1.0);
	const Float gVal = FPCONST(0.8);
	pfunc::HenyeyGreenstein *phase = new pfunc::HenyeyGreenstein(gVal);

	/*
	 * Initialize scene parameters.
	 */
	const Float iorMedium = FPCONST(1.0);
	const tvec::Vec3f mediumL(-FPCONST(1.25), -FPCONST(10.0), -FPCONST(10.0));
	const tvec::Vec3f mediumR(FPCONST(1.25), FPCONST(10.0), FPCONST(10.0));

	/*
	 * Initialize source parameters.
	 */
	const tvec::Vec3f rayOrigin(mediumL.x, FPCONST(0.0), FPCONST(0.0));
//	const Float rayAngle = -FPCONST(0.5236);
	const Float rayAngle = FPCONST(0.0);
	const tvec::Vec3f rayDir(std::cos(rayAngle), std::sin(rayAngle), FPCONST(0.0));
	const Float rayRadius = FPCONST(0.5);
	const Float Li = FPCONST(75000.0);

	/*
	 * Initialize camera parameters.
	 */
	const tvec::Vec2f viewOrigin(FPCONST(0.0), FPCONST(0.0));
	const tvec::Vec3f viewDir(-FPCONST(1.0), FPCONST(0.0), FPCONST(0.0));
	const tvec::Vec3f viewY(FPCONST(0.0), FPCONST(0.0), -FPCONST(1.0));
	const tvec::Vec2f viewPlane(FPCONST(20.0), FPCONST(20.0));
	const tvec::Vec2i viewReso(128, 128);

	/*
	 * Initialize rendering parameters.
	 */
	const int64 numPhotons = 5000000L;
//	const int64 numPhotons = 50000L;
//	const int64 numPhotons = 20000000L;
	const int maxDepth = -1;
	const bool useDirect = false;

	printf("\nnum photons = %ld\n", numPhotons);
	printf("max depth = %d\n", maxDepth);
	printf("Li = %.2f\n", Li);

	const med::Medium medium(sigmaT, albedo, phase);
	const scn::Scene scene(iorMedium, mediumL, mediumR, rayOrigin, rayDir, rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane);
	image::SmallImage img0(viewReso.x, viewReso.y);

	photon::Renderer renderer(maxDepth, useDirect);
	renderer.renderImage(img0, medium, scene, numPhotons);

	const std::string ofname("out.pfm");
	img0.writeToFile(ofname);

	delete phase;

	return 0;
}
