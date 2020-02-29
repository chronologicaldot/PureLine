// Copyright 2017 Nicolaus Anderson

#include <irrlicht.h>
#include "MainApp.h"

namespace pure_line {

using irr::gui::IGUIElement;
using irr::cbgui;

MainApp::MainApp(
	int argc, char* argv[]
)
	: device(nullptr)
	, guiEnvironment(nullptr)
	, fileSystem(nullptr)
	, actionsPanel(nullptr)
	, imageDisplay(nullptr)
	, activeRecord(nullptr)
{
}

MainApp::~MainApp()
{
	if ( device ) {
		actionsPanel->drop();
		imageDisplay->drop();
		device->drop();
	}
}

bool
MainApp::init() {

	irr::core::dimension2du screenSize {800,600};
	device = irr::createDevice(irr::video::EDT_OPENGL, screenSize, 16, false, false, false, this);

	if ( !device )
		return false;

	guiEnvironment = device->getGUIEnvironment();
	fileSystem = device->getFileSystem();
	actionsPanel = new ActionsPanel();
	imageDisplay = new GUIImageDisplay();

	createFileMenu();

	return true;
}

void
MainApp::run() {
	if ( !device )
		return;

	while( device->run() ) {
		guiEnvironment->drawAll();
	}
}

void
MainApp::queueFile(
	irr::io::path pFilePath
) {

}

bool
MainApp::OnEvent(
	const irr::SEvent& event
) {
	switch( event.EventType ) {

	case irr::EET_KEY_INPUT_EVENT:
		return OnKeyInput(event);

	case irr::EET_GUI_EVENT:
		return OnGUIEvent(event);

	default:
		return false;
	}
}

bool
MainApp::OnKeyInput(
	const irr::SEvent& event
) {

}


bool
MainApp::OnGUIEvent(
	const irr::SEvent& event
) {

}

void
MainApp::createFileMenu() {
	irr::core::dimension2du screenSize = device->getScreenSize();
	irr::core::recti mainPanelRect {0,0, screenSize.Width, 80};
	irr::core::recti buttonRect {0,0, 60, 60};
	irr::core::vector2di buttonShift {65, 0};
	IGUIElement* mainPanel;
	Button* openButton;
	Button* saveButton;
	Button* saveAsButton;

	// Create the menu panel itself. This is attached to the root.
	mainPanel = new IGUIElement(guiEnvironment, guiEnvironment->getRootGUIElement(), mainPanelRect, -1);

	openButton = new Button(guiEnvironment, mainPanel, buttonRect, [this]() {
		openFile( activeRecord );
	});
	openButton->getButton().setText(L"Open"); // temporary / until replaced by an icon?

	buttonRect += buttonShift;

/*
		Technicaly, the "save", "save as", and "close (image)" buttons should go on the image controls panel.
*/

	saveButton = new Button(guiEnvironment, mainPanel, buttonRect, [this]() {
		saveImage( *activeRecord, activeRecord->getSavePath() );
	});
	saveButton->getButton().setText(L"Save"); // temporary / until replaced by an icon?

	buttonRect += buttonShift;

	saveAsButton = new Button(guiEnvironment, mainPanel, buttonRect, [this]() {
		// Generate file dialog box and use the return to set the output path of the active record
	});
	saveAsButton->getButton().setText(L"Save As"); // temporary / until replaced by an icon?

	openButton->drop();
	saveButton->drop();
	saveAsButton->drop();
}

bool
MainApp::openFile(
	ImageRecord& imgRecord
) {
	// Check if the file has already been opened
	if ( imgRecord.baseImage )
		return true;

	// TODO: Open the file
}

bool
MainApp::saveImage(
	ImageRecord&	imgRecord,
	irr::io::path	savePath
) {
	// How does Irrlicht decide to save images?
}



} // End namespace pure_line
