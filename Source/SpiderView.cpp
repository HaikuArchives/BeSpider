/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderView.h"

#include <Alert.h>
#include <Catalog.h>
#include <Entry.h>
#include <Path.h>
#include <MenuItem.h>
#include <Roster.h>
#include <String.h>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#undef B_TRANSLATION_CONTEXT
#define B_TRANSLATION_CONTEXT "BeSpider"


SpiderView::SpiderView(BMessage* settings)
	:
	SolitareView(BRect(0, 0, STARTING_WINDOW_WIDTH+10, STARTING_WINDOW_HEIGHT+10), "SpiderView",
		B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW | B_PULSE_NEEDED | B_FULL_UPDATE_ON_RESIZE, settings)
{
	//Load settings
	LoadSettings(settings); 	
}


SpiderView::~SpiderView()
{
}

status_t SpiderView::LoadSettings(BMessage* settings)
{
	SolitareView::LoadSettings(settings);
	
	if (!settings)
		return B_BAD_VALUE;
	
	// In case the value is not stored in settings, use default	
	if (settings->FindInt32("spider colors", &fColors) != B_OK)
		fColors = 1;
	
	return B_OK;
}

status_t SpiderView::SaveSettings(BMessage* settings)
{
	settings->SetInt32("spider colors", fColors);
	return SolitareView::SaveSettings(settings);
}

void SpiderView::Draw(BRect rect)
{
	//clock_t start, end;
	//start = clock();
	SetDrawingMode(B_OP_ALPHA);
	
	int hSpacing = _CardHSpacing();
	
	for(short i = 0; i < 10; i++) {
		BRect rect(hSpacing + i * (CARD_WIDTH + hSpacing), 15,
				hSpacing + (i + 1) * CARD_WIDTH + i * hSpacing, 15 + CARD_HEIGHT);
		if(solitare.fBoard[i] == NULL)
			DrawBitmap(fEmpty, rect);
		else {
			card* currentCard;
			for(currentCard = solitare.fBoard[i]; currentCard != NULL;
					currentCard = currentCard->fNextCard) {
				if(currentCard->fRevealed == false) {
					short numberOfBacks = 0; // 1 back
					while(currentCard->fNextCard != NULL &&
							currentCard->fNextCard->fRevealed == false &&
							numberOfBacks < CACHED_BACKS) {
						currentCard = currentCard->fNextCard;
						numberOfBacks++;
					}
					rect.bottom += numberOfBacks*15;
					DrawBitmap(fBack[numberOfBacks], rect);
					rect.top += 15*numberOfBacks;
				} else if(currentCard->fEffect != E_HIDDEN) {
					DrawBitmap(
						fCards[currentCard->fColor*CARDS_IN_SUIT+currentCard->fValue],
						rect);
					switch(currentCard->fEffect) {
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
						SetHighColor(0, 204, 0, 127);
						break;
					case E_RED:
						SetHighColor(255, 0, 0, 127);
						break;
					default:
						SetHighColor(0, 85, 0, 0);
					}
					FillRect(rect);
				}
				rect.top += 15;
				rect.bottom = rect.top + CARD_HEIGHT;
			}
		}
	}
	
	for (short i = 0; i != fStacked; i++)
		DrawBitmap(fCards[fStackedColor[i]*CARDS_IN_SUIT+CARDS_IN_SUIT-1],
			BRect(i*15 + 10, windowHeight - CARD_HEIGHT, i*15 + 10 + CARD_WIDTH, windowHeight));

	for (short i = 0; i != fStock; i++)
		DrawBitmap(fBack[0], BRect(windowWidth - CARD_WIDTH - i*15,
			windowHeight - CARD_HEIGHT, windowWidth - i*15, windowHeight));

	BString points = BString();
	points << fPoints;
	BString moves = BString();
	moves << fMoves;

	BFont bigFont = BFont();
	bigFont.SetFace(B_BOLD_FACE);
	bigFont.SetSize(18);

	BFont smallFont = BFont();
	smallFont.SetSize(12);

	SetHighColor(255,255,255);

	SetFont(&bigFont);
	DrawString(points, BPoint((windowWidth+10 - bigFont.StringWidth(points)) / 2, windowHeight-50));

	SetFont(&smallFont);
	DrawString(B_TRANSLATE("points"), BPoint((windowWidth+10
		- smallFont.StringWidth(B_TRANSLATE("points"))) / 2, windowHeight-35));

	SetFont(&bigFont);
	DrawString(moves, BPoint((windowWidth+10 - bigFont.StringWidth(points)) / 2, windowHeight-15));

	SetFont(&smallFont);
	DrawString(B_TRANSLATE("moves"), BPoint((windowWidth+10
		- smallFont.StringWidth(B_TRANSLATE("moves"))) / 2, windowHeight));

	if (fNoMoves > -1) {
		SetFont(&bigFont);
		SetHighColor(255, 0, 0);

		DrawString(B_TRANSLATE("there are no constructive moves!"),
			BPoint((windowWidth+10 - bigFont.StringWidth(
			B_TRANSLATE("there are no constructive moves!"))) / 2, windowHeight-75));
	}
	
	//end = clock();
	//clock_t diff = end - start;
	//printf("Time: %.6f\n", diff / (double)CLOCKS_PER_SEC);
}

