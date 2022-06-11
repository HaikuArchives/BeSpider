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
const uint32 sToggleSoundMessage = 'TSnd';

class SpiderView : public SolitareView {
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
	virtual void ReciveOptionMessage(BMessage* message);
	virtual BMenu* GetOptionMenu();
	
	virtual void NewGame();
	virtual void Resize(float newWidth, float newHeight);
	virtual void Hint();

private:
	BSimpleGameSound* _LoadSound(const char* resourceName);
	void _LoadBitmaps();
	void _GenerateBoard();
	void _CheckBoard();
	int _CardHSpacing();
	void ChangeDifficulty(int difficulty);

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
	
	bool		fSoundEnabled;
	
	BMenuItem*	fToggleSoundItem;
	
	int fPoints;
	int fMoves;

	Solitare solitare;
};

#endif // _SPIDERVIEW_H_
