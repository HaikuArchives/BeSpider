/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERWINDOW_H_
#define _SPIDERWINDOW_H__

#include "SpiderView.h"
#include "KlondikeView.h"
#include "SolitareView.h"

#include <Window.h>

class SpiderWindow : public BWindow {
public:
	SpiderWindow(BRect frame, const char* title, bool is_klondike);

	virtual void MessageReceived(BMessage* message);
	virtual void FrameResized(float newWidth, float newHeight);

private:
	BMenuBar* _CreateMenuBar();

	SolitareView* fView;
};

#endif	// _SPIDERWINDOW_H_