void SpiderView::ReceiveOptionMessage(BMessage* message) {
	SolitareView::ReceiveOptionMessage(message);
	BAlert* question;
	switch (message->what) {
		case sDifficultyMessage:
			question = new BAlert("DiffAlert", B_TRANSLATE("Choose difficulty level."),
				B_TRANSLATE("Easy (1 color)"), B_TRANSLATE("Medium (2 colors)"),
				B_TRANSLATE("Hard (4 colors)"),
				B_WIDTH_AS_USUAL, B_IDEA_ALERT);
			ChangeDifficulty(question->Go());
			break;
	}
}

BMenu* SpiderView::GetOptionMenu() {
	BMenuItem* menuItem;
	BMenu* mOptions = SolitareView::GetOptionMenu();
	
	menuItem = new BMenuItem(B_TRANSLATE("Change difficulty"), NewSolitareOptionMessage(sDifficultyMessage));
	menuItem->SetShortcut('D', B_COMMAND_KEY);
	mOptions->AddItem(menuItem);

	return mOptions;
}

void SpiderView::Pulse()
{
	if (fDealing > 9) {
		fDealing = -1;
		Window()->SetPulseRate(500000);
		_CheckBoard();
	}

	if (fDealing != -1) { // dealing a new row
		card* lastCard = solitare._FindLastUsed(fDealing);
		
		switch (lastCard->fEffect) {
		case E_ALPHA75:
			lastCard->fEffect = E_ALPHA50;
			break;
		case E_ALPHA50:
			lastCard->fEffect = E_ALPHA25;
			// start next animation
			if(fDealing < 9)
				solitare._FindLastUsed(fDealing+1)->fEffect = E_ALPHA75;
			break;
		case E_ALPHA25:
			lastCard->fEffect = E_NONE;
			fDealing++; // move to next card
			break;
		}

		Invalidate();
	} else if (fStacking != -1) {
		card* lastCard = solitare._FindLastUsed(fStacking);
		
		switch (lastCard->fEffect) {
		case E_ALPHA25:
			lastCard->fEffect = E_ALPHA50;
			break;
		case E_ALPHA50:
			lastCard->fEffect = E_ALPHA75;
			break;
		case E_ALPHA75:
			// start next animation (if next card not stacking,
			//   it will be reset later)
			if(lastCard->fPrevCard != NULL)
				lastCard->fPrevCard->fEffect = E_ALPHA25;
			// detach current card
			solitare._RemoveCardFromPile(fStacking, lastCard);
			// move to next card
			fStackingCard++;
			break;
		}

		Invalidate();

		if (fStackingCard == 14) {
			card* lastCard = solitare._FindLastUsed(fStacking);
			if(lastCard != NULL) {
				lastCard->fRevealed = true;
				lastCard->fEffect = E_NONE;
			}
			if(fStacked == 8)
				(new BAlert("WinAlert", B_TRANSLATE("YOU WON!"),
					B_TRANSLATE_CONTEXT("OK!", "Win alert")))->Go();

			fStacking = -1;
			Window()->SetPulseRate(500000);
		}
	} else if (fIsHintShown > 0)
		fIsHintShown--;
	else if (fIsHintShown == 0) {
		for (card* currentCard = fHints[0]; currentCard != NULL;
				currentCard = currentCard->fNextCard) {
			if(currentCard->fEffect == E_GREEN)
				currentCard->fEffect = E_NONE;
		}

		fHints[1]->fEffect = E_NONE;

		fIsHintShown = -1;

		Invalidate();
	} else if (fNoMoves > -1) {
		fNoMoves--;
		if(fNoMoves == -1)
			Invalidate();
	}
}


