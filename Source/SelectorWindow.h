/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SELECTOR_H
#define SELECTOR_H

#include "BeSpiderWindow.h"

#include <SupportDefs.h>
#include <Rect.h>
#include <RadioButton.h>
#include <GroupLayout.h>
#include <Button.h>

enum selector_messages { SELECTOR_RADIO1, SELECTOR_RADIO2,  SELECTOR_START, SELECTOR_KLONDIKE, SELECTOR_SPIDER };

class SelectorWindow : public BeSpiderWindow {
public:
	SelectorWindow(BRect frame, const char* title, BMessage* settings);
	
	virtual void MessageReceived(BMessage* message);
	virtual status_t SaveSettings(BMessage* settings);
private:
	BRadioButton* radio1;
	BRadioButton* radio2;
	BButton* button;
	bool klondike;
};


#endif // SELECTOR_H
