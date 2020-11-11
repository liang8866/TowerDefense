#ifndef __TOWER_OBJECTS_H__  
#define __TOWER_OBJECTS_H__  
#include "GameObjects.h"
#include "GameConfig.h"
USING_NS_CC;


enum TowerAttackType{
	noAttack,
	floorAttack,
	skyAttack,
	floorAndSkyAttack
};


class towerObjects : public GameObjects
{
public:
	towerObjects(void);
	~towerObjects(void);
	virtual bool towerObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //�Զ���ĳ�ʼ��
	//---------
	//static towerObjects* create();
	static towerObjects* create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l);
	static towerObjects* create(const std::string& filename);

public:

	//���������¼�����
	EventListenerTouchOneByOne *listener;
	void CreateListenerTouch_Sprite();//���ھ����
	 
	//ȥ����������
	void RemoveListenerTouch();
	//Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex
	void Firing();
	void FlyFiring();
	TowerAttackType attak_fly;   

	void MoveFinished(Node* sender);
public:
	GameObjects* getClosesTarget();
	GameObjects* _target;
	GameObjects* _fly_target;
	Point _currentTargetPoint;
	Point _currFlyTargetPoint;

	float _boss2_skill3_of_Tower;//�ڶ���Ӣ�۶Է������Ĺ��������ٷֱ�

public:
	void setProperty();
	void logicUpdate(float dt);
	void onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

    Armature *dragsmokeArm;
	void dragObjectSmoke();//�϶����壬��������ʱ�򲥷ŵ��̳�����
	void upgradeObjectAnimation(int level);//�����Ķ�������
	bool		onTouchBegan(Touch *touch, Event *unused_event);
	void		onTouchMoved(Touch *touch, Event *unused_event); 
	void		onTouchEnded(Touch *touch, Event *unused_event); 

	void resumeObjScheduler();

	void tower1atkEffect();
	void tower1atkEffectCallBack(Node *pSender);
	void tower2atkEffect();
	void tower2atkEffectCallBack(Node *pSender);
	void removeArrow(Node *pSender);
	void tower2ResumeLoad(float dt);
	void targetHide(Node *pSender);

	void createDaodan();
	void daodanEffectCallBack(Node *pSender);

	void load();

	void checkHp();
	void checkDirection();

	GameObjects *FindObject(int addr);

	void resumeStatus(float dt);

	GameObjects *getFlyEnemy();
	void attackFlyObj();

	int getTower4AtkCount();
	void tower4atkEffect(float dt);
	void tower4cd();
	void tower4AtkedObjs();
	void tower4resumeLoad(float dt);
	void tower4atkCallfunc(float dt);

	void tower5atkEffect();
	void tower5atkObjsReduceHp();
	void tower5cd();
	void resumeTower5Status(float dt);

	virtual void onExit() override;
private:
	int _gap;

	bool isHalf;

	int  loadNum;

	int paodanTime;
	int checkHpTime;
 };


#endif