// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_IMAGE_RECORD_H
#define PURELINE_IMAGE_RECORD_H

#include <IImage.h>
#include "ImageFormat.h"

namespace pure_line {

class MonochromeImage;


struct ImageRecord {
	ImageFormat			format;
	irr::io::path		inputPath;
	irr::io::path		savePath;

	enum State {
		base_is_active =0,
		colored_is_active,
		grey_is_active
	} state;

protected:
	irr::video::IImage*	baseImage; // Never modified; remains in Irrlicht cache until closed
	irr::video::IImage* moddedImage; // Color image
	MonochromeImage*	greyImage;

public:
	/*
		cstor
	*/
	ImageRecord(
		irr::io::path baseImagePath
	);

	/*
		dstor
		(remember to drop the images)
	*/
	~ImageRecord();

	/*
		Sets the new output path for the file in which final image is automatically saved.
	*/
	void
	setSavePath(
		irr::io::path newPath
	);

	/*
		Returns the path where the active image should be saved.
	*/
	irr::io::path
	getSavePath() const;

	/*
		Returns the original image.
	*/
	irr::video::IImage*
	getBaseImage();

	/*
		Returns the modified color image.
	*/
	irr::video::IImage*
	getModdedColorImage();

	/*
		Returns the modified grey image.
	*/
	MonochromeImage*
	getGreyImage();

	/*
		Creates an output path from the given input path.
		Normally, the output path should default to being:
				the original file path - extension + "_out" + new extension
	*/
	static irr::io::path
	generateDefaultSavePath(
		irr::io::path	inputPath
	);
};

} // End namespace pure_line

#endif
