#ifndef __GAME_OBJECTS_H__  
#define __GAME_OBJECTS_H__  
#include "cocos2d.h"
#include "GameConfig.h"
#include "audioPlayer.h"
#include "Ui_ObjectClicked.h"
class StoreyData;
class SceneData;

 USING_NS_CC;
class GameObjects : public cocos2d::Sprite
{

public:
	GameObjects(void);
   ~GameObjects(void);

   virtual bool init();
    // implement the "static create()" method manually
   CREATE_FUNC(GameObjects);
  
   //属性列表。。。。。。。。。。。。。。。。。。。。。。。。。。。。。

   CC_SYNTHESIZE(int,_isIntimidate,isIntimidate);//是否被恐吓)

    int isPlaySkill;//是否在释放技能

   //主要的类型，是，陷阱，士兵还是BOSS
   int _objectType;
   void setobjectType(int t);
   int getobjectType();


   CC_SYNTHESIZE(int,_addHPAddr,addHPAddr);

   CC_SYNTHESIZE(int,CruisingDirectX,CruisingDirectX);//巡逻方向 巡逻方向默认是向右
    //int CruisingDirectX;//巡逻方向 巡逻方向默认是向右

   CC_SYNTHESIZE(int,_curTargetType,curTargetType);// int _curTargetType;//当前攻击类型 主要是敌人要用到
   
   //战斗状态，是正在跑步还是正在战斗中
   int _states;
   void setstates(int t);
   int getstates();

   CC_SYNTHESIZE(int, _Batch, Batch);	// 敌人出兵的时候所属的波次

   //设置血量
   int _hp;
   void sethp(int h);
   int gethp();


   CC_SYNTHESIZE(float,_maxLevelHp,maxLevelHp);//最大等级时候的血量
   CC_SYNTHESIZE(int,_maxLevelAtk,maxLevelAtk);//最大等级时候的攻击力

   //在地下第几层 
   int _floorNum;
   void setfloorNum(int f);
   int getfloorNum();

   CC_SYNTHESIZE(int, _NextFloorNum, NextFloorNum); // 准备移动到下一层数，提前预警用

   int locationIndx;//第几格
   void setlocationIndx(int i);//设置在第几个个格子上
   int getlocationIndx();//获取在第几个格子上


    //小分类，是敌人的哪一种
   int _selfType;
   void setSelfType(int tt);
   int getSelfType();


   CC_SYNTHESIZE(int,_isBackWind,isBackWind);//是否正在被电风扇吹走
   //类型中的判断，飞行或者陆地的

  CC_SYNTHESIZE(int,_isFly,isFly);//是否是飞行的。默认是不飞行的

  CC_SYNTHESIZE(bool,_isSpecialColorFlag,isSpecialColorFlag);//是否存在特殊变色，否则的话，打斗受击颜色不换

   //暴击率，有些是有暴击率的
   CC_SYNTHESIZE(float,_CRI,CRI);//暴击率100%是1.0,30%的话是0.30
   //暴击伤害，这个是固定的
   CC_SYNTHESIZE(float,_CRI_damage,CRI_damage);//暴击伤害,固定值是200%，表示的话就是2.0

   //点击效果
   void clickedEffect();

   //闪烁效果
   void startTint();
   void stopTint();

   std::string _oname;//这个是骨骼动画的工程名
   std::string _oFileName;//这个是骨骼动画文件名，带有后缀的
   std::string getoName();
   std::string getoFileName();
  //读取的列表属性
   /* 近战小兵  近战中兵 近战大兵 近战王 铠甲小兵 铠甲中兵 铠甲大兵 铠甲王 远程小兵 远程中兵
   远程大兵 远程王 飞行小兵 飞行中兵 飞行大兵 飞行王 魔法小兵 魔法中兵 魔法大兵 魔法王 BOSS1 BOSS2 BOSS3 BOSS4 BOSS5
   */

