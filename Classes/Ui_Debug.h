#ifndef __Ui_DEBUG_H__
#define __Ui_DEBUG_H__

#include "cocos2d.h"
#include "UserRecord.h"

USING_NS_CC;
using namespace std;

class Ui_Debug : public Widget
{
public:
	Ui_Debug();
	~Ui_Debug();

	static Ui_Debug* getInstance();
	void destroyInstance();
public:
	virtual bool init() override;
	CREATE_FUNC(Ui_Debug);

	virtual void onEnter() override;
	virtual void onExit() override;

public:
	Widget*		mWidget;
	
	Button*			mButtonClose;
	Button*			mButtonClear;
	Text*			mTextInfo;
	ui::ScrollView*	mScrollView;

	bool			mIsRun;
public:
	void onButtonEvent(Ref *pSender , TouchEventType type);

	void addText(string _text);

	void setRun(bool _isRun);
};

#endif