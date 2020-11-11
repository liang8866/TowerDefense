#ifndef __GAME_CONFIG__
#define __GAME_CONFIG__

#include <iostream>
#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"
#include "LoadingBarTimer.h"
#include <time.h>


 USING_NS_CC;
 USING_NS_CC_EXT;
 using namespace cocostudio;
 using namespace ui;
 using namespace CocosDenshion;
 using namespace std;

#define PTM_RATIO 32

#define MyfontName "Arial-BoldMT"

#define STATES_DEFAULT 0
#define STATES_LOAD 1
#define STATES_RUN 2
#define STATES_FIGHTING 3
#define STATES_DEATH 4
#define STATES_READY 5 //防卸塔１ready的状态　　
#define STATES_LOCK  6 //防卸塔２发射时的状态

//各个场景的TAG
#define  START_SCENE_TAG 1
#define  LEVEL_SCENE_TAG 2
#define  GAME_SCENE_TAG 3
#define  LOAD_SCENE_TAG 4

 #define  TYPE_BOSS 0			// Hero Boss
 #define  TYPE_ENEMY 1			// 敌人
 #define  TYPE_TRAP 2			// 陷阱
 #define  TYPE_TOWER 3			// 塔
 #define  TYPE_WARRIOR 4		// 战士
 #define  TYPE_NPC 5			// Npc
 #define  TYPE_DOOR 6			// 门
 #define  TYPE_ALTAR 7			// 祭坛
 #define  TYPE_HEADSTONE 8		// 墓碑
 #define  TYPE_MAGIC  9			//魔法
 #define  TYPE_EVENT1 10		// 事件1，渗透魔法
 #define  TYPE_EVENT106 11		// 事件106，特殊魔法


 #define TYPE_BOSS_MAX_LEVEL 40	// Boss最大等级
 #define TYPE_ENEMY_MAX_LEVEL 3	
 #define TYPE_TRAP_MAX_LEVEL 3	
 #define TYPE_TOWER_MAX_LEVEL 3	
 #define TYPE_WARRIOR_MAX_LEVEL 3	


#define YES 1
#define NO 0

 //战士的
 /************************************************************************/
 /*  对于的战士
 近程攻击的士兵 1
 远程攻击的士兵 2
 飞行攻击的士兵 3
 自爆攻击的士兵 4

 */
 /************************************************************************/
  
 #define  WAR_ITEM1 1
 #define  WAR_ITEM2 2
 #define  WAR_ITEM3 3
 #define  WAR_ITEM4 4
 #define  WAR_ITEM5 5

#define WAR_ITEM(a) WAR_ITEM##a

//#define WARRRIOR_SHORT 1//近程攻击的士兵
//#define WARRRIOR_LONG 2//远程攻击的士兵
//#define WARRRIOR_FLY 3//飞行攻击的士兵
//#define WARRRIOR_BOOM 4//自爆攻击的士兵
//
 /*
 防御塔   tower1=攻击速度快的塔 
	 tower2=攻击速度慢的塔  tower3=攻击速度慢的塔
	 tower4=攻击速度慢的塔 tower5=攻击速度慢的塔
	 */
#define TOWER_ITEM1 1
#define TOWER_ITEM2 2
#define TOWER_ITEM3 3
#define TOWER_ITEM4 4
#define TOWER_ITEM5 5

#define TOWER_ATK_LEN_NEAR   200
#define TOWER_ATK_LEN_MID    300
#define TOWER_ATK_LEN_FAR    400
#define TOWER_ITEM4_ATK_DELAY 4.0
#define TOWER_ITEM4_CD_TIME   15.0
#define TOWER_ITEM4_ATK_LAST  2.0

#define TOWER_ITEM5_CD_TIME  5.0

#define TOWER_ITEM(a) TOWER_ITEM##a

  /************************************************************************/
   /**  陷阱的  增加的属性 trap1,trap2 trap3 trap4 trap5分别对应为：
     trap1=攻击陷阱，trap2=固定目标陷阱，trap3=手动引爆陷阱，trap4=漏到下一层陷阱，trap5=DEBUFF陷阱   
	 **/
   /************************************************************************/

#define TRAP_IETM1 1
#define TRAP_IETM2 2
#define TRAP_IETM3 3
#define TRAP_IETM4 4
#define TRAP_IETM5 5


