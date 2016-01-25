/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERVIEW_H_
#define _SPIDERVIEW_H_


#include <Bitmap.h>
#include <Entry.h>
#include <FileGameSound.h>
#include <Point.h>
#include <Rect.h>
#include <Resources.h>
#include <SimpleGameSound.h>
#include <View.h>

#include "Spider.h"

#define STARTING_WINDOW_WIDTH 900
#define STARTING_WINDOW_HEIGHT 490
#define CARD_WIDTH 80
#define CARD_HEIGHT 116
#define CARDS_IN_SUIT 13
#define CARD_IMAGE_BACK 52
#define CARD_IMAGE_EMPTY 53
#define CACHED_BACKS 6


enum effect { E_NONE, E_ALPHA25, E_ALPHA50, E_ALPHA75,
	E_HIDDEN, E_GREEN, E_RED };


class SpiderView : public BView {
public:
	SpiderView();
	~SpiderView();

	virtual void AllAttached();
	virtual void Draw(BRect rect);
	virtual void Pulse();
	virtual void MouseDown(BPoint point);
	virtual void MouseMoved(BPoint point,
		uint32 transit, const BMessage* message);
	virtual void MouseUp(BPoint point);

	void NewGame();
	void ChangeDifficulty(int difficulty);
	void Hint();
	void Resize(float newWidth, float newHeight);

private:
	BSimpleGameSound* _LoadSound(const char* resourceName);
	void _LoadBitmaps();
	void _GenerateBoard();
	void _CheckBoard();
	int _CardHSpacing();

	BBitmap* fCards[CARDS_IN_DECK];
	BBitmap* fBack[CACHED_BACKS];
	BBitmap* fEmpty;

	BResources* fResources;
	BSimpleGameSound* fShuffle;
	BSimpleGameSound* fFanfare;

	int windowWidth;
	int windowHeight;

	short fStock; // number of stocks left
	short fDealing; // the card that will become opaque next, -1 if none
	short fStacking; // the pile that is stacking, -1 if none
	short fStackingCard; // the card in the pile that will stack next, -1 if none

	int fColors; // difficulty

	short fPickedCardBoardPos; // pile picked card is from
	card* fPickedCard;
	bool fIsCardPicked;
	bool fMouseLock;

	short fIsHintShown;
	card* fHints[2];
	short fHintStatus[2];
	short fNoMoves;

	short fStacked;
	short fStackedColor[8];

	int fPoints;
	int fMoves;

	Spider spider;
};

#endif // _SPIDERVIEW_H_
