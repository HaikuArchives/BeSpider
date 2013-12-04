/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERWINDOW_H_
#define _SPIDERWINDOW_H__

#include "SpiderView.h"

#include <Invoker.h>
#include <Window.h>


const int32 kNewGameMessage = 'NewG';
const int32 kDifficultyMessage = 'Diff';
const int32 kDiffChosenMessage = 'DiCh';


class SpiderWindow : public BWindow {
public:
	SpiderWindow(BRect frame, const char* title);

	virtual void MessageReceived(BMessage* message);

private:
	BMenuBar* _CreateMenuBar();

	SpiderView* fView;
	BInvoker* fDiffSet;
};

#endif	// _SPIDERWINDOW_H_
