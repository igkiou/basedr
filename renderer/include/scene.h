/*
 * scene.h
 *
 *  Created on: Nov 26, 2015
 *      Author: igkiou
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <stdio.h>
#include <vector>

#include "constants.h"
#include "image.h"
#include "phase.h"
#include "sampler.h"
#include "tvector.h"
#include "medium.h"
#include "bsdf.h"
#include "util.h"

#include <omp.h>

namespace scn {

struct Block {

	Block(const tvec::Vec3f &blockL, const tvec::Vec3f &blockR)
		: m_blockL(blockL),
		  m_blockR(blockR) { }

	/*
	 * TODO: Maybe replace these with comparisons to FPCONST(0.0)?
	 */
	inline bool inside(const tvec::Vec3f &p) const {
		return
			p.x - m_blockL.x > -M_EPSILON &&
			m_blockR.x - p.x > -M_EPSILON &&
			p.y - m_blockL.y > -M_EPSILON &&
			m_blockR.y - p.y > -M_EPSILON &&
			p.z - m_blockL.z > -M_EPSILON &&
			m_blockR.z - p.z > -M_EPSILON;
//			p.x - m_blockL.x > M_EPSILON && m_blockR.x - p.x > M_EPSILON &&
//			p.y - m_blockL.y > M_EPSILON && m_blockR.y - p.y > M_EPSILON &&
//			p.z - m_blockL.z > M_EPSILON && m_blockR.z - p.z > M_EPSILON;
//			p.x - m_blockL.x > FPCONST(0.0) && m_blockR.x - p.x > FPCONST(0.0) &&
//			p.y - m_blockL.y > FPCONST(0.0) && m_blockR.y - p.y > FPCONST(0.0) &&
//			p.z - m_blockL.z > FPCONST(0.0) && m_blockR.z - p.z > FPCONST(0.0);
	}

	inline bool inside(const tvec::Vec3f &p, bool &xInside, bool &yInside, bool &zInside) const {
		xInside = p.x - m_blockL.x > -M_EPSILON &&
		m_blockR.x - p.x > -M_EPSILON;
		yInside = p.y - m_blockL.y > -M_EPSILON &&
		m_blockR.y - p.y > -M_EPSILON;
		zInside = p.z - m_blockL.z > -M_EPSILON &&
		m_blockR.z - p.z > -M_EPSILON;
//		xInside = p.x - m_blockL.x > M_EPSILON && m_blockR.x - p.x > M_EPSILON;
//		yInside = p.y - m_blockL.y > M_EPSILON && m_blockR.y - p.y > M_EPSILON;
//		zInside = p.z - m_blockL.z > M_EPSILON && m_blockR.z - p.z > M_EPSILON;
//		xInside = p.x - m_blockL.x > -M_EPSILON && m_blockR.x - p.x > -M_EPSILON;
//		yInside = p.y - m_blockL.y > -M_EPSILON && m_blockR.y - p.y > -M_EPSILON;
//		zInside = p.z - m_blockL.z > -M_EPSILON && m_blockR.z - p.z > -M_EPSILON;
		return xInside && yInside && zInside;
	}

//	bool intersect(const tvec::Vec3f &p, const tvec::Vec3f &d, tvec::Vec2d &dis) const;
	bool intersect(const tvec::Vec3f &p, const tvec::Vec3f &d,
				Float &disx, Float &disy) const;

	inline const tvec::Vec3f& getBlockL() const {
		return m_blockL;
	}

	inline const tvec::Vec3f& getBlockR() const {
		return m_blockR;
	}

	virtual ~Block() { }

protected:
	tvec::Vec3f m_blockL;
	tvec::Vec3f m_blockR;
};

struct Camera {

