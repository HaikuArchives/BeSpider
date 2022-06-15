/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */


#include "SolitareView.h"

#include <Application.h>
#include <Catalog.h>
#include <Menu.h>
#include <TranslationUtils.h>

#include <stdio.h>

const char* OPTION_MESSAGE_LABEL = "SolitareOptions";

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSolitare"

SolitareView::SolitareView(BRect frame, const char *name, uint32 resizingMode, uint32 flags) 
: BView(frame, name, resizingMode, flags) {
	SetViewColor(0, 85, 0);

	windowWidth = STARTING_WINDOW_WIDTH;
	windowHeight = STARTING_WINDOW_HEIGHT;

	_LoadBitmaps();
}

SolitareView::~SolitareView() {
	delete fShuffle;
	delete fFanfare;
	delete fEmpty;
	for(short i = 0; i < CACHED_BACKS; i++)
		delete fBack[i];
	for(short i = 0; i < CARDS_IN_DECK; i++)
		delete fCards[i];
}


void SolitareView::AllAttached()
{
	_GenerateBoard();
}


void SolitareView::NewGame() {
	_GenerateBoard();
	Invalidate();
}

BMenu* SolitareView::GetOptionMenu() {
	BMenu* mOptions = new BMenu(B_TRANSLATE("Options"));
	fToggleSoundItem = new BMenuItem(B_TRANSLATE("Enable sound"),
		NewSolitareOptionMessage(sToggleSoundMessage));
	fToggleSoundItem->SetMarked(true);
	mOptions->AddItem(fToggleSoundItem);
	fSoundEnabled = true;
	return mOptions;
}

void SolitareView::ReceiveOptionMessage(BMessage* message) {
	switch (message->what) {
	case sToggleSoundMessage:
			fSoundEnabled = !fSoundEnabled;
			if (fToggleSoundItem->IsMarked()) {
				fToggleSoundItem->SetMarked(false);
			} else {
				fToggleSoundItem->SetMarked(true);
			}
			break;
	}
}

void SolitareView::Resize(float newWidth, float newHeight) {
	ResizeTo(newWidth, newHeight-20);
	windowWidth = (int)newWidth - 20;
	windowHeight = (int)newHeight - 30;
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

void SolitareView::_GenerateBoard()
{
	return;
}

void SolitareView::_LoadBitmaps()
{
	BString suits[] = {
		"spade",
		"heart",
		"club",
		"diamond"
	};

	// load images
	BString filename;
	for (short i = 0; i < CARDS_IN_SUIT; i++) {
		for (short j = 0; j < 4; j++) {
			filename = "";
			filename << "Artwork/" << i + 1 << "_" << suits[j] << ".png";
			fCards[j * CARDS_IN_SUIT + i]
				= BTranslationUtils::GetBitmap('rGFX', filename);
		}
	}
	fBack[0] = BTranslationUtils::GetBitmap('rGFX', "Artwork/back.png");
	fEmpty = BTranslationUtils::GetBitmap('rGFX', "Artwork/empty.png");

	// load audio
	fResources = be_app->AppResources();
	fShuffle = _LoadSound("Artwork/shuffle.wav");
	fFanfare = _LoadSound("Artwork/fanfare.wav");
	
	// cache multiple backs in a row
	for (short i = 1; i < CACHED_BACKS; i++) {
		fBack[i] = new BBitmap(BRect(0, 0, CARD_WIDTH - 1,
			CARD_HEIGHT + i * 18), fBack[0]->ColorSpace(), true);
		
		BView* fBackView = new BView(fBack[i]->Bounds(), NULL, 0, 0);
		BRect destRect = fBack[0]->Bounds();
		fBack[i]->AddChild(fBackView);
		fBack[i]->Lock();
		
		fBackView->SetDrawingMode(B_OP_COPY);
		fBackView->DrawBitmap(fBack[0], destRect);
		destRect.top = i * 18;
		destRect.bottom = destRect.top + CARD_HEIGHT;
		fBackView->DrawBitmap(fBack[0], destRect);
		
		fBackView->SetDrawingMode(B_OP_ALPHA);
		for (short j = 0; j < i + 1; j++) {
			destRect.top = j * 18;
			destRect.bottom = destRect.top + CARD_HEIGHT;
			fBackView->DrawBitmap(fBack[0], destRect);
		}
		fBackView->Sync();
		fBack[i]->Unlock();
	}

	Invalidate();
}

BSimpleGameSound* SolitareView::_LoadSound(const char* resourceName)
{
	size_t size;
	const void* data = fResources->LoadResource('rSFX', resourceName, &size);
	if (data == NULL) {
		printf("Sound resource not found: %s\n", resourceName);
		return new BSimpleGameSound("");
	}
	
	gs_audio_format format;
	format.frame_rate = 22050;
	format.channel_count = 1;
		// mono
	format.format = 0x2;
	format.byte_order = 0;
	format.buffer_size = 0;
		// auto
	
	BSimpleGameSound* sound = new BSimpleGameSound(data, size / 2, &format);
	
	status_t status = sound->InitCheck();
	if (status != B_OK) {
		printf("Error loading sound resource: %s. Error code: %d\n",
			resourceName, status);
	}
		
	return sound;
}
