#pragma once
#include "GameConfig.h"
#include "UserRecord.h"

class Ui_Login : public Widget
{
public:
	Ui_Login();
	~Ui_Login();
	virtual bool init();
	CREATE_FUNC(Ui_Login);
	virtual void onEnter() override;
	virtual void onExit() override;

	void fitScreen();
	void eventTouchButton(Ref* sender, TouchEventType type);
	void selectServer(Ref* sender, TouchEventType type);
	void closeServerList(Ref* sender, TouchEventType type);
	void btnComeOut1(Ref*r);
	void btnComeOut2(Ref*r);
	void fadeOutMoveOut(Widget* sender);
	void fadeInMoveIn(Widget* sender);
	Widget* _widget;
	Widget* _warp;
	Button* playBtn;


	ImageView*	disable_icon_music;
	ImageView*	disable_icon_effect;
	Button*		mMusicBtn;
	Button*		mEffectBtn;
	UserRecord* userData;
};



