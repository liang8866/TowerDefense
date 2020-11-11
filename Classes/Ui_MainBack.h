#pragma once
#include "cocos2d.h"
#include "Window.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "enemyObjects.h"
#include "warriorObjects.h"
#include "bossObjects.h"
#include "towerObjects.h"
#include "trapObjects.h"
#include "WindowManager.h"
#include "doorObject.h"
#include "AltarObject.h"
#include "npcObjects.h"

class Ui_NextRound;

class Ui_MainBack : public Window
{
public:
	Ui_MainBack(void);
	~Ui_MainBack(void);
public:
	static Ui_MainBack* create(const char* fileName);

	bool initialise(const char* fileName);

	void updateDragMenu();		// 更新拖动创建的格子类型
	void updateSceneStorey();	// 更新场景初始层
	void updateHomeDoor();		// 更新场景Home结束门
	void updateAddStoreyCost();	// 更新开层消耗

	void addDragMenuCell(size_t _index);
	void resetCloundsPosition();

	GameObjects*	createObject(int objType, Point objPos);							// 创建逻辑物件
	void			addStorey(size_t _index, size_t _storeyIndex);						// 添加层
	void			addStoreyObject(size_t index, GameObjects* _obj);					// 添加层上逻辑物件, ui索引
	void			addStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);	// 添加层上逻辑物件, 数据索引
	void			removeStoreyObject(size_t index, GameObjects* _obj);				// 删除层上指定物件, ui索引
	void			removeStoreyObjectFormStoreyIndex(size_t _storeyIndex);				// 删除层上指定物件, 数据索引
	void			removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);	// 删除层上指定物件, 数据索引
	void			clearAllObject();								// 删除所有物件, ui索引

	void eventScrollView(Ref* sender, ScrollviewEventType event);

	void itemBtnClickedCallfuc(Ref *pSender, TouchEventType type ,int id);

	void eventTouchButton(Ref* sender, TouchEventType type);
	void dragObjectFinish(Node* sender);

	Widget*	findChild(const char* _widgetName);

	int		getDragUiIndex();					// 取当前拖动的格子位置
	Point	getScreenPos(Point _pos);

	void finishMoveCallback(Node *psender);
	

	void update(float dt);
	void updateMouseDragItemCD(float dt);
	void updateStoreyObjectMoveToNext(float dt);
	void updateStoreyObjectMoveToLast(float dt);

	void fitScreen();

	void updateCenterStorey(int _storeyIndex);
	void createHeroToStorey(int _storeyIndex, int _heroType, Point _heroPos, int _heroLevel);


public:
	ui::ScrollView*	    mScrollViewWidget;//背景图
	ui::ListView*		mListViewWidget;//添加层的ListView
	float				mListViewItemsMargin;

	size_t				mMouseDragBeginItemIndex;
	Widget*				mMouseDragBeginItem;
	bool				mIsMouseDragMode;			// 是否拖动创建模式
	GameObjects*		mMouseClickDragObject;			// 点击创建虚拟obj
	int					mMouseClickDragFirstObjTag;		// 点击创建虚拟obj Tag
	bool				mIsMouseClickDragCreate;		// 是否可以 创建虚拟obj Tag
	bool				mIsMouseClickPosMoving;			// 是否移动出原地
	int					mMouseClickDragStoreyIndex;		// 点击创建虚拟obj的层
	Point				mMouseClickDragBeginPos;		// 点击创建起始点

	Widget*				mGroundPanelWidget;	// 第六层(地面)背景控件

	Ui_NextRound*		mGroundButtonWidget; //下一波按钮UI

	int					mAddStoreyCost;		// 开层消耗
