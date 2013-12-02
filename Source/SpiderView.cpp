/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#include "SpiderView.h"

#include <Alert.h>
#include <Application.h>
#include <Entry.h>
#include <Path.h>
#include <Roster.h>
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
					DrawBitmap(fCards[fBoard[i][j].fColor][fBoard[i][j].fValue], BRect(10 + i * (CARD_WIDTH + 10), (j + 1) * 15, 10 + (i + 1) * CARD_WIDTH + i * 10, CARD_HEIGHT + (j + 1) * 15));
				} else {
					DrawBitmap(fBack, BRect(10 + i * (CARD_WIDTH + 10), (j + 1) * 15, 10 + (i + 1) * CARD_WIDTH + i * 10, CARD_HEIGHT + (j + 1) * 15));
				}
			} else break;
	
	for (short i = 0; i != fStacked; i++)
		DrawBitmap(fCards[fStackedColor[i]][12], BRect(i*15 + 10, 390 - CARD_HEIGHT, i*15 + 10 + CARD_WIDTH, 390));
	
	for (short i = 0; i != fStock; i++)
		DrawBitmap(fBack, BRect(900 - CARD_WIDTH - i*15, 390 - CARD_HEIGHT, 900 - i*15, 390));
	
	if (fIsCardPicked)
		DrawBitmap(fCards[fPickedCard.fColor][fPickedCard.fValue], BRect(fPickedCardPos.x, fPickedCardPos.y, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT));
	
	if (fIsStackPicked) {
		DrawBitmap(fCards[fPickedCard.fColor][fPickedCard.fValue], BRect(fPickedCardPos.x, fPickedCardPos.y, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT));
		for (short i = 0; i != fLastPickedCardPos - fPickedCardBoardPos[1] + 1; i++) {
			DrawBitmap(fCards[fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1] + i].fColor][fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1] + i].fValue], BRect(fPickedCardPos.x, fPickedCardPos.y + i * 15, fPickedCardPos.x + CARD_WIDTH, fPickedCardPos.y + CARD_HEIGHT + i * 15));
		}
	}
	
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
	DrawString(points, BPoint(455 - bigFont.StringWidth(points) / 2, 340));
	
	SetFont(&smallFont);
	DrawString("points", BPoint(455 - bigFont.StringWidth("points") / 2, 355));
	
	SetFont(&bigFont);
	DrawString(moves, BPoint(455 - bigFont.StringWidth(points) / 2, 375));
	
	SetFont(&smallFont);
	DrawString("moves", BPoint(455 - bigFont.StringWidth("moves") / 2, 390));
}