   /**********
   war1-5  近程战士，远程攻击 肉盾兵种 飞行兵种 自爆兵种
   *******/
   CC_SYNTHESIZE(int,_maxlev,maxlev);//最大等级。。。。目前的所有的BOSS，战士，防御塔，陷阱的等级最大都是
   CC_SYNTHESIZE(int,_lev,lev);//等级，每一级的属性肯能不一样
   CC_SYNTHESIZE(float,_moveSpeedScaleOption,moveSpeedScaleOption);//分为1表示快，2表示中 3表示慢 
   CC_SYNTHESIZE(float,_moveSpeedScale,moveSpeedScale);//  读取list的时候是分为1表示快，2表示中 3表示慢  程序实现的时候自行判断啊，速度看Gameconfig的宏定义
   CC_SYNTHESIZE(float,_atkLen,atkLen);//攻击范围长度，是根据下面的atkLenOption 来写死的
   CC_SYNTHESIZE(int,_unlockMoney,unlockMoney);//解锁需要花的勇士之魂 比如：20
   CC_SYNTHESIZE(int,_callMoney,callMoney);//召唤花费的金额 比如：150
   CC_SYNTHESIZE(int,_upgradeMoney,upgradeMoney);//升级花费的金额，如果是金币类型或者是BOSS的勇士之魂 比如：200 如果是塔的话是到了第四级时候显示的是属于分支1的价格
   CC_SYNTHESIZE(int,_sellOutMoney,sellOutMoney);//卖掉所花的钱：比如：180
   CC_SYNTHESIZE(int,_maxHp,maxHp);//血量 比如：1500
   CC_SYNTHESIZE(int,_atkLenOption,atkLenOption);//攻击类型范围 1表示近程 2表示中程 3表示远程 
   CC_SYNTHESIZE(int,_atkOption,atkOption);//攻击类型 分为A1，A2，A3  1表示A1，2表示A2，3表示A3
   CC_SYNTHESIZE(int,_atk,atk);//攻击力 比如：300
   CC_SYNTHESIZE(int,_dfeOption,dfeOption);//防御力类型 分为D1，D2 ,1表示D1，2表示D2
   CC_SYNTHESIZE(float,_dfe,dfe);//防御力 这个是百分比：范围是0%-100% 比如20%填 0.2  如果是35%填0.35  
   CC_SYNTHESIZE(int,_atkSpeed,atkSpeed);//攻击速度 分为快中慢 1表示快，2表示中 3表示慢
   CC_SYNTHESIZE(float,_cdTime,cdTime);//部署冷却时间 比如冷却时间是3.5秒 填 3.5
   CC_SYNTHESIZE(int,_isGround,isGround);//是否是群体攻击，单体是1 ,群体是2
 
   /************************************************************************/
   /*                防御塔 
                     tower1=攻击速度快的塔 
                     tower2=攻击速度慢的塔  tower3=攻击速度慢的塔
				   tower4=攻击速度慢的塔 tower5=攻击速度慢的塔
  */
   /************************************************************************/
   CC_SYNTHESIZE(int,_tower_fly_atk,tower_fly_atk);//防御塔对空攻击力
   CC_SYNTHESIZE(float,_tower_cdTime,tower_cdTime);//这个主要是用于第二个防御塔的攻击间隔时间
  // CC_SYNTHESIZE(int,_tower_upgradeMoney,tower_upgradeMoney);//塔到了第4级需要的金钱分支2需要的金钱
  // CC_SYNTHESIZE(float,_tower_debuff_value,tower_debuff_value);//如果有debuff的时候，填写debuff的伤害值，没有的话为0
  // CC_SYNTHESIZE(float,_tower_debuff_keepTime,tower_debuff_keepTime);//有debuff属性的时候的持续时间 3.0
  // CC_SYNTHESIZE(float,_tower_Odds,tower_Odds);//触发几率 比如：40% 相当于0.4
   CC_SYNTHESIZE(int,_tower_targetNum,tower_targetNum);//是群体攻击时候的目标数目
   CC_SYNTHESIZE(float,_tower_atkCDTime,tower_atkCDTime);//是群体攻击时候的目标数目
  // CC_SYNTHESIZE(int,_tower_selectId,tower_selectId);//选择的分支ID值，0表示在4级前没有分支的，1表示第一个分支，2表示第二个分支

