/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERWINDOW_H_
#define _SPIDERWINDOW_H__

#include "SpiderView.h"

#include <Window.h>


const int32 kNewGameMessage = 'NewG';
const int32 kDifficulityMessage = 'Diff';


class SpiderWindow : public BWindow {
public:
	SpiderWindow(BRect frame, const char* title);
	
	virtual void MessageReceived(BMessage* message);

private:
	BMenuBar* _CreateMenuBar();
	
	SpiderView* fView;
};

#endif	// _SPIDERWINDOW_H_
