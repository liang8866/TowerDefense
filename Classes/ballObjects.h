#ifndef __BALL_OBJECTS_H__  
#define __BALL_OBJECTS_H__

#include "cocos2d.h"
#include "GameConfig.h"
class StoreyData;
class SceneData;

USING_NS_CC;
class ballObjects: public cocos2d::Sprite
{

public:
	ballObjects(void);
	~ballObjects(void);
	static ballObjects* create(int tagAddr,Point tagPos,int Type,int floorNum,int derect);
	virtual bool ObjectsInit(int tagAddr,Point tagPos,int Type,int floorNum,int derect); //自定义的初始化

	void myupdate(float dt);
	Animate* CreateAnimation(const char*FrameName,int max);

	bool FindGameObject();
	void ExplosionEffect();//爆炸效果
	void myPauseAllAction(Ref *obj);
	void myResumeAllAction(Ref *obj);

	void bombEffect();
	void bombEffectCallback(Object *p);

public:

	CC_SYNTHESIZE(int,b_targetAddr,b_targetAddr);//获取的地址、
	CC_SYNTHESIZE(int,b_type,b_type);//你要创建的子弹类型和子弹爆炸
	Point b_targetPos;
	Point b_shooterPos;
	void setB_shooterPos(Point pos){ b_shooterPos=pos;};
	

	CC_SYNTHESIZE(int,floorNum,floorNum);
	 StoreyData *_storeydata;

	 float moveLineX;
	 int b_direct;
	//这里要写扣血的多少
	 CC_SYNTHESIZE(int,b_atkOption,b_atkOption);//攻击类型 分为A1，A2，A3  1表示A1，2表示A2，3表示A3
	 CC_SYNTHESIZE(int,b_atk,b_atk);//攻击力 比如：300
	 CC_SYNTHESIZE(float,b_cri,b_cri);//暴击率
	 CC_SYNTHESIZE(float,b_cri_damage,b_cri_damage);//暴击伤害值

};
#endif