   /************************************************************************/
   /**  陷阱的  增加的属性 trap1,trap2 trap3 trap4 trap5分别对应为：
     trap1=攻击陷阱，trap2=固定目标陷阱，trap3=手动引爆陷阱，trap4=漏到下一层陷阱，trap5=DEBUFF陷阱   
	 **/
   /************************************************************************/
   CC_SYNTHESIZE(float,_trap_atkCDTime,trap_atkCDTime);//攻击间隔时间 比如是2秒就是2.0 这个只有trap2，trap4有，其他的全为0
   CC_SYNTHESIZE(float,_trap_buffValue,trap_buffValue);//buff伤害值  这个只有 trap5拥有 其他的全为0
   CC_SYNTHESIZE(float,_trap_keepTime,trap_keepTime);//持续时间 这个只有trap2,trap5拥有的属性 其他的全为0
   CC_SYNTHESIZE(int,_trap_targetNum,trap_targetNum);//范围内的攻击数量，如果是群攻的话，有最大的攻击数目
   
 
   /************************************************************************/
   /*     plist 列表里面的boss1,boss2 ,boss3 ,boss4 ,boss5分别对应为
          远程DPS英雄，近程DPS英雄，远程治疗英雄，近程肉盾英雄，远程支援英雄
		  */
   /************************************************************************/

   CC_SYNTHESIZE(int,_boss_exp,boss_exp);//英雄升级需要的是经验不是钱 比如是：300
   CC_SYNTHESIZE(int,_boss_reviveGlod,boss_reviveGold);		//英雄复活消耗金币
   CC_SYNTHESIZE(int,_boss_reviveSoul,boss_reviveSoul);		//英雄复活消耗英雄之魂

   //英雄的主动技能的
   CC_SYNTHESIZE(int,_bossSkillOneLevel,bossSkillOneLevel);//英雄技能1的等级
   CC_SYNTHESIZE(float,_skill1_skillCDTime,skill1_skillCDTime);//英雄技能冷却时间 比如：4.0
   CC_SYNTHESIZE(float,_skill1_skillKeepTime,skill1_skillKeepTime);//英雄技能持续时间 比如3.0 没有的话填0,召唤物的持续时间也是这个
   CC_SYNTHESIZE(float,_skill1_buff_atkScale,skill1_buff_atkScale);//buff 的攻击提升倍数 比如提升50% 就填 0.50，没有的话就填0
   CC_SYNTHESIZE(float,_skill1_buff_atkSpeed,skill1_buff_atkSpeed);//Buff的攻击速度提升，没有的话就填0 比如提升110%就填1.10
   CC_SYNTHESIZE(int,_skill1_buff_atk,skill1_buff_atk);//buff的功能攻击力，没有的话填0
   CC_SYNTHESIZE(int,_skill1_buff_target,skill1_buff_target);//buff对象目标类型：0表示自己，1表示敌人，2表示友军，3表示防御塔，4士兵
    CC_SYNTHESIZE(int,_skill1_buff_isGround,skill1_buff_isGround);//是否是群攻还是单体， 单体是1 ,群体是2
   CC_SYNTHESIZE(int,_skill1_buff_targetNum,skill1_buff_targeNum);//Buff 作用的数量
   CC_SYNTHESIZE(int,_skill1_buff_addHp,skill1_buff_addHp);//buff 效果的 的增加的血量,
   CC_SYNTHESIZE(int,_skill1_upgradeMoney,skill1_upgradeMoney);//技能1升级花费金额

