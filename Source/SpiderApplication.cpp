/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderApplication.h"
#include "SelectorWindow.h"
#include "SpiderWindow.h"

#include <cstdio>

#include <Alert.h>
#include <Catalog.h>
#include <File.h>
#include <FindDirectory.h>
#include <Path.h>
#include <String.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"

#define STARTING_WINDOW_WIDTH 900
#define STARTING_WINDOW_HEIGHT 490

static const char* kSettingsFileName = "BeSpider_settings";

SpiderApplication::SpiderApplication()
	:
	BApplication("application/x-vnd.przemub.BeSpider")
{
	LoadSettings(settings);
	fWindow = new SelectorWindow(BRect(150, 150, 300, 280), B_TRANSLATE("Pick Game"), &settings);
	
	fWindow->ResizeToPreferred();
	fWindow->CenterOnScreen();
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

bool SpiderApplication::QuitRequested()
{
	SaveSettings();
	return true;
}

void SpiderApplication::MessageReceived(BMessage* message) {
	BRect frame = BRect(150, 150, 1080, 760);
	switch (message->what) {
		case SELECTOR_KLONDIKE:
			fWindow = new SpiderWindow(frame, "BeKlondike", true, &settings);
			fWindow->Lock();
			fWindow->Show();
			fWindow->Unlock();
			break;
		case SELECTOR_SPIDER:
			fWindow = new SpiderWindow(frame, "BeSpider", false, &settings);
			fWindow->Lock();
			fWindow->Show();
			fWindow->Unlock();
			break;
	}
}

status_t
SpiderApplication::InitSettingsFile(BFile* file, bool write)
{
	// find user settings directory
	BPath prefsPath;
	status_t ret = find_directory(B_USER_SETTINGS_DIRECTORY, &prefsPath);
	if (ret < B_OK)
		return ret;

	ret = prefsPath.Append(kSettingsFileName);
	if (ret < B_OK)
		return ret;

	if (write) {
		ret = file->SetTo(prefsPath.Path(),
			B_CREATE_FILE | B_ERASE_FILE | B_WRITE_ONLY);
	} else
		ret = file->SetTo(prefsPath.Path(), B_READ_ONLY);

	return ret;
}

void SpiderApplication::LoadSettings(BMessage& archive)
{
	// locate preferences file
	BFile prefsFile;
	if (InitSettingsFile(&prefsFile, false) < B_OK) {
		printf("No preference file found.\n");
		return;
	}
	
	// unflatten settings data
	if (archive.Unflatten(&prefsFile) < B_OK) {
		printf("Error unflattening settings.\n");
	}
}		

void SpiderApplication::SaveSettings()
{
	if (!fWindow->Lock())
		return;

	// archive the current state
	status_t ret = fWindow->SaveSettings(&settings);

	fWindow->Unlock();

	if (ret < B_OK) {
		fprintf(stderr, "SpiderApplication::SaveSettings() - error from window: "
			"%s\n", strerror(ret));
		return;
	}

	// flatten entire archive and write to settings file
	BFile prefsFile;
	ret = InitSettingsFile(&prefsFile, true);
	if (ret < B_OK) {
		fprintf(stderr, "SpiderApplication::SaveSettings() - "
			"error creating file: %s\n", strerror(ret));
		return;
	}

	ret = settings.Flatten(&prefsFile);
	if (ret < B_OK) {
		fprintf(stderr, "SpiderApplication::SaveSettings() - error flattening "
			"to file: %s\n", strerror(ret));
		return;
	}
}

int main(int argc, char** argv)
{
	SpiderApplication app;

	app.Run();

	return 0;
}