#define TRAP_IETM(a) TRAP_IETM##a



 /*敌人的   enemy1---enemy15ememy2=近战小兵  近战中兵 近战大兵 近战王 铠甲小兵 铠甲中兵 铠甲大兵 铠甲王 远程小兵 远程中兵
   远程大兵 远程王 飞行小兵 飞行中兵 飞行大兵 飞行王 魔法小兵 魔法中兵 魔法大兵 魔法王 BOSS1 BOSS2 BOSS3 BOSS4 BOSS5 
	   **/
 #define ENEMY_ITEM1 1//近战小兵
 #define ENEMY_ITEM2 2 //近战中兵
 #define ENEMY_ITEM3 3 //近战大兵
 #define ENEMY_ITEM4 4//近战王
 #define ENEMY_ITEM5 5//铠甲小兵
 #define ENEMY_ITEM6 6// 铠甲中兵
 #define ENEMY_ITEM7 7//铠甲大兵
 #define ENEMY_ITEM8 8//铠甲王
 #define ENEMY_ITEM9 9 //远程小兵
 #define ENEMY_ITEM10 10 //远程中兵
 #define ENEMY_ITEM11 11//程大兵
 #define ENEMY_ITEM12 12//远程王
 #define ENEMY_ITEM13 13//飞行小兵
 #define ENEMY_ITEM14 14//飞行中兵
 #define ENEMY_ITEM15 15//飞行大兵
 #define ENEMY_ITEM16 16// 飞行王
 #define ENEMY_ITEM17 17//魔法小兵
 #define ENEMY_ITEM18 18//魔法中兵
 #define ENEMY_ITEM19 19//魔法大兵
 #define ENEMY_ITEM20 20//魔法王
 #define ENEMY_ITEM21 21//宝箱王(每天每章节一个)
 #define ENEMY_ITEM22 22//魔法敌人

 #define ENEMY_BOSS1 30//BOSS1
 #define ENEMY_BOSS2 31//BOSS2
 #define ENEMY_BOSS3 32//BOSS3
 #define ENEMY_BOSS4 33//BOSS4
 #define ENEMY_BOSS5 34//BOSS5


 /************************************************************************/
   /*     plist 列表里面的boss1,boss2 ,boss3 ,boss4 ,boss5分别对应为
          远程DPS英雄，近程DPS英雄，远程治疗英雄，近程肉盾英雄，远程支援英雄
		  */
   /************************************************************************/
 #define  BOSS_ITEM1  1
 #define  BOSS_ITEM2  2
 #define  BOSS_ITEM3  3
 #define  BOSS_ITEM4  4
 #define  BOSS_ITEM5  5

#define BOSS_ITEM(a) BOSS_ITEM##a

enum DoorType{
	InsideDoor = 1,
	OutDoor
};


#define MAGIC_ATK 1           //攻击类型的
#define MAGIC_SHEEP 2         //变羊类型的
#define MAGIC_TRANSMIT 3      //传送类型的
#define MAGIC_GODZILLA 4      //哥斯拉类型的
#define MAGIC_BOSS5SKILL 5    //英雄5的技能


#define TRANSMIT_IN    1 
#define TRANSMIT_OUT   2

 #define FLOOR_NUM1 1
 #define FLOOR_NUM2 2
 #define FLOOR_NUM3 3
 #define FLOOR_NUM4 4
 #define FLOOR_NUM5 5
 #define FLOOR_NUM6 6

#define  MISPLACElOCATION 5



#define SCENE_MAXCOUNT 6	// 一个场景最大层数6
#define SCENE_STOREY_HOME 0		// 0层老巢
#define SCENE_STOREY_ONE 1		// 1层
#define SCENE_STOREY_TWO 2		// 2层
#define SCENE_STOREY_THREE 3	// 3层
#define SCENE_STOREY_FOUR 4	// 4层
#define SCENE_STOREY_TOP 5		// 5层
#define SCENE_STOREY_GROUND 6	// 6层_ 没用的地面

#define SCENE_STOREY_GROUND_INDEX 10086		// 6层_ 没用的地面索引
#define SCENE_STOREY_MOUSE_CLICK_TAG 10087	// 鼠标点击tag
#define SCENE_STOREY_DOOR_INDEX 10088		// Home结束门的tag

