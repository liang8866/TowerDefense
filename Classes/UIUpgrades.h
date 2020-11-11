
#ifndef __UI_UPGRADES_H__
#define __UI_UPGRADES_H__

#include "GameConfig.h"
#include "levelScene.h"
#include "network/HttpClient.h"

USING_NS_CC;

enum TechTreeItem{
	TechEconomyItemIdx,
	TechTowerItemIdx,
	TechTrapItemIdx,
	TechSoliderItemIdx,
	TechHeroItemIdx,
	TechMagicItemIdx,
};

class TechItemInfo
{
public:

	int			needMoney;
	string		des;
	bool		isSelect;
	float		val;
	bool		isGradeUp;  //能不能升级
	Widget*		itemBack;	
};

class UIUpgrades:public Widget
{
public:
	
	UIUpgrades(void);
   ~UIUpgrades(void);

	virtual bool init() override;

	CREATE_FUNC(UIUpgrades);

	vector<TechItemInfo> mSelectItemInfo;

	int selectedIndex;
	
public:
	void BookMarkEventCallFunc(Ref *pSender , TouchEventType type);//上面的书签页切换的回掉
	void closeEventCallFunc(Ref *pSender , TouchEventType type);//关闭按钮的回调
    void ShopEventCallFunc(Ref *pSender , TouchEventType type);//转去商店的回调
	void resetEventCallFunc(Ref *pSender , TouchEventType type);//重设的按钮回调
	void upgradeEventCallFunc(Ref *pSender , TouchEventType type);//升级的按钮回调
	//model上的按钮回调
	void modeleventCallFunc(Ref *pSender , TouchEventType type);
	void refreshMarkBook(int index);

	void updateSelectedFramePos(int tag);

	void updateSelectItemIcon();
	void updateSelectItemInfo();

	void confirmResetCallBack();

	void setValue(string &key , int &itemCtn , Widget* &curr_model);

	CC_SYNTHESIZE(TechTreeItem,_selectedItem,SelectedItem);

	virtual void onEnter() override;
	virtual void onExit() override;
	void listenerCallFunc(Ref *obj);
	void fitScreen();
	int currentTag;

	//是否在连网中
	bool _connecting;
	//当前升级的类型
	std::string upType;
	//当前升级的item
	std::string upItem;
	//当前升级的等级
	int upLevel;

	//升级天赋网络回调
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	
	//重置天赋网络回调
	void onHttpRequestCompletedReset(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	
	//发送请求
	void doRequest();
	void showError(int errType);
	void beginAnim_UpSkillLevel();	// 成功特效
	void endAnim_UpSkillLevel(Ref *sender);
	Animate* createAnimation(const char*FrameName,int max);
public:
	Widget *colorBg; 
	Widget *mainWidget;
	Button *Btn_tech,*Btn_magic,*Btn_war,*Btn_hero,*Btn_tower,*Btn_trap;
	Button *closeBtn;//关闭按钮
	Button *resetBtn;//重设按钮
	
	//升级花费的
	Button *costBtn;//升级按钮
	Text *costText;
	
	//youMoneyBg 下面的
	Button *shopBtn;//转去商店的按钮
	TextBMFont *MoneyText;

	//describebg下面的
	Widget*descWidget;
	TextBMFont *descText;
	TextBMFont *descPrice; //价格

	Widget *model5;//5列模板,25个按钮
	Widget *model4;//4列模板,20个按钮
	Widget *model3;//3列模板,15个按钮

	ImageView* selectFrame; //选中框

	Point FixedPos;//固定的位置的
	Point OutPos;//移动到外面的位置

	string resetPrompt; //洗点提示文字

	Sprite*	mSkillLevelSprite;	// 成功特效
};
#endif