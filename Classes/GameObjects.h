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
  
   //�����б���������������������������������������������������������

   CC_SYNTHESIZE(int,_isIntimidate,isIntimidate);//�Ƿ񱻿���)

    int isPlaySkill;//�Ƿ����ͷż���

   //��Ҫ�����ͣ��ǣ����壬ʿ������BOSS
   int _objectType;
   void setobjectType(int t);
   int getobjectType();


   CC_SYNTHESIZE(int,_addHPAddr,addHPAddr);

   CC_SYNTHESIZE(int,CruisingDirectX,CruisingDirectX);//Ѳ�߷��� Ѳ�߷���Ĭ��������
    //int CruisingDirectX;//Ѳ�߷��� Ѳ�߷���Ĭ��������

   CC_SYNTHESIZE(int,_curTargetType,curTargetType);// int _curTargetType;//��ǰ�������� ��Ҫ�ǵ���Ҫ�õ�
   
   //ս��״̬���������ܲ���������ս����
   int _states;
   void setstates(int t);
   int getstates();

   CC_SYNTHESIZE(int, _Batch, Batch);	// ���˳�����ʱ�������Ĳ���

   //����Ѫ��
   int _hp;
   void sethp(int h);
   int gethp();


   CC_SYNTHESIZE(float,_maxLevelHp,maxLevelHp);//���ȼ�ʱ���Ѫ��
   CC_SYNTHESIZE(int,_maxLevelAtk,maxLevelAtk);//���ȼ�ʱ��Ĺ�����

   //�ڵ��µڼ��� 
   int _floorNum;
   void setfloorNum(int f);
   int getfloorNum();

   CC_SYNTHESIZE(int, _NextFloorNum, NextFloorNum); // ׼���ƶ�����һ��������ǰԤ����

   int locationIndx;//�ڼ���
   void setlocationIndx(int i);//�����ڵڼ�����������
   int getlocationIndx();//��ȡ�ڵڼ���������


    //С���࣬�ǵ��˵���һ��
   int _selfType;
   void setSelfType(int tt);
   int getSelfType();


   CC_SYNTHESIZE(int,_isBackWind,isBackWind);//�Ƿ����ڱ�����ȴ���
   //�����е��жϣ����л���½�ص�

  CC_SYNTHESIZE(int,_isFly,isFly);//�Ƿ��Ƿ��еġ�Ĭ���ǲ����е�

  CC_SYNTHESIZE(bool,_isSpecialColorFlag,isSpecialColorFlag);//�Ƿ���������ɫ������Ļ������ܻ���ɫ����

   //�����ʣ���Щ���б����ʵ�
   CC_SYNTHESIZE(float,_CRI,CRI);//������100%��1.0,30%�Ļ���0.30
   //�����˺�������ǹ̶���
   CC_SYNTHESIZE(float,_CRI_damage,CRI_damage);//�����˺�,�̶�ֵ��200%����ʾ�Ļ�����2.0

   //���Ч��
   void clickedEffect();

   //��˸Ч��
   void startTint();
   void stopTint();

   std::string _oname;//����ǹ��������Ĺ�����
   std::string _oFileName;//����ǹ��������ļ��������к�׺��
   std::string getoName();
   std::string getoFileName();
  //��ȡ���б�����
   /* ��սС��  ��ս�б� ��ս��� ��ս�� ����С�� �����б� ���״�� ������ Զ��С�� Զ���б�
   Զ�̴�� Զ���� ����С�� �����б� ���д�� ������ ħ��С�� ħ���б� ħ����� ħ���� BOSS1 BOSS2 BOSS3 BOSS4 BOSS5
   */

   /**********
   war1-5  ����սʿ��Զ�̹��� ��ܱ��� ���б��� �Ա�����
   *******/
   CC_SYNTHESIZE(int,_maxlev,maxlev);//���ȼ���������Ŀǰ�����е�BOSS��սʿ��������������ĵȼ������
   CC_SYNTHESIZE(int,_lev,lev);//�ȼ���ÿһ�������Կ��ܲ�һ��
   CC_SYNTHESIZE(float,_moveSpeedScaleOption,moveSpeedScaleOption);//��Ϊ1��ʾ�죬2��ʾ�� 3��ʾ�� 
   CC_SYNTHESIZE(float,_moveSpeedScale,moveSpeedScale);//  ��ȡlist��ʱ���Ƿ�Ϊ1��ʾ�죬2��ʾ�� 3��ʾ��  ����ʵ�ֵ�ʱ�������жϰ����ٶȿ�Gameconfig�ĺ궨��
   CC_SYNTHESIZE(float,_atkLen,atkLen);//������Χ���ȣ��Ǹ��������atkLenOption ��д����
   CC_SYNTHESIZE(int,_unlockMoney,unlockMoney);//������Ҫ������ʿ֮�� ���磺20
   CC_SYNTHESIZE(int,_callMoney,callMoney);//�ٻ����ѵĽ�� ���磺150
   CC_SYNTHESIZE(int,_upgradeMoney,upgradeMoney);//�������ѵĽ�����ǽ�����ͻ�����BOSS����ʿ֮�� ���磺200 ��������Ļ��ǵ��˵��ļ�ʱ����ʾ�������ڷ�֧1�ļ۸�
   CC_SYNTHESIZE(int,_sellOutMoney,sellOutMoney);//����������Ǯ�����磺180
   CC_SYNTHESIZE(int,_maxHp,maxHp);//Ѫ�� ���磺1500
   CC_SYNTHESIZE(int,_atkLenOption,atkLenOption);//�������ͷ�Χ 1��ʾ���� 2��ʾ�г� 3��ʾԶ�� 
   CC_SYNTHESIZE(int,_atkOption,atkOption);//�������� ��ΪA1��A2��A3  1��ʾA1��2��ʾA2��3��ʾA3
   CC_SYNTHESIZE(int,_atk,atk);//������ ���磺300
   CC_SYNTHESIZE(int,_dfeOption,dfeOption);//���������� ��ΪD1��D2 ,1��ʾD1��2��ʾD2
   CC_SYNTHESIZE(float,_dfe,dfe);//������ ����ǰٷֱȣ���Χ��0%-100% ����20%�� 0.2  �����35%��0.35  
   CC_SYNTHESIZE(int,_atkSpeed,atkSpeed);//�����ٶ� ��Ϊ������ 1��ʾ�죬2��ʾ�� 3��ʾ��
   CC_SYNTHESIZE(float,_cdTime,cdTime);//������ȴʱ�� ������ȴʱ����3.5�� �� 3.5
   CC_SYNTHESIZE(int,_isGround,isGround);//�Ƿ���Ⱥ�幥����������1 ,Ⱥ����2
 
   /************************************************************************/
   /*                ������ 
                     tower1=�����ٶȿ���� 
                     tower2=�����ٶ�������  tower3=�����ٶ�������
				   tower4=�����ٶ������� tower5=�����ٶ�������
  */
   /************************************************************************/
   CC_SYNTHESIZE(int,_tower_fly_atk,tower_fly_atk);//�������Կչ�����
   CC_SYNTHESIZE(float,_tower_cdTime,tower_cdTime);//�����Ҫ�����ڵڶ����������Ĺ������ʱ��
  // CC_SYNTHESIZE(int,_tower_upgradeMoney,tower_upgradeMoney);//�����˵�4����Ҫ�Ľ�Ǯ��֧2��Ҫ�Ľ�Ǯ
  // CC_SYNTHESIZE(float,_tower_debuff_value,tower_debuff_value);//�����debuff��ʱ����дdebuff���˺�ֵ��û�еĻ�Ϊ0
  // CC_SYNTHESIZE(float,_tower_debuff_keepTime,tower_debuff_keepTime);//��debuff���Ե�ʱ��ĳ���ʱ�� 3.0
  // CC_SYNTHESIZE(float,_tower_Odds,tower_Odds);//�������� ���磺40% �൱��0.4
   CC_SYNTHESIZE(int,_tower_targetNum,tower_targetNum);//��Ⱥ�幥��ʱ���Ŀ����Ŀ
   CC_SYNTHESIZE(float,_tower_atkCDTime,tower_atkCDTime);//��Ⱥ�幥��ʱ���Ŀ����Ŀ
  // CC_SYNTHESIZE(int,_tower_selectId,tower_selectId);//ѡ��ķ�֧IDֵ��0��ʾ��4��ǰû�з�֧�ģ�1��ʾ��һ����֧��2��ʾ�ڶ�����֧

   /************************************************************************/
   /**  �����  ���ӵ����� trap1,trap2 trap3 trap4 trap5�ֱ��ӦΪ��
     trap1=�������壬trap2=�̶�Ŀ�����壬trap3=�ֶ��������壬trap4=©����һ�����壬trap5=DEBUFF����   
	 **/
   /************************************************************************/
   CC_SYNTHESIZE(float,_trap_atkCDTime,trap_atkCDTime);//�������ʱ�� ������2�����2.0 ���ֻ��trap2��trap4�У�������ȫΪ0
   CC_SYNTHESIZE(float,_trap_buffValue,trap_buffValue);//buff�˺�ֵ  ���ֻ�� trap5ӵ�� ������ȫΪ0
   CC_SYNTHESIZE(float,_trap_keepTime,trap_keepTime);//����ʱ�� ���ֻ��trap2,trap5ӵ�е����� ������ȫΪ0
   CC_SYNTHESIZE(int,_trap_targetNum,trap_targetNum);//��Χ�ڵĹ��������������Ⱥ���Ļ��������Ĺ�����Ŀ
   
 
   /************************************************************************/
   /*     plist �б������boss1,boss2 ,boss3 ,boss4 ,boss5�ֱ��ӦΪ
          Զ��DPSӢ�ۣ�����DPSӢ�ۣ�Զ������Ӣ�ۣ��������Ӣ�ۣ�Զ��֧ԮӢ��
		  */
   /************************************************************************/

   CC_SYNTHESIZE(int,_boss_exp,boss_exp);//Ӣ��������Ҫ���Ǿ��鲻��Ǯ �����ǣ�300
   CC_SYNTHESIZE(int,_boss_reviveGlod,boss_reviveGold);		//Ӣ�۸������Ľ��
   CC_SYNTHESIZE(int,_boss_reviveSoul,boss_reviveSoul);		//Ӣ�۸�������Ӣ��֮��

   //Ӣ�۵��������ܵ�
   CC_SYNTHESIZE(int,_bossSkillOneLevel,bossSkillOneLevel);//Ӣ�ۼ���1�ĵȼ�
   CC_SYNTHESIZE(float,_skill1_skillCDTime,skill1_skillCDTime);//Ӣ�ۼ�����ȴʱ�� ���磺4.0
   CC_SYNTHESIZE(float,_skill1_skillKeepTime,skill1_skillKeepTime);//Ӣ�ۼ��ܳ���ʱ�� ����3.0 û�еĻ���0,�ٻ���ĳ���ʱ��Ҳ�����
   CC_SYNTHESIZE(float,_skill1_buff_atkScale,skill1_buff_atkScale);//buff �Ĺ����������� ��������50% ���� 0.50��û�еĻ�����0
   CC_SYNTHESIZE(float,_skill1_buff_atkSpeed,skill1_buff_atkSpeed);//Buff�Ĺ����ٶ�������û�еĻ�����0 ��������110%����1.10
   CC_SYNTHESIZE(int,_skill1_buff_atk,skill1_buff_atk);//buff�Ĺ��ܹ�������û�еĻ���0
   CC_SYNTHESIZE(int,_skill1_buff_target,skill1_buff_target);//buff����Ŀ�����ͣ�0��ʾ�Լ���1��ʾ���ˣ�2��ʾ�Ѿ���3��ʾ��������4ʿ��
    CC_SYNTHESIZE(int,_skill1_buff_isGround,skill1_buff_isGround);//�Ƿ���Ⱥ�����ǵ��壬 ������1 ,Ⱥ����2
   CC_SYNTHESIZE(int,_skill1_buff_targetNum,skill1_buff_targeNum);//Buff ���õ�����
   CC_SYNTHESIZE(int,_skill1_buff_addHp,skill1_buff_addHp);//buff Ч���� �����ӵ�Ѫ��,
   CC_SYNTHESIZE(int,_skill1_upgradeMoney,skill1_upgradeMoney);//����1�������ѽ��

   //Ӣ�۵ı�������1  
   CC_SYNTHESIZE(float,_skill2_buffValue,skill2_buffValue);//buffValue ��Ӧ���ǣ� ���������ӣ�����ֵ���ӣ���Χʿ���Ĺ��������ӣ��������ӣ��������Ļ�������  �������ڰٷֱ� 0.2
   CC_SYNTHESIZE(int,_skill2_extraMoney,skill2_extraMoney);//��������Ƕ��⽱���Ľ�Ǯ�������Ӣ�۵Ĳ���
   CC_SYNTHESIZE(int,_skill2_target,skill2_target);//Ŀ�����ͣ�0��ʾ�Լ���1��ʾ���ˣ�2��ʾ�Ѿ���3��ʾ��������4ʿ��
   CC_SYNTHESIZE(int,_skill2_isGround,skill2_isGround);//�Ƿ���Ⱥ�����ǵ��壬 ������1 ,Ⱥ����2
   CC_SYNTHESIZE(int,_skill2_targetNum,skill2_targetNum);//����Ŀ����Ŀ
   CC_SYNTHESIZE(int,_skill2_upgradeMoney,skill2_upgradeMoney);//����2�������ѽ��
    
   //Ӣ�۵ı�������2 
   CC_SYNTHESIZE(float,_skill3_buffValue,skill3_buffValue);// buffValue ��Ӧ���ǣ� �����ʣ�����������������Χʿ�����������ӣ��˺����䣬ʿ���ı���������,�������ڰٷֱ� 0.2
   CC_SYNTHESIZE(int,_skill3_target,skill3_target);//Ŀ�����ͣ�0��ʾ�Լ���1��ʾ���ˣ�2��ʾ�Ѿ���3.��ʾ��������4ʿ��
   CC_SYNTHESIZE(int,_skill3_isGround,skill3_isGround);//�Ƿ���Ⱥ�����ǵ��壬 ������1 ,Ⱥ����2
   CC_SYNTHESIZE(int,_skill3_targetNum,skill3_targetNum);//����Ŀ����Ŀ
   CC_SYNTHESIZE(int,_skill3_upgradeMoney,skill3_upgradeMoney);//����1�������ѽ��


   //�����Ӣ���ٻ����
   CC_SYNTHESIZE(int,_skill1_callMonster_Hp,skill1_callMonster_Hp);//�ٻ������Ѫ��
   CC_SYNTHESIZE(int,_skill1_callMonster_atkOption,skill1_callMonster_atkOption);//�ٻ�����Ĺ�������  �������� ��ΪA1��A2��A3  1��ʾA1��2��ʾA2��3��ʾA3 
   CC_SYNTHESIZE(int,_skill1_callMonster_atk,skill1_callMonster_atk);//�ٻ���Ĺ����� 
   CC_SYNTHESIZE(int,_skill1_callMonster_dfeOption,skill1_callMonster_dfeOption);//�ٻ���ķ�������   ��ΪD1��D2 ,1��ʾD1��2��ʾD2
   CC_SYNTHESIZE(float,_skill1_callMonster_dfe,skill1_callMonster_dfe);//�ٻ���ķ�����������ǰٷֱȣ�����0.25�൱��25%
   CC_SYNTHESIZE(int,_skill1_callMonster_atkLenOption,skill1_callMonster_atkLenOption);//�ٻ���Ĺ������� �������ͷ�Χ 1��ʾ���� 2��ʾ�г� 3��ʾԶ��  ATKLEN_REMOTE  3 Զ���� ATKLEN_MID 2 �г̾���   ATKLEN_NEAR 1  ������
   CC_SYNTHESIZE(int,_skill1_callMonster_atkSpeed,skill1_callMonster_atkSpeed);//�ٻ���Ĺ����ٶ� �����ٶ� ��Ϊ������ 1��ʾ�죬2��ʾ�� 3��ʾ��
   CC_SYNTHESIZE(int,_skill1_callMonster_moveSpeedScale,skill1_callMonster_moveSpeedScale);// ��ȡlist��ʱ���Ƿ�Ϊ1��ʾ�죬2��ʾ�� 3��ʾ��
 
   CC_SYNTHESIZE(bool,_isRun,isRun);//�Ƿ������ܲ�
     int targetAddr;//��������ĵ�ֵַ
	 int gettargetAddr();
	 void settargetAddr(int t){ targetAddr=t;};

	 int fly_target_addr;
	 int getTargetsAddrs(){return fly_target_addr;};
	 void setTargetsAddrs(int addrs){fly_target_addr = addrs;};
	 bool _canAtkFlyObj;

	 cocostudio::Armature *_armature;
	 Armature* getArmature();//��ȡ ��������
	 cocos2d::Layer *_layer;//�����õ�
	 int _MisPixel;//��λ����
	  

