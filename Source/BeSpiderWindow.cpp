/*
 * Copyright 2022 Javier Steinaker <jsteinaker@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "BeSpiderWindow.h"

BeSpiderWindow::BeSpiderWindow(BRect frame, const char* title, window_type type, uint32 flags, BMessage* settings)
	:BWindow(frame, title, type, flags)
{
}

BeSpiderWindow::~BeSpiderWindow()
{
}

status_t BeSpiderWindow::SaveSettings(BMessage* settings)
{
	return B_OK;
}
