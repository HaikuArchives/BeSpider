/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERWINDOW_H_
#define _SPIDERWINDOW_H_

#include "BeSpiderWindow.h"
#include "SpiderView.h"
#include "KlondikeView.h"
#include "SolitareView.h"

class SpiderWindow : public BeSpiderWindow {
public:
	SpiderWindow(BRect frame, const char* title, bool is_klondike,
		BMessage* settings);

	virtual void MessageReceived(BMessage* message);
	virtual void FrameResized(float newWidth, float newHeight);
	
	virtual status_t SaveSettings(BMessage* settings);
	virtual bool QuitRequested();

private:
	BMenuBar* _CreateMenuBar();

	SolitareView* fView;
};

#endif	// _SPIDERWINDOW_H_
