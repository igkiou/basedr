/*
 * image.cpp
 *
 *  Created on: Jul 30, 2012
 *      Author: igkiou
 */

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#ifdef USE_OPENEXR
#include <ImathBox.h>
#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include <ImfInputFile.h>
#include <ImfOutputFile.h>
#include <ImfHeader.h>
#include <ImathBox.h>
#include <ImfInputFile.h>
#include <ImfHeader.h>
#include <ImfArray.h>
#include <ImfChannelList.h>
#include <ImfAttribute.h>
#include <ImfStandardAttributes.h>
#include <ImfPixelType.h>
#include <ImfFrameBuffer.h>
#endif

#include "image.h"

namespace image {

#ifdef USE_OPENEXR
void SmallImage::writeOpenEXR(const std::string& fileName) const {

	Imf::Header head((int) m_xRes, (int) m_yRes);
	head.channels().insert("Y", Imf::Channel(kEXRFloatUsed));
	Imf::FrameBuffer frameBuffer;
	frameBuffer.insert("Y", Imf::Slice(kEXRFloatUsed, (char *) m_pixels,
					sizeof(*m_pixels) * 1, sizeof(*m_pixels) * m_xRes));

	Imf::OutputFile file(fileName.c_str(), head);
	file.setFrameBuffer(frameBuffer);
	file.writePixels(m_yRes);
}
#endif

namespace {

SmallImage::EByteOrder getHostByteOrder() {
	union {
		std::uint8_t  charValue[2];
		std::uint16_t shortValue;
	};
	charValue[0] = 1;
	charValue[1] = 0;

	return (shortValue == 1)?(SmallImage::EByteOrder::ELittleEndian)
							:(SmallImage::EByteOrder::EBigEndian);
}

} /* namespace */

void SmallImage::writePFM(const std::string& fileName) const {//,
//						const EByteOrder fileEndianness) const {

//	Assert((fileEndianness == EBigEndian) || (fileEndianness == ELittleEndian));

	std::ofstream file(fileName.c_str(),
					std::ofstream::out | std::ofstream::binary);
	AssertEx(file, "Problem writing output file.");

	file << "Pf";
	AssertEx(file, "Problem writing output file.");
	file << '\n';
	AssertEx(file, "Problem writing output file.");
	file << m_xRes;
	AssertEx(file, "Problem writing output file.");
	file << ' ';
	AssertEx(file, "Problem writing output file.");
	file << m_yRes;
	AssertEx(file, "Problem writing output file.");
	file << '\n';
	AssertEx(file, "Problem writing output file.");
	file << ((getHostByteOrder() == SmallImage::EByteOrder::ELittleEndian)
							?(static_cast<float>(-1.0))
							:(static_cast<float>(1.0)));
	AssertEx(file, "Problem writing output file.");
	file << '\n';
	AssertEx(file, "Problem writing output file.");

	file.write(reinterpret_cast<char*>(m_pixels),
			m_xRes * m_yRes * sizeof(float));
	AssertEx(file, "Problem writing output file.");
}

void SmallImageSet::mergeImages(SmallImage &mergedImage) const {
	Assert(mergedImage.getXRes() == m_xRes && mergedImage.getYRes() == m_yRes);
	for (int i = 0; i < m_yRes; ++i) {
		for (int j = 0; j < m_xRes; ++j) {
			float val = 0.0f;
			for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
				val += m_images[iterImage].getPixel(j, i);
			}
			mergedImage.setPixel(j, i, val);
		}
	}
}

SmallImageSet::~SmallImageSet()  {
	delete[] m_images;
}

void SmallImageStackSet::mergeImages(SmallImageStack &mergedImage) const {
	Assert(mergedImage.getXRes() == m_xRes && mergedImage.getYRes() == m_yRes && mergedImage.getZRes() == m_zRes);
	for (int h = 0; h < m_zRes; ++h) {
		for (int i = 0; i < m_yRes; ++i) {
			for (int j = 0; j < m_xRes; ++j) {
				float val = 0.0f;
				for (int iterImage = 0; iterImage < m_numImages; ++iterImage) {
					val += m_images[iterImage].getPixel(j, i, h);
				}
				mergedImage.setPixel(j, i, h, val);
			}
		}
	}
}

SmallImageStackSet::~SmallImageStackSet()  {
	delete[] m_images;
}

}	/* namespace image */
