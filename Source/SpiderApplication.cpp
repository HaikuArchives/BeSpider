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
	fWindow = new SpiderWindow(BRect(150, 150, 1060, 550), "BeSpider");

	fWindow->Lock();
	fWindow->Show();
	fWindow->Unlock();
}


void SpiderApplication::AboutRequested()
{
	// TODO Replace with real about window
	(new BAlert("AboutAlert", "BeSpider", "OK"))->Go();
}


int main(int argc, char** argv)
{
	SpiderApplication app;

	app.Run();

	return 0;
}
