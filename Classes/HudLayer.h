
#ifndef __HUD_LAYER_H__  
#define __HUD_LAYER_H__  

#include "cocos2d.h"
#include "GameConfig.h"
 USING_NS_CC;

class HudLayer : public cocos2d::Layer
{

	MenuItemImage *var_pauseItem;//��ͣ

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
    // a selector callback
    // implement the "static create()" method manually
	CREATE_FUNC(HudLayer);

	void update(float dt);   //��ʱ��

	void fun_createPauseMenu();
		 
	void fun_pauseMenuCallback();//��ͣ�ص�����
	void labelForCallback(Ref* pSender);
private:
    Size visibleSize;
    Point origin;
	Size winSize;


};
#endif  