
#ifndef __UI_BAG_H__
#define __UI_BAG_H__

#include "GameConfig.h"
#include "levelScene.h"
#include "network/HttpClient.h"
#include "Ui_Bag_Message.h"

USING_NS_CC;

// vector��������
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
	void onButtonEvent_Tag(Ref *pSender , TouchEventType type);		// ��ǩҳ�л�
	void onButtonEvent(Ref *pSender , TouchEventType type);			// ��ť�¼�
	void onButtonEvent_Item(Ref *pSender , TouchEventType type);	// Item��� 
	
	void updateData();	// ���±����ڵ�����

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

	Button*		mButton1;		// ȫ��
	Button*		mButton2;		// ����
	Button*		mButton3;		// ����
	Button*		mButton4;		// ħ��
	Button*		mButton5;		// ����
	Button*     mButton6;       // ʱװ
	int			mCurPageIndex;	// ��ǰ��ǩҳ����

	Button*		mButtonUse;			// ʹ��
	Button*		mButtonCancle;		// ����

	// ѡ�и�����ϸ��Ϣ
	TextBMFont*		mTextSelectDesc;
	TextBMFont*		mTextSelectName;
	TextBMFont*		mTextSelectCount;
	ImageView*	mImageSelectIcon;

	// ѡ��item����
	Widget*		mCurSelectItem;
	int			mCurSelectItemIndex;

	Button*		mButtonShop;
	Text*		mTextMoney;
	Text*		mTextHeroSoul;

	ui::ScrollView*	mScrollViewBag;	// ScrollView

	string _BoxType_ItemIds;

	Armature* _boxArmature;
	bool _isPlayFinish; //�����Ƿ񲥷���
	bool _isHttpFinish;//�����Ƿ񷵻�


	int mCellMaxCount;	// һ����������������

	Ui_Bag_Message* _messagWidget;
	void request();
	void cancelRequest();
	void showError();
};
#endif