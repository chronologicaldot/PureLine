// Copyright 2017 Nicolaus Anderson

#include <coreutil.h>
#include "MonochromeImage.h"

namespace pure_line {

ImageRecord::ImageRecord(
	irr::io::path baseImagePath
)
	: format()
	, inputPath(baseImagePath)
	, savePath()
	, state( base_is_active )
	, baseImage(nullptr)
	, moddedImage(nullptr)
	, greyImage(nullptr)
{
	//
}

ImageRecord::~ImageRecord() {
	if ( baseImage )
		baseImage->drop();

	if ( moddedImage )
		moddedImage->drop();

	if ( greyImage )
		greyImage->drop();
}

void
ImageRecord::setSavePath(
	irr::io::path pNewPath
) {
	savePath = pNewPath;
}

irr::io::path
ImageRecord::getSavePath() const {
	if ( savePath.size() == 0 ) {
		savePath = generateDefaultOutputPath(inputPath);
	}
	return savePath;
}

static irr::io::path
ImageRecord::generateDefaultSavePath(
	irr::io::path	startPath
) {
	// Remove old extension and add "_out" and re-add the old extension
	irr::io::path extension;
	irr::core::getFileNameExtension(extension, startPath);
	irr::core::stringc outPath;
	irr::core::cutFilenameExtension(outPath, startPath); // from coreutil.h
	outPath += "_out";
	outPath += extension;
	return irr::io::path(outPath);
}


}
