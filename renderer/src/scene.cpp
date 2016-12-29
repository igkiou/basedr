/*
 * scene.cpp
 *
 *  Created on: Aug 26, 2012
 *      Author: igkiou
 */

#include "scene.h"
#include "util.h"
#include <iostream>

namespace scn {

/*
 * Returns true if it is possible to intersect, and false otherwise.
 * disx is the smallest distance that must be traveled till intersection if outside the box. If inside the box, dis.x = 0;
 * disy is the smallest distance that must be traveled till intersection if inside the box.
 */
bool Block::intersect(const tvec::Vec3f &p, const tvec::Vec3f &d, Float &disx, Float &disy) const {
	tvec::Vec3d l(M_MIN), r(M_MAX);

	for (int i = 0; i < 3; ++i) {
		if (std::abs(d[i]) > M_EPSILON * std::max(FPCONST(1.0), std::abs(d[i]))) {
			l[i] = (m_blockL[i] - p[i])/d[i];
			r[i] = (m_blockR[i] - p[i])/d[i];
			if (l[i] > r[i]) {
				std::swap(l[i], r[i]);
			}
		} else if (m_blockL[i] > p[i] || p[i] > m_blockR[i]) {
			return false;
		}
	}

	disx = l[0];
	if (l[1] > disx) {
		disx = l[1];
	}
	if (l[2] > disx) {
		disx = l[2];
	}
	disy = r[0];
	if (disy > r[1]) {
		disy = r[1];
	}
	if (disy > r[2]) {
		disy = r[2];
	}
	if (disx < FPCONST(0.0)) {
		disx = FPCONST(0.0);
	}
	if (disx - disy > -M_EPSILON) {
		return false;
	}

	return true;
}

bool RaySource::sampleRay(tvec::Vec3f &pos, tvec::Vec3f &dir, smp::Sampler &sampler) const {
	Float xi1 = sampler();
	Float xi2 = sampler();
	Float r = m_radius * std::sqrt(xi1);
	Float theta = static_cast<Float>(2.0 * M_PI * xi2);
	Float sinTheta, cosTheta;
//#ifdef USE_GCC
//	std::sincos(theta, &sinTheta, &cosTheta);
//#else
	sinTheta = std::sin(theta);
	cosTheta = std::cos(theta);
//#endif

	pos = tvec::Vec3f(m_origin);
	pos += r * cosTheta * m_coord1 + r * sinTheta * m_coord2;
//	pos = tvec::Vec3f(m_origin.x, r * std::sin(theta) / m_dir.x + m_origin.y, r * std::cos(theta) + m_origin.z);
//	dir = tvec::Vec3f(std::cos(m_angle), std::sin(m_angle), FPCONST(0.0));
	dir = m_dir;
	return true;
}

bool Scene::genRay(tvec::Vec3f &pos, tvec::Vec3f &dir,
						smp::Sampler &sampler) const {

	if (m_source.sampleRay(pos, dir, sampler)) {
		Float dist = FPCONST(0.0);
		Assert(std::abs(dir.x) >= M_EPSILON);
		if (dir.x >= M_EPSILON) {
			dist = (m_mediumBlock.getBlockL().x - pos.x) / dir.x;
		} else if (dir.x <= -M_EPSILON) {
			dist = (m_mediumBlock.getBlockR().x - pos.x) / dir.x;
		}
		pos += dist * dir;
//		pos.x += M_EPSILON * 2;
		dir = m_refrDir;
		return true;
	} else {
		return false;
	}
}

bool Scene::genRay(tvec::Vec3f &pos, tvec::Vec3f &dir,
						smp::Sampler &sampler,
						tvec::Vec3f &possrc, tvec::Vec3f &dirsrc) const {

	if (m_source.sampleRay(pos, dir, sampler)) {
		possrc = pos;
		dirsrc = dir;
		Float dist = FPCONST(0.0);
		Assert(std::abs(dir.x) >= M_EPSILON);
		if (dir.x >= M_EPSILON) {
			dist = (m_mediumBlock.getBlockL().x - pos.x) / dir.x;
		} else if (dir.x <= -M_EPSILON) {
			dist = (m_mediumBlock.getBlockR().x - pos.x) / dir.x;
		}
		pos += dist * dir;
		dir = m_refrDir;
		return true;
	} else {
		return false;
	}
}

//bool Scene::movePhoton(tvec::Vec3f &p, tvec::Vec3f &d, Float dist, smp::Sampler &sampler) const {
//
//	tvec::Vec3f p1 = p + dist * d;
//	while (!m_mediumBlock.inside(p1)) {
//		Float disx, disy;
//		if (!m_mediumBlock.intersect(p, d, disx, disy)) {
//			return false;
//		}
//		Assert(disx < M_EPSILON && dist - disy > -M_EPSILON);
//
//		p += static_cast<Float>(disy)*d;
//		dist -= static_cast<Float>(disy);
//
//		int i = 0;
//		while (i < 3 && std::abs(m_mediumBlock.getBlockL()[i] - p[i]) > M_EPSILON &&
//				std::abs(m_mediumBlock.getBlockR()[i] - p[i]) > M_EPSILON) {
//			++i;
//		}
//		Assert(i < 3);
//
//		Float d1 = d[(i + 1) % 3]*m_mediumIor;
//		Float d2 = d[(i + 2) % 3]*m_mediumIor;
//		if (d1*d1 + d2*d2 > FPCONST(1.0)) {
//			d[i] = -d[i];
//		} else {
//			return false;
//		}
//		p1 = p + dist*d;
//	}
//
//	p = p1;
//	return true;
//}

bool Scene::movePhoton(tvec::Vec3f &p, tvec::Vec3f &d, Float dist,
					smp::Sampler &sampler) const {

	tvec::Vec3f p1 = p + dist * d;
    tvec::Vec3f d1, norm;

	while (!m_mediumBlock.inside(p1)) {
		Float disx, disy;
		if (!m_mediumBlock.intersect(p, d, disx, disy)) {
			return false;
		}
		Assert(disx < M_EPSILON && dist - disy > -M_EPSILON);

		p += static_cast<Float>(disy)*d;
		dist -= static_cast<Float>(disy);

		int i;
		norm.zero();
		for (i = 0; i < 3; ++i) {
			if (std::abs(m_mediumBlock.getBlockL()[i] - p[i]) < M_EPSILON) {
				norm[i] = -FPCONST(1.0);
				break;
			}
			else if (std::abs(m_mediumBlock.getBlockR()[i] - p[i]) < M_EPSILON) {
				norm[i] = FPCONST(1.0);
				break;
			}
		}
		Assert(i < 3);

		Float minDiff = M_MAX;
		Float minDir = FPCONST(0.0);
		tvec::Vec3f normalt;
		normalt.zero();
		int chosenI = 3;
		for (i = 0; i < 3; ++i) {
			Float diff = std::abs(m_mediumBlock.getBlockL()[i] - p[i]);
			if (diff < minDiff) {
				minDiff = diff;
				chosenI = i;
				minDir = -FPCONST(1.0);
			}
			diff = std::abs(m_mediumBlock.getBlockR()[i] - p[i]);
			if (diff < minDiff) {
				minDiff = diff;
				chosenI = i;
				minDir = FPCONST(1.0);
			}
		}
		normalt[chosenI] = minDir;
		Assert(normalt == norm);

        m_bsdf.sample(d, norm, sampler, d1);
        if (tvec::dot(d1, norm) < FPCONST(0.0)) {
			// re-enter the medium through reflection
			d = d1;
		} else {
			return false;
		}
		p1 = p + dist*d;
	}

	p = p1;
	return true;
}

void Scene::addEnergyToImage(image::SmallImage &img, const tvec::Vec3f &p, Float val) const {
	/*
	 * TODO: Changed sign for y, to make camera coordinate system right-hand.
	 */
//	Float x = tvec::dot(m_camera.getViewX(), p), y = -tvec::dot(m_camera.getViewY(), p);
	Float x = tvec::dot(m_camera.getViewX(), p) - m_camera.getViewOrigin().x;
	Float y = tvec::dot(m_camera.getViewY(), p) - m_camera.getViewOrigin().y;

	if (std::abs(x) < FPCONST(0.5)*m_camera.getViewPlane().x && std::abs(y) < FPCONST(0.5)*m_camera.getViewPlane().y) {
		x = (x / m_camera.getViewPlane().x + FPCONST(0.5)) * static_cast<Float>(img.getXRes());
		y = (y / m_camera.getViewPlane().y + FPCONST(0.5)) * static_cast<Float>(img.getXRes());

//		int ix = static_cast<int>(img.getXRes()/2) + static_cast<int>(std::floor(x));
//		int iy = static_cast<int>(img.getYRes()/2) + static_cast<int>(std::floor(y));
		int ix = static_cast<int>(std::floor(x));
		int iy = static_cast<int>(std::floor(y));

#ifdef USE_PIXEL_SHARING
		Float fx = x - std::floor(x);
		Float fy = y - std::floor(y);


		addPixel(img, ix, iy, val*(FPCONST(1.0) - fx)*(FPCONST(1.0) - fy));
		addPixel(img, ix + 1, iy, val*fx*(FPCONST(1.0) - fy));
		addPixel(img, ix, iy + 1, val*(FPCONST(1.0) - fx)*fy);
		addPixel(img, ix + 1, iy + 1, val*fx*fy);
#else
		addPixel(img, ix, iy, val);
#endif
    }
}

//void Scene::addEnergy(image::SmallImage &img,
//			const tvec::Vec3f &p, const tvec::Vec3f &d, Float distTravelled,
//			Float val, const med::Medium &medium) const {
//
//	tvec::Vec3f q;
//	Float t;
//
//#ifdef USE_WEIGHT_NORMALIZATION
//	val *=	static_cast<Float>(img.getXRes()) * static_cast<Float>(img.getYRes())
//		/ (m_camera.getViewPlane().x * m_camera.getViewPlane().y);
//#endif
//
//	if (std::abs(m_refX.x) > M_EPSILON) {
//		t = ((m_refX.x > FPCONST(0.0) ? m_mediumBlock.getBlockR().x : m_mediumBlock.getBlockL().x) - p.x)/m_refX.x;
//		q = p + t*m_refX;
//		if (m_mediumBlock.inside(q)) {
//			addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refX));
//		}
//	}
//
//	if (std::abs(m_refY.y) > M_EPSILON) {
//		t = ((m_refY.y > FPCONST(0.0) ? m_mediumBlock.getBlockR().y : m_mediumBlock.getBlockL().y) - p.y)/m_refY.y;
//		q = p + t*m_refY;
//		if (m_mediumBlock.inside(q)) {
//			addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refY));
//		}
//	}
//
//	if (std::abs(m_refZ.z) > M_EPSILON) {
//		t = ((m_refZ.z > FPCONST(0.0) ? m_mediumBlock.getBlockR().z : m_mediumBlock.getBlockL().z) - p.z)/m_refZ.z;
//		q = p + t*m_refZ;
//		if (m_mediumBlock.inside(q)) {
//			addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refZ));
//		}
//	}
//}

void Scene::addEnergy(image::SmallImage &img,
			const tvec::Vec3f &p, const tvec::Vec3f &d, Float distTravelled,
			Float val, const med::Medium &medium) const {

	tvec::Vec3f q, refD;
	Float t;
	Float val1;

#ifdef USE_WEIGHT_NORMALIZATION
	val *=	static_cast<Float>(img.getXRes()) * static_cast<Float>(img.getYRes())
		/ (m_camera.getViewPlane().x * m_camera.getViewPlane().y);
#ifdef USE_PRINTING
		std::cout << "using weight normalization " << std::endl;
#endif
#endif

	if (std::abs(m_refX.x) > M_EPSILON) {
		t = ((m_refX.x > FPCONST(0.0) ? m_mediumBlock.getBlockR().x : m_mediumBlock.getBlockL().x) - p.x)/m_refX.x;
		refD = m_refX;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refX);
			addEnergyToImage(img, q, val1);
		}
	}

	if (std::abs(m_refY.y) > M_EPSILON) {
		t = ((m_refY.y > FPCONST(0.0) ? m_mediumBlock.getBlockR().y : m_mediumBlock.getBlockL().y) - p.y)/m_refY.y;
		refD = m_refY;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refX);
			addEnergyToImage(img, q, val1);
		}
	}

	if (std::abs(m_refZ.z) > M_EPSILON) {
		t = ((m_refZ.z > FPCONST(0.0) ? m_mediumBlock.getBlockR().z : m_mediumBlock.getBlockL().z) - p.z)/m_refZ.z;
		refD = m_refZ;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, m_refX);
			addEnergyToImage(img, q, val1);
		}
	}