void SpiderView::MouseDown(BPoint point)
{
	if (fDealing != -1 || fIsHintShown != -1 || fStacking != -1 || fMouseLock)
		return;

	fMouseLock = true;

	int hSpacing = _CardHSpacing();

	// use a stock
	if (point.x > (windowWidth - CARD_WIDTH - fStock * 15) && point.x < windowWidth
		&& point.y > windowHeight - CARD_HEIGHT && point.y < windowHeight) {
		if (fStock == 0) return;
		for (short i = 0; i != 10; i++) {
			// add a random card to the pile
			card* randomCard = solitare._PickRandomCard();
			solitare._AddCardToPile(i, randomCard);

			// hide card
			randomCard->fEffect = E_HIDDEN;
			randomCard->fRevealed = true;
			
			if(i == 0) { // if first pile
				// start appearing animation
				randomCard->fEffect = E_ALPHA75;
				fDealing = 0;
				Window()->SetPulseRate(50000);
			}
		}

		fStock--; // one less stock available

		if (fSoundEnabled)
			fShuffle->StartPlaying();
		
		Invalidate();
	}

	// pick up a stack
	short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));
	if (stack <= 9 && solitare.fBoard[stack] != NULL) {
		// find clicked on card
		int cardNumber = 1;
		card* picked = solitare.fBoard[stack];
		while(picked->fNextCard != NULL) {
			if(point.y - 15 * cardNumber < 15) {
				break;
			}
			picked = picked->fNextCard;
			cardNumber++;
		}
		if(picked->fNextCard == NULL) {
			// on last card, if below than not clicking on card
			if(point.y - 15 * cardNumber >= CARD_HEIGHT) {
				return;
			}
		}
		
		if(picked->fRevealed == false)
			return;
		
		card* currentCard = picked->fNextCard;
		short pickedHeight = 1;
		for(short i = 1; currentCard != NULL;
				i++) {
			pickedHeight++;
			if(picked->fColor != currentCard->fColor ||
					picked->fValue - currentCard->fValue != i)
				return;
			currentCard = currentCard->fNextCard;
		}
		
		fPickedCardBoardPos = stack;
		fPickedCard = picked;
		fIsCardPicked = true;

		solitare._RemoveCardFromPile(stack, picked);

		BMessage msg(B_SIMPLE_DATA);
		msg.AddPointer("view", this);
		BBitmap* img;
		if(pickedHeight == 1)
			img = new BBitmap(fCards[picked->fColor*CARDS_IN_SUIT+picked->fValue]);
		else {
			img = new BBitmap(BRect(0, 0, CARD_WIDTH-1, CARD_HEIGHT + (pickedHeight-1) * 15), fBack[0]->ColorSpace(), true);
			BView* imgView = new BView(img->Bounds(), NULL, 0, 0);
			BRect destRect = fBack[0]->Bounds();
			img->AddChild(imgView);
			img->Lock();
			currentCard = picked;
			
			imgView->SetDrawingMode(B_OP_COPY);
			imgView->DrawBitmap(fCards
				[currentCard->fColor*CARDS_IN_SUIT+currentCard->fValue],
				destRect);
			destRect.top = (pickedHeight-1)*15;
			destRect.bottom = destRect.top + CARD_HEIGHT;
			// we don't know the top card yet, so we'll overwrite this
			imgView->DrawBitmap(fBack[0], destRect);
			
			imgView->SetDrawingMode(B_OP_ALPHA);
			for(short j = 0; j < pickedHeight; j++) {
				destRect.top = j*15;
				destRect.bottom = destRect.top + CARD_HEIGHT;
				imgView->DrawBitmap(fCards[currentCard->fColor*CARDS_IN_SUIT+currentCard->fValue], destRect);
				currentCard = currentCard->fNextCard;
			}
			
			imgView->Sync();
			img->Unlock();
			img->RemoveChild(imgView);
			delete imgView;
		}
		DragMessage(&msg, img, B_OP_BLEND,
			BPoint((int)(point.x - hSpacing) % (CARD_WIDTH + hSpacing),
			point.y - cardNumber * 15));
		
		Invalidate();
	}
}


