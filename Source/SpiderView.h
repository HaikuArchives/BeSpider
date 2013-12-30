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

#define CARD_WIDTH 80
#define CARD_HEIGHT 116


enum effect { E_NONE, E_ALPHA25, E_ALPHA50, E_ALPHA75,
	E_HIDDEN, E_GREEN, E_RED };


typedef struct card {
	short fValue;
	short fColor;
	bool fRevealed;
	short fEffect;
} card;


class SpiderView : public BView {
public:
	SpiderView();

	virtual void Draw(BRect rect);
	virtual void Pulse();
	virtual void MouseDown(BPoint point);
	virtual void MouseMoved(BPoint point,
		uint32 transit, const BMessage* message);
	virtual void MouseUp(BPoint point);

	void NewGame();
	void ChangeDifficulty(int difficulty);
	void Hint();

private:
	BSimpleGameSound* _LoadSound(const char* resourceName);
	void _LoadBitmaps();
	void _GenerateBoard();
	void _CheckBoard();
	short _FindFirstFree(short stock);

	BBitmap* fCards[4][13];
	BBitmap* fBack;
	BBitmap* fEmpty;

	BResources* fResources;
	BSimpleGameSound* fShuffle;
	BSimpleGameSound* fFanfare;

	card fBoard[10][25];
	short fStock;
	short fDealing;
	short fStacking;
	short fStackingCard;
	short fFreeCards[4][13];

	int fColors;
	int fDecks;

	short fPickedCardBoardPos[2];
	BPoint fPickedCardPos;
	BPoint fPickedCardMouse;
	card fPickedCard;
	bool fIsCardPicked;
	bool fIsStackPicked;
	short fLastPickedCardPos;
	bool fMouseLock;

	short fIsHintShown;
	card* fHints[2];
	short fHintBoardPos[2];
	short fNoMoves;

	short fStacked;
	short fStackedColor[8];

	int fPoints;
	int fMoves;
};

#endif // _SPIDERVIEW_H_
