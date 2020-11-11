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
	
	void setHeroSelfType(int _selfType);						//����Ӣ������, Ϊ������׼��
	void setHeroCostMoney(int _costGold, int _costSoul);		//����Ӣ�۸�������
	void setHeroCostPosition(Point _pos); 

	void onEventImageClick(Ref *pSender,TouchEventType type);

	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

	void createEventListenerTouch();
	void removeEventListenerTouch();

	void objectScheduler();

	void removeSelfAndClearup();

	void beginProgressBar(int _value);	// ��ʼ����ʱ
	void reviveHeroByCostGold();		// ���Ľ�Ҹ���Ӣ��

	void isPauseHeadstone(bool _isBool);
public:
	EventListenerTouchOneByOne *listener;
	int mHeroSelfType;
	int mHeroCostSweet;
	int mHeroCostSoul;
	Point mHeroPosition;

	CC_SYNTHESIZE(bool,	_IsGoldReviveMode, IsGoldReviveMode);	// �Ƿ��ڽ�Ҹ���ģʽ��
};




