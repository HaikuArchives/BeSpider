/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERVIEW_H_
#define _SPIDERVIEW_H_


#include <Bitmap.h>
#include <Point.h>
#include <Rect.h>
#include <View.h>

#define CARD_WIDTH 80
#define CARD_HEIGHT 116


typedef struct card {
	short fValue = -1;
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

private:
	void _LoadBitmaps();
	void _GenerateBoard();
	void _CheckBoard();
	inline short _FindFirstFree(short stock);
	
	BBitmap* fSpades[13];
	BBitmap* fBack;
	
	card fBoard[10][25];
	short fFreeCards[13];
	short fStock;
	
	short fPickedCardBoardPos[1];
	BPoint fPickedCardPos;
	card fPickedCard;
	bool fIsCardPicked;
	bool fIsStackPicked;
	short fLastPickedCardPos;
};

#endif // _SPIDERVIEW_H_
