#pragma once
#include "cocos2d.h"
#include "GameObjects.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "Window.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "effectsObject.h"
USING_NS_CC;
class BulletObjects:public GameObjects
{
public:
	CREATE_FUNC(BulletObjects);
	bool init();
	void update(float dt);
	void setAtk(int atk);
	int _atk;
	int _floorNum;
	void setFloorNum(int floornum);
	CC_SYNTHESIZE(int,_tagAddr,tagAddr);
	void MoveFinished(Node* sender);
	void run(Point pos,float timer);
	

	void createWithSpriteFrameName(const string& frameName);
};
