// Copyright 2017 Nicolaus Anderson

#include <irrlicht.h>
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include <vector>
#include <string>
#include <cstdio>
//#include "ImageRecord.h"
#include "ImageModifiers.h"


#include <coreutil.h>

irr::io::path
generateDefaultSavePath(irr::io::path startPath) {
	// Remove old extension and add "_out" and re-add the old extension
	irr::io::path extension;
	irr::core::getFileNameExtension(extension, startPath);
	irr::core::stringc outPath;
	irr::core::cutFilenameExtension(outPath, startPath); // from coreutil.h
	outPath += "_out.";
	//outPath += extension;
	outPath += "png";
	return irr::io::path(outPath);
}


int main(int argc, char* argv[]) {
	// Options
	bool invert = true;

	std::vector<std::string> arguments(argv + 1, argv + argc);
	std::string option;
	irr::io::path writePath;
	irr::io::path filePath;

	std::printf("Parsing arguments.\n");

	int i=0;
	for (; i < argc - 1; ++i) {
		if ( arguments.at(i).compare(0,2,"-o") == 0 ) {
			++i; // Skip to name
			if ( i < argc - 1 ) {
				writePath = irr::io::path(
							//std::string( arguments[i].begin() + 2, arguments[i].end() ).c_str()
							std::string( arguments[i].begin(), arguments[i].end() ).c_str()
						);
			} else {
				std::printf("No output file name given.\n");
			}
		}
		else if ( arguments.at(i).compare(0,3,"-ni") == 0 ) {
			invert = false;
		}
		else {
			// Must be the file path
			filePath = irr::io::path(
							arguments.at(i).c_str()
						);
		}
	}

	if ( filePath.size() == 0 ) {
		std::printf("No file given.\n");
		return 1;
	}
	if ( writePath.size() == 0 ) {
		std::printf("Auto-generating save path.\n");
		//writePath = pure_line::ImageRecord::generateDefaultSavePath(filePath);
		writePath = generateDefaultSavePath(filePath);
	}

	//---------
	std::printf("Creating engine.\n");

	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_NULL);

	if ( !device ) {
		std::printf("Irrlicht device could not be created.\n");
		return 1;
	}

	irr::video::IVideoDriver* videoDriver = device->getVideoDriver();

	irr::video::IImage* srcImage = videoDriver->createImageFromFile(filePath);
	//pure_line::ImageRecord imgRecord{ filePath };
	//imgRecord.setBaseImage(srcImage);
	//srcImage->drop();

	if ( !srcImage ) {
		std::printf("Image could not be loaded.\n");
		return 0;
	}

	pure_line::MonochromeImage* greyImage = new pure_line::MonochromeImage{ srcImage };
	pure_line::MonochromeImage* threshholdImage = nullptr;
	pure_line::MonochromeImage* finalGreyImage = nullptr;

	if ( invert ) {
		std::printf("Inverting image color.\n");
		greyImage->invertColor();
	}

	pure_line::ColorByte_t threshhold = 255/3;
	std::printf("Threshhold at %d\n", threshhold);
	threshholdImage = pure_line::threshholdImage(*greyImage, threshhold);

	std::printf("Thinning.\n");
	//finalGreyImage = pure_line::imageThinning(*threshholdImage);
	finalGreyImage = pure_line::imageThinning_GuoHall(*threshholdImage, 50);

	if ( invert ) {
		std::printf("Inverting image color.\n");
		finalGreyImage->invertColor();
	}

	std::printf("Creating color image from grey.\n");
	irr::video::IImage* finalImage = pure_line::createColorImageFromGrey(*finalGreyImage, videoDriver);

	if ( !finalImage ) {
		std::printf("Failed to create final image.\n");
	}

	if ( ! videoDriver->writeImageToFile( finalImage, writePath ) ) {
		std::printf("Failed to write image to file.\n");
	}

	srcImage->drop();
	greyImage->drop();
	threshholdImage->drop();
	finalGreyImage->drop();
	finalImage->drop();

	device->closeDevice();
	device->drop();

	std::printf("Program exit.\n");

	return 0;
}
