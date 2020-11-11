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
  void loadingCallBack(cocos2d::CCObject *obj);//�ص�������ÿ����һ������͵���һ��  
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
	cocos2d::CCLabelTTF *m_pLabelLoading;//���ֱ�ǩ��������ʾ��Loading������  
    cocos2d::CCLabelTTF *m_pLabelPercent;//���ֱ�ǩ��������ʾ�ٷֱ�  
	int currentNum;//��ǰ���ص��ڼ���  
    int totalNum;//��������  
	cocostudio::Armature* _loading;
	cocostudio::Armature* _hero;
	int _sceneTag;
};

#endif  