public:
	//Ч����Ѫ��Ч��
	virtual void reduceHpEffect(int t);
	void labelForCallback(Ref* pSender);
	void setReduceHp(int m_atkOption,int m_atk,float cri,float criDamge);
   

	//�Ƿ�ѡ�еĲ���
	 bool _isSelect;
	 void setisSelect(bool f);
	 bool getisSelect();

	  int TimeCount;
	  void changeColorWithAtk();//�ܵ�����ʱ����ɫ�仯
	  void changeBackColor(Ref *f);
	 
	

	 int isStart;
	 void setisStart(int b);
	 int getisStart();
	 void setUpdateForisStart();
	 

	  Animate* CreateAnimation(const char*FrameName,int max);//FrameName �����ͼƬ
	  void CreateAnimationForDeathObject(Point pos);
	  void DeathObjectCallBack(Ref *psender, bool _isOutDeath = false);		// �����ص���������_isOutDeath �Ƿ����������

	  void BackHp();//3.0���ص� updateBackHp()�������ǲ����Ե��õ�
	  void updateBackHp(Ref*f);//��Ѫ�ġ�������ÿ��ص�һ��isBackHp()
	  void isBackHp(float dt);//�ж��Ƿ����Ѫ��������
	
	  bool updateLevel(int _level);

	  //...........��ȡ��Ϣ....... ���ڲ�����ĵ�ͳ��ȡ�������
	  void showLevel();
	  void hideLevel();
	  SceneData *_scenedata;
	  StoreyData *_storeydata;
	  Widget *_swg;

	  CC_SYNTHESIZE(int,_targetSelect,targetSelect);//��Ҫ�����жϷ��б���½�ص��˵�

	  CC_SYNTHESIZE(int,_tower3_selectNum,tower3_selectNum);//���ڷ�����3���������˵ļ�����
	  CC_SYNTHESIZE(int,_tower3_selectNumMax,tower3_selectNumMax);//���ڷ�����3���������˵������Ŀ
	  CC_SYNTHESIZE(int,_isAtkTower3Flag,isAtkTower3Flag);//�ж��Ƿ��ڴ�������3��
	 /************************************************************************/
	 /* 
	 ���� ��ʾ������Χ�ĵ�Ƭ ���أ���ʾ
	 */
	 /************************************************************************/
	  Sprite *l_atklenObject1,*l_atklenObject2;
	  void l_createCAtkLenObject();//����������Χ�ĵ�Ƭ
	  void l_atklenObjectHide();//���ع�����Χ�ĵ�Ƭ
	  void l_atklenObjectShow();//��ʾ������Χ�ĵ�Ƭ
	  void l_atklenUpdateSize();//��Ҫ���ڵ����ʱ�򻹿����ƶ��Ķ�������,������ʾ��Χ
	  //������Χ����ʾ


	  void myPauseAllAction(Ref *obj);
	  void myResumeAllAction(Ref *obj);
	  CC_SYNTHESIZE(int,_direct,direct);//���򡣡�����

	  void pauseMyActionsAndSchedulers();
	  void resumeMyActionsAndSchedulers();

	  void MakeEnemyNoAttkMe();//�õ��˲��ڹ��� ��
	  void MakeWarToLoad();//��սʿ��ͣ��

	  CC_SYNTHESIZE(int,_isFrozen,isFrozen);//�Ƿ񱻱���ס��
	  CC_SYNTHESIZE(int,_isSpeedDown,isSpeedDown);//�Ƿ񱻼���

	  //����������������������������������������������������������
	  float findBossSkillBuffValue(int bossSelfType,int indexSkill);//Ѱ��Ӣ�ۼ��ܵ�buffֵ

	  void skilladdHpEffect(int t,int skillLev);//����Ӣ�۵ļ�Ѫ������Ч������
	  void skilladdHpCallFunc(Ref *f);

	  //������Ч�ġ���������
	  void criEffect();
	   Armature *criarm;
	  void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); 
	 // void criEffectCallback(Ref *f);
 };
#endif