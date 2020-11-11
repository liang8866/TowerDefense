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
    virtual bool warriorObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //自定义的初始化
	
	/****************************
	定时器永远判定位置，攻击范围，掉血等及时更新
	**************************/
	 void warriorupdate(float dt);   //主定时器
	CC_SYNTHESIZE(int,ttag,TTag);//备用
	bool isArmature;
	EventListenerTouchOneByOne *listener;
	 void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
	 void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调
	 //创建触摸事件监听
	 void CreateListenerTouch_Armature();//对于骨骼的
	 //去掉触摸监听
	 void RemoveListenerTouch();

public:
   void setProperty(int fnum);//设置属性, flnum 是第几层
   void upgradeObjectAnimation();//升级的动画播放
   void upgradeObjectAnimationCallfun(Object *psender);
   void dragObjectSmoke();
   void reSetProperty(int mLv);//升级的时候重新设置属性
   void WarStatesLoad();//状态判断，用于升级和被点击的时候
public:  //动作,范围

	void Run();//跑步
	void load();//待机动作
	void Fight();//战斗
	void FlyFlyFight();//飞行兵对飞行敌人的战斗
	void FlyFlyCallback(Object *psend);

	void beginWithCruising(float dt);//开始巡逻了
	
	void SleepWar();//睡眠
	void CdbackLoad();//CD冷却结束后变回load
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

	void flyWarBombCreate1(Point pos,int zord);//创建飞行兵的爆炸
	void flyWarBombCreate2(Point pos,Point movePos,int zord);//创建飞行兵的爆炸

	void War2Shooter(GameObjects *tempObj);//发射子弹

	GameObjects *FindObject(int addr);//根据地址寻找某一对象
	bool FindSameTypeObject();//寻找周边是否有相同的类型的战士
	int ctNum;//计数器，用于不在战斗状态下的跑步每隔1秒寻找一次周边是否有相同类型的战士



	

};
#endif  



