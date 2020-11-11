#pragma once
#include "cocos2d.h"
#include "GameConfig.h"

class Window : public Widget
{
public:
	Window(void);
	~Window(void);

public:
	bool		initialise(const char* fileName);
	Widget*		findChild(const char* _widgetName);
public:
	const char* mWindowName;
	const char* mJsonName;
	size_t		mState;

	Widget*		mMainWidget;

	void fitScreen();
};

