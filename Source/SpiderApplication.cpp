/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderApplication.h"
#include "SelectorWindow.h"
#include "SpiderWindow.h"

#include <Alert.h>
#include <Catalog.h>
#include <String.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"

#define STARTING_WINDOW_WIDTH 900
#define STARTING_WINDOW_HEIGHT 490

SpiderApplication::SpiderApplication()
	:
	BApplication("application/x-vnd.przemub.BeSpider")
{
	fWindow = new SelectorWindow(BRect(150, 150, 300, 240), B_TRANSLATE("Pick Game"));

	fWindow->Lock();
	fWindow->Show();
	fWindow->Unlock();
}


void SpiderApplication::AboutRequested()
{
	BString aboutText = "";
	aboutText << "BeSpider v0.1" << "\n"
	<< B_TRANSLATE("A spider solitaire clone for Haiku.")<<"\n\n"
	<< B_TRANSLATE("BeSpider (c) 2013-17 Przemysław Buczkowski <przemub@przemub.pl>")<<"\n"
	<< B_TRANSLATE("Distributed under the terms of the MIT license.")<<"\n\n"
	<< B_TRANSLATE("SVG-Z-cards (c) 2008 David Bellot, Brandon Ardiente.")<<"\n"
	<< B_TRANSLATE("Licensed under GNU Lesser General Public License 2.1.")<<"\n\n"
	<< B_TRANSLATE("Fanfare sound (c) 2009 Anthony Batiste.")<<"\n"
	<< B_TRANSLATE("Licensed under Creative Commons Attribution 3.0.");
	
	(new BAlert("AboutAlert", aboutText, B_TRANSLATE_CONTEXT("OK", "About alert")))->Go();
}

void SpiderApplication::MessageReceived(BMessage* message) {
	switch (message->what) {
		case SELECTOR_KLONDIKE:
			fWindow = new SpiderWindow(BRect(150, 150,
				150 + STARTING_WINDOW_WIDTH + 20,
				150 + STARTING_WINDOW_HEIGHT + 30), "BeKlondike", true);
			fWindow->Lock();
			fWindow->Show();
			fWindow->Unlock();
			break;
		case SELECTOR_SPIDER:
			fWindow = new SpiderWindow(BRect(150, 150,
				150 + STARTING_WINDOW_WIDTH + 20,
				150 + STARTING_WINDOW_HEIGHT + 30), "BeSpider", false);
			fWindow->Lock();
			fWindow->Show();
			fWindow->Unlock();
			break;
	}
}

int main(int argc, char** argv)
{
	SpiderApplication app;

	app.Run();

	return 0;
}
