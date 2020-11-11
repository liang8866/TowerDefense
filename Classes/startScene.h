#ifndef _START_SENECE_H_ 
#define _START_SENECE_H_  
#include "cocos2d.h"
#include "network/HttpClient.h"
#include "Ui_Login.h"

class startScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	startScene(void);
	~startScene(void);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	// implement the "static create()" method manually
	CREATE_FUNC(startScene);

	//暂时的调换场景
// 	void creatMenu();
// 	// a selector callback
// 	void menuCloseCallback(Ref* pSender);
	void myupdate(float dt);   //定时器
	void onEnter();
	void onExit();
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) override;
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调
	void guangCallback(Ref *r);
	
	void reConnect();
	Ui_Login* _widget;
private:
	cocos2d::Size visibleSize;
	cocos2d::Point origin;
	cocos2d::Size winSize;

	Sprite *startguang1,*startguang2;
	Armature* _boxArmature;
	
};

#endif  