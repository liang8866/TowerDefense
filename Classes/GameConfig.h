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
#define STATES_READY 5 //��ж����ready��״̬����
#define STATES_LOCK  6 //��ж��������ʱ��״̬

//����������TAG
#define  START_SCENE_TAG 1
#define  LEVEL_SCENE_TAG 2
#define  GAME_SCENE_TAG 3
#define  LOAD_SCENE_TAG 4

 #define  TYPE_BOSS 0			// Hero Boss
 #define  TYPE_ENEMY 1			// ����
 #define  TYPE_TRAP 2			// ����
 #define  TYPE_TOWER 3			// ��
 #define  TYPE_WARRIOR 4		// սʿ
 #define  TYPE_NPC 5			// Npc
 #define  TYPE_DOOR 6			// ��
 #define  TYPE_ALTAR 7			// ��̳
 #define  TYPE_HEADSTONE 8		// Ĺ��
 #define  TYPE_MAGIC  9			//ħ��
 #define  TYPE_EVENT1 10		// �¼�1����͸ħ��
 #define  TYPE_EVENT106 11		// �¼�106������ħ��


 #define TYPE_BOSS_MAX_LEVEL 40	// Boss���ȼ�
 #define TYPE_ENEMY_MAX_LEVEL 3	
 #define TYPE_TRAP_MAX_LEVEL 3	
 #define TYPE_TOWER_MAX_LEVEL 3	
 #define TYPE_WARRIOR_MAX_LEVEL 3	


#define YES 1
#define NO 0

 //սʿ��
 /************************************************************************/
 /*  ���ڵ�սʿ
 ���̹�����ʿ�� 1
 Զ�̹�����ʿ�� 2
 ���й�����ʿ�� 3
 �Ա�������ʿ�� 4

 */
 /************************************************************************/
  
 #define  WAR_ITEM1 1
 #define  WAR_ITEM2 2
 #define  WAR_ITEM3 3
 #define  WAR_ITEM4 4
 #define  WAR_ITEM5 5

#define WAR_ITEM(a) WAR_ITEM##a

//#define WARRRIOR_SHORT 1//���̹�����ʿ��
//#define WARRRIOR_LONG 2//Զ�̹�����ʿ��
//#define WARRRIOR_FLY 3//���й�����ʿ��
//#define WARRRIOR_BOOM 4//�Ա�������ʿ��
//
 /*
 ������   tower1=�����ٶȿ���� 
	 tower2=�����ٶ�������  tower3=�����ٶ�������
	 tower4=�����ٶ������� tower5=�����ٶ�������
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
   /**  �����  ���ӵ����� trap1,trap2 trap3 trap4 trap5�ֱ��ӦΪ��
     trap1=�������壬trap2=�̶�Ŀ�����壬trap3=�ֶ��������壬trap4=©����һ�����壬trap5=DEBUFF����   
	 **/
   /************************************************************************/

#define TRAP_IETM1 1
#define TRAP_IETM2 2
#define TRAP_IETM3 3
#define TRAP_IETM4 4
#define TRAP_IETM5 5


