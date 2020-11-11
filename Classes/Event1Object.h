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
	virtual bool initEvent(); //�Զ���ĳ�ʼ��

	bool isArmature;
	Armature* createAnimation(); 
	void onAnimationFrameEvent_Down(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void setProperty();
	void createToLastStorey();

	void onCountdownBeginEvent(Ref* _sender);
	void onCountdownRemoveEvent(Ref* _sender);

	void setWarningPause(bool _isBool);
public:
	int mEventId;			// �¼�id
	int mCountdown;			// ׼������ʱ

	int mCreateStoreyIndex;		// �������ڲ㼶
	int mCreateStoreyPos;		// �������ڲ㼶��posλ��
	string mDesc;				// ����

	int	mImageTag;
	int mImageTag2;
	int mImageTagYuJing;
	//ImageView*	mImage2;
	Sprite*		mSpriteYuJing;

	float mBeginTime;
	float mRemoveTime;

	CC_SYNTHESIZE(bool,m_CanDownFlag,m_CanDownFlag);		// �Ƿ���Ե���ȥ
};