	Camera(const tvec::Vec2f &viewOrigin, const tvec::Vec3f &viewDir,
		const tvec::Vec3f &viewY, const tvec::Vec2f &viewPlane,
		const tvec::Vec2f &pathlengthRange) :
			m_viewOrigin(viewOrigin),
			m_viewDir(viewDir),
			m_viewY(viewY),
			m_viewPlane(viewPlane),
			m_pathlengthRange(pathlengthRange) {
		Assert(((m_pathlengthRange.x == -FPCONST(1.0)) && (m_pathlengthRange.y == -FPCONST(1.0))) ||
				((m_pathlengthRange.x >= 0) && (m_pathlengthRange.y >= m_pathlengthRange.x)));
		m_viewDir.normalize();
		m_viewY.normalize();
		/*
		 * TODO: Changed order of y, z, to make camera coordinate system right-hand.
		 */
//		m_viewX = tvec::cross(m_viewDir, m_viewY);
		m_viewX = tvec::cross(m_viewY, m_viewDir);
	}

	Camera(const tvec::Vec2f &viewOrigin, const tvec::Vec3f &viewDir,
		const tvec::Vec3f &viewY, const tvec::Vec2f &viewPlane) :
			Camera(viewOrigin, viewDir, viewY, viewPlane,
				tvec::Vec2f(-FPCONST(1.0), -FPCONST(1.0))) {	}

	inline const tvec::Vec2f& getViewOrigin() const {
		return m_viewOrigin;
	}

	inline const tvec::Vec3f& getViewDir() const {
		return m_viewDir;
	}

	inline const tvec::Vec3f& getViewY() const {
		return m_viewY;
	}

	inline const tvec::Vec3f& getViewX() const {
		return m_viewX;
	}

	inline const tvec::Vec2f& getViewPlane() const {
		return m_viewPlane;
	}

	inline const tvec::Vec2f& getPathlengthRange() const {
		return m_pathlengthRange;
	}

	virtual ~Camera() { }

protected:
	tvec::Vec2f m_viewOrigin;
	tvec::Vec3f m_viewDir;
	tvec::Vec3f m_viewY;
	tvec::Vec3f m_viewX;
	tvec::Vec2f m_viewPlane;
	tvec::Vec2f m_pathlengthRange;
};

struct RaySource {

	RaySource(const tvec::Vec3f &origin,
			const tvec::Vec3f &dir,
			const Float radius,
			const Float Li)
			:  m_origin(origin),
			  m_dir(dir),
			  m_coord1(),
			  m_coord2(),
			  m_radius(radius),
			  m_Li(Li) { /* m_dir(std::cos(angle), std::sin(angle), FPCONST(0.0)) */
		m_dir.normalize();
		bool xZero(std::abs(m_dir.x) < M_EPSILON);
		bool yZero(std::abs(m_dir.y) < M_EPSILON);
		bool zZero(std::abs(m_dir.z) < M_EPSILON);
		if (!(xZero || yZero || zZero)) {
			/* (z,z,-x-y) */
			m_coord1.x = m_dir.z;
			m_coord1.y = m_dir.z;
			m_coord1.z = - m_dir.x - m_dir.y;
		} else if (!(xZero || yZero)) {
			m_coord1.x = m_dir.y;
			m_coord1.y = - m_dir.x;
			m_coord1.z = FPCONST(0.0);
		} else if (!(yZero || zZero)) {
			m_coord1.x = FPCONST(0.0);
			m_coord1.y = m_dir.z;
			m_coord1.z = - m_dir.y;
		} else if (!(zZero || xZero)) {
			m_coord1.x = - m_dir.z;
			m_coord1.y = FPCONST(0.0);
			m_coord1.z = m_dir.x;
		} else if (!xZero) {
			m_coord1.x = FPCONST(0.0);
			m_coord1.y = FPCONST(1.0);
			m_coord1.z = FPCONST(0.0);
		} else if (!yZero) {
			m_coord1.x = FPCONST(0.0);
			m_coord1.y = FPCONST(0.0);
			m_coord1.z = FPCONST(1.0);
		} else if (!zZero) {
			m_coord1.x = FPCONST(1.0);
			m_coord1.y = FPCONST(0.0);
			m_coord1.z = FPCONST(0.0);
		}
		m_coord1.normalize();
		m_coord2 = tvec::cross(m_dir, m_coord1);
		m_coord2.normalize();
#ifdef USE_PRINTING
		std::cout << " dir " << m_dir.x << " " << m_dir.y << " " << m_dir.z << std::endl;
		std::cout << " c1 " << m_coord1.x << " " << m_coord1.y << " " << m_coord1.z << std::endl;
		std::cout << " c2 " << m_coord2.x << " " << m_coord2.y << " " << m_coord2.z << std::endl;
#endif
	}

