/*
 * Copyright 2022 Javier Steinaker <jsteinaker@gmail.com>
 * All rights reserved. Distributed under the terms of the MIT license.
 */

#include "BeSpiderWindow.h"

BeSpiderWindow::BeSpiderWindow(BRect frame, const char* title, BMessage* settings)
	:BWindow(frame, title, B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE)
{
}

BeSpiderWindow::~BeSpiderWindow()
{
}

status_t BeSpiderWindow::SaveSettings(BMessage* settings)
{
	return B_OK;
}
