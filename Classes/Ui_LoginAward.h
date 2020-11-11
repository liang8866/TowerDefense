#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "Connect.h"
class Ui_LoginAward : public Widget
{
public:
	Ui_LoginAward(void);
	~Ui_LoginAward(void);
	virtual bool init();
	CREATE_FUNC(Ui_LoginAward);
	virtual void onEnter() override;
	virtual void onExit() override;

	void eventTouchButton(Ref* sender, TouchEventType type);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	bool clickFlag ;
	void fitScreen();
private:
	Widget*  _widget;
	ValueMap _config;
};

