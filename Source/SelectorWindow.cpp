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

SelectorWindow::SelectorWindow(BRect frame, const char* title, BMessage* settings) 
	:
	BeSpiderWindow(frame, title, B_TITLED_WINDOW, B_NOT_RESIZABLE | B_QUIT_ON_WINDOW_CLOSE, settings)
{
	radio1 = new BRadioButton
	(B_TRANSLATE("Klondike"), new BMessage(SELECTOR_RADIO1));
	radio2 = new BRadioButton
	(B_TRANSLATE("Spider"), new BMessage(SELECTOR_RADIO2));
	button = new BButton
	(B_TRANSLATE("Play"), new BMessage(SELECTOR_START));
	radio1->SetValue(1);
	SetDefaultButton(button);
	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(radio1)
		.Add(radio2)
		.Add(button)
		.SetInsets(40, 5, 40, 5);
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

status_t SelectorWindow::SaveSettings(BMessage* settings)
{
	return B_OK;
}
