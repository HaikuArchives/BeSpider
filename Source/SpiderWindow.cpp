/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderWindow.h"

#include "SpiderView.h"

#include <Application.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>


SpiderWindow::SpiderWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE | B_NOT_RESIZABLE)
{
	fView = new SpiderView();
	
	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(_CreateMenuBar())
		.Add(fView);
}


void SpiderWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
	case kNewGameMessage:
		fView->NewGame();
		break;
	default:
		BWindow::MessageReceived(message);
	}
}


BMenuBar* SpiderWindow::_CreateMenuBar()
{
	BMenuBar* menu = new BMenuBar("MenuBar");
	
	menu->AddItem(new BMenuItem("New game", new BMessage(kNewGameMessage)));
	menu->AddItem(new BMenuItem("Change difficulity", new BMessage(kDifficulityMessage)));
	
	BMenuItem* about = new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED));
	about->SetTarget(be_app);
	menu->AddItem(about);

	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));
	
	return menu;
}