//	Assert((refD == m_refX) || (refD == m_refY) || (refD == m_refZ));
}

void Scene::addEnergyDeriv(image::SmallImage &img, image::SmallImage &dSigmaT,
						image::SmallImage &dAlbedo, image::SmallImage &dGVal,
						const tvec::Vec3f &p, const tvec::Vec3f &d,
						Float distTravelled, Float val, Float sumScoreSigmaT,
						Float sumScoreAlbedo, Float sumScoreGVal,
						const med::Medium &medium) const {

	tvec::Vec3f q, refD;
	Float t;
	Float val1, valDSigmaT, valDAlbedo, valDGVal;

#ifdef USE_WEIGHT_NORMALIZATION
	val *=	static_cast<Float>(img.getXRes()) * static_cast<Float>(img.getYRes())
		/ (m_camera.getViewPlane().x * m_camera.getViewPlane().y);
#ifdef USE_PRINTING
		std::cout << "using weight normalization " << std::endl;
#endif
#endif
#ifdef USE_PRINTING
	std::cout << "total = " << distTravelled << std::endl;
#endif

	if (std::abs(m_refX.x) > M_EPSILON) {
		t = ((m_refX.x > FPCONST(0.0) ? m_mediumBlock.getBlockR().x : m_mediumBlock.getBlockL().x) - p.x)/m_refX.x;
#ifdef USE_PRINTING
		std::cout << "connection = " << t << std::endl;
#endif
		refD = m_refX;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, refD);
			addEnergyToImage(img, q, val1);
			valDSigmaT = val1 * (sumScoreSigmaT - t);
			addEnergyToImage(dSigmaT, q, valDSigmaT);
			valDAlbedo = val1 * sumScoreAlbedo;
			addEnergyToImage(dAlbedo, q, valDAlbedo);
			valDGVal = val1 * (sumScoreGVal + medium.getPhaseFunction()->score(d, refD));
			addEnergyToImage(dGVal, q, valDGVal);
		}
	}

	if (std::abs(m_refY.y) > M_EPSILON) {
		t = ((m_refY.y > FPCONST(0.0) ? m_mediumBlock.getBlockR().y : m_mediumBlock.getBlockL().y) - p.y)/m_refY.y;
#ifdef USE_PRINTING
		std::cout << "connection = " << t << std::endl;
#endif
		refD = m_refY;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, refD);
			addEnergyToImage(img, q, val1);
			valDSigmaT = val1 * (sumScoreSigmaT - t);
			addEnergyToImage(dSigmaT, q, valDSigmaT);
			valDAlbedo = val1 * sumScoreAlbedo;
			addEnergyToImage(dAlbedo, q, valDAlbedo);
			valDGVal = val1 * (sumScoreGVal + medium.getPhaseFunction()->score(d, refD));
			addEnergyToImage(dGVal, q, valDGVal);
		}
	}

	if (std::abs(m_refZ.z) > M_EPSILON) {
		t = ((m_refZ.z > FPCONST(0.0) ? m_mediumBlock.getBlockR().z : m_mediumBlock.getBlockL().z) - p.z)/m_refZ.z;
#ifdef USE_PRINTING
		std::cout << "connection = " << t << std::endl;
#endif
		refD = m_refZ;
		q = p + t*refD;
		if (m_mediumBlock.inside(q)) {
			val1 = val * std::exp(-medium.getSigmaT()*t)*medium.getPhaseFunction()->f(d, refD);
			addEnergyToImage(img, q, val1);
			valDSigmaT = val1 * (sumScoreSigmaT - t);
			addEnergyToImage(dSigmaT, q, valDSigmaT);
			valDAlbedo = val1 * sumScoreAlbedo;
			addEnergyToImage(dAlbedo, q, valDAlbedo);
			valDGVal = val1 * (sumScoreGVal + medium.getPhaseFunction()->score(d, refD));
			addEnergyToImage(dGVal, q, valDGVal);
		}
	}
}

