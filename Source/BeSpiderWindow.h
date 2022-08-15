/*
 * Copyright 2022 Javier Steinaker <jsteinaker@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#ifndef BESPIDERWINDOW_H
#define BESPIDERWINDOW_H

#include <Rect.h>
#include <Window.h>

class BeSpiderWindow : public BWindow {
	public:
		BeSpiderWindow(BRect frame, const char* title, window_type type, uint32 flags, BMessage* settings);
		
		virtual ~BeSpiderWindow();
		virtual status_t SaveSettings(BMessage* settings);
};

#endif // BESPIDERWINDOW_H
