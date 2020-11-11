#ifndef _READY_SENECE_H_ 
#define _READY_SENECE_H_  
#include "levelScene.h"
#include "cocos2d.h"

USING_NS_CC;
class ReadyScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	ReadyScene(void);
	~ReadyScene(void);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// a selector callback

	// implement the "static create()" method manually
	CREATE_FUNC(ReadyScene);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;

};

#endif  

