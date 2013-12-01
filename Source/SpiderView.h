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
	bool fRevealed = true;	
} card;


class SpiderView : public BView {
public:
	SpiderView();
	
	virtual void Draw(BRect rect);
	virtual void MouseDown(BPoint point);
								
private:
	void _LoadBitmaps();
	void _GenerateBoard();
	short _FindFirstFree(short stock);
	
	BBitmap* fSpades[13];
	BBitmap* fBack;
	
	card fBoard[10][15];
	short fFreeCards[13];
	short fStock;
};

#endif // _SPIDERVIEW_H_