   //英雄的被动技能1  
   CC_SYNTHESIZE(float,_skill2_buffValue,skill2_buffValue);//buffValue 对应的是： 攻击力增加，生命值增加，周围士兵的攻击力增加，护甲增加，防御塔的护甲增加  都是属于百分比 0.2
   CC_SYNTHESIZE(int,_skill2_extraMoney,skill2_extraMoney);//这个属于是额外奖励的金钱，第五个英雄的才有
   CC_SYNTHESIZE(int,_skill2_target,skill2_target);//目标类型：0表示自己，1表示敌人，2表示友军，3表示防御塔，4士兵
   CC_SYNTHESIZE(int,_skill2_isGround,skill2_isGround);//是否是群攻还是单体， 单体是1 ,群体是2
   CC_SYNTHESIZE(int,_skill2_targetNum,skill2_targetNum);//作用目标数目
   CC_SYNTHESIZE(int,_skill2_upgradeMoney,skill2_upgradeMoney);//技能2升级花费金额
    
   //英雄的被动技能2 
   CC_SYNTHESIZE(float,_skill3_buffValue,skill3_buffValue);// buffValue 对应的是： 暴击率，防御塔攻击力，周围士兵防御力增加，伤害反射，士兵的暴击率增加,都是属于百分比 0.2
   CC_SYNTHESIZE(int,_skill3_target,skill3_target);//目标类型：0表示自己，1表示敌人，2表示友军，3.表示防御塔，4士兵
   CC_SYNTHESIZE(int,_skill3_isGround,skill3_isGround);//是否是群攻还是单体， 单体是1 ,群体是2
   CC_SYNTHESIZE(int,_skill3_targetNum,skill3_targetNum);//作用目标数目
   CC_SYNTHESIZE(int,_skill3_upgradeMoney,skill3_upgradeMoney);//技能1升级花费金额


   //第五个英雄召唤物的
   CC_SYNTHESIZE(int,_skill1_callMonster_Hp,skill1_callMonster_Hp);//召唤怪物的血量
   CC_SYNTHESIZE(int,_skill1_callMonster_atkOption,skill1_callMonster_atkOption);//召唤怪物的攻击类型  攻击类型 分为A1，A2，A3  1表示A1，2表示A2，3表示A3 
   CC_SYNTHESIZE(int,_skill1_callMonster_atk,skill1_callMonster_atk);//召唤物的攻击力 
   CC_SYNTHESIZE(int,_skill1_callMonster_dfeOption,skill1_callMonster_dfeOption);//召唤物的防御类型   分为D1，D2 ,1表示D1，2表示D2
   CC_SYNTHESIZE(float,_skill1_callMonster_dfe,skill1_callMonster_dfe);//召唤物的防御力，这个是百分比，比如0.25相当于25%
   CC_SYNTHESIZE(int,_skill1_callMonster_atkLenOption,skill1_callMonster_atkLenOption);//召唤物的攻击距离 攻击类型范围 1表示近程 2表示中程 3表示远程  ATKLEN_REMOTE  3 远距离 ATKLEN_MID 2 中程距离   ATKLEN_NEAR 1  近距离
   CC_SYNTHESIZE(int,_skill1_callMonster_atkSpeed,skill1_callMonster_atkSpeed);//召唤物的攻击速度 攻击速度 分为快中慢 1表示快，2表示中 3表示慢
   CC_SYNTHESIZE(int,_skill1_callMonster_moveSpeedScale,skill1_callMonster_moveSpeedScale);// 读取list的时候是分为1表示快，2表示中 3表示慢
 
   CC_SYNTHESIZE(bool,_isRun,isRun);//是否正在跑步
     int targetAddr;//攻击对象的地址值
	 int gettargetAddr();
	 void settargetAddr(int t){ targetAddr=t;};

	 int fly_target_addr;
	 int getTargetsAddrs(){return fly_target_addr;};
	 void setTargetsAddrs(int addrs){fly_target_addr = addrs;};
	 bool _canAtkFlyObj;

