/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderWindow.h"

#include "SpiderView.h"
#include "SolitareView.h"

#include <cstdio>

#include <Alert.h>
#include <Application.h>
#include <Catalog.h>
#include <LayoutBuilder.h>
#include <MenuBar.h>
#include <MenuItem.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"


SpiderWindow::SpiderWindow(BRect frame, const char* title, bool is_klondike,
	BMessage* settings)
	:
	BeSpiderWindow(frame, title, B_DOCUMENT_WINDOW, B_QUIT_ON_WINDOW_CLOSE, settings)
{
	LoadSettings(settings);
	if (is_klondike) {
		fView = new KlondikeView(settings);
	} else {
		fView = new SpiderView(settings);
	}

	SetPulseRate(500000);

	BLayoutBuilder::Group<>(this, B_VERTICAL, 1)
		.Add(_CreateMenuBar())
		.Add(fView);
}


void SpiderWindow::FrameResized(float newWidth, float newHeight)
{
	fView->Resize(newWidth, newHeight);
}


void SpiderWindow::MessageReceived(BMessage* message)
{
	BMessage* option_message = new BMessage();
	switch (message->what) {
	case sNewGameMessage:
		fView->NewGame();
		break;
	case sHintMessage:
		fView->Hint();
		break;
	case OPTION_MESSAGE_TYPE:
		message->FindMessage(OPTION_MESSAGE_LABEL, option_message);
		fView->ReceiveOptionMessage(option_message);
		break;
	case 'DATA':
		if(message->WasDropped()) {
			fView->MouseUp(message->DropPoint());
		} else {
			BWindow::MessageReceived(message);
		}
		break;
	default:
		BWindow::MessageReceived(message);
	}
}


BMenuBar* SpiderWindow::_CreateMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("MenuBar");
	BMenu* mGame = new BMenu(B_TRANSLATE("Game"));
	BMenu* mOptions = fView->GetOptionMenu();
	BMenuItem* menuItem;
	menuBar->AddItem(mGame);
	
	if (mOptions != NULL) {
		menuBar->AddItem(fView->GetOptionMenu());
	}
	
	menuItem = new BMenuItem(B_TRANSLATE("New game"), new BMessage(sNewGameMessage));
	menuItem->SetShortcut('N', B_COMMAND_KEY);
	mGame->AddItem(menuItem);
	
	menuItem = new BMenuItem(B_TRANSLATE_CONTEXT("Hint", "Menu bar"), new BMessage(sHintMessage));
	menuItem->SetShortcut('H', B_COMMAND_KEY);
	mGame->AddItem(menuItem);
	mGame->AddSeparatorItem();

	BMenuItem* about = new BMenuItem(B_TRANSLATE_CONTEXT("About" B_UTF8_ELLIPSIS, "Menu bar"),
		new BMessage(B_ABOUT_REQUESTED));
	about->SetTarget(be_app);
	mGame->AddItem(about);
	mGame->AddSeparatorItem();
	
	menuItem = new BMenuItem(B_TRANSLATE_CONTEXT("Quit", "Menu bar"), new BMessage(B_QUIT_REQUESTED));
	menuItem->SetShortcut('Q', B_COMMAND_KEY);
	mGame->AddItem(menuItem);

	return menuBar;
}

status_t SpiderWindow::LoadSettings(BMessage* settings)
{
	BRect frame;
	if(settings->FindRect("window frame", &frame) == B_OK)
	{
		MoveTo(frame.left, frame.top);
		ResizeTo(frame.Width(), frame.Height());
	}
	return B_OK;
}

status_t SpiderWindow::SaveSettings(BMessage* settings)
{
	settings->SetRect("window frame", Frame());
	return fView->SaveSettings(settings);
}

bool SpiderWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	Hide();

	// NOTE: don't quit, since the app needs us
	// for saving settings yet...
	return false;
}
