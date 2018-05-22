/*
 * image.h
 *
 *  Created on: Nov 30, 2015
 *      Author: igkiou
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstdint>
#include <malloc.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>

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
template <typename T>
class Image2 {
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

	Image2() :
			m_xRes(0),
			m_yRes(0),
			m_pixels(NULL) {	}

	Image2(const int xRes, const int yRes) :
			m_xRes(xRes),
			m_yRes(yRes),
			m_pixels(NULL) {
			Assert(xRes > 0 && yRes > 0);
		m_pixels = (T *) memalign(L1_CACHE_LINE_SIZE,
									m_xRes * m_yRes * sizeof(T));
	}

	void setSize(const int xRes, const int yRes) {
		Assert(m_xRes == 0 && m_yRes == 0 && m_pixels == NULL);
		Assert(xRes > 0 && yRes > 0);
		m_xRes = xRes;
		m_yRes = yRes;
		m_pixels = (T *) malloc(m_xRes * m_yRes * sizeof(T));
	}

	inline void zero() {
		memset((void *) m_pixels, 0, m_xRes * m_yRes * sizeof(T));
	}

	inline T getPixel(const int x, const int y) const {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
		return m_pixels[y * m_xRes + x];
	}

	inline void setPixel(const int x, const int y, const T val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
//		Assert(val >= 0);
		m_pixels[y * m_xRes + x] = val;
	}

	inline void addEnergy(const int x, const int y, const T val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes);
//		Assert(val >= 0);
		m_pixels[y * m_xRes + x] += val;
	}

//	inline const T* getImage() const {
//		return m_pixels;
//	}

	inline int getXRes() const {
		return m_xRes;
	}

	inline int getYRes() const {
		return m_yRes;
	}

#ifdef NDEBUG
	inline void copyImage(T *buffer, const int) const {
#else
	inline void copyImage(T *buffer, const int size) const {
#endif
		Assert(size == m_xRes * m_yRes);
		memcpy((void *) buffer, (void *) m_pixels,
			m_xRes * m_yRes * sizeof(T));
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

	~Image2() {
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
	T *m_pixels;
};

template <typename T>
class Image3 {
public:

	Image3() :
			m_xRes(0),
			m_yRes(0),
			m_zRes(0),
			m_pixels(NULL) {	}

	Image3(const int xRes, const int yRes, const int zRes) :
			m_xRes(xRes),
			m_yRes(yRes),
			m_zRes(zRes),
			m_pixels(NULL) {
		Assert(xRes > 0 && yRes > 0 && zRes > 0);
		m_pixels = (T *) memalign(L1_CACHE_LINE_SIZE,
									m_xRes * m_yRes * m_zRes * sizeof(T));
	}

	void setSize(const int xRes, const int yRes, const int zRes) {
		Assert(m_xRes == 0 && m_yRes == 0 && m_zRes == 0 && m_pixels == NULL);
		Assert(xRes > 0 && yRes > 0 && zRes > 0);
		m_xRes = xRes;
		m_yRes = yRes;
		m_zRes = zRes;
		m_pixels = (T *) malloc(m_xRes * m_yRes * m_zRes * sizeof(T));
	}

	inline void zero() {
		memset((void *) m_pixels, 0, m_xRes * m_yRes * m_zRes * sizeof(T));
	}

	inline T getPixel(const int x, const int y, const int z) const {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
		return m_pixels[z * m_xRes * m_yRes + y * m_yRes + x];
	}

	inline void setPixel(const int x, const int y, const int z, const T val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
//		Assert(val >= 0);
		m_pixels[z * m_xRes * m_yRes + y * m_yRes + x] = val;
	}

	inline void addEnergy(const int x, const int y, const int z, const T val) {
		Assert(x >= 0 && x < m_xRes && y >= 0 && y < m_yRes && z >= 0 && z < m_zRes);
//		Assert(val >= 0);
		m_pixels[z * m_xRes * m_yRes + y * m_yRes + x] += val;
	}

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
	inline void copyImage(T *buffer, const int) const {
#else
	inline void copyImage(T *buffer, const int size) const {
#endif
		Assert(size == m_xRes * m_yRes * m_zRes);
		memcpy((void *) buffer, (void *) m_pixels,
			m_xRes * m_yRes * m_zRes * sizeof(T));
	}

	~Image3() {
		free(m_pixels);
	}

private:

	int m_xRes;
	int m_yRes;
	int m_zRes;
	T *m_pixels;
};

template <typename T>
class Image2Set {
public:
	explicit Image2Set(const int numImages)
				: m_xRes(0),
				  m_yRes(0),
				  m_numImages(numImages) {
		m_images = new Image2<T>[m_numImages];
	}

	Image2Set(const int xRes, const int yRes, const int numImages)
				: m_xRes(xRes),
				  m_yRes(yRes),
				  m_numImages(numImages) {
		m_images = new Image2<T>[m_numImages];
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

	inline Image2<T> &operator[](int i) {
		return m_images[i];
	}

	inline const Image2<T> &operator[](int i) const {
		return m_images[i];
	}

	inline void mergeImages(Image2<T> &mergedImage) const {
		Assert(mergedImage.getXRes() == m_xRes && mergedImage.getYRes() == m_yRes);
		for (int i = 0; i < m_yRes; ++i) {
			for (int j = 0; j < m_xRes; ++j) {
				T val = 0.0;
				for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
					val += m_images[iterImage].getPixel(j, i);
				}
				mergedImage.setPixel(j, i, val);
			}
		}
	}

	~Image2Set() {
		delete[] m_images;
	}

private:
	int m_xRes;
	int m_yRes;
	int m_numImages;
	Image2<T> *m_images;
};


template <typename T>
class Image3Set {
public:
	explicit Image3Set(const int numImages) :
			m_xRes(0),
			m_yRes(0),
			m_zRes(0),
			m_numImages(numImages) {
		m_images = new Image3<T>[m_numImages];
	}

	Image3Set(const int xRes, const int yRes, const int zRes, const int numImages) :
			m_xRes(xRes),
			m_yRes(yRes),
			m_zRes(zRes),
			m_numImages(numImages) {
		m_images = new Image3<T>[m_numImages];
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

	inline Image3<T> &operator[](int i) {
		return m_images[i];
	}

	inline const Image3<T> &operator[](int i) const {
		return m_images[i];
	}

	void mergeImages(Image3<T> &mergedImage) const {
		Assert(mergedImage.getXRes() == m_xRes && mergedImage.getYRes() == m_yRes && mergedImage.getZRes() == m_zRes);
		for (int h = 0; h < m_zRes; ++h) {
			for (int i = 0; i < m_yRes; ++i) {
				for (int j = 0; j < m_xRes; ++j) {
					T val = 0.0;
					for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
						val += m_images[iterImage].getPixel(j, i, h);
					}
					mergedImage.setPixel(j, i, h, val);
				}
			}
		}
	}

	~Image3Set() {
		delete[] m_images;
	}

private:
	int m_xRes;
	int m_yRes;
	int m_zRes;
	int m_numImages;
	Image3<T> *m_images;
};

typedef Image3<Float> SmallImage;
typedef Image3Set<Float> SmallImageSet;

} /* namespace image */

#endif /* IMAGE_H_ */
