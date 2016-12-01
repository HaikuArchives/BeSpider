/*
 * Copyright 2013 Przemysław Buczkowski <przemub@przemub.pl>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef _SPIDERAPPLICATION_H_
#define _SPIDERAPPLICATION_H_

#include "SelectorWindow.h"

#include <Application.h>


class SpiderWindow;


class SpiderApplication : public BApplication {
public:
	SpiderApplication();

	virtual void AboutRequested();

private:
	SelectorWindow* fWindow;
};


#endif	// _SPIDERAPPLICATION_H_
