/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderView.h"

#include <Alert.h>
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
	
	for (short i = 0; i != 10; i++)
		for (short j = 0; j != 26; j++)
			if (fBoard[i][j].fValue != -1) {
				if (fBoard[i][j].fRevealed == true) {
					DrawBitmap(fSpades[fBoard[i][j].fValue], BRect(10 + i * (CARD_WIDTH + 10), (j + 1) * 15, 10 + (i + 1) * CARD_WIDTH + i * 10, CARD_HEIGHT + (j + 1) * 15));
				} else {
					DrawBitmap(fBack, BRect(10 + i * (CARD_WIDTH + 10), (j + 1) * 15, 10 + (i + 1) * CARD_WIDTH + i * 10, CARD_HEIGHT + (j + 1) * 15));
				}
			} else break;
	
	for (short i = 0; i != fStock; i++) {
		DrawBitmap(fBack, BRect(900 - CARD_WIDTH - i*15, 390 - CARD_HEIGHT, 900 - i*15, 390));
	}
	
	if (fIsCardPicked)
		DrawBitmap(fSpades[fPickedCard.fValue], BRect(fPickedCardPos.x, fPickedCardPos.y, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT));
	
	if (fIsStackPicked) {
		DrawBitmap(fSpades[fPickedCard.fValue], BRect(fPickedCardPos.x, fPickedCardPos.y, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT));
		for (short i = 0; i != fLastPickedCardPos - fPickedCardBoardPos[1] + 1; i++) {
			DrawBitmap(fSpades[fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1] + i].fValue], BRect(fPickedCardPos.x, fPickedCardPos.y + i * 15, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT + i * 15));
		}
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
			fBoard[i][_FindFirstFree(i)-1].fRevealed = true;
			fFreeCards[random]--;
		}
		
		fStock--;
		Invalidate();
	}

	short stack = (point.x - 10) / (CARD_WIDTH + 10);
	if (stack <= 9) {
		short first = _FindFirstFree(stack);

		if (point.y > 15 * first &&
			point.y < 15 * first + CARD_HEIGHT) {
			fPickedCardBoardPos[0] = stack;
			fPickedCardBoardPos[1] = first-1;
			fPickedCardPos = point;
			fPickedCard = fBoard[stack][first-1];
			fBoard[stack][first-1].fValue = -1;
			fIsCardPicked = true;
			
			Invalidate();
		} else if (point.y < 15 * first + 10) {
			short picked = point.y / 15 - 1;
			
			if (fBoard[stack][picked].fRevealed == false)
				return;
			
			bool pickable = true;
			
			for (short i = picked + 1; i != first; i++) {
				if (fBoard[stack][picked].fValue - fBoard[stack][i].fValue == i - picked) {
					pickable = true;
					fLastPickedCardPos = i;
				}
				else
					pickable = false;
			}
			
			if (pickable) {
				fIsStackPicked = true;
				
				fPickedCardBoardPos[0] = stack;
				fPickedCardBoardPos[1] = picked;
				fPickedCardPos = point;
				fPickedCard = fBoard[stack][picked];
				fBoard[stack][picked].fValue = -1;
				
				Invalidate();
			}
		}
	}
}


void SpiderView::MouseMoved(BPoint point,
	uint32 transit, const BMessage* message) {
	if (transit == B_EXITED_VIEW) {
		MouseUp(point);
		return;
	}
	
	if (fIsCardPicked || fIsStackPicked) {
		fPickedCardPos = point;
		Invalidate();
	}
}


void SpiderView::MouseUp(BPoint point) {
	if (fIsCardPicked) {
		short stack = (point.x - 10) / (CARD_WIDTH + 10);
	
		if (fBoard[stack][_FindFirstFree(stack)-1].fValue - fPickedCard.fValue == 1
			|| _FindFirstFree(stack) == 0) {
			fBoard[stack][_FindFirstFree(stack)] = fPickedCard;
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]-1].fRevealed = true;
		}
		else
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]] = fPickedCard;
	
		fIsCardPicked = false;
	
		Invalidate();
	} else if (fIsStackPicked) {
		short stack = (point.x - 10) / (CARD_WIDTH + 10);
		if (fBoard[stack][_FindFirstFree(stack)-1].fValue - fPickedCard.fValue == 1
			|| _FindFirstFree(stack) == 0) {
			fBoard[stack][_FindFirstFree(stack)] = fPickedCard;
			for (short i = 0; i != fLastPickedCardPos - fPickedCardBoardPos[1] + 1; i++) {
				fBoard[stack][_FindFirstFree(stack)] = fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1] + i];
				fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1] + i].fValue = -1;
			}
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]-1].fRevealed = true;
		}
		else
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]] = fPickedCard;
	
		fIsStackPicked = false;
	
		Invalidate();
	}
	
	_CheckBoard();
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
	fStacked = 0;
	fIsCardPicked = false;
	fIsStackPicked = false;
	
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
	
	for (short i = 0; i != 10; i++) {			
		fBoard[i][_FindFirstFree(i)-1].fRevealed = true;
	}
}


void SpiderView::_CheckBoard()
{
	for (short i = 0; i != 10; i++) {
		short needed = 0;
		bool stacked = true;
		short last = _FindFirstFree(i) - 1;
		while (stacked) {
			if (fBoard[i][last - needed].fValue == needed)
				needed++;
			else
				stacked = false;
		}

		
		if (needed == 13)
			stacked = true;
		
		if (stacked);
			//(new BAlert("AboutAlert", "Ułożyłeś stos!", "OK"))->Go();
	}
}


inline short SpiderView::_FindFirstFree(short stock) {
	short res = 0;
	while (fBoard[stock][res].fValue != -1) res++;
	
	return res;
}
