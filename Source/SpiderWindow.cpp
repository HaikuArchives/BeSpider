/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderWindow.h"

#include "SpiderView.h"

#include <Alert.h>
#include <Application.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>


SpiderWindow::SpiderWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_DOCUMENT_WINDOW,
	B_QUIT_ON_WINDOW_CLOSE | B_NOT_RESIZABLE)
{
	fView = new SpiderView();
	fDiffSet = new BInvoker(new BMessage(kDiffChosenMessage), this);
	
	SetPulseRate(50000);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(_CreateMenuBar())
		.Add(fView);
}


void SpiderWindow::MessageReceived(BMessage* message)
{
	BAlert* question;
	int32 response;

	switch (message->what) {
	case kNewGameMessage:
		fView->NewGame();
		break;
	case kDifficulityMessage:
		question = new BAlert("DiffAlert", "Choose difficulity level.",
			"Easy (1 color)", "Medium (2 colors)", "Hard (4 colors)",
			B_WIDTH_AS_USUAL, B_IDEA_ALERT);
		question->Go(fDiffSet);
		break;
	case kDiffChosenMessage:
		message->FindInt32("which", &response);
		fView->ChangeDifficulity(response);
		break;
	default:
		BWindow::MessageReceived(message);
	}
}


BMenuBar* SpiderWindow::_CreateMenuBar()
{
	BMenuBar* menu = new BMenuBar("MenuBar");

	menu->AddItem(new BMenuItem("New game", new BMessage(kNewGameMessage)));
	menu->AddItem(new BMenuItem("Change difficulity",
		new BMessage(kDifficulityMessage)));

	BMenuItem* about = new BMenuItem("About", new BMessage(B_ABOUT_REQUESTED));
	about->SetTarget(be_app);
	menu->AddItem(about);

	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED)));

	return menu;
}
