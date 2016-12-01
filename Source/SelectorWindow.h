/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SELECTOR_H
#define SELECTOR_H


#include <SupportDefs.h>
#include <Window.h>
#include <Rect.h>
#include <RadioButton.h>
#include <GroupLayout.h>
#include <Button.h>

class SelectorWindow: public BWindow {
public:
	SelectorWindow(BRect frame, const char* title);
private:
	BGroupLayout* layout;
	BRadioButton* radio1;
	BRadioButton* radio2;
	BButton* button;
};


#endif // SELECTOR_H
