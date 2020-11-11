#pragma once
#include "GameConfig.h"
#include "GameObjects.h"

class headstoneObjects : public GameObjects
{
public:
	headstoneObjects(void);
	~headstoneObjects(void);
public:
	CREATE_FUNC(headstoneObjects);
	bool init();
	void setProperty();
	
	void setHeroSelfType(int _selfType);						//设置英雄类型, 为复活做准备
	void setHeroCostMoney(int _costGold, int _costSoul);		//设置英雄复活消耗
	void setHeroCostPosition(Point _pos); 

	void onEventImageClick(Ref *pSender,TouchEventType type);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

	void createEventListenerTouch();
	void removeEventListenerTouch();

	void objectScheduler();

	void removeSelfAndClearup();

	void beginProgressBar(int _value);	// 开始倒计时
	void reviveHeroByCostGold();		// 消耗金币复活英雄

	void isPauseHeadstone(bool _isBool);
public:
	EventListenerTouchOneByOne *listener;
	int mHeroSelfType;
	int mHeroCostSweet;
	int mHeroCostSoul;
	Point mHeroPosition;

	CC_SYNTHESIZE(bool,	_IsGoldReviveMode, IsGoldReviveMode);	// 是否在金币复活模式中
};




