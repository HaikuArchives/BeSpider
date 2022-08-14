/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERAPPLICATION_H_
#define _SPIDERAPPLICATION_H_

#include "BeSpiderWindow.h"

#include <Application.h>
#include <File.h>

class SpiderApplication : public BApplication {
public:
	SpiderApplication();

	virtual void AboutRequested();
	virtual void MessageReceived(BMessage* message);
	virtual bool QuitRequested();

private:
	status_t	InitSettingsFile(BFile* file, bool write);
	void		LoadSettings(BMessage& settings);
	void		SaveSettings();
	
	BeSpiderWindow* fWindow;
	BMessage settings;
};


#endif	// _SPIDERAPPLICATION_H_
