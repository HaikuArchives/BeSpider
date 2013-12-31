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
#define CARDS_IN_DECK 52
#define CARDS_IN_PLAY CARDS_IN_DECK*2
#define CARDS_IN_SUIT 13
#define CARD_IMAGE_BACK 52
#define CARD_IMAGE_EMPTY 53
#define CACHED_BACKS 6


enum effect { E_NONE, E_ALPHA25, E_ALPHA50, E_ALPHA75,
	E_HIDDEN, E_GREEN, E_RED };


typedef struct card {
	short fValue;
	short fColor;
	bool fRevealed;
	short fEffect;
	bool fInPlay;
	card* fNextCard;
	card* fPrevCard;
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
	card* _FindLastUsed(short stock);
	card* _PickRandomCard();
	void _AddCardToPile(int pile, card* cardToAdd);
	void _RemoveCardFromPile(int pile, card* cardToRemove);

	BBitmap* fCards[CARDS_IN_DECK];
	BBitmap* fBack[CACHED_BACKS];
	BBitmap* fEmpty;

	BResources* fResources;
	BSimpleGameSound* fShuffle;
	BSimpleGameSound* fFanfare;

	card* fBoard[10]; // first card in each pile
	short fStock; // number of stocks left
	short fDealing; // the card that will become opaque next, -1 if none
	short fStacking; // the pile that is stacking, -1 if none
	short fStackingCard; // the card in the pile that will stack next, -1 if none
	card* fAllCards[CARDS_IN_PLAY]; // all cards

	int fColors; // difficulty

	short fPickedCardBoardPos; // pile picked card is from
	BPoint fPickedCardPos;
	BPoint fPickedCardMouse;
	card* fPickedCard;
	bool fIsCardPicked;
	short fLastPickedCardPos;
	bool fMouseLock;

	short fIsHintShown;
	card* fHints[2];
	short fHintStatus[2];
	short fNoMoves;

	short fStacked;
	short fStackedColor[8];

	int fPoints;
	int fMoves;
};

#endif // _SPIDERVIEW_H_