#define TRAP_IETM(a) TRAP_IETM##a



 /*���˵�   enemy1---enemy15ememy2=��սС��  ��ս�б� ��ս��� ��ս�� ����С�� �����б� ���״�� ������ Զ��С�� Զ���б�
   Զ�̴�� Զ���� ����С�� �����б� ���д�� ������ ħ��С�� ħ���б� ħ����� ħ���� BOSS1 BOSS2 BOSS3 BOSS4 BOSS5 
	   **/
 #define ENEMY_ITEM1 1//��սС��
 #define ENEMY_ITEM2 2 //��ս�б�
 #define ENEMY_ITEM3 3 //��ս���
 #define ENEMY_ITEM4 4//��ս��
 #define ENEMY_ITEM5 5//����С��
 #define ENEMY_ITEM6 6// �����б�
 #define ENEMY_ITEM7 7//���״��
 #define ENEMY_ITEM8 8//������
 #define ENEMY_ITEM9 9 //Զ��С��
 #define ENEMY_ITEM10 10 //Զ���б�
 #define ENEMY_ITEM11 11//�̴��
 #define ENEMY_ITEM12 12//Զ����
 #define ENEMY_ITEM13 13//����С��
 #define ENEMY_ITEM14 14//�����б�
 #define ENEMY_ITEM15 15//���д��
 #define ENEMY_ITEM16 16// ������
 #define ENEMY_ITEM17 17//ħ��С��
 #define ENEMY_ITEM18 18//ħ���б�
 #define ENEMY_ITEM19 19//ħ�����
 #define ENEMY_ITEM20 20//ħ����
 #define ENEMY_ITEM21 21//������(ÿ��ÿ�½�һ��)
 #define ENEMY_ITEM22 22//ħ������

 #define ENEMY_BOSS1 30//BOSS1
 #define ENEMY_BOSS2 31//BOSS2
 #define ENEMY_BOSS3 32//BOSS3
 #define ENEMY_BOSS4 33//BOSS4
 #define ENEMY_BOSS5 34//BOSS5


 /************************************************************************/
   /*     plist �б������boss1,boss2 ,boss3 ,boss4 ,boss5�ֱ��ӦΪ
          Զ��DPSӢ�ۣ�����DPSӢ�ۣ�Զ������Ӣ�ۣ��������Ӣ�ۣ�Զ��֧ԮӢ��
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


#define MAGIC_ATK 1           //�������͵�
#define MAGIC_SHEEP 2         //�������͵�
#define MAGIC_TRANSMIT 3      //�������͵�
#define MAGIC_GODZILLA 4      //��˹�����͵�
#define MAGIC_BOSS5SKILL 5    //Ӣ��5�ļ���


#define TRANSMIT_IN    1 
#define TRANSMIT_OUT   2

 #define FLOOR_NUM1 1
 #define FLOOR_NUM2 2
 #define FLOOR_NUM3 3
 #define FLOOR_NUM4 4
 #define FLOOR_NUM5 5
 #define FLOOR_NUM6 6

#define  MISPLACElOCATION 5



#define SCENE_MAXCOUNT 6	// һ������������6
#define SCENE_STOREY_HOME 0		// 0���ϳ�
#define SCENE_STOREY_ONE 1		// 1��
#define SCENE_STOREY_TWO 2		// 2��
#define SCENE_STOREY_THREE 3	// 3��
#define SCENE_STOREY_FOUR 4	// 4��
#define SCENE_STOREY_TOP 5		// 5��
#define SCENE_STOREY_GROUND 6	// 6��_ û�õĵ���

#define SCENE_STOREY_GROUND_INDEX 10086		// 6��_ û�õĵ�������
#define SCENE_STOREY_MOUSE_CLICK_TAG 10087	// �����tag
#define SCENE_STOREY_DOOR_INDEX 10088		// Home�����ŵ�tag

#define CAN_ADD_OBJECT_POS 0			// ������������
#define NOT_ADD_OBJECT_POS 1			// λ�ñ�ռ
#define NOT_ADD_OBJECT_PK_FULL 2		// �ҷ����������
#define NOT_ADD_OBJECT_HERO_FULL 3		// �ҷ�Ӣ��������
#define NOT_ADD_OBJECT_HOME 4			// �ϳ�
#define NOT_ADD_OBJECT_STOREY 5			// �Ҳ���������
 
#define  SECOND_BACK_BLOOD 300 //��ս��״̬��ʱ��Ļ�Ѫ�� 

#define ATK_OPTION1  1 //�������� 1
#define ATK_OPTION2  2  //�������� 2
#define ATK_OPTION3  3  //�������� 3

#define DFE_OPTION1  1   //�������� 1
#define DFE_OPTION2  2   //�������� 2


#define  MOVESPEED_FAST 1 //��
#define  MOVESPEED_MID  2 //�� 
#define  MOVESPEED_SLOW 3 //��

#define  MOVESPEED_FAST_VALUE 1.5 //��
#define  MOVESPEED_MID_VALUE  1.0 //�� 
#define  MOVESPEED_SLOW_VALUE 0.5 //��


#define ATKLEN_REMOTE  3  //Զ����
#define ATKLEN_MID     2  //�г̾���
#define ATKLEN_NEAR    1  // ������
#define LEN_REMOTE   350
#define LEN_MID      250
#define LEN_NEAR     10

#define LEN_SCALE    -20 //��������ս������

#define  GROUP_NO  1 //�ǵ���
#define  GROUP_YES 2 //��Ⱥ��


#define  TARGET_SELF    0 //��ʾ�������Լ�
#define  TARGET_ENEMY   1 //��ʾ���ǵ���
#define  TARGET_FRIEND  2 //��ʾ�����Ѿ�
#define  TARGET_TOWER   3 //��ʾ���Ƿ�����
#define  TARGET_WAR     4 //��ʾ����ʿ��

#define  WINDOW_TOUCH_TYPE_BEGIN	0	// �����¼� begin
#define  WINDOW_TOUCH_TYPE_MOVE		1	// �����¼� move
#define  WINDOW_TOUCH_TYPE_END		2	// �����¼� end
#define  WINDOW_TOUCH_TYPE_CANCEL	3	// �����¼� cancel

#define  FILE_CONFIG_PLIST	"config.plist"
#define  FILE_SCENE_PLIST	"plist/scene.plist"


// �����㼶ԽС���ȼ�Խ�� (ע��UI jsonĬ�ϲ㼶Ϊ0)
#define  TOUCH_CREATE_OBJECT -20
#define  TOUCH_Do_Magic -16				// �ͷż���
#define  TOUCH_UI_BagMessage -15         // �������汳����Ϣ��
#define  TOUCH_UI_PRIORITY -10         // ui��������  
#define  TOUCH_EVENT106		-7			// �����ħ��
#define  TOUCH_HEADSTONE	-6			// Ĺ��
#define  TOUCH_BOSS_ZORDER -5			// boss�Ĵ����㼶
#define  TOUCH_WARRIOR_ZORDER -4		// ʿ���Ĵ����㼶
#define  TOUCH_WARRIOR_TOWER -3		  // �������Ĵ����㼶
#define  TOUCH_TRAP_ZORDER -2        //��������ȼ�


// �������Ĭ��tagֵ
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


#define  D_LEFT  -1 //������
#define  D_RIGHT  1 //������


#define READY_MAX_COUNT	7	// �����棬�·����ֵ�׼�����ӣ��������Ϊ7��

#define  BALL_WAR02 1 // Զ�̹�����սʿ�ŵ������ӵ�
#define  BALL_BOSS1 2 // ��ʾBOSS1����Ķ���
#define  BALL_BOSS3 3 //��ʾBOSS3����Ķ���
#define  BALL_BOSS5 4 //��ʾboss5�����ȥ�Ķ���


#define STOREY_MAX_OBJECT 4	// ÿ�����ɷ��õ�λ



#define BackHpTime 3.0 


//--------------------- Boss Ӣ�������� -----------------------------
#define  ALTAR_REVIVE_SWEET_COST	400		// Ӣ�ۼ�̳��������ǹ�
#define  ALTAR_REVIVE_SWEET_TIME	60		// Ӣ�ۼ�̳��������ǹ�����ʱ�䣬��λ��
#define  ALTAR_REVIVE_SOUL_COST		8		// Ӣ�ۼ�̳�������Ӣ��֮��
#define  ALTAR_REVIVE_SOUL_TIME		0		// Ӣ�ۼ�̳�������Ӣ�۸���ʱ�䣬��λ��

#define  AlTAR_PARENT_TAG 18355
#define  AlTAR_MY_TAG     18356



#define PASSIVESKILLS1 2 //Ӣ�۵ĵ�һ����������
#define PASSIVESKILLS2 3 //Ӣ�۵ĵڶ�����������

#define  RESETBOSSSKILL "changebossattribute"

#define HeroSkillMaxLevel 10	// Ӣ�ۼ��ܵȼ����10��

// ��Ʒ����
enum _BagItemType{
	ALL			= 0,	// ȫ��
	Property	= 1,	// ����
	Magic		= 2,	// ħ��
	Material	= 3,	// ����
	Box			= 4,	// ����
	Other		= 5,	// ����.��ң�Ӣ��֮���
	Clothes             // ʱװ
};

// Item ��Ʒ����
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

// ItemBox ��������
class _ItemBoxData
{
public:
	int			id;
	int			count;
	float		percent;
	string		key;
	string		name;
};

// �ӷ�����������Ӣ������
class _HeroData
{
public:
	int			id;					// id
	string		name;				// ��������
	int			unLock;				// �Ƿ����
	int			level;				// �ȼ�
	int			star;				// �Ǽ�
	int			blessing;			// ף��ֵ
	int			skill1_level;		// ����1�ȼ�
	int			skill2_level;		// ����2�ȼ�
	int			skill3_level;		// ����3�ȼ�
	float		successRate;		// ��ǰ�ɹ���
	int			deletTime;			// ף��ֵ���ʱ����

	int			isReviveTime;		// �Ƿ��ǹ�����ʱ��	
	float		revivePosX;			// ������x�����

	int			isDead;				// �Ƿ�����
	int			storeyIndex;		// ���ڲ㼶
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