#define CAN_ADD_OBJECT_POS 0			// 可以正常创建
#define NOT_ADD_OBJECT_POS 1			// 位置被占
#define NOT_ADD_OBJECT_PK_FULL 2		// 我方物件数量满
#define NOT_ADD_OBJECT_HERO_FULL 3		// 我方英雄数量满
#define NOT_ADD_OBJECT_HOME 4			// 老巢
#define NOT_ADD_OBJECT_STOREY 5			// 找不到层数据
 
#define  SECOND_BACK_BLOOD 300 //不战斗状态的时候的回血量 

#define ATK_OPTION1  1 //攻击类型 1
#define ATK_OPTION2  2  //攻击类型 2
#define ATK_OPTION3  3  //攻击类型 3

#define DFE_OPTION1  1   //防御类型 1
#define DFE_OPTION2  2   //防御类型 2


#define  MOVESPEED_FAST 1 //快
#define  MOVESPEED_MID  2 //中 
#define  MOVESPEED_SLOW 3 //慢

#define  MOVESPEED_FAST_VALUE 1.5 //快
#define  MOVESPEED_MID_VALUE  1.0 //中 
#define  MOVESPEED_SLOW_VALUE 0.5 //慢


#define ATKLEN_REMOTE  3  //远距离
#define ATKLEN_MID     2  //中程距离
#define ATKLEN_NEAR    1  // 近距离
#define LEN_REMOTE   350
#define LEN_MID      250
#define LEN_NEAR     10

#define LEN_SCALE    -20 //用于缩进战斗区间

#define  GROUP_NO  1 //是单体
#define  GROUP_YES 2 //是群体


#define  TARGET_SELF    0 //表示对象是自己
#define  TARGET_ENEMY   1 //表示的是敌人
#define  TARGET_FRIEND  2 //表示的是友军
#define  TARGET_TOWER   3 //表示的是防御塔
#define  TARGET_WAR     4 //表示的是士兵

#define  WINDOW_TOUCH_TYPE_BEGIN	0	// 窗口事件 begin
#define  WINDOW_TOUCH_TYPE_MOVE		1	// 窗口事件 move
#define  WINDOW_TOUCH_TYPE_END		2	// 窗口事件 end
#define  WINDOW_TOUCH_TYPE_CANCEL	3	// 窗口事件 cancel

#define  FILE_CONFIG_PLIST	"config.plist"
#define  FILE_SCENE_PLIST	"plist/scene.plist"


// 触摸层级越小优先级越高 (注意UI json默认层级为0)
#define  TOUCH_CREATE_OBJECT -20
#define  TOUCH_Do_Magic -16				// 释放技能
#define  TOUCH_UI_BagMessage -15         // 二级界面背包信息框
#define  TOUCH_UI_PRIORITY -10         // ui界面屏蔽  
#define  TOUCH_EVENT106		-7			// 掉落的魔法
#define  TOUCH_HEADSTONE	-6			// 墓碑
#define  TOUCH_BOSS_ZORDER -5			// boss的触摸层级
#define  TOUCH_WARRIOR_ZORDER -4		// 士兵的触摸层级
#define  TOUCH_WARRIOR_TOWER -3		  // 防御塔的触摸层级
#define  TOUCH_TRAP_ZORDER -2        //陷阱的优先级


// 弹出框的默认tag值
#define  UI_PROMPT_TAG  251


#define  CHANGE_UNIT_PRICE  "change_unit_price"
#define  CHANGE_GOLD_NUM    "change_gold_number"

typedef void (Ref::*SEL_COMM)();
#define comm_selector(_SELECTOR) static_cast<SEL_COMM>(&_SELECTOR)

#define  PAUSEKEY "PAUSEKEY"
#define RESUMEKEY "RESUMEKEY"
#define  FORCESTART "enemy_forced_comeIn"

#define UInextPAUSEKEY "UInextPAUSEKEY"
#define UInextRESUMEKEY "UInextRESUMEKEY"
#define SELECTEDUIREMOVE "removeSelectedUI"

#define BUYHEROSUCCESS "pay_hero_successful"
#define BUYSOULSUCCESS "pay_soul_successful"


