#ifndef __TRAP_OBJECTS_H__  
#define __TRAP_OBJECTS_H__  

#include "GameObjects.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
 USING_NS_CC;

 class trapObjects : public GameObjects
{

public:
	 std::string  _filenameFrame;
	 int _aktFlag;//�Ƿ���Թ���
	 int _touchFlag;//�Ƿ���Դ���
	 
	 unsigned int trap1_SoundId;
public:
	trapObjects(void);
   ~trapObjects(void);

   static trapObjects* create(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame);
   virtual bool trapObjectsInit(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame); //�Զ���ĳ�ʼ��
    void trapupdate(float dt);   //����ʱ��

	  //���������¼�����
   EventListenerTouchOneByOne *listener;
   void CreateArmature(const std::string& filename,const std::string&name );//������������������:�ļ�Ŀ¼ ����2:�����Ĺ�������
   void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //֡�¼��ص�
   //���������¼�����
   void CreateListenerTouch_Armature();//���ڹ�����
	 
	 //ȥ����������
   void RemoveListenerTouch();


  
public:

	 void setProperty();//��������, flnum �ǵڼ���

	 void upgradeObjectAnimation();//�����Ķ�������


   Armature *dragsmokeArm;
   void dragObjectSmoke();//�϶����壬��������ʱ�򲥷ŵ��̳�����
   void atkCDtimeBack(Ref*psender);

   void BombWithTrap3();
   void BombWithTrap3CDCallback();//��ȴ�ص�

   void actionForTrap4_LOAD();//����״̬
   void actionForTrap4_ATK();//����״̬
   void actionForTrap4_ATKCD();//�������״̬
   void actionForTrap4_BACK();//���ص�������״̬

   void Load();
   void Fighting();




public:   //

	bool		onTouchBegan(Touch *touch, Event *unused_event) ;
	void		onTouchMoved(Touch *touch, Event *unused_event) ; 
	void		onTouchEnded(Touch *touch, Event *unused_event) ; 

	void resumeObjScheduler();
	GameObjects *FindObject(int addr);//���ݵ�ַѰ��ĳһ����
  
 };
#endif