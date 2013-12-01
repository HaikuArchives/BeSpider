/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderWindow.h"

#include "SpiderView.h"


SpiderWindow::SpiderWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE | B_NOT_RESIZABLE)
{
	AddChild(new SpiderView());
}

