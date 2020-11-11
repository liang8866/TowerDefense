#ifndef _LOADING_SENECE_H_ 
#define _LOADING_SENECE_H_  
#include "cocos2d.h"
#include "GameConfig.h"
#include "Connect.h"
USING_NS_CC;
class LoadingScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();
	 LoadingScene(void);
	~ LoadingScene(void);
	virtual void onEnter() override;
	virtual void onExit() override;
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	// implement the "static create()" method manually
	CREATE_FUNC(LoadingScene);
  void loadingCallBack(cocos2d::CCObject *obj);//回调函数，每加载一张纹理就调用一次  
  void loadingCallback2(float dt);
  void loadingCallback3(float dt);
  void loadScene(int SceneTag);
  void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
  void parseData();
  void showError();
  void requestLogin();
  void requestGameStar();
  void backToStarScene();
  void showTips();

  void EnterFreshMan();
private:
	cocos2d::CCLabelTTF *m_pLabelLoading;//文字标签，用于显示“Loading”字样  
    cocos2d::CCLabelTTF *m_pLabelPercent;//文字标签，用于显示百分比  
	int currentNum;//当前加载到第几张  
    int totalNum;//加载总数  
	cocostudio::Armature* _loading;
	cocostudio::Armature* _hero;
	int _sceneTag;
};

#endif  