#pragma once
#include "GameConfig.h"
#include "GameObjects.h"
class EventObject_106 : public GameObjects
{
public:
	EventObject_106(void);
	~EventObject_106(void);
public:
	static EventObject_106* createEvent();
	virtual bool initEvent();	//�Զ���ĳ�ʼ��
	void setProperty();

	/*bool isArmature;
	Armature* createAnimation(); 
	void onAnimationFrameEvent_Down(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);*/

	void updateView();

	void createEventListenerTouch();
	void removeEventListenerTouch();
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchMoved(Touch *touch, Event *unused_event); 
	void onTouchEnded(Touch *touch, Event *unused_event); 

	
	void showAction();				// ���ų���ʱ��Ķ���
	void onShowFinish(Ref* sender);
	void removeSelf(Ref* sender);

	void createFlyAction();			// �������붯��
	void onFlyFinish(Ref* sender);	// ���붯������

public:
	EventListenerTouchOneByOne *listener;

	vector<_ItemBoxData> mMagicItems;
	_BagItemInfo	mCurMagic;

	float			mRemoveTime;
	ImageView*		mImageAction;
	int				mImageActionTag;
	bool			mIsRemoveSelf;
};




