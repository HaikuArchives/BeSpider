/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SelectorWindow.h"
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <Application.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"

SelectorWindow::SelectorWindow(BRect frame, const char* title) 
	:
	BWindow(frame, title, B_DOCUMENT_WINDOW,
	B_QUIT_ON_WINDOW_CLOSE)
{
	radio1 = new BRadioButton
	(B_TRANSLATE("Klondike"), new BMessage(SELECTOR_RADIO1));
	radio2 = new BRadioButton
	(B_TRANSLATE("Spider"), new BMessage(SELECTOR_RADIO2));
	button = new BButton
	(B_TRANSLATE("Play"), new BMessage(SELECTOR_START));
	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(radio1)
		.Add(radio2)
		.Add(button);
	klondike = true;
}

void SelectorWindow::MessageReceived(BMessage* message) {
	switch (message->what) {
		case SELECTOR_RADIO1:
			klondike = true;
			break;
		case SELECTOR_RADIO2:
			klondike = false;
			break;
		case SELECTOR_START:
			SetFlags(0); // Make app not close when new window is opened and this one closed
			if (klondike) {
				be_app->PostMessage(SELECTOR_KLONDIKE);
			} else {
				be_app->PostMessage(SELECTOR_SPIDER);
			}
			Quit();
			break;
	}
}
