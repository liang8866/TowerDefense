#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "Window.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
USING_NS_CC;
class effectsObject : public Sprite
{
public:
	void Death(int t);
	void TowerDeath();
	void Bullet();
	void DeathObjectCallBack(Ref *psender);
	bool init();
	void enemyItem17AttackEffects();
	void enemyItem20AttackEffects();
	CREATE_FUNC(effectsObject);

	void towerItem1AttackEffects(float dt);

	 Animate* CreateAnimation(const char*FrameName,int max);//参数 FrameName 和最大图片
	void bottleExplod();//瓶子的爆炸效果
	void bottleMoveExplod(Point mbPos);//移动和爆炸
	void moveCallback(Ref *psender);
	
	Sprite *bomb;

	void tower1_paodan_bomb();

	void tower4_wind_effect(int level);

	void tower5_jiguang_effect();

	void tower5_shexian_effect(float dis);

	void tower5_atk_obj_effect();

	void War04_bomb();
	void Trap03_bomb();
	Armature *Arm;
   void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调

    void trap02_ring_effect();
	void trap02_born_effect();


   //变成羊的烟雾
   void sheepEffect();
 
   void magic3Effect();

   void noCDEffects();

   void addSpeedEffects();



   void Boss4ChangeEffect();//变成胖子的烟雾

   //商店购买成功特效
   void shopBuyGoodsSuccessEffect();
   
};