	/*
	 * TODO: Inline this method.
	 */
	bool sampleRay(tvec::Vec3f &pos, tvec::Vec3f &dir, smp::Sampler &sampler) const;

	inline const tvec::Vec3f& getOrigin() const {
		return m_origin;
	}

	inline const tvec::Vec3f& getDir() const {
		return m_dir;
	}

	inline Float getRadius() const {
		return m_radius;
	}

	inline Float getLi() const {
		return m_Li;
	}

	virtual ~RaySource() { }

protected:
	tvec::Vec3f m_origin;
	tvec::Vec3f m_dir;
	tvec::Vec3f m_coord1;
	tvec::Vec3f m_coord2;
	Float m_radius;
	Float m_Li;
};

class Scene {
public:
	Scene(const Float mediumIor,
			const tvec::Vec3f &mediumBlockL,
			const tvec::Vec3f &mediumBlockR,
			const tvec::Vec3f &rayOrigin,
			const tvec::Vec3f &rayDir,
			const Float rayRadius,
			const Float Li,
			const tvec::Vec2f &viewOrigin,
			const tvec::Vec3f &viewDir,
			const tvec::Vec3f &viewY,
			const tvec::Vec2f &viewPlane,
			const tvec::Vec2f &pathlengthRange) :
				m_mediumIor(mediumIor),
				m_fresnelTrans(FPCONST(1.0)),
				m_refrDir(FPCONST(1.0), FPCONST(0.0), FPCONST(0.0)),
				m_mediumBlock(mediumBlockL, mediumBlockR),
				m_source(rayOrigin, rayDir, rayRadius, Li),
				m_camera(viewOrigin, viewDir, viewY, viewPlane, pathlengthRange),
				m_bsdf(FPCONST(1.0), mediumIor) {

		Assert(((std::abs(m_source.getOrigin().x - m_mediumBlock.getBlockL().x) < M_EPSILON) && (m_source.getDir().x > FPCONST(0.0)))||
				((std::abs(m_source.getOrigin().x - m_mediumBlock.getBlockR().x) < M_EPSILON) && (m_source.getDir().x < FPCONST(0.0))));

		m_refX.zero(); m_refY.zero(); m_refZ.zero();
		if (std::abs(m_camera.getViewDir().x) > M_EPSILON) {
			m_refX.y = -m_camera.getViewDir().y/m_mediumIor;
			m_refX.z = -m_camera.getViewDir().z/m_mediumIor;
			m_refX.x = std::sqrt(FPCONST(1.0) - m_refX.y*m_refX.y - m_refX.z*m_refX.z);
			if (m_camera.getViewDir().x > FPCONST(0.0)) {
				m_refX.x *= -FPCONST(1.0);
			}
		}
		if (std::abs(m_camera.getViewDir().y) > M_EPSILON) {
			m_refY.z = -m_camera.getViewDir().z/m_mediumIor;
			m_refY.x = -m_camera.getViewDir().x/m_mediumIor;
			m_refY.y = std::sqrt(FPCONST(1.0) - m_refY.z*m_refY.z - m_refY.x*m_refY.x);
			if (m_camera.getViewDir().y > FPCONST(0.0)) {
				m_refY.y *= -FPCONST(1.0);
			}
		}
		if (std::abs(m_camera.getViewDir().z) > M_EPSILON)
		{
			m_refZ.x = -m_camera.getViewDir().x/m_mediumIor;
			m_refZ.y = -m_camera.getViewDir().y/m_mediumIor;
			m_refZ.z = std::sqrt(FPCONST(1.0) - m_refZ.x*m_refZ.x - m_refZ.y*m_refZ.y);
			if (m_camera.getViewDir().z > FPCONST(0.0)) {
				m_refZ.z *= -FPCONST(1.0);
			}
		}

		m_refrDir.y = m_source.getDir().y / m_mediumIor;
		m_refrDir.z = m_source.getDir().z / m_mediumIor;
		m_refrDir.x = std::sqrt(FPCONST(1.0) - m_refrDir.y * m_refrDir.y - m_refrDir.z * m_refrDir.z);
		if (m_source.getDir().x < FPCONST(0.0)) {
			m_refrDir.x *= -FPCONST(1.0);
		}
#ifndef USE_NO_FRESNEL
		m_fresnelTrans = FPCONST(1.0) - util::fresnelDielectric(m_source.getDir().x, m_refrDir.x, m_mediumIor);
#endif
#ifdef USE_PRINTING
		std::cout << "fresnel " << m_fresnelTrans << std::endl;
#endif
	}

