#ifndef _ANDROID_STARTUP_H_ 
#define _ANDROID_STARTUP_H_   
#include "cocos2d.h"
#include "startScene.h"

class AndroidStartup : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	AndroidStartup(void);
	~AndroidStartup(void);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	// implement the "static create()" method manually
	CREATE_FUNC(AndroidStartup);
	
	
private:
	cocos2d::Size visibleSize;
	cocos2d::Point origin;
	cocos2d::Size winSize;

	
};

#endif  