
#ifndef __UI_BAG_H__
#define __UI_BAG_H__

#include "GameConfig.h"
#include "levelScene.h"
#include "network/HttpClient.h"
#include "Ui_Bag_Message.h"

USING_NS_CC;

// vector升序排序
class CompLess  
{  
public:  
	bool operator ()(const _BagItemInfo& _item1, const _BagItemInfo& _item2)  
	{  
		if (_item1._type == _item2._type)
		{
			return _item1._id < _item2._id;
		}
		return _item1._type < _item2._type;
	}  
}; 
 

class Ui_Bag:public Widget
{
public:
	Ui_Bag(void);
   ~Ui_Bag(void);

	virtual bool init() override;
	CREATE_FUNC(Ui_Bag);
	virtual void onEnter() override;
	virtual void onExit() override;
public:
	void onButtonEvent_Tag(Ref *pSender , TouchEventType type);		// 标签页切换
	void onButtonEvent(Ref *pSender , TouchEventType type);			// 按钮事件
	void onButtonEvent_Item(Ref *pSender , TouchEventType type);	// Item点击 
	
	void updateData();	// 更新背包内的数据

	void selectPage(int _pageIndex);
	void selectItem(int _itemIndex);

	void resetTagButton();
	void setWidgetGray(Widget* _widget, bool _isGray);
	void setMessageVisible(bool _visible); 
	int getItemNum();

	void addOpenBoxAnimation(const char* id);
	void removeArmature(float dt);
	void onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void fadeInAction(Node *pSender,float delay);

	void fitScreen();

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void equipClothes(int itemid);
public:
	vector<_BagItemInfo> mBagItems;
	vector<_BagItemInfo> mShowItems;

	Widget*		mMainWidget;

	Button*		mButton1;		// 全部
	Button*		mButton2;		// 宝箱
	Button*		mButton3;		// 材料
	Button*		mButton4;		// 魔法
	Button*		mButton5;		// 道具
	Button*     mButton6;       // 时装
	int			mCurPageIndex;	// 当前标签页索引

	Button*		mButtonUse;			// 使用
	Button*		mButtonCancle;		// 返回

	// 选中格子详细信息
	TextBMFont*		mTextSelectDesc;
	TextBMFont*		mTextSelectName;
	TextBMFont*		mTextSelectCount;
	ImageView*	mImageSelectIcon;

	// 选中item格子
	Widget*		mCurSelectItem;
	int			mCurSelectItemIndex;

	Button*		mButtonShop;
	Text*		mTextMoney;
	Text*		mTextHeroSoul;

	ui::ScrollView*	mScrollViewBag;	// ScrollView

	string _BoxType_ItemIds;

	Armature* _boxArmature;
	bool _isPlayFinish; //动画是否播放完
	bool _isHttpFinish;//数据是否返回


	int mCellMaxCount;	// 一个格子最大叠加数量

	Ui_Bag_Message* _messagWidget;
	void request();
	void cancelRequest();
	void showError();
};
#endif