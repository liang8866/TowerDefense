
#ifndef __GAME_LAYER_H__  
#define __GAME_LAYER_H__  

#include "cocos2d.h"
#include "HudLayer.h"
#include "GameConfig.h"

#include "PlistManager.h"
#include "bossObjects.h"
#include "enemyObjects.h"
#include "npcObjects.h"
#include "towerObjects.h"
#include "trapObjects.h"
#include "warriorObjects.h"
#include "WindowManager.h"
#include "SceneData.h"


 USING_NS_CC;
class GameLayer: public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
 GameLayer();
 ~GameLayer();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
    void Gameupdate(float dt);   //¶¨Ê±Æ÷
  
    // implement the "static create()" method manually
	CREATE_FUNC(GameLayer);
	CC_SYNTHESIZE(HudLayer*, _hud, Hud);
	

	

private:
    Size	visibleSize;
    Point	origin;
	Size	winSize;
};
#endif   