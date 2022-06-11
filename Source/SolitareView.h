/*
 * Copyright 2015 Your Name <your@email.address>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef SOLITAREVIEW_H
#define SOLITAREVIEW_H


#include <SupportDefs.h>

#include <View.h>
#include <Menu.h>
#include <Rect.h>

#define OPTION_MESSAGE_TYPE 'OPTI'
extern const char* OPTION_MESSAGE_LABEL;

BMessage* NewSolitareOptionMessage(uint32 what);

class SolitareView: public BView {
public:
	SolitareView(BRect frame, const char *name, uint32 resizingMode, uint32 flags);
	
	virtual void NewGame();
	virtual BMenu* GetOptionMenu();
	virtual void Resize(float newWidth, float newHeight);
	virtual void ReciveOptionMessage(BMessage* message);
	virtual void Hint();
	
private:
};


#endif // SOLITAREVIEW_H
