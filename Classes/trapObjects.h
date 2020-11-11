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
	 int _aktFlag;//是否可以攻击
	 int _touchFlag;//是否可以触摸
	 
	 unsigned int trap1_SoundId;
public:
	trapObjects(void);
   ~trapObjects(void);

   static trapObjects* create(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame);
   virtual bool trapObjectsInit(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame); //自定义的初始化
    void trapupdate(float dt);   //主定时器

	  //创建触摸事件监听
   EventListenerTouchOneByOne *listener;
   void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
   void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调
   //创建触摸事件监听
   void CreateListenerTouch_Armature();//对于骨骼的
	 
	 //去掉触摸监听
   void RemoveListenerTouch();


  
public:

	 void setProperty();//设置属性, flnum 是第几层

	 void upgradeObjectAnimation();//升级的动画播放


   Armature *dragsmokeArm;
   void dragObjectSmoke();//拖动陷阱，防御塔的时候播放的烟尘动画
   void atkCDtimeBack(Ref*psender);

   void BombWithTrap3();
   void BombWithTrap3CDCallback();//冷却回调

   void actionForTrap4_LOAD();//待机状态
   void actionForTrap4_ATK();//攻击状态
   void actionForTrap4_ATKCD();//攻击完的状态
   void actionForTrap4_BACK();//返回到待机的状态

   void Load();
   void Fighting();




public:   //

	bool		onTouchBegan(Touch *touch, Event *unused_event) ;
	void		onTouchMoved(Touch *touch, Event *unused_event) ; 
	void		onTouchEnded(Touch *touch, Event *unused_event) ; 

	void resumeObjScheduler();
	GameObjects *FindObject(int addr);//根据地址寻找某一对象
  
 };
#endif