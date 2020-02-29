// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_MONOCHROME_IMAGE_H
#define PURELINE_MONOCHROME_IMAGE_H

#include "ColorTypes.h"
#include <SColor.h>
#include <IReferenceCounted.h>

namespace irr {
	namespace video {
		class IImage;
	}
}

namespace pure_line {

ColorByte_t
SColorToGrey(
	const irr::video::SColor inColor
);

irr::video::SColor
GreyToSColor(
	const ColorByte_t alpha,
	const ColorByte_t inColor
);


class MonochromeImage
	: public irr::IReferenceCounted // Only because std::shared_ptr won't work.
{

	ImgDim_t		width;
	ImgDim_t		height;
	ColorByte_t*	data;

public:
	/*
		cstor
	*/
	explicit MonochromeImage(
		ImgDim_t pWidth,
		ImgDim_t pHeight
	);

	/*
		Copy cstor
	*/
	MonochromeImage(
		const MonochromeImage& pOtherImage
	);

	/*
		From-color constructor
	*/
	MonochromeImage(
		const irr::video::IImage* pSourceImage
	);

	/*
		dstor
	*/
	~MonochromeImage();

	/*
		Returns a direct pointer to the byte data.
	*/
	ColorByte_t*
	getData();

	/*
		Returns a constant direct pointer to the byte data.
	*/
	const ColorByte_t*
	getData() const;

	/*
		Sets all of the data to zero.
		Note: This does NOT change the size nor delete the data.
	*/
	void
	resetData();

	/*
		Returns the width of the image.
	*/
	ImgDim_t
	getWidth() const;

	/*
		Returns the height of the image.
	*/
	ImgDim_t
	getHeight() const;

	/*
		Sets the pixel at the given location if in bounds.
	*/
	void
	setPixel(
		ImgDim_t	x,
		ImgDim_t	y,
		ColorByte_t	value
	);

	/*
		Returns the value of the pixel at the given location if in bounds.
	*/
	ColorByte_t
	getPixel(
		ImgDim_t	x,
		ImgDim_t	y
	) const;

	/*
		Inverts the colors of this image.
	*/
	void
	invertColor();

	/*
		Returns a new copy of this image with the values inverted.
		The returned image should be dropped when no longer needed.
	*/
	MonochromeImage*
	createInvertedClone() const;

	/*
		Attempts to copy this image to a color image.
		Image format must be ECF_A8R8G8B8.
		Red, green, and blue values will all be given the same value as the monochrome pixel.
		DO NOT PASS THIS FUNCTION A COMPRESSED IMAGE.
	*/
	bool
	copyToColorImage(
		irr::video::IImage* pTarget
	) const;
};

}

#endif
