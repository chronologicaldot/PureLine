// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_IMAGE_FORMAT_H
#define PURELINE_IMAGE_FORMAT_H

namespace pure_line {

// Supported image formats of this application.

struct EImageFormat {
enum Value {

	bmp,
	jpeg,
	pcx,
	png,
	ppm,
	psd,
	tga,

	// can be read but not written
	dds,
	wal,

	ALL

};};

} // End namespace pure_line

#endif
