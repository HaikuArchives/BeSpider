/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderView.h"

#include <String.h>
#include <TranslationUtils.h>

#include <stdlib.h>


SpiderView::SpiderView()
	: BView(BRect(0, 0, 910, 400), "SpiderView", B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW)
{
	SetViewColor(0, 85, 0);
	
	_LoadBitmaps();
	
	_GenerateBoard();
}


void SpiderView::Draw(BRect rect)
{
	SetDrawingMode(B_OP_ALPHA);
	
	for (short i = 0; i != 15; i++) 
		for (short j = 0; j != 10; j++)
			if (fBoard[j][i].fValue != -1)
				DrawBitmap(fSpades[fBoard[j][i].fValue], BRect(10 + j * (CARD_WIDTH + 10), (i + 1) * 15, 10 + (j + 1) * CARD_WIDTH + j * 10, CARD_HEIGHT + (i + 1) * 15));
	
	for (short i = 0; i != fStock; i++) {
		DrawBitmap(fBack, BRect(900 - CARD_WIDTH - i*15, 390 - CARD_HEIGHT, 900 - i*15, 390));
	}
}


void SpiderView::MouseDown(BPoint point)
{
	if (point.x > (900 - CARD_WIDTH - fStock * 15) && point.x < 900
		&& point.y > 390 - CARD_HEIGHT && point.y < 390) {
		if (fStock == 0) return;
		for (short i = 0; i != 10; i++) {
			short random = rand() % 13;
			while (fFreeCards[random] == 0)
				random = rand() % 13;

			fBoard[i][_FindFirstFree(i)].fValue = random;
			fFreeCards[random]--;
		}
		
		fStock--;
		Invalidate();
	}
}


void SpiderView::_LoadBitmaps()
{
	for (short i = 0; i != 14; i++) {
		BString filename = BString();
		filename << "Artwork/" << i + 1 << "_spade.png";
		
		fSpades[i] = BTranslationUtils::GetBitmapFile(filename.String());
	}
	
	fBack = BTranslationUtils::GetBitmapFile("Artwork/back.png");
}


void SpiderView::_GenerateBoard()
{
	for (short i = 0; i != 13; i++)
		fFreeCards[i] = 8;
	
	fStock = 4;
	
	srand(time(NULL));

	for (short i = 0; i != 6; i++)
		for (short j = 0; j != 10; j++) {			
			short random = rand() % 13;
			while (fFreeCards[random] == 0)
				random = rand() % 13;
			
			fBoard[j][i].fValue = random;
			fFreeCards[random]--;
		}
	
	for (short i = 0; i != 4; i++) {
		short random = rand() % 13;
		while (fFreeCards[random] == 0)
			random = rand() % 13;
		
		fBoard[i][6].fValue = random;
		fFreeCards[random]--;
	}
}


short SpiderView::_FindFirstFree(short stock) {
	short res = 0;
	while (fBoard[stock][res].fValue != -1) res++;
	
	return res;
}
