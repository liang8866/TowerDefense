#ifndef _LEVEL_SENECE_H_ 
#define _LEVEL_SENECE_H_  

#include "cocos2d.h"
#include "GameConfig.h"
USING_NS_CC;


class levelScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	levelScene(void);
	~levelScene(void);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// implement the "static create()" method manually
	CREATE_FUNC(levelScene);

	//暂时的调换场景
	void creatMenu();
	// a selector callback
	void menuCloseCallback(Ref* pSender);


	void selectUiMoveOut();
	void selectUiComeIn();
	void upgradesUiMoveOut();
	void upgradesUiComeIn();

	void addFinger(Widget* wg , Rect _rect);
	void logicUpdate(float dt);

	void freshmanPrompt();
	void enterFreshMan();
	void jumpFreshMan();
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
private: 
	Size visibleSize;
	Point origin;
	Size winSize;
};

#endif  