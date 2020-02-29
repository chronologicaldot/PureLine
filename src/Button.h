// Copyright 2017 Nicolaus Anderson
#ifndef IRR_CB_BUTTON_H
#define IRR_CB_BUTTON_H

#include <functional>
#include <rect.h>

namespace irr {

namespace gui {
	class IGUIElement;
	class IGUIEnvironment;
	class IGUIButton;
}

namespace cbgui {

/*
	A button wrapper that receives a callback function meant to be called when the button is pressed.
*/
class Button
	: gui::IGUIElement
{
	gui::IGUIButton*		button;
	std::function<void()>	onClickCallback;

public:
	/*
		cstor
	*/
	Button(
		gui::IGUIEnvironment*	pEnvironment,
		gui::IGUIElement*		pParent,
		core::rect<s32>			pBounds,
		std::function<void()>	pOnClickCallback,
		const wchar_t*			pText=0,
		const wchar_t*			pToolTipText=0
	);

	/*
		Irrlicht event handler override.
		Needed for catching the button click event.
	*/
	virtual bool
	OnEvent(
		const SEvent& event
	);

	/*
		Returns an address to the button owned by this class.
		This is useful for editing button features.
	*/
	gui::IGUIButton&
	getButton();
};

}}

#endif
