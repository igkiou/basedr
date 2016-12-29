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

Image2<float>::EByteOrder getHostByteOrder() {
	union {
		std::uint8_t  charValue[2];
		std::uint16_t shortValue;
	};
	charValue[0] = 1;
	charValue[1] = 0;

	return (shortValue == 1)?(Image2<float>::EByteOrder::ELittleEndian)
							:(Image2<float>::EByteOrder::EBigEndian);
}

} /* namespace */

template <>
void Image2<float>::writePFM(const std::string& fileName) const {//,
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
	file << ((getHostByteOrder() == Image2<float>::EByteOrder::ELittleEndian)
							?(static_cast<float>(-1.0))
							:(static_cast<float>(1.0)));
	AssertEx(file, "Problem writing output file.");
	file << '\n';
	AssertEx(file, "Problem writing output file.");

	file.write(reinterpret_cast<char*>(m_pixels),
			m_xRes * m_yRes * sizeof(float));
	AssertEx(file, "Problem writing output file.");
}

}	/* namespace image */
