#ifndef __UI_HEROSELECT_H__
#define __UI_HEROSELECT_H__

#include "GameConfig.h"
using namespace std;

class HeroSelectItemInfo
{
public:
	int			index;
	int			price;
	int			heroType;
	bool		isUnLock;
	bool		isDead;
	string		name;
	string		keyName;
	string		iconName;
	Widget*		item;
};

class Ui_HeroSelect:public Widget
{
public:
	Ui_HeroSelect();
	~Ui_HeroSelect();

	virtual bool init();
	CREATE_FUNC(Ui_HeroSelect);
	virtual void onEnter() override;
	virtual void onExit() override;

	//////////////////////////////
	//CC_SYNTHESIZE(HeroItemInfo, _selectedItem, SelectedItem);

	//////////////////////////////
	void onListViewEvent(Ref* sender, ListViewEventType type);
	void onButtonEvent(Ref* sender, TouchEventType type);
	void onButtonEvent_In(Ref* sender, TouchEventType type);
	void onButtonEvent_Out(Ref* sender, TouchEventType type);


	//////////////////////////////
	void updateListView();
	Widget* addListViewItem();

	void showView(int _storeyIndex, bool _isAuto, bool _isFree = false);
	void hideView();

	void setWidgetGray(Widget* _widget, bool _isGray);

	bool getDisHeroInScene(int _selfType);
	bool isStoreyHero(int _storeyIndex);
	void setIsFree(bool _isFree);
	void setIsDoEvent(bool _isBool);

	void fadeInAction(Node *pSender , float delay);
	void fitScreen();
public:
	vector<HeroSelectItemInfo> mVectorItems;
	ListView*	mListViewHeroSelect;

private:
	Widget*		m_widget;

	int			mCurStoreyIndex;
	bool		mIsAutoRemove;

	Button*		mButtonClose;
	bool		mIsFree;		// 是否免费
	bool		mIsDoEvent103;		// 是否有103后续事件
};



#endif
