// Copyright 2017 Nicolaus Anderson
#include "ImageModifiers.h"
#include <IImage.h>
#include <IVideoDriver.h>
#include "logic.h"


#include <cstdio>


namespace pure_line {

using namespace logic;

void
getCircularPixelDistribution(
	const MonochromeImage&	image,
	ImgDim_t				centerX,
	ImgDim_t				centerY,
	ImgDim_t				radius,
	unsigned int&			pixelDistribution[] // Must be 256 in size
) {
	int  x = -(int)radius;
	int  y = -(int)radius;
	ImgDim_t  r2 = radius * radius;
	ColorByte_t  pixelColor;

	// Rather than try to calculate a circle, cheat by simply checking if the point is within
	// the bounds of a circle.
	for (; y <= radius; ++y) {
		if ( y + centerY < 0 )
			continue;
		if ( y + centerY > image.getHeight() )
			break;

		for( x = -radius; x <= radius; ++x ) {
			if ( x + centerX < 0 )
				continue;
			if ( x + centerX > image.getWidth() )
				break;

			if ( x*x + y*y < r2 ) {
				pixelColor = image.getPixel(centerX + x, centerY + y);
				pixelDistribution[pixelColor] += 1;
			}
		}
	}
}

MonochromeImage*
createGreyImageFromColor(
	const irr::video::IImage* colorImage
) {
	if ( colorImage->getColorFormat() != irr::video::ECF_A8R8G8B8 ) {
		// Should throw error or print error.
		return nullptr;
	}

	if ( colorImage->isCompressed() ) {
		// Should throw error or print error.
		return nullptr;
	}

	return new MonochromeImage(colorImage);
}

MonochromeImage*
invertGreyImageValue (
	const MonochromeImage& image
) {
	return image.createInvertedClone();
}

MonochromeImage*
threshholdImage(
	const MonochromeImage&	image,
	ColorByte_t				threshhold
) {
	MonochromeImage* finalImage = new MonochromeImage{ image };

	ImgDim_t x,y;
	for (y=0; y < image.getHeight(); ++y) {
		for (x=0; x < image.getWidth(); ++x) {
			// Unoptimized
			if ( image.getPixel(x,y) < threshhold ) {
				finalImage->setPixel(x, y, ColorByte_t{0} );
			} else {
				finalImage->setPixel(x, y, ColorByte_Max );
			}
		}
	}
	return finalImage;
}

MonochromeImage*
localThreshholdImage(
	const MonochromeImage&	image,
	ImgDim_t				localAreaRadius,
	ColorByte_t				threshholdMax
) {
	unsigned int pixelDistribution[256];
	double mean;
	double meanDif;
	double pixelVariance[256];
	ColorByte_t localThreshhold = 0;
	MonochromeImage* finalImage = new MonochromeImage{ image };

	ImgDim_t x,y;
	for (y=0; y < image.getHeight(); ++y) {
		for (x=0; x < image.getWidth(); ++x) {
			// Reset values
			for (unsigned int i=0; i < 256; i++) {
				pixelDistribution[i] = 0;
			}

			getCircularPixelDistribution(image, x, y, localAreaRadius, &pixelDistribution);

			// Check for even distribution
				// First, calculate mean
			mean = 0;
			for (unsigned int i=0; i < 256; i++) {
				mean += static_cast<double>(pixelDistribution[i] * i);
			}
			mean /= 255;
				// Calculate the variance
			for (unsigned int i=0; i < 256; i++) {
				meanDif = static_cast<double>(i) - mean;
				pixelVariance[i] = meanDif * meanDif * pixelDistribution[i];
			}
			meanDif = 0;
			for (unsigned int i=0; i < 256; i++) {
				meanDif += pixelVariance[i];
			}
				// Finish calculating variance and Calculate standard deviation
			meanDif = sqrt(meanDif/255); // meanDif/(n-1)
				// Pick threshhold value based on standard deviation
			if ( meanDif < localAreaRadius * 2 ) {
				localThreshhold = threshholdMax;
			} else {
				localThreshhold = threshholdMax / 2;
			}

			// Unoptimized
			if ( image.getPixel(x,y) < localThreshhold ) {
				finalImage->setPixel(x, y, ColorByte_t{0} );
			} else {
				finalImage->setPixel(x, y, ColorByte_Max );
			}
		}
	}
	return finalImage;
}

MonochromeImage*
imageThinning(
	const MonochromeImage& image
) {
	// Approach:
	// Sampling only from the original image, check to see if a pixel
	// is adjacent to exactly two pixels: one below/above and one beside.
	// Original algorithm is based on the Octopus_S algorithm I created in Java.

	MonochromeImage* midImage = new MonochromeImage{ image };
	MonochromeImage* finalImage = new MonochromeImage{ image };

	std::printf("Image height = %d, width = %d\n", image.getHeight(), image.getWidth());

	bool pixelChanged;
	ImgDim_t x,y;
	do {
		pixelChanged = false;
		for (y=0; y < midImage->getHeight(); ++y) {
			for (x=0; x < midImage->getWidth(); ++x) {
				// Unoptimized
				if ( midImage->getPixel(x,y) > 0 ) {
					// Check x-dimension
					if ( Xor( midImage->getPixel(x-1, y) > 0, midImage->getPixel(x+1, y) > 0 ) ) {
						// Check y-dimension
						if ( Xor( midImage->getPixel(x, y-1) > 0, midImage->getPixel(x, y+1) > 0 ) ) {
							finalImage->setPixel(x, y, ColorByte_t{0} );
							pixelChanged = true;
						}
					}
				}
			}
		}
		// The process must begin again but with the new updates to the image
		midImage->drop();
		midImage = new MonochromeImage{ *finalImage };
	} while ( pixelChanged );

	midImage->drop(); // Cleanup
	return finalImage;
}

MonochromeImage*
imageThinning_GuoHall(
	const MonochromeImage& image,
	int maxIterations
) {
	MonochromeImage* midImage = new MonochromeImage{ image };
	MonochromeImage* finalImage = new MonochromeImage{ image };

	int iterations = 0;
	bool pixelChanged = false;
	for (; iterations != maxIterations; ++iterations ) {
		pixelChanged = imageThinning_GuoHall_once(*midImage, *finalImage, false);
		midImage->drop();
		midImage = finalImage;
		finalImage = new MonochromeImage{ *midImage };

		pixelChanged |= imageThinning_GuoHall_once(*midImage, *finalImage, true);
		midImage->drop();

		if ( !pixelChanged )
			break;

		// Don't bother with this code if we're done processing
		midImage = finalImage;
		finalImage = new MonochromeImage{ *midImage };
	}
	return finalImage;
}

bool
imageThinning_GuoHall_once(
	const MonochromeImage&	image,
	MonochromeImage&		target,
	bool					secondPass
) {
	ColorByte_t const * data = image.getData();
	ImgDim_t	x,
				y,
				width = image.getWidth(),
				height = image.getHeight();

	// top=t, middle=m, bottom=b ; rear=r (left), center=c, front=f (right)
	ColorByte_t
				tr, tc, tf,
				mr,		mf,
				br, bc, bf;
	int crn, n1, n2, n, m;
	bool changed = false;
	// Start at <1,1> since we don't do bounds checking
	// This also means we must end at <width-1, height-1>
	for( y=1; y < height - 1; ++y ) {
		for ( x=1; x < width - 1; ++x ) {
			tr = ColorByte_t(data[(y-1)*width + (x-1)] > 0);
			tc = ColorByte_t(data[(y-1)*width + (x-0)] > 0);
			tf = ColorByte_t(data[(y-1)*width + (x+1)] > 0);
			mr = ColorByte_t(data[(y-0)*width + (x-1)] > 0);
			mf = ColorByte_t(data[(y-0)*width + (x+1)] > 0);
			br = ColorByte_t(data[(y+1)*width + (x-1)] > 0);
			bc = ColorByte_t(data[(y+1)*width + (x-0)] > 0);
			bf = ColorByte_t(data[(y+1)*width + (x+1)] > 0);

			crn = (!tc & (tf | mf)) + (!mf & (bf | bc)) + (!bc & (br | mr)) + (!mr & (tr | tc));
			n1 = (tr | tc) + (tf | mf) + (bf | bc) + (br | mr);
			n2 = (tc | tf) + (mf | bf) + (bc | br) + (mr | tr);
			n  = n1 < n2 ? n1 : n2;
			m = ( secondPass ? ((tc | tf | !bf) & mf) : ((bc | br | !tr) & mr) );

			if ( crn == 1 && (n >= 2 && n <= 3) && m == 0 ) {
				if ( image.getPixel(x,y) > 0 ) {
					changed = true;
					target.setPixel(x,y,0);
				}
			}
		}
	}
	return changed;

/*
	From voronoi.h by Arnaud Ramey

#if 0
        uchar p2 = imdata[(row-1) * cols + col];
        uchar p3 = imdata[(row-1) * cols + col+1];
        uchar p4 = imdata[row     * cols + col+1];
        uchar p5 = imdata[(row+1) * cols + col+1];
        uchar p6 = imdata[(row+1) * cols + col];
        uchar p7 = imdata[(row+1) * cols + col-1];
        uchar p8 = imdata[row     * cols + col-1];
        uchar p9 = imdata[(row-1) * cols + col-1];

        int C  = (!p2 & (p3 | p4)) + (!p4 & (p5 | p6)) +
                 (!p6 & (p7 | p8)) + (!p8 & (p9 | p2));
        int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
        int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
        int N  = N1 < N2 ? N1 : N2;
        int m  = iter == 0 ? ((p6 | p7 | !p9) & p8) : ((p2 | p3 | !p5) & p4);

        if (C == 1 && (N >= 2 && N <= 3) && m == 0) {
#else
        if (need_set_guo_hall(imdata, iter, col, row, cols)) {
#endif
          if (!haschanged)
            haschanged = (tempdata[row * cols +col] != 0);
          tempdata[row * cols +col] = 0;
*/
}

irr::video::IImage*
createColorImageFromGrey(
	const MonochromeImage& greyImage,
	irr::video::IVideoDriver* videoDriver
) {
	const irr::core::dimension2du imageSize( greyImage.getWidth(), greyImage.getHeight() );
	irr::video::IImage* outImage = videoDriver->createImage( irr::video::ECF_A8R8G8B8, imageSize );
	if ( greyImage.copyToColorImage(outImage) ) {
		return outImage;
	}
	// Should throw or print an error
	return nullptr;
}

}
