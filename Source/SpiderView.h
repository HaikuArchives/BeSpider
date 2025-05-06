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
#include <Menu.h>
#include <Rect.h>
#include <Resources.h>
#include <SimpleGameSound.h>
#include <View.h>

#include "SolitareView.h"
#include "Solitare.h"

const uint32 sNewGameMessage = 'NewG';
const uint32 sDifficultyMessage = 'Diff';
const uint32 sDiffChosenMessage = 'DiCh';
const uint32 sHintMessage = 'Hint';

class SpiderView : public SolitareView {
public:
	SpiderView(BMessage* settings);
	~SpiderView();

	virtual void Draw(BRect rect);
	virtual void Pulse();
	virtual void MouseDown(BPoint point);
	virtual void MouseMoved(BPoint point,
		uint32 transit, const BMessage* message);
	virtual void MouseUp(BPoint point);
	virtual void ReceiveOptionMessage(BMessage* message);
	virtual BMenu* GetOptionMenu();
	
	virtual void Hint();
	
	status_t LoadSettings(BMessage* settings);
	status_t SaveSettings(BMessage* settings);

private:
	void _GenerateBoard();
	void _CheckBoard();
	int _CardHSpacing();
	void ChangeDifficulty(int difficulty);

	short fStock; // number of stocks left
	short fDealing; // the card that will become opaque next, -1 if none
	short fStacking; // the pile that is stacking, -1 if none
	short fStackingCard; // the card in the pile that will stack next, -1 if none

	int32 fColors; // difficulty

	short fIsHintShown;
	card* fHints[2];
	short fHintStatus[2];
	short fNoMoves;

	short fStacked;
	short fStackedColor[8];
	
	int fMoves;
};

#endif // _SPIDERVIEW_H_