	Scene(const Float mediumIor,
				const tvec::Vec3f &mediumBlockL,
				const tvec::Vec3f &mediumBlockR,
				const tvec::Vec3f &rayOrigin,
				const tvec::Vec3f &rayDir,
				const Float rayRadius,
				const Float Li,
				const tvec::Vec2f &viewOrigin,
				const tvec::Vec3f &viewDir,
				const tvec::Vec3f &viewY,
				const tvec::Vec2f &viewPlane) :
			Scene(mediumIor, mediumBlockL, mediumBlockR, rayOrigin, rayDir,
				rayRadius, Li, viewOrigin, viewDir, viewY, viewPlane,
				tvec::Vec2f(-FPCONST(1.0), -FPCONST(1.0))) { }

	/*
	 * TODO: Inline these methods in implementations.
	 */
	bool movePhoton(tvec::Vec3f &p, tvec::Vec3f &d, Float dist,
					smp::Sampler &sampler) const;
	bool genRay(tvec::Vec3f &pos, tvec::Vec3f &dir, smp::Sampler &sampler) const;
	bool genRay(tvec::Vec3f &pos, tvec::Vec3f &dir, smp::Sampler &sampler,
				tvec::Vec3f &possrc, tvec::Vec3f &dirsrc) const;
	void addEnergyToImage(image::SmallImage &img, const tvec::Vec3f &p,
						Float pathlength, Float val) const;

	inline void addPixel(image::SmallImage &img, int x, int y, int z, Float val) const {
		if (x >= 0 && x < img.getXRes() && y >= 0 && y < img.getYRes() &&
			z >= 0 && z < img.getZRes()) {
			img.addEnergy(x, y, z, static_cast<Float>(val));
		}
	}

	void addEnergy(image::SmallImage &img, const tvec::Vec3f &p,
						const tvec::Vec3f &d, Float distTravelled, Float val,
						const med::Medium &medium) const;

	void addEnergyDeriv(image::SmallImage &img, image::SmallImage &dSigmaT,
						image::SmallImage &dAlbedo, image::SmallImage &dGVal,
						const tvec::Vec3f &p, const tvec::Vec3f &d,
						Float distTravelled, Float val, Float sumScoreSigmaT,
						Float sumScoreAlbedo, Float sumScoreGVal,
						const med::Medium &medium) const;

	void addEnergyDirect(image::SmallImage &img, const tvec::Vec3f &p,
						const tvec::Vec3f &d, Float val,
						const med::Medium &medium) const;

	inline Float getMediumIor() const {
		return m_mediumIor;
	}

	inline Float getFresnelTrans() const {
		return m_fresnelTrans;
	}

	inline const tvec::Vec3f& getRefrDir() const {
		return m_refrDir;
	}

	inline const Block& getMediumBlock() const {
		return m_mediumBlock;
	}

	inline const RaySource& getRaySource() const {
		return m_source;
	}

	inline const tvec::Vec3f& getRefX() const {
		return m_refX;
	}

	inline const tvec::Vec3f& getRefY() const {
		return m_refY;
	}

	inline const tvec::Vec3f& getRefZ() const {
		return m_refZ;
	}

	~Scene() { }

protected:
	Float m_mediumIor;
	Float m_fresnelTrans;
	tvec::Vec3f m_refrDir;
	Block m_mediumBlock;
	RaySource m_source;
	Camera m_camera;
	bsdf::SmoothDielectric m_bsdf;
	tvec::Vec3f m_refX;
	tvec::Vec3f m_refY;
	tvec::Vec3f m_refZ;
};

}	/* namespace scn */

#endif /* SCENE_H_ */
