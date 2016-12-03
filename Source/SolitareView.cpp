/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SolitareView.h"

const char* OPTION_MESSAGE_LABEL = "SolitareOptions";

SolitareView::SolitareView(BRect frame, const char *name, uint32 resizingMode, uint32 flags) 
: BView(frame, name, resizingMode, flags) {
	
}

void SolitareView::NewGame() {
	return;
}

BMenu* SolitareView::GetOptionMenu() {
	return NULL;
}

void SolitareView::ReciveOptionMessage(BMessage* message) {
	return;
}

void SolitareView::Resize(float newWidth, float newHeight) {
	return;
}

void SolitareView::Hint() {
	return;
}

BMessage* NewSolitareOptionMessage(uint32 what) {
	BMessage* option_message = new BMessage(OPTION_MESSAGE_TYPE);
	BMessage* message = new BMessage(what);
	option_message->AddMessage(OPTION_MESSAGE_LABEL, message);
	return option_message;
}
