
#ifndef __UI_BAG_MESSAGE_H__
#define __UI_BAG_MESSAGE_H__

#include "GameConfig.h"
#include "levelScene.h"

USING_NS_CC;

class Ui_Bag_Message:public Widget
{
public:
	Ui_Bag_Message(void);
   ~Ui_Bag_Message(void);

	virtual bool init() override;
	CREATE_FUNC(Ui_Bag_Message);
	virtual void onEnter() override;
	virtual void onExit() override;
public:
	void onButtonEvent_Message(Ref *pSender , TouchEventType type);	// Messageµã»÷ 
	void updateView(int _itemId);
	void updateView(int _itemId,const int itemCount);
	void setWidgetGray(Widget* _widget, bool _isGray);
	void fitScreen();
public:
	Widget*		mMainWidget;

	Button*		mButtonEnterMessage;
	ImageView*	mImageItemIconMessage;
	Text*		mTextItemCountMessage;

	vector<_ItemBoxData> mBoxItems;

	int			mItemId;
};
#endif