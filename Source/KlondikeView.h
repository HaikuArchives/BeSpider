/*
 * Copyright 2013-2014 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _KLONDIKEVIEW_H_
#define _KLONDIKEVIEW_H_


#include <Bitmap.h>
#include <Entry.h>
#include <FileGameSound.h>
#include <Point.h>
#include <Rect.h>
#include <Resources.h>
#include <SimpleGameSound.h>
#include <View.h>

#include "Solitare.h"

#define KLONDIKE_CARDS_IN_PLAY CARDS_IN_DECK

class KlondikeView : public BView {
public:
								KlondikeView();
								~KlondikeView();

	virtual	void				AllAttached();
	virtual	void				Draw(BRect rect);
	virtual	void				Pulse();
	virtual	void				MouseDown(BPoint point);
	virtual	void				MouseUp(BPoint point);

			void				NewGame();
			void				Cheat();
			void				SetAutoPlay(bool enabled, bool quick);
			bool				MoveOneToFoundation(short stack = 0,
									short endStack = 6);
			void				Resize(float newWidth, float newHeight);
			void				CheckBoard();

			
			bool				fAutoPlayStarted;


private:
			BSimpleGameSound*	_LoadSound(const char* resourceName);
			void				_LoadBitmaps();
			void				_GenerateBoard();
			bool				_MoveWasteToFoundation();
			int					_CardHSpacing();

			BBitmap*			fCards[CARDS_IN_DECK];
			BBitmap*			fBack[CACHED_BACKS];
			BBitmap*			fEmpty;

			BResources*			fResources;
			BSimpleGameSound*	fShuffle;
			BSimpleGameSound*	fFanfare;

			int					windowWidth;
			int					windowHeight;

			card*				fStock[24];
			short				fWasteCard;
			bool				fIsWasteCardPicked;
			short				fFoundations[4];
			short				fFoundationsColors[4];
			bool				fIsFoundationCardPicked;
			
			bool				fWon;
			short				fPickedCardBoardPos;
				// pile picked card is from
			card*				fPickedCard;
			bool				fIsCardPicked;
			bool				fMouseLock;

			bool				fAutoPlayEnabled;
			bool				fQuickAutoPlay;
			short				fDoubleClick;
			short				fAutoPlayCountdown;

			int					fPoints;
			Solitare solitare;
};


#endif // _KLONDIKEVIEW_H_
