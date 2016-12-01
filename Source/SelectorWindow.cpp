/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SelectorWindow.h"
#include <Catalog.h>
#include <LayoutBuilder.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"

enum selector_messages { SELECTOR_START };

SelectorWindow::SelectorWindow(BRect frame, const char* title) 
	:
	BWindow(frame, title, B_DOCUMENT_WINDOW,
	B_QUIT_ON_WINDOW_CLOSE)
{
	radio1 = new BRadioButton
	(B_TRANSLATE("Klondike"), new BMessage(-1));
	radio2 = new BRadioButton
	(B_TRANSLATE("Spider"), new BMessage(-1));
	button = new BButton
	(B_TRANSLATE("Play"), new BMessage(SELECTOR_START));
	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(radio1)
		.Add(radio2)
		.Add(button);
}
