#ifndef _PROGRESS_SENECE_H_ 
#define _PROGRESS_SENECE_H_  
#include "levelScene.h"
#include "cocos2d.h"


USING_NS_CC;
class progressScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	progressScene(void);
	~progressScene(void);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// a selector callback

	// implement the "static create()" method manually
	CREATE_FUNC(progressScene);

	//暂时的调换场景
	void creatMenu();
	// a selector callback
	void menuCloseCallback(Ref* pSender);
private:
	Size visibleSize;
	Point origin;
	Size winSize;

public:
		

};

#endif  