#pragma once
#include "GameConfig.h"
#include "GameObjects.h"
#include "audioPlayer.h"

class StoreyData : public cocos2d::Node
{
public:
	StoreyData(void);
	~StoreyData(void);

public:
	StoreyData(size_t _index);
	StoreyData(size_t _index, Widget* _item);
	StoreyData(size_t _uiIndex, size_t _storeyIndex);

	bool __init();
	void	eventTouchButton(Ref* sender, TouchEventType type);

	// ���¸���
	void updateCell();

	// ȡ��ƽ��
	Widget* getHorizon();
	// ȡ�����
	Widget* getLeftDoor();
	// ȡ�ұ���
	Widget* getRightDoor();
	// ȡ�����
	Widget* getMovementWidget();
	
	// ȡ�߼����Vector
	std::list<GameObjects*> getObjectData() { 
			return mObjectData;
		  };
	// �Ƿ��������߼����
	bool isAddObject(){ return mObjectData.size() < mObjMaxCount;};
	// �����߼����
	bool findObject(GameObjects* _obj);
	// ���һ���߼����
	void addObject(GameObjects* _obj);
	// ɾ��һ���߼����
	void removeObject(GameObjects* _obj);
	// ����߼����
	void clearObject();

	size_t getUiIndex(){ return mIndex; };
	size_t getStoreyIndex(){ return mStoreyIndex; };


	GameObjects* getHero();		// �����Ƿ���Ӣ��
	bool isCanMouseClick();		// �����Ƿ���Ե��

	GameObjects* getDoor();		// ȡ�ؿ���
	void removeDoorObject();	// ɾ���ؿ���object

	bool isClearMonster();		// �Ƿ��ޱ���

	int  getObjCount();
	Vector<GameObjects*> findObjByType(int _objtype , int _objSelfType);

	void onButtonEvent(Ref* sender, TouchEventType type);
	void onButtonEvent_Move(Object* sender, TouchEventType type);

	void setIsCanMove(bool _isBool);

	void createArmature1();
	void beginArmature1();
	void closeArmature1();
	void onAnimationFrameEvent1(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void doArmature2(Ref* sender);
	void createArmature2();
	void beginArmature2();
	void closeArmature2();
	void onAnimationFrameEvent2(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	void beginGamTanHaoAction();
	void onGanTanHaoCallBack(Ref* sender);
	bool isYuJing();

	void propBtnsTouchEvent(Ref* pSender,TouchEventType type);

	void resumeStrawberryBtnTouch(Ref* pSender);
	void resumeLemonBtnTouch(Ref* pSender);

	bool isFinishBatch(int _batch);	// �Ƿ���굱ǰ�㲨�εĹ�

	void doEvent103Action();
	void doEvent103ActionCallBack(Ref* _sender);
	void setIsProAddStorey(bool _isPro);
	bool getIsProAddStorey();

	void setWidgetGray(Widget* _widget, bool _isGray);

	void updatePropsNum(float dt);//����home ����ĵ��ߵ���Ŀ����ֹ�������ʾ����������

	void setLineQZVisible(bool _isBool);		// ���ò������ڵ�������ʾ�ر�
	void updateAddStoreyCostView(int _num);		// ����Home ����������ʾ
	void updateImageCtrlNumber(ImageView* _imageCtrl, int _num);

	void setForbidVisible(bool _isVisible);		// ���ú�ɫ�ڵ����Ƿ���ʾ
	void isPauseHeadstone(bool _isBool);		// ��������Ĺ��(������)��ͣ������ͣ
	void setPause_AllEvent1(bool _isBool);		// ���ǲ�event1��͸�¼���ͣ
	void setPause_GanTanHao(bool _isBool);		// ���ò���ʾ��̾����ͣ

	GameObjects* getHeadstone();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
public:
	// Ui�ؼ��õĲ�����
	size_t		mIndex;
	// Game��Ϸ�е��õĲ�����
	size_t		mStoreyIndex;
	// ��UI��ʾ����
	size_t		mStoreyType;

	// ��ui�ؼ�
	Widget*		mItem;
	
	// ÿ���߼�����������
	size_t		mObjMaxCount;
	// ÿ���߼��������
	std::list<GameObjects*> mObjectData;
	
	Point       m_beginPos;
	// ����ʿ������
	// ��������
	// ������

	Button*		mButtonHomeHeroAltar;
	Button*		mButtonCreateHero;

	Button*		mButtonMoveLeft;
	Button*		mButtonMoveRight;

	Button*		mButtonAddStorey;

	ImageView*	mImageAnim;
	Armature*	mArmature1;
	Armature*	mArmature2;

	ImageView*	mImageGanTanHao;
	int			mImageGuanTanHaoCount;
	bool		mIsYuJing;

	Button *strawberryBtn ;
	Button *mintBtn       ;
	Button *lemonBtn      ;

	TextBMFont*       strawberryCnt;
	TextBMFont*       mintCnt;
	TextBMFont*       lemonCnt;

	bool		mIsProAddStorey;	// �Ƿ����ƿ���

	ImageView*  mImageLineQZ;		// �����غ���
	ImageView*  mImageForbid;		// ��ֹ�ĺ�ɫ��
};