public:

	void CreateAnimationForDeathObject(Point pos);
	void DeathObjectCallBack(Ref *psender);

	int curWave;
	int TotalWave;
	float CountTime;
	void updateGroundObjectPos(float dt);
	
	void CreateEnemyForStorey(float posx ,const int enemy_type,const int num, int _batch, int _event106_count = 0);
	void CreateEnemyForStorey(float dt);							// 在第六层创建怪物出怪


	void toNextStorey(size_t _curStoreyIndex, GameObjects* _obj, int _nextStoreyIndex = -1);	// 进入下一层
	void toUpStorey(GameObjects *_obj);                              //被陷阱弹到上一层的单位
	void toLastStorey(GameObjects *_obj);                            //敌人进入上一层
	void toDownNextStorey(GameObjects *_obj);                        //漏洞到下一层的
	void toDownNextStoreyCallback(Ref *f);
	void reduceDoorHp(doorObject *door,int enemyType);

	enemyObjects *CopyDataToNewEnemy(enemyObjects *emeny);			// 复制Object

	int isAddStoreyObjectType(size_t _storeyIndex, GameObjects* _obj);	// 是否能添加Object物件的判断 判断是否可以添加兵或者英雄

	

	//敌人在陆地上行走的运动轨迹
	void EmenyMoveOnGround(GameObjects *obj);//敌人在陆地上行走的运动轨迹
	void MoveOnGroundCallBack(Ref *obj);//敌人在表面上行走的时候的结束回调

	void resetTouchAll(bool _isTouch);

	void freshmanArrowPrompt(const char* id);
	int  getAnEmptyIndex();  //从1找起，找一个未放物件的索引

	void setAddStoreyNum(int _num);
	void doAddStorey(bool _isCost);
	void onMoveCurStorey(Node* sender, StoreyData* _item, float _x);
	void onMoveCurStoreyFinish(Node* sender, StoreyData* _item);
	int mAddYuJingStoreyIndex;		// 是否添加了层延迟预警
	void onIsAddStoreyDisYuJingTimeCallBack(Ref* sender);
	void onAddStoreyAinmFinish();
	void updateAddStoreyButtonState();

	void setWidgetGray(Widget* _widget, bool _isGray);

	int mAddStoreyNum; // 免费开几层
	void doStoreyArmature(Node* sender, StoreyData* _storeyData);
	void doEvent2(int _batch);	// 处理事件2 免费开层
	void doEventCallBack(Ref*f);

	void doEvent103();			// 处理事件103, 限制手动开层

	bool mIsEvent105;
	void doEvent105();	// 处理事件5, 限制怪物随机层

	ImageView *_towerfeng;

	GameObjects * CreateMagicObjets(int stype);//创建魔法技能类
	//传送技能的
	void CreateMagicTransmit(int stype,Point pos,int floor); //第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的
	void magicTransmitUpToGround(GameObjects *_obj);//传送到陆地上去
	void magicTransmitMoveOnGround(Ref *obj);//新的位置的移动

	//变羊技能的。。
	void magicChangeToSheep(Point  pos,int floornum);//位置和第几层上面释放的 
	//雷屁的攻击魔法技能
	void CreateMagicAtk(int stype,Point pos,int floor);//第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的
	//变成哥斯拉的魔法技能
	void CreateMagicGosla(int stype,Point pos,int floor);//第一个参数是魔法技能的那种类型 第二是摆放的位置,第三个是表示第几层上面的

	int mMoveToRandStoreyIndex;
	void onMoveToRandStoreyCallBack(Ref* _obj);
	void updateYuJing();
	void createFirstHero(float _dt);
	void CreateBoss5jingling(GameObjects *_obj);
	void initAnimation(); //背景动画
	void marqueeCloud(ImageView* cloud);
	void createJinglingAttribute(GameObjects *_obj,GameObjects *jing);

	// 创建Obj监听
	EventListenerTouchOneByOne* _createObjListener;
	void createCreateObjListenerTouch();
	void removeCreateObjListenerTouch();
	bool onCreateObjTouchBegan(Touch* _touch, Event* _unusedEvent);
	void onCreateObjTouchMove(Touch* _touch, Event* _unusedEvent);
	void onCreateObjTouchEnd(Touch* _touch, Event* _unusedEvent);

	Point updateCreateMouseClickDragObject(Point _touchPos, bool _isDontCreate = false);	// 更新创建点击模式下的。object, 是否不创建
	void createMouseClickDragObject(Point _touchPos);			// 创建点击模式下的。object
};