void Scene::addEnergyDirect(image::SmallImage &img,
			const tvec::Vec3f &p, const tvec::Vec3f &d, Float val,
			const med::Medium &medium) const {

	tvec::Vec3f q;
	Float t;

#ifdef USE_WEIGHT_NORMALIZATION
	val *=	static_cast<Float>(img.getXRes()) * static_cast<Float>(img.getYRes())
		/ (m_camera.getViewPlane().x * m_camera.getViewPlane().y);
#ifdef USE_PRINTING
		std::cout << "using weight normalization " << std::endl;
#endif
#endif

	if (std::abs(m_refX.x) > M_EPSILON) {
		t = ((m_refX.x > FPCONST(0.0) ? m_mediumBlock.getBlockR().x : m_mediumBlock.getBlockL().x) - p.x)/m_refX.x;
		q = p + t*m_refX;
		if (m_mediumBlock.inside(q)) {
			if (d.aproxEqual(m_refX)) {
				addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t));
			}
		}
	}

	if (std::abs(m_refY.y) > M_EPSILON) {
		t = ((m_refY.y > FPCONST(0.0) ? m_mediumBlock.getBlockR().y : m_mediumBlock.getBlockL().y) - p.y)/m_refY.y;
		q = p + t*m_refY;
		if (m_mediumBlock.inside(q)) {
			if (d.aproxEqual(m_refY)) {
				addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t));
			}
		}
	}

	if (std::abs(m_refZ.z) > M_EPSILON) {
		t = ((m_refZ.z > FPCONST(0.0) ? m_mediumBlock.getBlockR().z : m_mediumBlock.getBlockL().z) - p.z)/m_refZ.z;
		q = p + t*m_refZ;
		if (m_mediumBlock.inside(q)) {
			if (d.aproxEqual(m_refZ)) {
				addEnergyToImage(img, q, val*std::exp(-medium.getSigmaT()*t));
			}
		}
	}
}

}	/* namespace scn */
