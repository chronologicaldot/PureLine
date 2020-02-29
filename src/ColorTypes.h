// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_IMAGE_COLORS_H
#define PURELINE_IMAGE_COLORS_H

#include <cstdint>

namespace pure_line {

// Color Byte
// for Red, Green, Blue, or Grey
typedef uint8_t		ColorByte_t;

// Color Byte Max Value
// Compilers may try to optimize out (ColorByte_t)-1, so the exact value is used.
static const uint8_t ColorByte_Max = 255; // 0xff

// Image Dimension Type
// for width, height, and pitch
typedef uint32_t	ImgDim_t;

}

#endif