void SpiderView::MouseMoved(BPoint point,
	uint32 transit, const BMessage* message)
{
	/*if (transit == B_EXITED_VIEW) {
		MouseUp(point);
		return;
	}*/
}


void SpiderView::MouseUp(BPoint point)
{
	if (fIsCardPicked) {
		int hSpacing = _CardHSpacing();
		short stack = (int)((point.x - hSpacing) / (CARD_WIDTH + hSpacing));

		if(stack >= 0 && stack < 10 && (solitare._FindLastUsed(stack) == NULL ||
				solitare._FindLastUsed(stack)->fValue - fPickedCard->fValue == 1)) {
			// attach to stack
			solitare._AddCardToPile(stack, fPickedCard);
			
			// reveal last card from pile the cards were from
			if(solitare._FindLastUsed(fPickedCardBoardPos) != NULL)
				solitare._FindLastUsed(fPickedCardBoardPos)->fRevealed = true;
			
			fPoints--;
			fMoves++;
		} else {
			// reattach to old stack
			solitare._AddCardToPile(fPickedCardBoardPos, fPickedCard);
		}

		fIsCardPicked = false;

		_CheckBoard();
		Invalidate();
	}

	fMouseLock = false;
}


int SpiderView::_CardHSpacing()
{
	return((windowWidth - (CARD_WIDTH*10)) / 11);
}


void SpiderView::ChangeDifficulty(int difficulty)
{
	switch (difficulty) {
	default:
	case 0:
		fColors = 1;
		break;
	case 1:
		fColors = 2;
		break;
	case 2:
		fColors = 4;
	}

	NewGame();
}


void SpiderView::Hint()
{
	if (fIsHintShown != -1 || fDealing != -1)
		return;

	card* highestCard[10];
	short stocksValues[10];

	for (short i = 0; i != 10; i++) {
		highestCard[i] = solitare._FindLastUsed(i);
		if(highestCard[i] == NULL) {
			stocksValues[i] = -1;
			continue;
		}
		stocksValues[i] = highestCard[i]->fValue;

		for (card* currentCard = highestCard[i]->fPrevCard;
				currentCard != NULL; currentCard = currentCard->fPrevCard) {
			if (currentCard->fRevealed == false || 
					currentCard->fValue - stocksValues[i] != 1)
				break;

			highestCard[i] = currentCard;
			stocksValues[i] = currentCard->fValue;
		}
	}

	
	short status = 0;
	short x = fHintStatus[0];
	short y = fHintStatus[1]+1;
	
	for(;; x = (x+1) % 10) {
		for(; y < 10; y++) {
			if(solitare._FindLastUsed(y) != NULL &&
					solitare._FindLastUsed(y)->fValue - stocksValues[x] == 1) {
				status = 1; // found a match
				break;
			}
			if(x == fHintStatus[0] && y == fHintStatus[1]) {
				status = 2; // didn't find a match
				break;
			}
		}
		if(status != 0) break;
		y = 0;
	}
	
	fHintStatus[0] = x;
	fHintStatus[1] = y;
	
	if(status == 1) {
		fIsHintShown = 2;
		fHints[0] = highestCard[x];
		fHints[1] = solitare._FindLastUsed(y);

		for (card* currentCard = fHints[0];
				currentCard != NULL; currentCard = currentCard->fNextCard)
			currentCard->fEffect = E_GREEN;

		fHints[1]->fEffect = E_RED;
	} else {
		fNoMoves = 4;
	}

	Invalidate();
}


