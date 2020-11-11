#pragma once
#include "cocos2d.h"
#include "Window.h"
#include "GameConfig.h"
#include "ReadyScene.h"
#include "UserRecord.h"
#include "audioPlayer.h"
class Ui_MessageBox : public Widget
{
public:
	Ui_MessageBox(void);
	~Ui_MessageBox(void);
public:
	

	virtual bool init() override;
	CREATE_FUNC(Ui_MessageBox);

	virtual void onEnter() override;
	virtual void onExit() override;

	Widget* findChild(const char* _widgetName);
	bool showMessageBox();
	bool closeMessageBox();
	void eventTouchButton(Ref* sender, TouchEventType type);

	void pauseGame(float dt);

	void musicLock();
	void effectLock();

	void fitScreen();

	void setPause_StoreyObject(bool _isBool);		// 设置层上的某些物件暂停
public:

	Button*		mButtonEnter;
	Button*		mButtonCencel;
	Button*     mMusicBtn;
	Button*     mEffectBtn;
	Button*     mCloseBtn;

	EventListenerTouchOneByOne *_listener;

private:
	Widget    *mMainWidget;

	ImageView *disable_icon_music;
	ImageView *disable_icon_effect;
	UserRecord*userData;
	audioPlayer *audPlayer;
};

