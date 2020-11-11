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

	// 更新格子
	void updateCell();

	// 取地平线
	Widget* getHorizon();
	// 取左边门
	Widget* getLeftDoor();
	// 取右边门
	Widget* getRightDoor();
	// 取活动区域
	Widget* getMovementWidget();
	
	// 取逻辑物件Vector
	std::list<GameObjects*> getObjectData() { 
			return mObjectData;
		  };
	// 是否可以添加逻辑物件
	bool isAddObject(){ return mObjectData.size() < mObjMaxCount;};
	// 查找逻辑物件
	bool findObject(GameObjects* _obj);
	// 添加一个逻辑物件
	void addObject(GameObjects* _obj);
	// 删除一个逻辑物件
	void removeObject(GameObjects* _obj);
	// 清空逻辑物件
	void clearObject();

	size_t getUiIndex(){ return mIndex; };
	size_t getStoreyIndex(){ return mStoreyIndex; };


	GameObjects* getHero();		// 层内是否有英雄
	bool isCanMouseClick();		// 层内是否可以点击

	GameObjects* getDoor();		// 取关卡门
	void removeDoorObject();	// 删除关卡门object

	bool isClearMonster();		// 是否无兵了

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

	bool isFinishBatch(int _batch);	// 是否打完当前层波次的怪

	void doEvent103Action();
	void doEvent103ActionCallBack(Ref* _sender);
	void setIsProAddStorey(bool _isPro);
	bool getIsProAddStorey();

	void setWidgetGray(Widget* _widget, bool _isGray);

	void updatePropsNum(float dt);//更新home 下面的道具的数目，防止购买后显示数量有问题

	void setLineQZVisible(bool _isBool);		// 设置层土地遮挡横条显示关闭
	void updateAddStoreyCostView(int _num);		// 更新Home 开层消耗显示
	void updateImageCtrlNumber(ImageView* _imageCtrl, int _num);

	void setForbidVisible(bool _isVisible);		// 设置红色遮挡层是否显示
	void isPauseHeadstone(bool _isBool);		// 设置所有墓碑(进度条)暂停。非暂停
	void setPause_AllEvent1(bool _isBool);		// 这是层event1渗透事件暂停
	void setPause_GanTanHao(bool _isBool);		// 设置层提示感叹号暂停

	GameObjects* getHeadstone();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
public:
	// Ui控件用的层索引
	size_t		mIndex;
	// Game游戏中的用的层索引
	size_t		mStoreyIndex;
	// 层UI显示类型
	size_t		mStoreyType;

	// 层ui控件
	Widget*		mItem;
	
	// 每层逻辑物件最大数量
	size_t		mObjMaxCount;
	// 每层逻辑物件数据
	std::list<GameObjects*> mObjectData;
	
	Point       m_beginPos;
	// 敌人士兵数据
	// 陷阱数据
	// 塔数据

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

	bool		mIsProAddStorey;	// 是否限制开层

	ImageView*  mImageLineQZ;		// 层土地横条
	ImageView*  mImageForbid;		// 禁止的红色层
};


