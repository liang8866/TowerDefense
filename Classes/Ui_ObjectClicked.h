#ifndef __UI_OBJECT_CLICKED_H__
#define __UI_OBJECT_CLICKED_H__

#include "GameConfig.h"
#include "GameObjects.h"
#include "network/HttpClient.h"

class Ui_ObjectClicked : public Widget
{
public:
	Ui_ObjectClicked();
	~Ui_ObjectClicked();

	virtual bool init();
	CREATE_FUNC(Ui_ObjectClicked);

	void removeSelf(Ref *obj);

	void clickedSoldier(Ref *obj , Point pos);			//点击士兵相应的函数
	void clickedTower(Ref *obj ,Point pos);				//点击哨塔相应的函数
	void clickedTrap(Ref *obj , Point pos);   //点击陷阱相应的函数
	void clickedDeathHero(Ref *obj ,Point pos, int selftype, int costSweet, int costSoul, bool isGoldReviveMode);			//点击死亡的英雄相应的函数


	void soldierUpGradeBtnCallFuc(Ref *pSender,TouchEventType type);
	void soldierRemoveBtnCallFuc(Ref *pSender,TouchEventType type);

	void towerUpGradeBtnCallFuc(Ref *pSender,TouchEventType type);
	void towerRemoveBtnCallFuc(Ref *pSender,TouchEventType type);

	void trapUpGradeBtnCallFuc(Ref *pSender,TouchEventType type);
	void trapRemoveBtnCallFuc(Ref *pSender,TouchEventType type);

	void heroReviveBtnCallFunc(Ref *pSender,TouchEventType type);
	void heroReviveSoonBtnCallFunc(Ref *pSender,TouchEventType type);

	void deathHeroReviveGoldBtnCallFuc(Ref *pSender,TouchEventType type);
	void deathHeroReviveSoulBtnCallFuc(Ref *pSender,TouchEventType type);
	void deathHeroCancleReviveBtnCallFuc(Ref *pSender,TouchEventType type);

	bool	onTouchBegan(Touch *touch, Event *unused_event) ;
	void	onTouchMoved(Touch *touch, Event *unused_event) ; 
	void	onTouchEnded(Touch *touch, Event *unused_event) ;

	void onHttpRequestCompleted_ReviveSoul(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	SEL_COMM m_CallFunc;
	Ref *mObj;
	void setSelector(Ref* obj,SEL_COMM callfunc);

	void resumeObjStatus();

	void update(float fDelta);

private:
	Widget*    m_widget;
	Button*    m_selected_Btn; 

	Point	mHeroRevivePos;
	int		mHeroReviveSweet;
	int		mHeroReviveSoul;
	int		mHeroReviveStoreyIndex;
	int		mHeroReviveSelfType;
	bool	mHeroIsGoldReviveMode;
	Ref*	mHeroHeadstoneObject;
};


#endif