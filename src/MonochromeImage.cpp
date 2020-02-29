// Copyright 2017 Nicolaus Anderson

#include "MonochromeImage.h"
#include <IImage.h>

namespace pure_line {

ColorByte_t
SColorToGrey(
	const irr::video::SColor inColor
) {
	ColorByte_t r,g,b,m;
	r = inColor.getRed();
	g = inColor.getGreen();
	b = inColor.getBlue();
	m = (g > b)? g : b;
	return ( (r > m) ? r : m );
}

irr::video::SColor
GreyToSColor(
	const ColorByte_t alpha,
	const ColorByte_t inColor
) {
	return irr::video::SColor(alpha, inColor, inColor, inColor);
}

MonochromeImage::MonochromeImage(
	ImgDim_t pWidth,
	ImgDim_t pHeight
)
	: width( pWidth )
	, height( pHeight )
	, data( nullptr )
{
	data = new ColorByte_t [width * height];
	resetData();
}

MonochromeImage::MonochromeImage(
	const MonochromeImage& pOtherImage
)
	: width( pOtherImage.width )
	, height( pOtherImage.height )
	, data( nullptr )
{
	data = new ColorByte_t [width * height];
	resetData();

	ImgDim_t i = 0;

	for (; i < width * height; ++i) {
		data[i] = ColorByte_t{ pOtherImage.data[i] };
	}
}

MonochromeImage::MonochromeImage(
	const irr::video::IImage* pSourceImage
)
	: width(0)
	, height(0)
	, data( nullptr )
{
	if ( !pSourceImage ) {
		// Should throw
		throw 0;
		return;
	}

	//if (pSourceImage->getColorFormat() != irr::video::ECF_A8R8G8B8) {
	//	resetData();
	//	return; // Should throw unless checked elsewhere
	//}

	irr::core::dimension2du srcSize = pSourceImage->getDimension();
	width = srcSize.Width;
	height = srcSize.Height;
	ImgDim_t fullSize = width * height;
	data = new ColorByte_t [fullSize];

	ImgDim_t	x = 0,
				y = 0,
				i = 0;

	for (; y < height; ++y) {
		for (x = 0; x < width; ++x, ++i) {
			data[i] = SColorToGrey( pSourceImage->getPixel(x,y) );
		}
	}
}

MonochromeImage::~MonochromeImage() {
	if ( data )
		delete[] data;
}

ColorByte_t*
MonochromeImage::getData() {
	return data;
}

const ColorByte_t*
MonochromeImage::getData() const {
	return data;
}

void
MonochromeImage::resetData() {
	ColorByte_t*	dataPtr = data;
	ImgDim_t		i=0,
					fullSize;
	fullSize = width * height;
	for (; i < fullSize; ++i) {
		*dataPtr = ColorByte_t{0};
		++dataPtr;
	}
}

ImgDim_t
MonochromeImage::getWidth() const {
	return width;
}

ImgDim_t
MonochromeImage::getHeight() const {
	return height;
}

void
MonochromeImage::setPixel(
	ImgDim_t	x,
	ImgDim_t	y,
	ColorByte_t	value
) {
	if ( x < width && y < height ) {
		data[y*width + x] = value;
	}
}

ColorByte_t
MonochromeImage::getPixel(
	ImgDim_t	x,
	ImgDim_t	y
) const {
	if ( x < width && y < height )
		return data[y*width + x];

	return ColorByte_t{0};
}

void
MonochromeImage::invertColor() {
	ColorByte_t* srcPtr = data;
	ImgDim_t fullSize = width * height;
	ImgDim_t index = 0;
	for (; index < fullSize; ++index, ++srcPtr) {
		//(*srcPtr) = ColorByte_Max - (*srcPtr);
		data[index] = ColorByte_Max - data[index];
	}
}

MonochromeImage*
MonochromeImage::createInvertedClone() const {
	MonochromeImage* outImage = new MonochromeImage(*this);
	outImage->invertColor();
	return outImage;
}

bool
MonochromeImage::copyToColorImage(
	irr::video::IImage* pTarget
) const {
	if ( !pTarget )
		return false;

	irr::core::dimension2du targetSize = pTarget->getDimension();
	if ( targetSize.Width != width || targetSize.Height != height )
		return false;

	ColorByte_t* pixelPtr = data;
	ImgDim_t 	x = 0,
				y = 0;

	// Technically, IImage::lock() could be used, but then we would need
	// to check the color format.
	for (; y < height; ++y) {
		for (x = 0; x < width; ++x, ++pixelPtr) {
			pTarget->setPixel(x, y, GreyToSColor(255, *pixelPtr));
		}
	}
	return true;
}

}
