// Copyright 2017 Nicolaus Anderson
#ifndef PURELINE_MAIN_APP
#define PURELINE_MAIN_APP

#include <functional>
#include <irrlicht.h>
#include "ImageRecord.h"

namespace pure_line {

class MainApp
	: public IEventReceiver
{
	IrrlichtDevice* device;
	IGUIEnvironment* guiEnvironment;
	IFileSystem* fileSystem;
	ActionsPanel* actionsPanel;
	GUIImageDisplay* imageDisplay;

	ImageRecord* activeRecord;
	std::vector<ImageRecord> imageQueue;

public:
	/*
		cstor
	*/
	MainApp(
		int argc, char* argv[]
	);

	/*
		dstor
	*/
	~MainApp();

	/*
		Constructs the IrrlichtDevice and the GUI.
	*/
	bool
	init();

	/*
		Runs the application, performing the execution loop.
	*/
	void
	run();

	/*
		Adds an file to the list of files to be processed.
		If there is no currently-open file, the image file is opened.
	*/
	void
	queueFile(
		irr::io::path pFilePath
	);

	/*
		Responds to GUI events.
	*/
	virtual bool
	OnEvent(
		const irr::SEvent& event
	);

protected:
	/*
		Response to key-input events.
	*/
	bool
	OnKeyInput(
		const irr::SEvent& event
	);

	/*
		Response to GUI events.
		These should be for the menu itself.
	*/
	bool
	OnGUIEvent(
		const irr::SEvent& event
	);

	/*
		Creates the menu and all of its buttons, assigning callbacks and so forth.
	*/
	void
	createFileMenu();

	/*
		Changes the GUI setup to display the image at its current progress.
		If no image exists, it opens it.
		If the image cannot be opened, it returns false.
	*/
	bool
	openFile(
		ImageRecord& imgRecord
	);

	/*
		Attempts to write the image to a file with the given filename.
		\param imgRecord - The image data. Note: only a completed/finalized A8R8G8B8 image can be written.
		\param format - The output format (jpg, png, etc.) in which to encode the result.
		\param outputFilePath - The file path that serves as the new name of the result.
			Normally, the outputPath should default to being:
				the original file path - extension + "_out" + new extension
	*/
	bool
	saveImage(
		ImageRecord&	imgRecord,
		irr::io::path	savePath
	);

	/*
		Closes the image and removes the record from the list.
		Unsaved changes will be lost.
	*/
	bool
	closeImage(
		ImageRecord*	imgRecord
	);
};

}

#endif
