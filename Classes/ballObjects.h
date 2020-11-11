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
	virtual bool ObjectsInit(int tagAddr,Point tagPos,int Type,int floorNum,int derect); //�Զ���ĳ�ʼ��

	void myupdate(float dt);
	Animate* CreateAnimation(const char*FrameName,int max);

	bool FindGameObject();
	void ExplosionEffect();//��ըЧ��
	void myPauseAllAction(Ref *obj);
	void myResumeAllAction(Ref *obj);

	void bombEffect();
	void bombEffectCallback(Object *p);

public:

	CC_SYNTHESIZE(int,b_targetAddr,b_targetAddr);//��ȡ�ĵ�ַ��
	CC_SYNTHESIZE(int,b_type,b_type);//��Ҫ�������ӵ����ͺ��ӵ���ը
	Point b_targetPos;
	Point b_shooterPos;
	void setB_shooterPos(Point pos){ b_shooterPos=pos;};
	

	CC_SYNTHESIZE(int,floorNum,floorNum);
	 StoreyData *_storeydata;

	 float moveLineX;
	 int b_direct;
	//����Ҫд��Ѫ�Ķ���
	 CC_SYNTHESIZE(int,b_atkOption,b_atkOption);//�������� ��ΪA1��A2��A3  1��ʾA1��2��ʾA2��3��ʾA3
	 CC_SYNTHESIZE(int,b_atk,b_atk);//������ ���磺300
	 CC_SYNTHESIZE(float,b_cri,b_cri);//������
	 CC_SYNTHESIZE(float,b_cri_damage,b_cri_damage);//�����˺�ֵ

};
#endif