#ifndef __UI_SHOP_H__
#define __UI_SHOP_H__

#include "GameConfig.h"
#include "Connect.h"
#include "Ui_HeroAltar.h"
#include "audioPlayer.h"
using namespace std;

enum ShopItem
{
	SweetItem=1,
	MagicItem,
	MaterialItem,
	BoxItem,
	GoldItem,
	SoulItem,
	ClothesItem
};

enum ShopItemZorder
{
	ClothesItemZorder = 4,
	SoulItemZorder,
	GoldItemZorder,
	BoxItemZorder,
	MaterialItemZorder,
	MagicItemZorder,
	SweetItemZorder,
	ShopHighLightZorder=15,
};

class ShopItemInfo
{
public:
	string	   shop_id;
	string     item_id;
	int        count;
	int        price;
	int        payType;
	string     name;
	string     introduce;
	string	   item_icon;
	int		   limit_count; //限制数量
	Widget*    widget;
	string     payid;
};

class Ui_Shop:public Widget
{
public:
	Ui_Shop();
	~Ui_Shop();

	virtual bool init();
	CREATE_FUNC(Ui_Shop);

	virtual void onEnter() override;
	virtual void onExit() override;

	void fitScreen();
	void pauseGame(float dt);
	void resumeGame();
	void selectIndex(ShopItem idx);
	void freshData(ShopItem idx);

	void itemBtnClickedCallFuc(Ref *pSender , TouchEventType type);
	void initItemZOrder();
	void updateItemInfo(int idx);

	void buyBtnEventCallFunc(Ref *pSender , TouchEventType type);
	void close(Ref *pSender , TouchEventType type);
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void buyPromptCallFunc();

	void createNoEnoughGoldFrame(float dt);
	void goldNoEnoughCallFunc();

	void delayUpdateItemInfo(float);
	CC_SYNTHESIZE(ShopItem,_selectedItem,SelectedItem);
	CC_SYNTHESIZE(int,_isFirstTime,isFirstTime);//第一次初始化的时候不播放声音

	void fadeInAction(Node *pSender,float delay);

	void heroAltarUpdateCallBack(Widget* _window);
	void createMoveAction(Widget* _widget, float _speedTime = 0.5f, int _disY = 5);
	void createScaleAction(Widget* _widget, float _speedTime = 0.5f, float _scale = 0.1f);

	void buyGoodsSuccessEffect();

	void buySoulSuccessCallfuc(Ref *obj);

public:
	vector<ShopItemInfo> _itemInfo;
	int      clickedBtnTag;
	string   noEnoughString;
private:
	Widget   *m_widget;
	ListView *listView;

	Button   *sweetBtn;
	Button   *magicBtn;
	Button   *materialBtn;
	Button   *boxBtn;
	Button   *soulBtn; 
	Button   *goldBtn; 
	Button   *clothesBtn;
	Ui_HeroAltar*  mHeroAltarWindow;
};



#endif // !__UI_SHOP_H__