void SpiderView::_GenerateBoard()
{
	srand(time(NULL));
	
	// create cards
	card* orderedCards[SPIDER_CARDS_IN_PLAY];
	for (short i = 0; i < SPIDER_CARDS_IN_PLAY; i++) {
		orderedCards[i] = new card();
		orderedCards[i]->fValue = i % CARDS_IN_SUIT; // A->K, repeat
		orderedCards[i]->fColor = (i/CARDS_IN_SUIT) % fColors;
		orderedCards[i]->fRevealed = false;
		orderedCards[i]->fEffect = E_NONE;
		orderedCards[i]->fInPlay = false;
		orderedCards[i]->fNextCard = NULL;
		orderedCards[i]->fPrevCard = NULL;
	}
	
	// randomize order of card array
	for(short cardsLeft = SPIDER_CARDS_IN_PLAY; cardsLeft > 0; cardsLeft--) {
		// random number between 0 and (52, 51, 50, ...)
		short randomCard = rand() % cardsLeft;
		
		// move card to actual deck
		if(solitare.fAllCards[cardsLeft-1] != NULL) delete solitare.fAllCards[cardsLeft-1];
		solitare.fAllCards[cardsLeft-1] = orderedCards[randomCard];
		
		// replace picked card with card at back of deck
		orderedCards[randomCard] = orderedCards[cardsLeft-1];
	}

	fStock = 4;
	fDealing = -1;
	fStacking = -1;
	fStackingCard = -1;
	fStacked = 0;
	fIsCardPicked = false;
	fIsHintShown = -1;
	fNoMoves = -1;
	fMouseLock = false;
	fHintStatus[0] = 0;
	fHintStatus[1] = 0;

	fPoints = 200;
	fMoves = 0;

	for (short i = 0; i != 10; i++) {
		solitare.fBoard[i] = NULL; // clear
		short j = 6; // usually 6 cards
		if(i < 4) j = 7; // if first 4 piles, 5 cards
		for(; j > 0; j--) { // for each card
			// pick a random next card, and add it
			solitare._AddCardToPile(i, solitare._PickRandomCard());
		}
		// at the last card, show it
		card* lastCard = solitare._FindLastUsed(i);
		lastCard->fRevealed = true;
		lastCard->fEffect = E_HIDDEN;
		if(i == 0) { // if the first pile
			// start showing animation
			lastCard->fEffect = E_ALPHA75;
			fDealing = 0;
			Window()->SetPulseRate(50000);
		}
	}

	if (fSoundEnabled)
		fShuffle->StartPlaying();
}


void SpiderView::_CheckBoard()
{
	for (short i = 0; i != 10; i++) {
		short needed = 0;
		bool stacked = true;
		card* currentCard = solitare._FindLastUsed(i);
		if(currentCard == NULL)
			continue;
		short color = currentCard->fColor;

		while(stacked) {
			if(currentCard != NULL
				&& currentCard->fValue == needed
				&& currentCard->fColor == color) {
				needed++;
				currentCard = currentCard->fPrevCard;
			} else
				stacked = false;
		}

		if (needed == CARDS_IN_SUIT)
			stacked = true;

		if (stacked) {
			card* first = solitare._FindLastUsed(i);
			fStackedColor[fStacked] = first->fColor;
			fStacking = i;
			fStackingCard = 1;
			fStacked++;

			first->fEffect = E_ALPHA25;
			Window()->SetPulseRate(50000);

			fPoints += 100;

			Invalidate();
			
			if (fSoundEnabled) {
				if (fStacked == 8) {
					fFanfare->StartPlaying();
				} else
					fShuffle->StartPlaying();
			}
		}
	}
}
