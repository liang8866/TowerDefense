#pragma once
#include "GameConfig.h"
#include "GameObjects.h"
class event1Object : public GameObjects
{
public:
	event1Object(void);
	~event1Object(void);
public:
	static event1Object* createEvent();
	virtual bool initEvent(); //自定义的初始化

	bool isArmature;
	Armature* createAnimation(); 
	void onAnimationFrameEvent_Down(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void setProperty();
	void createToLastStorey();

	void onCountdownBeginEvent(Ref* _sender);
	void onCountdownRemoveEvent(Ref* _sender);

	void setWarningPause(bool _isBool);
public:
	int mEventId;			// 事件id
	int mCountdown;			// 准备倒计时

	int mCreateStoreyIndex;		// 创建所在层级
	int mCreateStoreyPos;		// 创建所在层级的pos位置
	string mDesc;				// 介绍

	int	mImageTag;
	int mImageTag2;
	int mImageTagYuJing;
	//ImageView*	mImage2;
	Sprite*		mSpriteYuJing;

	float mBeginTime;
	float mRemoveTime;

	CC_SYNTHESIZE(bool,m_CanDownFlag,m_CanDownFlag);		// 是否可以掉下去
};




