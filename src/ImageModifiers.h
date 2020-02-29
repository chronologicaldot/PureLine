// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_IMAGE_MODIFIERS_H
#define PURELINE_IMAGE_MODIFIERS_H

#include "MonochromeImage.h"

namespace irr {
	namespace video {
		class IImage;
		class IVideoDriver;
	}
}

namespace pure_line {

/*
	Gets the distribution of pixel values of a monochrome image within a given radius from a center point.
*/
void
getCircularPixelDistribution(
	const MonochromeImage&	image,
	ImgDim_t				centerX,
	ImgDim_t				centerY,
	ImgDim_t				radius,
	unsigned int&			pixelDistribution[] // Must be 256 in size
);

/*
	Converts a A8R8G8B8 image to a "grey" Monochrome image.
*/
MonochromeImage*
createGreyImageFromColor(
	const irr::video::IImage& colorImage
);

/*
	Inverts the color of a monochrome image.
*/
MonochromeImage*
invertGreyImageValue (
	const MonochromeImage& image
);

/*
	Tasks a monochrome image and makes all of the color above the given value black.
*/
MonochromeImage*
threshholdImage(
	const MonochromeImage&	image,
	ColorByte_t				threshhold
);

/*
	Tasks a monochrome image and makes pixels black based on the local distribution of colors.
	The given threshhold value argument is used as a maximum.
*/
MonochromeImage*
localThreshholdImage(
	const MonochromeImage&	image,
	ImgDim_t				localAreaRadius,
	ColorByte_t				threshholdMax
);

/*
	Takes a "grey" image, creates a clone of that image, and thins all of the lines in that image.
*/
MonochromeImage*
imageThinning(
	const MonochromeImage& image
);

/*
	Takes a "grey" image, creates a clone of that image, and thins it out according to Guo Hall algorithm.
*/
MonochromeImage*
imageThinning_GuoHall(
	const MonochromeImage& image,
	int maxIterations = -1
);

/*
	Performs a single Guo-Hall iteration on an image. Normally, you would not call this function directly.
*/
bool
imageThinning_GuoHall_once(
	const MonochromeImage&	image,
	MonochromeImage&		target,
	bool					secondPass
);

/*
	Converts a "grey" Monochrome image to an A8R8G8B8 image.
	It requires a video driver for creating the image.
*/
irr::video::IImage*
createColorImageFromGrey(
	const MonochromeImage& greyImage,
	irr::video::IVideoDriver* videoDriver
);

}

#endif
