#ifndef __GAME_SCENE_H__  
#define __GAME_SCENE_H__  


#include "cocos2d.h"
#include "GameLayer.h"
 USING_NS_CC;

//#include "HudLayer.h"
class GameScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
	GameScene(void);
   ~GameScene(void);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init(int sectionNum,int sceneNum);  
    // a selector callback
   
    
    // implement the "static create()" method manually
    //CREATE_FUNC(GameScene);

	/************************************************************************/
	/* @param sectionNum 章节数
	/* @param sceneNum 关卡数
	/************************************************************************/
	static GameScene* create(int sectionNum,int sceneNum)
	{
		GameScene *pRet = new GameScene(); 
		if (pRet && pRet->init(sectionNum,sceneNum)) 
		{ 
			pRet->autorelease();
			return pRet;
		} else 
		{ 
			delete pRet; 
			pRet = NULL; 
			return NULL; 
		} 
	};

	CC_SYNTHESIZE(GameLayer*, _gameLayer, GameLayer);
    CC_SYNTHESIZE(HudLayer*, _hudLayer, HudLayer);
	
	int _sectionNum;
	int _sceneNum;
	
	void setRoundNum(int currRound , int totalRound);

	void addFreshman(float dt);

	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
private:
    Size visibleSize;
    Point origin;
	Size winSize;
	void logicUpdate(float dt);
	int _timeCount;
	int _disTime;
};

#endif  