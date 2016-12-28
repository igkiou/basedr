/*
 * image.h
 *
 *  Created on: Jul 30, 2012
 *      Author: igkiou
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <iostream>
#include <malloc.h>

#ifdef USE_OPENEXR
#include <ImfPixelType.h>
const Imf::PixelType kEXRFloatUsed = Imf::FLOAT;
#endif

#include "constants.h"
#include "util.h"
#include "tvector.h"

namespace image {

/*
 * NOTE: Replace the image data and access operations with ipp ones, for faster
 * parallelism.
 * NOTE: Add version for reference return, or unary operators to single pixels.
 */
class SmallImage {
public:
	enum EFileFormat {
		EOpenEXR = 0,
		EPFM = 1,
		EFileFormatLength = 2,
		EFileFormatInvalid = -1
	};

	enum EByteOrder {
		EBigEndian = 0,
		ELittleEndian = 1,
		EByteOrderLength = 2,
		EByteOrderInvalid = -1
	};

	SmallImage()
			: m_xRes(0),
			  m_yRes(0),
			  m_pixels(NULL) {	}

	SmallImage(const int xRes, const int yRes)
			: m_xRes(xRes),
			  m_yRes(yRes) {
		Assert(xRes > 0 && yRes > 0);
		m_pixels = (float *) memalign(L1_CACHE_LINE_SIZE,
									m_xRes * m_yRes * sizeof(float));
	}

	void setSize(const int xRes, const int yRes) {
		Assert(m_xRes == 0 && m_yRes == 0 && m_pixels == NULL);
		Assert(xRes > 0 && yRes > 0);
		m_xRes = xRes;
		m_yRes = yRes;
		m_pixels = (float *) malloc(m_xRes * m_yRes * sizeof(float));
	}

	inline void zero() {
		memset((void *) m_pixels, 0, m_xRes * m_yRes * sizeof(float));
	}

	inline float getPixel(const int x, const int y) const {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
		return m_pixels[y * m_xRes + x];
	}

	inline void setPixel(const int x, const int y, const float val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
//		Assert(val >= 0);
		m_pixels[y * m_xRes + x] = val;
	}

	inline void addEnergy(const int x, const int y, const float val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
//		Assert(val >= 0);
		m_pixels[y * m_xRes + x] += val;
	}

//	inline const float* getImage() const {
//		return m_pixels;
//	}

	inline int getXRes() const {
		return m_xRes;
	}

	inline int getYRes() const {
		return m_yRes;
	}

#ifdef NDEBUG
	inline void copyImage(float *buffer, const int) const {
#else
	inline void copyImage(float *buffer, const int size) const {
#endif
		Assert(size == m_xRes * m_yRes);
		memcpy((void *) buffer, (void *) m_pixels,
			m_xRes * m_yRes * sizeof(float));
	}

	inline void writeToFile(const std::string& fileName,
							const EFileFormat fileFormat = EPFM) const {
//							const EByteOrder fileEndianness = ELittleEndian)

		switch (fileFormat) {
			case EOpenEXR: {
				writeOpenEXR(fileName);
				break;
			}
			case EPFM: {
				writePFM(fileName);//, fileEndianness);
				break;
			}
			case EFileFormatInvalid:
			default: {
				Assert((fileFormat == EPFM) || (fileFormat == EOpenEXR));
				break;
			}
		}
	}

	~SmallImage() {
		free(m_pixels);
	}

private:
	void writePFM(const std::string& fileName) const; //, const EByteOrder fileEndianness) const;
#ifdef USE_OPENEXR
	void writeOpenEXR(const std::string& fileName) const;
#else
	inline void writeOpenEXR(const std::string&) const {
		std::cerr << "Writing to OpenEXR format disabled during compilation." << std::endl;
	}
#endif

	int m_xRes;
	int m_yRes;
	float *m_pixels;
};

class SmallImageStack {
public:

	SmallImageStack()
			: m_xRes(0),
			  m_yRes(0),
			  m_zRes(0),
			  m_pixels(NULL) {	}

	SmallImageStack(const int xRes, const int yRes, const int zRes)
			: m_xRes(xRes),
			  m_yRes(yRes),
			  m_zRes(zRes) {
		Assert(xRes > 0 && yRes > 0 && zRes > 0);
		m_pixels = (float *) memalign(L1_CACHE_LINE_SIZE,
									m_xRes * m_yRes * m_zRes * sizeof(float));
	}

