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
    virtual bool enemyObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //自定义的初始化
	/****************************
	定时器永远判定位置，攻击范围，掉血等及时更新
	**************************/
	void enemyupdate(float dt);   //主定时器

	const std::string  _filename;
	const std::string  _name;

	const std::string getFileName();
	const std::string getName();
	 
	int ttAddr;
	void delayStart();

	CC_SYNTHESIZE(bool,m_isScoffFlag,m_isScoffFlag);//是否被嘲讽了，主要用于被第四个英雄的技能嘲讽

	bool isArmature;
	EventListenerTouchOneByOne *listener;
	//骨骼动画的实现和获取
	 void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
	 void CreateListenerTouch_Armature();//对于骨骼的 /创建触摸事件监听
	 Animate* CreateAnimation(const char*FrameName,int max);//FrameName 和最大图片

	 //去掉触摸监听
	 void RemoveListenerTouch();
	 void setProperty(int fnum);//设置属性
	 

		
public:
	
	void Run();
	void Fight();
	void load();


	 void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); 

public:
	void bossAtkEffect(Point BossPos);//被boss攻击时候的效果
	void bossAtkEffectCallback(Ref *psend);
	
	void enymyItem20AttackEffect(float dt);
	void enymyItem17to19AttackEffect(float dt);

	GameObjects * FindObject(int addr);//根据地址寻找某一对象

	int GetWarAtkMeNum();//如果有2个小兵在攻击我了，就不能再攻击我了

	//被冰冻效果的
	void FrozenMyself(float dt , bool isFirst = false );
	void FrozenBomb(Ref*psender);
	cocostudio::Armature *frozenArm;


	//被减速效果的
	float pre_movespeed;
	void speedDown(float t,float buffValue);
	void  speedDownCallback(Ref*r);

	//被弹上去的效果
	void trap4FlyUp();
	void trap4FlyUpCallback(Ref *r);


	//被英雄2震昏的效果表现
	void boss2SkillEffect(float f);//被震昏的时间
	void boss2skillCallBack(Ref *sender);//回调函数
	Sprite *boss2Effct;


    //被嘲讽的愤怒表现

	void boss4SkillEeffect(float f);//f 被嘲讽的时间
	void boss4SkillEffectCallback(Ref *sender);//被嘲讽的回调函数
	Sprite *scoff;//嘲笑

	//漏到下一层
	void checkIsEvent1(GameObjects *obj);//检查是否碰到了漏到下一层的
	void downToNextStoreyBegin(GameObjects* _guangQuanObj);
	void downToNextStoreyEnd(Ref*f, Sprite* _sp);
	void onAnimationFrameEvent_GuangQuan(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	
	//被魔法传送到最上层陆地
	void UpToGroundStorey();
	void checkIsMagicTrans(GameObjects *obj);//检查是否碰到了魔法传送的


	 //变成小绵羊
	cocostudio::Armature *sheepArmature;
	void changeToSheep(float time);//变成羊，丧失攻击
	void createSheepArmature();//创建骨骼动画
	void backEnemyEffect();//先播放一次动画
	void backEnemy();//变回敌人

	//被小狗吓之后返回走的或者打自己人的
	
	void xiaogoufawei(int mlv);
	void xiaogoufaweiBackRun();
	//新的AI，用于被狗恐吓后打自己敌人的
	void newAiForBackRun();



	void setEnemyObjectEvent106();	// Event106事件：携带特殊魔法的 怪物

	//裁剪
	virtual void visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated) override;

	GroupCommand _groupCommand;
	CustomCommand _beforeVisitCmdScissor;
	CustomCommand _afterVisitCmdScissor;

	//左右门为参照物
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

	// 是否为106事件 携带特殊魔法的怪物
	CC_SYNTHESIZE(bool, mIsEvent106_Object, IsEvent106_Object);
};

#endif  



