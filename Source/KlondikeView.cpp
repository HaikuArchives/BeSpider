/*
 * Copyright 2013-2014 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "KlondikeView.h"

#include <Alert.h>
#include <Catalog.h>
#include <Entry.h>
#include <Path.h>
#include <Roster.h>
#include <MenuItem.h>
#include <String.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeKlondike"


KlondikeView::KlondikeView(BMessage *settings)
	:
	SolitareView(BRect(0, 0, STARTING_WINDOW_WIDTH + 10, STARTING_WINDOW_HEIGHT + 10),
		"KlondikeView", B_FOLLOW_LEFT | B_FOLLOW_TOP,
		B_WILL_DRAW | B_PULSE_NEEDED | B_FULL_UPDATE_ON_RESIZE, settings)
{
	LoadSettings(settings);
}


KlondikeView::~KlondikeView()
{
}

status_t KlondikeView::LoadSettings(BMessage *settings)
{
	SolitareView::LoadSettings(settings);
	
	if (!settings)
		return B_BAD_VALUE;
	
	// In case the value is not stored in settings, use default	
	if (settings->FindBool("klondike auto play", &fAutoPlayEnabled) != B_OK)
		fAutoPlayEnabled = true;
	if (settings->FindBool("klondike quick auto play", &fQuickAutoPlay) != B_OK)
		fQuickAutoPlay = false;
	
	return B_OK;
}

status_t KlondikeView::SaveSettings(BMessage* settings)
{
	settings->SetBool("klondike auto play", fAutoPlayEnabled);
	settings->SetBool("klondike quick auto play", fQuickAutoPlay);
	return SolitareView::SaveSettings(settings);
}

void KlondikeView::Draw(BRect rect)
{
	
	SetDrawingMode(B_OP_ALPHA);
	
	int hSpacing = _CardHSpacing();
	
	// stock
	int revealed = 0;
	for (short i = 0; i < 24; i++)
		if (fStock[i]->fRevealed)
			revealed++;
	
	if (revealed < 24)
		DrawBitmap(fBack[0], BRect(hSpacing, 15,
			hSpacing + CARD_WIDTH, 15 + CARD_HEIGHT));
	else
		DrawBitmap(fEmpty, BRect(hSpacing, 15,
		hSpacing + CARD_WIDTH, 15 + CARD_HEIGHT));
	
	// waste
	if (fIsWasteCardPicked) {
		int lastWasteCard = fWasteCard - 1;
		
		if (lastWasteCard != -1)
			while (fStock[lastWasteCard]->fRevealed) {
				lastWasteCard--;
				if (lastWasteCard == -1) {
					break;
				}
			}
		
		if (lastWasteCard != -1)
			DrawBitmap(
				fCards[fStock[lastWasteCard]->fColor
				* CARDS_IN_SUIT + fStock[lastWasteCard]->fValue],
				BRect(2 * hSpacing + CARD_WIDTH, 15,
				2 * hSpacing + 2 * CARD_WIDTH, 15 + CARD_HEIGHT));
		else
			DrawBitmap(fEmpty, BRect(2 * hSpacing + CARD_WIDTH, 15,
				2 * hSpacing + 2 * CARD_WIDTH, 15 + CARD_HEIGHT));
	} else if (fWasteCard != -1) {
		if (fWasteCard > 23) {
			fWasteCard = -1;
			
			fPoints -= 100;
			if (fPoints < 0)
				fPoints = 0;
			
			Invalidate();
			return;
		}
		
		while (fStock[fWasteCard]->fRevealed) {
			fWasteCard++;
			if (fWasteCard > 23) {
				fWasteCard = -1;
				
				fPoints -= 100;
				if (fPoints < 0)
					fPoints = 0;
				
				break;
			}
		}
		
		rect = BRect(2 * hSpacing + CARD_WIDTH, 15,
			2 * hSpacing + 2 * CARD_WIDTH, 15 + CARD_HEIGHT);
		
		if (fWasteCard != -1)
			DrawBitmap(
				fCards[fStock[fWasteCard]->fColor
				* CARDS_IN_SUIT + fStock[fWasteCard]->fValue], rect);
		else
			DrawBitmap(fEmpty, rect);
			
	} else
		DrawBitmap(fEmpty, BRect(2 * hSpacing + CARD_WIDTH, 15,
			2 * hSpacing + 2 * CARD_WIDTH, 15 + CARD_HEIGHT));
	
	// foundations
	for (short i = 0; i < 4; i++) {
		BRect rect = BRect((i + 4)*hSpacing + (i + 3)*CARD_WIDTH, 15,
			(i + 4)*hSpacing + (i + 4)*CARD_WIDTH, 15 + CARD_HEIGHT);
		
		if (fFoundations[i]	== -1) {
			DrawBitmap(fEmpty, rect);
		} else {
			DrawBitmap(
				fCards[fFoundationsColors[i] * CARDS_IN_SUIT + fFoundations[i]],
				rect);
		}
	}
	
	// tableaux
	for (short i = 0; i < 7; i++) {
		BRect rect(hSpacing + i * (CARD_WIDTH + hSpacing), 146,
				hSpacing + (i + 1) * CARD_WIDTH + i * hSpacing,
				146 + CARD_HEIGHT);
		if (solitare.fBoard[i] == NULL)
			DrawBitmap(fEmpty, rect);
		else {
			card* currentCard;
			for (currentCard = solitare.fBoard[i]; currentCard != NULL;
					currentCard = currentCard->fNextCard) {
				if (currentCard->fRevealed == false) {
					short numberOfBacks = 0;
						// 1 back
					while (currentCard->fNextCard != NULL
						&& currentCard->fNextCard->fRevealed == false
						&& numberOfBacks < CACHED_BACKS) {
						currentCard = currentCard->fNextCard;
						numberOfBacks++;
					}
					rect.bottom += numberOfBacks * 18;
					DrawBitmap(fBack[numberOfBacks], rect);
					rect.top += 18 * numberOfBacks;
				} else if (currentCard->fEffect != E_HIDDEN) {
					DrawBitmap(fCards[currentCard->fColor * CARDS_IN_SUIT
						+ currentCard->fValue], rect);
					switch (currentCard->fEffect) {
					case E_ALPHA25:
						SetHighColor(0, 85, 0, 63);
						break;
					case E_ALPHA50:
						SetHighColor(0, 85, 0, 127);
						break;
					case E_ALPHA75:
						SetHighColor(0, 85, 0, 190);
						break;
					case E_GREEN:
						SetHighColor(0, 184, 0, 127);
						break;
					case E_RED:
						SetHighColor(255, 0, 0, 127);
						break;
					default:
						SetHighColor(0, 85, 0, 0);
					}
					FillRect(rect);
				}
				rect.top += 18;
				rect.bottom = rect.top + CARD_HEIGHT;
			}
		}
	}

	BString points = BString();
	points << fPoints;

	BFont biggerFont = BFont();
	biggerFont.SetFace(B_BOLD_FACE);
	biggerFont.SetSize(20);

	BFont bigFont = BFont();
	bigFont.SetFace(B_BOLD_FACE);
	bigFont.SetSize(18);

	BFont smallFont = BFont();
	smallFont.SetSize(12);

	SetHighColor(255, 255, 255);

	SetFont(&bigFont);
	DrawString(points,
		BPoint((windowWidth + 10 - bigFont.StringWidth(points)) / 2,
		windowHeight - 15));

	SetFont(&smallFont);
	DrawString(B_TRANSLATE("points"), BPoint((windowWidth + 10
		- smallFont.StringWidth(B_TRANSLATE("points"))) / 2, windowHeight));
	
	if (fWon) {
		SetHighColor(255, 0, 0);
		SetFont(&biggerFont);
		DrawString(B_TRANSLATE("YOU WON"), BPoint((windowWidth + 10
			- biggerFont.StringWidth(B_TRANSLATE("YOU WON"))) / 2,
			windowHeight - 40));
	}
}

void KlondikeView::ReceiveOptionMessage(BMessage* message) {
	SolitareView::ReceiveOptionMessage(message);
	switch (message->what) {
		case kCheatMessage:
			Cheat();
			break;
		case kAutoPlayEnableMessage:
			fAutoPlayEnabled = !fAutoPlayEnabled;
			fAutoPlayEnabledItem->SetMarked(fAutoPlayEnabled);
			fQuickAutoPlayItem->SetEnabled(fAutoPlayEnabled);
			break;
		case kQuickAutoPlayMessage:
			fQuickAutoPlay = !fQuickAutoPlay;
			fQuickAutoPlayItem->SetMarked(fQuickAutoPlay);
			break;
	}
}

void KlondikeView::Pulse()
{
	if (fDoubleClick > -1)
		fDoubleClick--;
	
	if (fAutoPlayStarted) {
		if (fAutoPlayCountdown > -1) {
			fAutoPlayCountdown--;
			return;
		}

		if (!MoveOneToFoundation()) {
			fAutoPlayStarted = false;
			CheckBoard();
		}
		Invalidate();
		
		fAutoPlayCountdown = 1;
	} else
		fAutoPlayCountdown = -1;
}


void KlondikeView::MouseDown(BPoint point)
{
	if (fMouseLock)
		return;
	fMouseLock = true;
	
	uint32 mouse;
	GetMouse(&point, &mouse);
	
	if (mouse == B_SECONDARY_MOUSE_BUTTON) {
		// stop auto-play if it's started
		if (fAutoPlayStarted) {
			fAutoPlayStarted = false;
			return;
		}
		
		if (fQuickAutoPlay) {
			while(MoveOneToFoundation());
			
			Invalidate();
			CheckBoard();
		}
		else {
			fAutoPlayStarted = true;
		}
		
		return;
	}

	int hSpacing = _CardHSpacing();
	
	short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));
	
	if (point.x > (stack + 1) * (CARD_WIDTH + hSpacing))
		return;
	
	// stock
	if (point.y < 15 + CARD_HEIGHT && point.y > 15
		&& stack == 0 && point.x > hSpacing) {
		int revealed = 0;
		for (short i = 0; i < 24; i++)
			if (fStock[i]->fRevealed)
				revealed++;
		
		if (revealed < 24 && ++fWasteCard == 24) {
			fWasteCard = -1;
			
			fPoints -= 100;
			if (fPoints < 0)
				fPoints = 0;
		}
		
		Invalidate();
		return;
	}
	
	// pick up a card from waste
	if (stack == 1 && point.y < 15 + CARD_HEIGHT) {		
		if (fWasteCard == -1)
			return;
		
		if (fDoubleClick == -1)
			fDoubleClick = 1;
		else if (fDoubleClick > -1) {
			_MoveWasteToFoundation();
			
			CheckBoard();	
			Invalidate();
			fDoubleClick = -1;
			
			return;
		}
		
		card* picked = fStock[fWasteCard];
		fPickedCard = picked;
		fIsWasteCardPicked = true;
		
		BMessage msg(B_SIMPLE_DATA);
		msg.AddPointer("view", this);
		BBitmap* img = new BBitmap(
			fCards[picked->fColor * CARDS_IN_SUIT + picked->fValue]);
		
		DragMessage(&msg, img, B_OP_BLEND,
			BPoint((int)(point.x - hSpacing) % (CARD_WIDTH + hSpacing),
			point.y - 15));
		
		Invalidate();
		
		return;
	}
	
	// pick up a card from a foundation
	if (stack > 2 && stack < 7 && point.y < 15 + CARD_HEIGHT) {
		short foundation = stack - 3;
		short value = fFoundations[foundation];
		short color = fFoundationsColors[foundation];
		
		if (fFoundations[foundation] == -1)
			return;
		
		// find picked card
		for (short i = 0; i < CARDS_IN_DECK; i++) {
			if (solitare.fAllCards[i]->fValue == value
				&& solitare.fAllCards[i]->fColor == color)
			fPickedCard = solitare.fAllCards[i];
		}
		
		BMessage msg(B_SIMPLE_DATA);
		msg.AddPointer("view", this);
		BBitmap* img = new BBitmap(
			fCards[fPickedCard->fColor * CARDS_IN_SUIT + fPickedCard->fValue]);
		
		fIsFoundationCardPicked = true;
		fPickedCardBoardPos = foundation;
		fFoundations[foundation]--;
		
		DragMessage(&msg, img, B_OP_BLEND,
			BPoint((int)(point.x - hSpacing) % (CARD_WIDTH + hSpacing),
			point.y - 15));
		
		Invalidate();
		
		return;
	}

	// pick up a stack
	if (stack < 7 && solitare.fBoard[stack] != NULL
		&& point.x > hSpacing && point.y > 2 * 15 + CARD_HEIGHT) {
		// find clicked on card
		int cardNumber = 1;
		card* picked = solitare.fBoard[stack];
		while (picked->fNextCard != NULL) {
			if (point.y - 18 * cardNumber - CARD_HEIGHT - 15 < 18) {
				break;
			}
			picked = picked->fNextCard;
			cardNumber++;
		}
		if (picked->fNextCard == NULL) {
			// on last card, if below than not clicking on card
			if (point.y - 18 * cardNumber - CARD_HEIGHT - 15 >= CARD_HEIGHT) {
				return;
			}
			
			if (fDoubleClick == -1)
				fDoubleClick = 1;
			else if (fDoubleClick > -1 && fAutoPlayEnabled) {
				MoveOneToFoundation(stack, stack);
				
				CheckBoard();	
				Invalidate();
				fDoubleClick = -1;
				
				return;
			}
		}
		
		if (picked->fRevealed == false)
			return;
		
		card* currentCard = picked->fNextCard;
		card* lastCard = picked;
		short pickedHeight = 1;
		for (short i = 1; currentCard != NULL;
				i++) {
			pickedHeight++;
			if ((lastCard->fColor & 1) == (currentCard->fColor & 1))
				return;
			lastCard = currentCard;
			currentCard = currentCard->fNextCard;
		}
		
		fPickedCardBoardPos = stack;
		fPickedCard = picked;
		fIsCardPicked = true;

		solitare._RemoveCardFromPile(stack, picked);

		BMessage msg(B_SIMPLE_DATA);
		msg.AddPointer("view", this);
		BBitmap* img;
		if (pickedHeight == 1)
			img = new BBitmap(
				fCards[picked->fColor * CARDS_IN_SUIT + picked->fValue]);
		else {
			img = new BBitmap(BRect(0, 0, CARD_WIDTH - 1,
				CARD_HEIGHT + (pickedHeight - 1) * 18),
				fBack[0]->ColorSpace(), true);
			BView* imgView = new BView(img->Bounds(), NULL, 0, 0);
			BRect destRect = fBack[0]->Bounds();
			img->AddChild(imgView);
			img->Lock();
			currentCard = picked;

			imgView->SetDrawingMode(B_OP_COPY);
			imgView->DrawBitmap(fCards
				[currentCard->fColor * CARDS_IN_SUIT + currentCard->fValue],
				destRect);
			destRect.top = (pickedHeight - 1) * 18;
			destRect.bottom = destRect.top + CARD_HEIGHT;

			imgView->DrawBitmap(fBack[0], destRect);
				// we don't know the top card yet, so we'll overwrite this

			imgView->SetDrawingMode(B_OP_ALPHA);
			for (short j = 0; j < pickedHeight; j++) {
				destRect.top = j * 18;
				destRect.bottom = destRect.top + CARD_HEIGHT;
				imgView->DrawBitmap(fCards[currentCard->fColor
					* CARDS_IN_SUIT + currentCard->fValue], destRect);
				currentCard = currentCard->fNextCard;
			}
			
			imgView->Sync();
			img->Unlock();
			img->RemoveChild(imgView);
			delete imgView;
		}
		DragMessage(&msg, img, B_OP_BLEND,
			BPoint((int)(point.x - hSpacing) % (CARD_WIDTH + hSpacing),
			point.y - cardNumber * 18 - 131));
		
		Invalidate();
	}
}


void KlondikeView::MouseUp(BPoint point)
{
	if (fIsCardPicked) {
		int hSpacing = _CardHSpacing();
		short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));

		if (stack >= 3 && stack < 7 && point.y < 15 + CARD_HEIGHT) {
			short foundation = stack - 3;
			
			if (fPickedCard->fValue == 0)
				fFoundationsColors[foundation] = fPickedCard->fColor;
			
			if (fFoundationsColors[foundation] == fPickedCard->fColor
				&& fPickedCard->fValue - fFoundations[foundation] == 1) {
				fFoundations[foundation]++;
				if (solitare._FindLastUsed(fPickedCardBoardPos) != NULL)
						solitare._FindLastUsed(fPickedCardBoardPos)->fRevealed = true;
				
				fPoints += 10;
				
				CheckBoard();
			} else
				solitare._AddCardToPile(fPickedCardBoardPos, fPickedCard);
		}
		

		else if (stack >= 0 && stack < 7 && (solitare._FindLastUsed(stack) == NULL
			|| (solitare._FindLastUsed(stack)->fValue - fPickedCard->fValue == 1
			&& (solitare._FindLastUsed(stack)->fColor % 2)
			!= (fPickedCard->fColor % 2))) && fIsCardPicked
			&& point.y > 2 * 15 + CARD_HEIGHT) {
			// attach to stack, only kings on empty fields
			if (!(fPickedCard-> fValue != 12 && solitare.fBoard[stack] == NULL)) {
				solitare._AddCardToPile(stack, fPickedCard);
			
				// reveal last card from pile the cards were from
				if (solitare._FindLastUsed(fPickedCardBoardPos) != NULL) {
					solitare._FindLastUsed(fPickedCardBoardPos)->fRevealed = true;
					
					if (stack != fPickedCardBoardPos)
						fPoints += 5;
				}
			} else
				solitare._AddCardToPile(fPickedCardBoardPos, fPickedCard);
		} else {
			// reattach to old stack
			solitare._AddCardToPile(fPickedCardBoardPos, fPickedCard);
		}

		fIsCardPicked = false;

		Invalidate();
		
		return;
	}
	
	if (fIsWasteCardPicked) {
		int hSpacing = _CardHSpacing();
		short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));
		
		if (stack >= 3 && stack < 7 && point.y < 18 + CARD_HEIGHT) {
			short foundation = stack - 3;
			
			if (fPickedCard->fValue == 0)
				fFoundationsColors[foundation] = fPickedCard->fColor;
			
			if (fFoundationsColors[foundation] == fPickedCard->fColor
				&& fPickedCard->fValue - fFoundations[foundation] == 1) {
				fFoundations[foundation]++;
				fPickedCard->fRevealed = true;
				
				fWasteCard--;
				while (fWasteCard != -1 && fStock[fWasteCard]->fRevealed)
					fWasteCard--;
				
				fPoints += 10;
				
				CheckBoard();
			}
		}
		
		else if (stack >= 0 && stack < 7 && (solitare._FindLastUsed(stack) == NULL
				|| (solitare._FindLastUsed(stack)->fValue - fPickedCard->fValue == 1
				&& (solitare._FindLastUsed(stack)->fColor % 2) != (fPickedCard->fColor % 2)))) {
			// attach to stack, only kings on empty fields
			if (!(fPickedCard-> fValue != 12 && solitare.fBoard[stack] == NULL)) {
				solitare._AddCardToPile(stack, fPickedCard);
			
				fPickedCard->fRevealed = true;
				
				fWasteCard--;
				while (fWasteCard != -1 && fStock[fWasteCard]->fRevealed)
					fWasteCard--;
					
				fPoints += 5;
			}
		}

		fIsWasteCardPicked = false;

		Invalidate();
	}
	
	if (fIsFoundationCardPicked) {
		int hSpacing = _CardHSpacing();
		short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));
		
		if (stack >= 0 && stack < 7 && (solitare._FindLastUsed(stack) == NULL
				|| solitare._FindLastUsed(stack)->fValue - fPickedCard->fValue == 1)) {
			// attach to stack
			solitare._AddCardToPile(stack, fPickedCard);
			
			if (fFoundations[fPickedCardBoardPos] == -1)
				fFoundationsColors[fPickedCardBoardPos] = -1;
			
			fPoints -= 15;
			if (fPoints < 0)
				fPoints = 0;
		} else {
			fFoundations[fPickedCardBoardPos]++;
		}

		fIsFoundationCardPicked = false;

		Invalidate();
	}

	fMouseLock = false;
}


int KlondikeView::_CardHSpacing()
{
	return((windowWidth - (CARD_WIDTH * 7)) / 8);
}


void KlondikeView::Cheat() {
	for (short i = 0; i < 4; i++) {
		fFoundationsColors[i] = i;
		fFoundations[i] = 12;
	}
	
	for (short i = 0; i < 7; i++) {
		solitare._RemoveCardFromPile(i, solitare.fBoard[i]);
	}
	
	for (short i = 0; i < 24; i++) {
		fStock[i]->fRevealed = true;
	}
	
	Invalidate();
	
	CheckBoard();
}


void KlondikeView::_GenerateBoard()
{
	srand(time(NULL));
	
	// create cards
	card* orderedCards[KLONDIKE_CARDS_IN_PLAY];
	for (short i = 0; i < KLONDIKE_CARDS_IN_PLAY; i++) {
		orderedCards[i] = new card();
		orderedCards[i]->fValue = i % CARDS_IN_SUIT; // A->K, repeat
		orderedCards[i]->fColor = (i / CARDS_IN_SUIT) % 4;
			
		orderedCards[i]->fRevealed = false;
		orderedCards[i]->fEffect = E_NONE;
		orderedCards[i]->fInPlay = false;
		orderedCards[i]->fNextCard = NULL;
		orderedCards[i]->fPrevCard = NULL;
	}
	
	// randomize order of card array
	for (short cardsLeft = KLONDIKE_CARDS_IN_PLAY; cardsLeft > 0; cardsLeft--) {
		short randomCard = rand() % cardsLeft;
			// random number between 0 and (52, 51, 50, ...)
		
		// move card to actual deck
		if (solitare.fAllCards[cardsLeft - 1] != NULL) delete solitare.fAllCards[cardsLeft - 1];
		solitare.fAllCards[cardsLeft - 1] = orderedCards[randomCard];
		
		orderedCards[randomCard] = orderedCards[cardsLeft - 1];
		// replace picked card with card at back of deck
	}

	fWasteCard = -1;
	fIsCardPicked = false;
	fIsWasteCardPicked = false;
	fMouseLock = false;
	fWon = false;
	fAutoPlayStarted = false;
	fDoubleClick = -1;
	fAutoPlayCountdown = -1;
	
	for (short i = 0; i < 4; i++) {
		fFoundations[i] = -1;
		fFoundationsColors[i] = -1;
	}

	fPoints = 100;

	short cardsOnPile = 1;
	for (short i = 0; i != 7; i++) {
		solitare.fBoard[i] = NULL;
			// clear
		short j = cardsOnPile;
		for (; j > 0; j--) {
			// for each card
			solitare._AddCardToPile(i, solitare._PickRandomCard());
				// pick a random next card, and add it
		}
		cardsOnPile++;
		card* lastCard = solitare._FindLastUsed(i);
		lastCard->fRevealed = true;
			// at the last card, show it
	}
	
	for (short i = 0; i != 24; i++) {
		fStock[i] = solitare._PickRandomCard();
	}
	
	if (fSoundEnabled)
		fShuffle->StartPlaying();
}


void KlondikeView::CheckBoard() {
	for (short i = 0; i < 4; i++) {
		if (fFoundations[i] < 12)
			return;
	}
	
	if (fSoundEnabled)
		fFanfare->StartPlaying();
	
	fMouseLock = true;
	fWon = true;
}


bool KlondikeView::MoveOneToFoundation(short stack, short endStack) {
	if (!fAutoPlayEnabled)
		return false;
	
	if(_MoveWasteToFoundation())
		return true;
	
	for (short i = stack; i <= endStack; i++) {
		if (solitare.fBoard[i] == NULL)
			continue;
		
		card* currentCard = solitare._FindLastUsed(i);
		short color = currentCard->fColor;
		short value = currentCard->fValue;
		short foundation = -1;
		
		if (value == 0) {
			for (short j = 0; j < 4; j++) {
				if (fFoundationsColors[j] == -1) {
					fFoundationsColors[j] = color;
					fFoundations[j] = 0;
					
					if (currentCard->fPrevCard != NULL)
						currentCard->fPrevCard->fRevealed = true;
					solitare._RemoveCardFromPile(i, currentCard);
					fPoints += 10;
					return true;
				}
			}
			
			continue;
		}
		
		for (short j = 0; j < 4; j++) {
			if (fFoundationsColors[j] == color) {
				foundation = j;
				fFoundationsColors[j] = color;
				break;
			}
		}
				
		if (foundation == -1)
			continue;
		
		if (value - fFoundations[foundation] == 1) {
			if (currentCard->fPrevCard != NULL)
				currentCard->fPrevCard->fRevealed = true;
			solitare._RemoveCardFromPile(i, currentCard);
			fFoundations[foundation]++;
			fPoints += 10;
			return true;
		}
	}
	
	return false;
}


bool KlondikeView::_MoveWasteToFoundation() {
	if (fWasteCard == -1)
		return false;
	
	card* currentCard = fStock[fWasteCard];
	short color = currentCard->fColor;
	short value = currentCard->fValue;
	short foundation = -1;
	
	if (value == 0) {
		for (short j = 0; j < 4; j++) {
			if (fFoundationsColors[j] == -1) {
				fFoundationsColors[j] = color;
				fFoundations[j] = 0;
				
				currentCard->fRevealed = true;
				fWasteCard--;
				while (fWasteCard != -1 && fStock[fWasteCard]->fRevealed)
					fWasteCard--;
				fPoints += 10;

				break;
			}
		}
		
		return true;
	}
	
	for (short j = 0; j < 4; j++) {
		if (fFoundationsColors[j] == color) {
			foundation = j;
			fFoundationsColors[j] = color;
			break;
		}
	}
			
	if (foundation == -1)
		return false;
	
	if (value - fFoundations[foundation] == 1) {
		currentCard->fRevealed = true;
		fWasteCard--;
		while (fWasteCard != -1 && fStock[fWasteCard]->fRevealed)
			fWasteCard--;
		fFoundations[foundation]++;
		fPoints += 10;
		
		return true;
	}
	
	return false;
}

BMenu* KlondikeView::GetOptionMenu() {
	BMenuItem* menuItem;
	BMenu* mOptions = SolitareView::GetOptionMenu();
	
	fAutoPlayEnabledItem = new BMenuItem(B_TRANSLATE("Auto-play"),
	NewSolitareOptionMessage(kAutoPlayEnableMessage));
	if (fAutoPlayEnabled)
		fAutoPlayEnabledItem->SetMarked(true);
	mOptions->AddItem(fAutoPlayEnabledItem);
	
	fQuickAutoPlayItem = new BMenuItem(B_TRANSLATE("Quick auto-play"),
		NewSolitareOptionMessage(kQuickAutoPlayMessage));
	if (fQuickAutoPlay)
		fQuickAutoPlayItem->SetMarked(true);
	
	mOptions->AddItem(fQuickAutoPlayItem);
	mOptions->AddSeparatorItem();
	
	menuItem = new BMenuItem(B_TRANSLATE("Cheat!"),
		NewSolitareOptionMessage(kCheatMessage));
	menuItem->SetShortcut('C', B_COMMAND_KEY);
	mOptions->AddItem(menuItem);
	return mOptions;
}
