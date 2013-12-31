/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderApplication.h"

#include "SpiderWindow.h"

#include <Alert.h>
#include <Catalog.h>
#include <String.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"


SpiderApplication::SpiderApplication()
	:
	BApplication("application/x-vnd.przemub.BeSpider")
{
	fWindow = new SpiderWindow(BRect(150, 150, 1060, 575), "BeSpider");

	fWindow->Lock();
	fWindow->Show();
	fWindow->Unlock();
}


void SpiderApplication::AboutRequested()
{
	BString aboutText = "";
	aboutText << "BeSpider 0.1" << "\n"
	<< B_TRANSLATE("a spider solitaire clone for Haiku")<<"\n\n"
	<< B_TRANSLATE("BeSpider (c) 2013 Przemysław Buczkowski <przemub@przemub.pl>")<<"\n"
	<< B_TRANSLATE("Distributed under the terms of the MIT license.")<<"\n\n"
	<< B_TRANSLATE("SVG-Z-cards (c) 2008 David Bellot, Brandon Ardiente")<<"\n"
	<< B_TRANSLATE("Licensed under GNU Lesser General Public License 2.1.")<<"\n\n"
	<< B_TRANSLATE("Fanfare sound (c) 2009 Anthony Batiste")<<"\n"
	<< B_TRANSLATE("Licensed under Creative Commons Attribution 3.0.");
	
	(new BAlert("AboutAlert", aboutText, B_TRANSLATE_CONTEXT("OK", "About alert")))->Go();
}


int main(int argc, char** argv)
{
	SpiderApplication app;

	app.Run();

	return 0;
}
