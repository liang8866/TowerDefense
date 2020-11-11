#ifndef __UI_SCENE_SELECT_H__
#define __UI_SCENE_SELECT_H__

#include "cocos2d.h"
#include "GameConfig.h"
#include "UserRecord.h"
#include "ReadyScene.h"
#include "platform/CCFileUtils.h"

class Ui_SceneSelect : public Widget
{
public:
	~Ui_SceneSelect();
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	void eventTouchButton(Ref* sender, TouchEventType type);
	void buttonTouched(Ref* sender, TouchEventType type);
	CREATE_FUNC(Ui_SceneSelect);
	void fitScreen();
	void myEnabledTouch(Ref *obj);

	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	void onTouchesCancelled(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

	EventListenerTouchAllAtOnce* _touchListener;
	virtual void setTouchEnabled(bool enabled);
	cocos2d::Point autoAdjustPosition(cocos2d::Point target);
	cocos2d::Point getPointCenter(Point p1,Point p2);

	
	float cdTime; //拖动时间
	std::map<int,cocos2d::Point> points;
	std::vector<Point> touchPoints;
	bool _isRuning;
	bool _autoScroll;
	bool _autoScale;
	float _friction;
	float _gravity;
	float _distance;
	
	cocos2d::Point deltaPoint;
	bool moveEnabled;
	float _minScale;
	float _maxScale;


	
	void Zoom();
	void Move();
	void autoScroll();
	void stopAutoScale();
	void stopAutoScroll();
	void autoScale(cocos2d::Point p1,cocos2d::Point p2);
	

	void update(float dt);
	void changeSence(const int chapter_id);
	void loadMap(const int chapter_id);
	
	void initMapData(const int chapter_id);
	void initMapAnimation(const int chapter_id);
	
	void afterChangeMap(); //切换地图之后的一些操作


	void loopUpDownAnimation(int type ,Node* obj);
	void loopWindmillAnimation(Widget* windmill); //地图一风车动画
	
	void initAnimation();//Ui动画

	void marqueeCloud(ImageView* cloud); //云动画
	void marqueeSand(Widget* sand);//风沙动画
	bool needShowHeroTips();

	void addBoxEnemyPromptInfo(Button *btn);
	void hideBoxEnemyPromptInfo();
public:
	Widget* _widget; //关卡选择UI
	Widget* _mWidget; //地图warp
	Widget* _cWidget; //当前地图;
	std::map<int,Widget*> maps;
	void initMap();
	Button* _freshmanBtn;
	
private:
	int _currentSence;
	int _senceCount;
	int _curGold;
	int _curSoul;
	bool _firstLoad;
	Size _VisibleSize;
	Point _VisibleOrigin;
	float delayTime;
};
#endif