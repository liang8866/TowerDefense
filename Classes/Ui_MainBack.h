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

	void updateDragMenu();		// �����϶������ĸ�������
	void updateSceneStorey();	// ���³�����ʼ��
	void updateHomeDoor();		// ���³���Home������
	void updateAddStoreyCost();	// ���¿�������

	void addDragMenuCell(size_t _index);
	void resetCloundsPosition();

	GameObjects*	createObject(int objType, Point objPos);							// �����߼����
	void			addStorey(size_t _index, size_t _storeyIndex);						// ��Ӳ�
	void			addStoreyObject(size_t index, GameObjects* _obj);					// ��Ӳ����߼����, ui����
	void			addStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);	// ��Ӳ����߼����, ��������
	void			removeStoreyObject(size_t index, GameObjects* _obj);				// ɾ������ָ�����, ui����
	void			removeStoreyObjectFormStoreyIndex(size_t _storeyIndex);				// ɾ������ָ�����, ��������
	void			removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);	// ɾ������ָ�����, ��������
	void			clearAllObject();								// ɾ���������, ui����

	void eventScrollView(Ref* sender, ScrollviewEventType event);

	void itemBtnClickedCallfuc(Ref *pSender, TouchEventType type ,int id);

	void eventTouchButton(Ref* sender, TouchEventType type);
	void dragObjectFinish(Node* sender);

	Widget*	findChild(const char* _widgetName);

	int		getDragUiIndex();					// ȡ��ǰ�϶��ĸ���λ��
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
	ui::ScrollView*	    mScrollViewWidget;//����ͼ
	ui::ListView*		mListViewWidget;//��Ӳ��ListView
	float				mListViewItemsMargin;

	size_t				mMouseDragBeginItemIndex;
	Widget*				mMouseDragBeginItem;
	bool				mIsMouseDragMode;			// �Ƿ��϶�����ģʽ
	GameObjects*		mMouseClickDragObject;			// �����������obj
	int					mMouseClickDragFirstObjTag;		// �����������obj Tag
	bool				mIsMouseClickDragCreate;		// �Ƿ���� ��������obj Tag
	bool				mIsMouseClickPosMoving;			// �Ƿ��ƶ���ԭ��
	int					mMouseClickDragStoreyIndex;		// �����������obj�Ĳ�
	Point				mMouseClickDragBeginPos;		// ���������ʼ��

	Widget*				mGroundPanelWidget;	// ������(����)�����ؼ�

	Ui_NextRound*		mGroundButtonWidget; //��һ����ťUI

	int					mAddStoreyCost;		// ��������
public:

	void CreateAnimationForDeathObject(Point pos);
	void DeathObjectCallBack(Ref *psender);

	int curWave;
	int TotalWave;
	float CountTime;
	void updateGroundObjectPos(float dt);
	
	void CreateEnemyForStorey(float posx ,const int enemy_type,const int num, int _batch, int _event106_count = 0);
	void CreateEnemyForStorey(float dt);							// �ڵ����㴴���������


	void toNextStorey(size_t _curStoreyIndex, GameObjects* _obj, int _nextStoreyIndex = -1);	// ������һ��
	void toUpStorey(GameObjects *_obj);                              //�����嵯����һ��ĵ�λ
	void toLastStorey(GameObjects *_obj);                            //���˽�����һ��
	void toDownNextStorey(GameObjects *_obj);                        //©������һ���
	void toDownNextStoreyCallback(Ref *f);
	void reduceDoorHp(doorObject *door,int enemyType);

	enemyObjects *CopyDataToNewEnemy(enemyObjects *emeny);			// ����Object

	int isAddStoreyObjectType(size_t _storeyIndex, GameObjects* _obj);	// �Ƿ������Object������ж� �ж��Ƿ������ӱ�����Ӣ��

	

	//������½�������ߵ��˶��켣
	void EmenyMoveOnGround(GameObjects *obj);//������½�������ߵ��˶��켣
	void MoveOnGroundCallBack(Ref *obj);//�����ڱ��������ߵ�ʱ��Ľ����ص�

	void resetTouchAll(bool _isTouch);

	void freshmanArrowPrompt(const char* id);
	int  getAnEmptyIndex();  //��1������һ��δ�����������

	void setAddStoreyNum(int _num);
	void doAddStorey(bool _isCost);
	void onMoveCurStorey(Node* sender, StoreyData* _item, float _x);
	void onMoveCurStoreyFinish(Node* sender, StoreyData* _item);
	int mAddYuJingStoreyIndex;		// �Ƿ�����˲��ӳ�Ԥ��
	void onIsAddStoreyDisYuJingTimeCallBack(Ref* sender);
	void onAddStoreyAinmFinish();
	void updateAddStoreyButtonState();

	void setWidgetGray(Widget* _widget, bool _isGray);

	int mAddStoreyNum; // ��ѿ�����
	void doStoreyArmature(Node* sender, StoreyData* _storeyData);
	void doEvent2(int _batch);	// �����¼�2 ��ѿ���
	void doEventCallBack(Ref*f);

	void doEvent103();			// �����¼�103, �����ֶ�����

	bool mIsEvent105;
	void doEvent105();	// �����¼�5, ���ƹ��������

	ImageView *_towerfeng;

	GameObjects * CreateMagicObjets(int stype);//����ħ��������
	//���ͼ��ܵ�
	void CreateMagicTransmit(int stype,Point pos,int floor); //��һ��������ħ�����ܵ��������� �ڶ��ǰڷŵ�λ��,�������Ǳ�ʾ�ڼ��������
	void magicTransmitUpToGround(GameObjects *_obj);//���͵�½����ȥ
	void magicTransmitMoveOnGround(Ref *obj);//�µ�λ�õ��ƶ�

	//�����ܵġ���
	void magicChangeToSheep(Point  pos,int floornum);//λ�ú͵ڼ��������ͷŵ� 
	//��ƨ�Ĺ���ħ������
	void CreateMagicAtk(int stype,Point pos,int floor);//��һ��������ħ�����ܵ��������� �ڶ��ǰڷŵ�λ��,�������Ǳ�ʾ�ڼ��������
	//��ɸ�˹����ħ������
	void CreateMagicGosla(int stype,Point pos,int floor);//��һ��������ħ�����ܵ��������� �ڶ��ǰڷŵ�λ��,�������Ǳ�ʾ�ڼ��������

	int mMoveToRandStoreyIndex;
	void onMoveToRandStoreyCallBack(Ref* _obj);
	void updateYuJing();
	void createFirstHero(float _dt);
	void CreateBoss5jingling(GameObjects *_obj);
	void initAnimation(); //��������
	void marqueeCloud(ImageView* cloud);
	void createJinglingAttribute(GameObjects *_obj,GameObjects *jing);

	// ����Obj����
	EventListenerTouchOneByOne* _createObjListener;
	void createCreateObjListenerTouch();
	void removeCreateObjListenerTouch();
	bool onCreateObjTouchBegan(Touch* _touch, Event* _unusedEvent);
	void onCreateObjTouchMove(Touch* _touch, Event* _unusedEvent);
	void onCreateObjTouchEnd(Touch* _touch, Event* _unusedEvent);

	Point updateCreateMouseClickDragObject(Point _touchPos, bool _isDontCreate = false);	// ���´������ģʽ�µġ�object, �Ƿ񲻴���
	void createMouseClickDragObject(Point _touchPos);			// �������ģʽ�µġ�object
};

