// Copyright 2017 Nicolaus Anderson

#include <irrlicht.h>
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#include "MainApp.h"

int main(int argc, char* argv[]) {

	MainApp app(argc, argv);
	if ( app.init() ) {
		app.run();
	} else {
		return 1;
	}
	return 0;
}
