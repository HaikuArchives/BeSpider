/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderApplication.h"

#include "SpiderWindow.h"

#include <Alert.h>


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
	(new BAlert("AboutAlert", "BeSpider 0.1\n"
	"a spider solitaire clone for Haiku\n\n"
	"BeSpider (c) 2013 Przemysław Buczkowski <przemub@przemub.pl>\n"
	"Distributed under the terms of the MIT license.\n\n"
	"SVG-Z-cards (c) 2008 David Bellot, Brandon Ardiente\n"
	"Licensed under GNU Lesser General Public License 2.1.\n\n"
	"Fanfare sound (c) 2009 Anthony Batiste\n"
	"Licensed under Creative Commons Attribution 3.0.", "OK"))->Go();
}


int main(int argc, char** argv)
{
	SpiderApplication app;

	app.Run();

	return 0;
}
