// Copyright 2017 Nicolaus Anderson

#include <IGUIElement.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>

namespace irr {
namespace cbgui {

Button::Button(
		gui::IGUIEnvironment*	pEnvironment,
		gui::IGUIElement*		pParent,
		core::rect<s32>			pBounds,
		std::function<void()>	pOnClickCallback,
		const wchar_t*			pText,
		const wchar_t*			pToolTipText
)
	: gui::IGUIElement( gui::EGUIET_ELEMENT, pEnvironment, pParent, -1, pBounds )
	, button(
		pEnvironment->addButton(pBounds, this, -1, pText, pToolTipText )
		)
	, onClickCallback( pOnClickCallback )
{
}

bool
Button::OnEvent(
	const SEvent& event
) {
	if ( event.EventType == EET_GUI_EVENT ) {
		if (
			event.GUIEvent.Caller == button
			&& event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED
		) {
			onClickCallback();
			return true;
		}
	}
	return false;
}

gui::IGUIButton&
Button::getButton() {
	return *button;
}

}}