	 cocostudio::Armature *_armature;
	 Armature* getArmature();//获取 骨骼动画
	 cocos2d::Layer *_layer;//遍历用的
	 int _MisPixel;//错位像素
	  

public:
	//效果掉血的效果
	virtual void reduceHpEffect(int t);
	void labelForCallback(Ref* pSender);
	void setReduceHp(int m_atkOption,int m_atk,float cri,float criDamge);
   

	//是否被选中的参数
	 bool _isSelect;
	 void setisSelect(bool f);
	 bool getisSelect();

	  int TimeCount;
	  void changeColorWithAtk();//受到攻击时候颜色变化
	  void changeBackColor(Ref *f);
	 
	

	 int isStart;
	 void setisStart(int b);
	 int getisStart();
	 void setUpdateForisStart();
	 

	  Animate* CreateAnimation(const char*FrameName,int max);//FrameName 和最大图片
	  void CreateAnimationForDeathObject(Point pos);
	  void DeathObjectCallBack(Ref *psender, bool _isOutDeath = false);		// 死亡回调，参数：_isOutDeath 是否非正常死亡

	  void BackHp();//3.0秒后回调 updateBackHp()，敌人是不可以调用的
	  void updateBackHp(Ref*f);//回血的。。。。每秒回调一次isBackHp()
	  void isBackHp(float dt);//判断是否回满血或者其他
	
	  bool updateLevel(int _level);

	  //...........获取信息....... 所在层的中心点和长度。。。。
	  void showLevel();
	  void hideLevel();
	  SceneData *_scenedata;
	  StoreyData *_storeydata;
	  Widget *_swg;

	  CC_SYNTHESIZE(int,_targetSelect,targetSelect);//主要用于判断飞行兵对陆地敌人的

	  CC_SYNTHESIZE(int,_tower3_selectNum,tower3_selectNum);//用于防御塔3胖子吸引人的计算器
	  CC_SYNTHESIZE(int,_tower3_selectNumMax,tower3_selectNumMax);//用于防御塔3胖子吸引人的最大数目
	  CC_SYNTHESIZE(int,_isAtkTower3Flag,isAtkTower3Flag);//判断是否在打了胖子3了
	 /************************************************************************/
	 /* 
	 创建 显示攻击范围的底片 隐藏，显示
	 */
	 /************************************************************************/
	  Sprite *l_atklenObject1,*l_atklenObject2;
	  void l_createCAtkLenObject();//创建攻击范围的底片
	  void l_atklenObjectHide();//隐藏攻击范围的底片
	  void l_atklenObjectShow();//显示攻击范围的底片
	  void l_atklenUpdateSize();//主要用于点击的时候还可以移动的对象身上,更新显示范围
	  //创建范围的显示


	  void myPauseAllAction(Ref *obj);
	  void myResumeAllAction(Ref *obj);
	  CC_SYNTHESIZE(int,_direct,direct);//方向。。。。

	  void pauseMyActionsAndSchedulers();
	  void resumeMyActionsAndSchedulers();

	  void MakeEnemyNoAttkMe();//让敌人不在攻击 我
	  void MakeWarToLoad();//让战士暂停下

	  CC_SYNTHESIZE(int,_isFrozen,isFrozen);//是否被冰冻住了
	  CC_SYNTHESIZE(int,_isSpeedDown,isSpeedDown);//是否被减速

	  //。。。。。。。。。。。。。。。。。。。。。。。。。。。。。
	  float findBossSkillBuffValue(int bossSelfType,int indexSkill);//寻找英雄技能的buff值

	  void skilladdHpEffect(int t,int skillLev);//治疗英雄的加血技能特效。。。
	  void skilladdHpCallFunc(Ref *f);

	  //暴击特效的。。。。。
	  void criEffect();
	   Armature *criarm;
	  void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); 
	 // void criEffectCallback(Ref *f);
 };
#endif