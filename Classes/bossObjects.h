#ifndef __BOSS_OBJECTS_H__  
#define __BOSS_OBJECTS_H__  

#include "cocos2d.h"
#include "GameObjects.h"
#include "GameConfig.h"
#include "enemyObjects.h"
 USING_NS_CC;

 /**
 英雄AI——英雄AI基本逻辑，警戒，当有勇士进入警戒范围内与之进行战斗，
	直至双方一方HP=0结束，结束后搜索下一目标，（如果有多个目标则选择没有进入战斗的目标）
	重复战斗逻辑。如战斗中玩家操作英雄移动则优先执行移动命令，英雄拥有的技能战斗中由玩家
	手动使用，技能的使用优先级最高，打断当前动作优先执行技能释放动作
**/		


 class bossObjects : public GameObjects
{

public:
	bossObjects(void);
   ~bossObjects(void);

public:
    
	
	static bossObjects* create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l);

	 virtual bool bossObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l); //自定义的初始化
	
	CC_SYNTHESIZE(int,ttag,TTag);//备用

		/****************************
	定时器永远判定位置，攻击范围，掉血等及时更新
	**************************/
	 void bossupdate(float dt);   //主定时器
	 Animate* CreateAnimation(const char*FrameName,int max);
	bool isArmature;
	EventListenerTouchOneByOne *listener;
	//骨骼动画的实现和获取
	
	 void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
	 void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);

	 //创建触摸事件监听
	 void CreateListenerTouch_Armature();//对于骨骼的

	 //去掉触摸监听
	 void RemoveListenerTouch();
    //骨骼动画的回调
	
	 void createAnimationForSomeBody(const char*FrameName,int max,Node *parent,Point pos,int zd);//创建底部的动画和BOss身上的动画
	
	 void Run();//跑步时候的动画
	 void Load();//待机时候的动画
	 void Fight_Begin();
	 void Fight_Mid();
	 
	//点击播放技能后计算冷却时间
	 CC_SYNTHESIZE(float,cdleftTime,cdleftTime);
	

	
	void setProperty();

	
	 void BeginTouchMoveToPoint(Point _pos);		//手动点击移动到的坐标
	 void EndTouchMoveToPoint();					//手动点击移动到的坐标 结束

	 Sprite *_selectFrame;
	 bool onTouchBegan(Touch *touch, Event *unused_event) ;
	 void  onTouchMoved(Touch *touch, Event *unused_event) ; 
	 void  onTouchEnded(Touch *touch, Event *unused_event) ; 
	 void  CreateSelectFrame();//创建被选中时候的箭头
	 void  RemoveSelectFrame();//删除选中时候的箭头

	
	 void updateBossMove(float dt);
	 bool	mIsMouseMoving;
	 Point	mMouseMovePos;
	
	 void gantanhaoCallback(Object *psender);
	 void bossCreateBall(GameObjects *tempObj,int etype);
	 

public:
	
	GameObjects *FindObject(int addr);//根据地址寻找某一对象

	void beginPressBossRun(int dirction);//按下开始 dirction 方向 D_RIGHT 表示向右 D_LEFT 表示向左走 具体值看  gameconfig文件
	void endPressBossRun();//按下结束

	void reSetAttribute(Ref *f);//重新获取属性
	void getBossSkill();//获取技能属性的

	void playSkillForBoss();//释放英雄技能
	void boss1SkillEffect();//英雄1的技能效果
	void  boss1SkillHuoMiao();//火苗。。。。。。。。。
	void boss1SkillEffectFunc(Ref *f);
	int boss1tempAtk;//英雄1 的临时技能攻击力
	
	void boss1turnBackSkillBuff(Ref *f);//使用技能的提升后恢复以前 的

	void boss2FindObject();///英雄2技能的寻找附近有没敌人。

	void boss3FindObject();//英雄3的技能寻找
	void boss3TimesCallback();
	void boss3SkillAction();
	int  boss3count;

	void boss4FindObject();//英雄4的技能寻找敌人
	
	int isChangeFlag;
	Armature *_xiaohai;
	void  Boss4ChangeEffect();
	void CreateBoss4Armature1();
	void CreateBoss4Armature2();
	void comebackXiaohai();
	void playBoss4Skill();
	
	void playCommonSkillEffect();
	void playCommonSkillEffectCallfunc(Ref *sender);

public:
	// 鼠标监听事件
	EventListenerMouse* _mouseListener;
	void onMouseUp(Event* _event);
	void onMouseDown(Event* _event);  
	void onMouseMove(Event* _event);  
	void onMouseScroll(Event* _event); 

	void createMouseListener();
	void releaseMouseListener();

public:
	// 移动监听事件
	EventListenerTouchOneByOne* _moveListener;
	bool onMoveTouchBegan(Touch* _touch, Event* _unusedEvent);  
	void onMoveTouchMoved(Touch *touch, Event *unused_event);  
	void onMoveTouchEnded(Touch *touch, Event *unused_event);  
	void onMoveTouchCancelled(Touch *touch, Event *unused_event);  

	void createMoveListener();
	void releaseMoveListener();



 };
#endif  