void SpiderView::MouseDown(BPoint point)
{
	if (point.x > (900 - CARD_WIDTH - fStock * 15) && point.x < 900
		&& point.y > 390 - CARD_HEIGHT && point.y < 390) {
		if (fStock == 0) return;
		for (short i = 0; i != 10; i++) {
			short random = rand() % 13;
			short rcolor = rand() % fColors;
			while (fFreeCards[rcolor][random] == 0) {
				random = rand() % 13;
				rcolor = rand() % fColors;
			}

			fBoard[i][_FindFirstFree(i)].fColor = rcolor;
			fBoard[i][_FindFirstFree(i)].fValue = random;
			fBoard[i][_FindFirstFree(i)-1].fRevealed = true;
			fFreeCards[rcolor][random]--;
		}
		
		fStock--;
		
		fShuffle->StartPlaying();
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
			short color = fBoard[stack][picked].fColor; 
			
			for (short i = picked + 1; i != first; i++) {
				if (fBoard[stack][picked].fValue - fBoard[stack][i].fValue == i - picked
					&& fBoard[stack][i].fColor == color) {
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
	uint32 transit, const BMessage* message)
{
	if (transit == B_EXITED_VIEW) {
		MouseUp(point);
		return;
	}
	
	if (fIsCardPicked || fIsStackPicked) {
		fPickedCardPos = point;
		Invalidate();
	}
}


void SpiderView::MouseUp(BPoint point)
{
	if (fIsCardPicked) {
		short stack = (point.x - 10) / (CARD_WIDTH + 10);
	
		if (fBoard[stack][_FindFirstFree(stack)-1].fValue - fPickedCard.fValue == 1
			|| _FindFirstFree(stack) == 0) {
			fBoard[stack][_FindFirstFree(stack)] = fPickedCard;
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]-1].fRevealed = true;
			
			fPoints--;
			fMoves++;
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
			
			fPoints--;
			fMoves++;
		}
		else
			fBoard[fPickedCardBoardPos[0]][fPickedCardBoardPos[1]] = fPickedCard;
	
		fIsStackPicked = false;
	
		Invalidate();
	}
	
	_CheckBoard();
}


void SpiderView::NewGame()
{
	_GenerateBoard();
	Invalidate();
}


void SpiderView::ChangeDifficulity(int difficulity)
{
	switch (difficulity) {
	default:
	case 0:
		fColors = 1;
		fDecks = 8;
		break;
	case 1:
		fColors = 2;
		fDecks = 4;
		break;
	case 2:
		fColors = 4;
		fDecks = 2;
		break;
	}
	
	NewGame();
}


void SpiderView::_LoadBitmaps()
{
	for (short i = 0; i != 14; i++) {
		BString filename = BString();
		filename << "Artwork/" << i + 1 << "_spade.png";
		fCards[0][i] = BTranslationUtils::GetBitmapFile(filename.String());
		
		filename = BString();
		filename << "Artwork/" << i + 1 << "_heart.png";
		fCards[1][i] = BTranslationUtils::GetBitmapFile(filename.String());
		
		filename = BString();
		filename << "Artwork/" << i + 1 << "_club.png";
		fCards[2][i] = BTranslationUtils::GetBitmapFile(filename.String());
		
		filename = BString();
		filename << "Artwork/" << i + 1 << "_diamond.png";
		fCards[3][i] = BTranslationUtils::GetBitmapFile(filename.String());
	}

	fCards[1][0] = BTranslationUtils::GetBitmapFile("Artwork/1_heart.png");
	fCards[2][0] = BTranslationUtils::GetBitmapFile("Artwork/1_club.png");
	fCards[3][0] = BTranslationUtils::GetBitmapFile("Artwork/1_diamond.png");

	fBack = BTranslationUtils::GetBitmapFile("Artwork/back.png");
	
	/* Relative paths given to BSimpleGameSound work
	 * only when run from console */
	app_info info;
	be_app->GetAppInfo(&info);
	BPath* appDir = new BPath(&info.ref);
	appDir->GetParent(appDir);
	
	BString shufflePath, fanfarePath = BString();
	shufflePath << appDir->Path() << "/Artwork/shuffle.wav";
	fanfarePath << appDir->Path() << "/Artwork/fanfare.wav";
	
	entry_ref shuffleRef, fanfareRef;

	BEntry shuffleEntry = BEntry(shufflePath.String());
	shuffleEntry.GetRef(&shuffleRef);
	
	BEntry fanfareEntry = BEntry(fanfarePath.String());
	shuffleEntry.GetRef(&fanfareRef);
	
	fShuffle = new BSimpleGameSound(&shuffleRef);
	fFanfare = new BSimpleGameSound(&fanfareRef);
	
	Invalidate();
}


void SpiderView::_GenerateBoard()
{
	for (short i = 0; i != 13; i++)
		for (short j = 0; j != 4; j++)
			fFreeCards[j][i] = fDecks;
	
	fStock = 4;
	fStacked = 0;
	fIsCardPicked = false;
	fIsStackPicked = false;
	
	fPoints = 200;
	fMoves = 0;
	
	for (short i = 0; i != 10; i++)
		for (short j = 0; j != 25; j++) {
			fBoard[i][j].fValue = -1;
			fBoard[i][j].fRevealed = false;
		}
	
	srand(time(NULL));

	for (short i = 0; i != 6; i++)
		for (short j = 0; j != 10; j++) {			
			short random = rand() % 13;
			short rcolor = rand() % fColors;
			while (fFreeCards[rcolor][random] == 0) {
				rcolor = rand() % fColors;
				random = rand() % 13;
			}
			
			fBoard[j][i].fColor = rcolor;
			fBoard[j][i].fValue = random;
			fFreeCards[rcolor][random]--;
		}
	
	for (short i = 0; i != 4; i++) {
		short random = rand() % 13;
		short rcolor = rand() % fColors;
		while (fFreeCards[rcolor][random] == 0) {
			rcolor = rand() % fColors;
			random = rand() % 13;
		}
		
		fBoard[i][6].fColor = rcolor;
		fBoard[i][6].fValue = random;
		fFreeCards[rcolor][random]--;
	}
	
	for (short i = 0; i != 10; i++) {			
		fBoard[i][_FindFirstFree(i)-1].fRevealed = true;
	}
	
	fShuffle->StartPlaying();
}


void SpiderView::_CheckBoard()
{
	for (short i = 0; i != 10; i++) {
		short needed = 0;
		bool stacked = true;
		short last = _FindFirstFree(i) - 1;
		short color = fBoard[i][last].fColor;
		while (stacked) {
			if (fBoard[i][last - needed].fValue == needed
				&& fBoard[i][last - needed].fColor == color)
				needed++;
			else
				stacked = false;
		}

		
		if (needed == 13)
			stacked = true;
		
		if (stacked) {
			short first = _FindFirstFree(i);
			fStackedColor[fStacked] = fBoard[i][first].fColor;
			fStacked++;
			
			for (short j = 1; j != 15; j++) {
				fBoard[i][first-j].fValue = -1;
			}
			
			fBoard[i][_FindFirstFree(i)-1].fRevealed = true;
			
			fPoints += 100;
			
			Invalidate();
			
			fShuffle->StartPlaying();
		}
	}
	
	if (fStacked == 8) {
		fFanfare->StartPlaying();
		(new BAlert("WinAlert", "YOU WON!", "OK!"))->Go();
	}
}


short SpiderView::_FindFirstFree(short stock) {
	short res = 0;
	while (fBoard[stock][res].fValue != -1) res++;
	
	return res;
}
