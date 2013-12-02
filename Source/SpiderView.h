/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERVIEW_H_
#define _SPIDERVIEW_H_


#include <Bitmap.h>
#include <Point.h>
#include <Rect.h>
#include <SimpleGameSound.h>
#include <View.h>

#define CARD_WIDTH 80
#define CARD_HEIGHT 116


typedef struct card {
	short fValue = -1;
	short fColor = 0;
	bool fRevealed = false;	
} card;


class SpiderView : public BView {
public:
	SpiderView();
	
	virtual void Draw(BRect rect);
	virtual void MouseDown(BPoint point);
	virtual void MouseMoved(BPoint point,
		uint32 transit, const BMessage* message);
	virtual void MouseUp(BPoint point);
	
	void NewGame();
	void ChangeDifficulity(int difficulity);

private:
	void _LoadBitmaps();
	void _GenerateBoard();
	void _CheckBoard();
	short _FindFirstFree(short stock);
	
	BBitmap* fCards[4][13];
	BBitmap* fBack;
	
	BSimpleGameSound* fShuffle;
	BSimpleGameSound* fFanfare;
	
	card fBoard[10][25];
	short fStock;

	// Removing this line makes app not working...
	short fFreeCards[13];
	
	int fColors = 1;
	int fDecks = 8;
	
	short fPickedCardBoardPos[1];
	BPoint fPickedCardPos;
	card fPickedCard;
	bool fIsCardPicked;
	bool fIsStackPicked;
	short fLastPickedCardPos;
	
	short fStacked;
	
	int fPoints;
	int fMoves;
	
	short fAnFreeCards[4][14];
};

#endif // _SPIDERVIEW_H_