	void setSize(const int xRes, const int yRes, const int zRes) {
		Assert(m_xRes == 0 && m_yRes == 0 && m_zRes == 0 && m_pixels == NULL);
		Assert(xRes > 0 && yRes > 0 && zRes > 0);
		m_xRes = xRes;
		m_yRes = yRes;
		m_zRes = zRes;
		m_pixels = (float *) malloc(m_xRes * m_yRes * m_zRes * sizeof(float));
	}

	inline void zero() {
		memset((void *) m_pixels, 0, m_xRes * m_yRes * m_zRes * sizeof(float));
	}

	inline float getPixel(const int x, const int y, const int z) const {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
		return m_pixels[z * m_xRes * m_yRes + y * m_yRes + x];
	}

	inline void setPixel(const int x, const int y, const int z, const float val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
		Assert(val >= 0);
		m_pixels[z * m_xRes * m_yRes + y * m_yRes + x] = val;
	}

	inline void addEnergy(const int x, const int y, const int z, const float val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
		Assert(val >= 0);
		m_pixels[z * m_xRes * m_yRes + y * m_yRes + x] += val;
	}

//	inline const float* getImage() const {
//		return m_pixels;
//	}

	inline int getXRes() const {
		return m_xRes;
	}

	inline int getYRes() const {
		return m_yRes;
	}

	inline int getZRes() const {
		return m_zRes;
	}

#ifdef NDEBUG
	inline void copyImage(float *buffer, const int) const {
#else
	inline void copyImage(float *buffer, const int size) const {
#endif
		Assert(size == m_xRes * m_yRes * m_zRes);
		memcpy((void *) buffer, (void *) m_pixels,
			m_xRes * m_yRes * m_zRes * sizeof(float));
	}

	~SmallImageStack() {
		free(m_pixels);
	}

private:

	int m_xRes;
	int m_yRes;
	int m_zRes;
	float *m_pixels;
};

class SmallImageSet {
public:
	explicit SmallImageSet(const int numImages)
				: m_xRes(0),
				  m_yRes(0),
				  m_numImages(numImages) {
		m_images = new SmallImage[m_numImages];
	}

	SmallImageSet(const int xRes, const int yRes, const int numImages)
				: m_xRes(xRes),
				  m_yRes(yRes),
				  m_numImages(numImages) {
		m_images = new SmallImage[m_numImages];
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].setSize(m_xRes, m_yRes);
		}
	}

	inline void setSize(const int xRes, const int yRes) {
		Assert(m_xRes == 0 && m_yRes == 0);
		m_xRes = xRes;
		m_yRes = yRes;
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].setSize(m_xRes, m_yRes);
		}
	}

	inline void zero() {
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].zero();
		}
	}

	inline SmallImage &operator[](int i) {
		return m_images[i];
	}

	inline const SmallImage &operator[](int i) const {
		return m_images[i];
	}

	void mergeImages(SmallImage &mergedImage) const;

	~SmallImageSet();

private:
	int m_xRes;
	int m_yRes;
	int m_numImages;
	SmallImage *m_images;
};

class SmallImageStackSet {
public:
	explicit SmallImageStackSet(const int numImages)
				: m_xRes(0),
				  m_yRes(0),
				  m_zRes(0),
				  m_numImages(numImages) {
		m_images = new SmallImageStack[m_numImages];
	}

	SmallImageStackSet(const int xRes, const int yRes, const int zRes, const int numImages)
				: m_xRes(xRes),
				  m_yRes(yRes),
				  m_zRes(zRes),
				  m_numImages(numImages) {
		m_images = new SmallImageStack[m_numImages];
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].setSize(m_xRes, m_yRes, m_zRes);
		}
	}

	inline void setSize(const int xRes, const int yRes, const int zRes) {
		Assert(m_xRes == 0 && m_yRes == 0);
		m_xRes = xRes;
		m_yRes = yRes;
		m_zRes = zRes;
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].setSize(m_xRes, m_yRes, m_zRes);
		}
	}

	inline void zero() {
		for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
			m_images[iterImage].zero();
		}
	}

	inline SmallImageStack &operator[](int i) {
		return m_images[i];
	}

	inline const SmallImageStack &operator[](int i) const {
		return m_images[i];
	}

	void mergeImages(SmallImageStack &mergedImage) const;

	~SmallImageStackSet();

private:
	int m_xRes;
	int m_yRes;
	int m_zRes;
	int m_numImages;
	SmallImageStack *m_images;
};

} /* namespace image */

#endif /* IMAGE_H_ */
