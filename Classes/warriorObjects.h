#ifndef __WARRIOR_OBJECTS_H__  
#define __WARRIOR_OBJECTS_H__  

#include "cocos2d.h"

#include "GameObjects.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "effectsObject.h"
 USING_NS_CC;


//CCStandardTouchDelegate 
 class warriorObjects : public GameObjects 
{
	
public:
	warriorObjects(void);
   ~warriorObjects(void);

public:
   
    static warriorObjects* create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l);
    virtual bool warriorObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //�Զ���ĳ�ʼ��
	
	/****************************
	��ʱ����Զ�ж�λ�ã�������Χ����Ѫ�ȼ�ʱ����
	**************************/
	 void warriorupdate(float dt);   //����ʱ��
	CC_SYNTHESIZE(int,ttag,TTag);//����
	bool isArmature;
	EventListenerTouchOneByOne *listener;
	 void CreateArmature(const std::string& filename,const std::string&name );//������������������:�ļ�Ŀ¼ ����2:�����Ĺ�������
	 void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //֡�¼��ص�
	 //���������¼�����
	 void CreateListenerTouch_Armature();//���ڹ�����
	 //ȥ����������
	 void RemoveListenerTouch();

public:
   void setProperty(int fnum);//��������, flnum �ǵڼ���
   void upgradeObjectAnimation();//�����Ķ�������
   void upgradeObjectAnimationCallfun(Object *psender);
   void dragObjectSmoke();
   void reSetProperty(int mLv);//������ʱ��������������
   void WarStatesLoad();//״̬�жϣ����������ͱ������ʱ��
public:  //����,��Χ

	void Run();//�ܲ�
	void load();//��������
	void Fight();//ս��
	void FlyFlyFight();//���б��Է��е��˵�ս��
	void FlyFlyCallback(Object *psend);

	void beginWithCruising(float dt);//��ʼѲ����
	
	void SleepWar();//˯��
	void CdbackLoad();//CD��ȴ��������load
	void findIsBackRunEmeny();

public:
    bool isCruising;
	bool firstTimeFlag;
	float left_offset,right_offset;

	unsigned int mSleepLoopEffectId;
	
	float PrePosX;
   
public:
    bool		onTouchBegan(Touch *touch, Event *unused_event) ;
    void		onTouchMoved(Touch *touch, Event *unused_event) ; 
    void		onTouchEnded(Touch *touch, Event *unused_event) ; 

	void clickedCallFunc(float dt);

    void resumeObjScheduler();

	void flyWarBombCreate1(Point pos,int zord);//�������б��ı�ը
	void flyWarBombCreate2(Point pos,Point movePos,int zord);//�������б��ı�ը

	void War2Shooter(GameObjects *tempObj);//�����ӵ�

	GameObjects *FindObject(int addr);//���ݵ�ַѰ��ĳһ����
	bool FindSameTypeObject();//Ѱ���ܱ��Ƿ�����ͬ�����͵�սʿ
	int ctNum;//�����������ڲ���ս��״̬�µ��ܲ�ÿ��1��Ѱ��һ���ܱ��Ƿ�����ͬ���͵�սʿ



	

};
#endif  