#define  D_LEFT  -1 //向左走
#define  D_RIGHT  1 //向右走


#define READY_MAX_COUNT	7	// 主界面，下方兵种的准备格子，最大数量为7个

#define  BALL_WAR02 1 // 远程攻击的战士放的抢的子弹
#define  BALL_BOSS1 2 // 表示BOSS1发射的东东
#define  BALL_BOSS3 3 //表示BOSS3发射的东东
#define  BALL_BOSS5 4 //表示boss5发射出去的东东


#define STOREY_MAX_OBJECT 4	// 每层最多可放置单位



#define BackHpTime 3.0 


//--------------------- Boss 英雄屋数据 -----------------------------
#define  ALTAR_REVIVE_SWEET_COST	400		// 英雄祭坛复活，消耗糖果
#define  ALTAR_REVIVE_SWEET_TIME	60		// 英雄祭坛复活，消耗糖果复活时间，单位秒
#define  ALTAR_REVIVE_SOUL_COST		8		// 英雄祭坛复活，消耗英雄之魂
#define  ALTAR_REVIVE_SOUL_TIME		0		// 英雄祭坛复活，消耗英雄复活时间，单位秒

#define  AlTAR_PARENT_TAG 18355
#define  AlTAR_MY_TAG     18356



#define PASSIVESKILLS1 2 //英雄的第一个被动技能
#define PASSIVESKILLS2 3 //英雄的第二个被动技能

#define  RESETBOSSSKILL "changebossattribute"

#define HeroSkillMaxLevel 10	// 英雄技能等级最大10级

// 物品类型
enum _BagItemType{
	ALL			= 0,	// 全部
	Property	= 1,	// 道具
	Magic		= 2,	// 魔法
	Material	= 3,	// 材料
	Box			= 4,	// 宝箱
	Other		= 5,	// 虚拟.金币，英雄之魂等
	Clothes             // 时装
};

// Item 物品数据
class _BagItemInfo
{
public:
	int			_index;
	int			_id;		
	int			_type;
	int			_count;
	float		_value;
	string		_name;
	string		_desc;
	string		_icon;
	Widget*		_itemCell;
	int			_isUse;
};

// ItemBox 宝箱数据
class _ItemBoxData
{
public:
	int			id;
	int			count;
	float		percent;
	string		key;
	string		name;
};

// 从服务器过来的英雄数据
class _HeroData
{
public:
	int			id;					// id
	string		name;				// 中文名称
	int			unLock;				// 是否解锁
	int			level;				// 等级
	int			star;				// 星级
	int			blessing;			// 祝福值
	int			skill1_level;		// 技能1等级
	int			skill2_level;		// 技能2等级
	int			skill3_level;		// 技能3等级
	float		successRate;		// 当前成功率
	int			deletTime;			// 祝福值清空时间间隔

	int			isReviveTime;		// 是否糖果复活时间	
	float		revivePosX;			// 死亡的x坐标点

	int			isDead;				// 是否死亡
	int			storeyIndex;		// 所在层级
};

#define _HeroDataKey_Index_id			1		
#define _HeroDataKey_Index_unLock		2		
#define _HeroDataKey_Index_level		3		
#define _HeroDataKey_Index_star			4		
#define _HeroDataKey_Index_blessing		5
#define _HeroDataKey_Index_skill1_level 6		
#define _HeroDataKey_Index_skill2_level 7		
#define _HeroDataKey_Index_skill3_level 8	
#define _HeroDataKey_Index_successRate	9		
#define _HeroDataKey_Index_deletTime	10		
#define _HeroDataKey_Index_revivieTime	11
#define _HeroDataKey_Index_reviviePosX	12
#define _HeroDataKey_Index_isDead		13
#define _HeroDataKey_Index_storeyIndex	14

#define  SKILLCOLOR Color3B(158,78,58)

#ifdef __cplusplus
extern "C" {
#endif
	double getDeviceTime();

	double getServerTime();

	double getDeltaT();

	void setDeltaT(double t);

	tm* getTm(double t);
#ifdef __cplusplus
}
#endif

#define  bossSkillOne_Level_1 4
#define  bossSkillOne_Level_2  9


#include "IAPJni.h"

#endif // __GAME_CONFIG__
