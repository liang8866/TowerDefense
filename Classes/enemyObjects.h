#ifndef __ENEMY_OBJECTS_H__  
#define __ENEMY_OBJECTS_H__  

#include "cocos2d.h"
#include "GameObjects.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
 USING_NS_CC;
 class enemyObjects : public GameObjects
{

public:
	enemyObjects(void);
   ~enemyObjects(void);

public:
    
	//---------
	 static enemyObjects* create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l);
    virtual bool enemyObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //�Զ���ĳ�ʼ��
	/****************************
	��ʱ����Զ�ж�λ�ã�������Χ����Ѫ�ȼ�ʱ����
	**************************/
	void enemyupdate(float dt);   //����ʱ��

	const std::string  _filename;
	const std::string  _name;

	const std::string getFileName();
	const std::string getName();
	 
	int ttAddr;
	void delayStart();

	CC_SYNTHESIZE(bool,m_isScoffFlag,m_isScoffFlag);//�Ƿ񱻳����ˣ���Ҫ���ڱ����ĸ�Ӣ�۵ļ��ܳ���

	bool isArmature;
	EventListenerTouchOneByOne *listener;
	//����������ʵ�ֺͻ�ȡ
	 void CreateArmature(const std::string& filename,const std::string&name );//������������������:�ļ�Ŀ¼ ����2:�����Ĺ�������
	 void CreateListenerTouch_Armature();//���ڹ����� /���������¼�����
	 Animate* CreateAnimation(const char*FrameName,int max);//FrameName �����ͼƬ

	 //ȥ����������
	 void RemoveListenerTouch();
	 void setProperty(int fnum);//��������
	 

		
public:
	
	void Run();
	void Fight();
	void load();


	 void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); 

public:
	void bossAtkEffect(Point BossPos);//��boss����ʱ���Ч��
	void bossAtkEffectCallback(Ref *psend);
	
	void enymyItem20AttackEffect(float dt);
	void enymyItem17to19AttackEffect(float dt);

	GameObjects * FindObject(int addr);//���ݵ�ַѰ��ĳһ����

	int GetWarAtkMeNum();//�����2��С���ڹ������ˣ��Ͳ����ٹ�������

	//������Ч����
	void FrozenMyself(float dt , bool isFirst = false );
	void FrozenBomb(Ref*psender);
	cocostudio::Armature *frozenArm;


	//������Ч����
	float pre_movespeed;
	void speedDown(float t,float buffValue);
	void  speedDownCallback(Ref*r);

	//������ȥ��Ч��
	void trap4FlyUp();
	void trap4FlyUpCallback(Ref *r);


	//��Ӣ��2����Ч������
	void boss2SkillEffect(float f);//������ʱ��
	void boss2skillCallBack(Ref *sender);//�ص�����
	Sprite *boss2Effct;


    //������ķ�ŭ����

	void boss4SkillEeffect(float f);//f �������ʱ��
	void boss4SkillEffectCallback(Ref *sender);//������Ļص�����
	Sprite *scoff;//��Ц

	//©����һ��
	void checkIsEvent1(GameObjects *obj);//����Ƿ�������©����һ���
	void downToNextStoreyBegin(GameObjects* _guangQuanObj);
	void downToNextStoreyEnd(Ref*f, Sprite* _sp);
	void onAnimationFrameEvent_GuangQuan(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	
	//��ħ�����͵����ϲ�½��
	void UpToGroundStorey();
	void checkIsMagicTrans(GameObjects *obj);//����Ƿ�������ħ�����͵�


	 //���С����
	cocostudio::Armature *sheepArmature;
	void changeToSheep(float time);//�����ɥʧ����
	void createSheepArmature();//������������
	void backEnemyEffect();//�Ȳ���һ�ζ���
	void backEnemy();//��ص���

	//��С����֮�󷵻��ߵĻ��ߴ��Լ��˵�
	
	void xiaogoufawei(int mlv);
	void xiaogoufaweiBackRun();
	//�µ�AI�����ڱ������ź���Լ����˵�
	void newAiForBackRun();



	void setEnemyObjectEvent106();	// Event106�¼���Я������ħ���� ����

	//�ü�
	virtual void visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;

	GroupCommand _groupCommand;
	CustomCommand _beforeVisitCmdScissor;
	CustomCommand _afterVisitCmdScissor;

	//������Ϊ������
	CC_SYNTHESIZE(Point,_leftDoorPoint,LeftDoorPoint); 
	CC_SYNTHESIZE(Point,_rightDoorPoint,RightDoorPoint);

	CC_SYNTHESIZE(bool,_clippingEnabled,ClippingEnabled);
	CC_SYNTHESIZE(Rect,_clippingRect,ClippingRect);

	string _moveDirection;
	void   setMoveDirection(string direction);
	string getMoveDirection();
	
	void onBeforeVisitScissor();
	void onAfterVisitScissor();

	Armature* _GuangQuanArmature;

	// �Ƿ�Ϊ106�¼� Я������ħ���Ĺ���
	CC_SYNTHESIZE(bool, mIsEvent106_Object, IsEvent106_Object);
};

#endif  



