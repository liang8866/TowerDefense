#ifndef _FRESHMAN_MANAGER_H__
#define _FRESHMAN_MANAGER_H__

#include "GameConfig.h"

class FreshManManager:public Widget
{
public:
	FreshManManager();
	~FreshManManager();

public:
	static FreshManManager* create(const char* id);
	bool initWithID(const char* id);

	void setHasFinger(bool bvar);
	void addFingerCallfunc(float dt);
	void setFingerDirection(string direct);
	void setTitlePos(int type);
	void setContent(const char* text);
	void setContentTitle(const char* title);
	void setPenetrateZone(Rect rect);
	void spTouchEventCallback(Ref *pSender, TouchEventType type);

	void fitScreen();

	void update(float dt);
	void nextFreshMan(const char* currentID);

	void initView(const char* id);

	virtual void onExit();
	virtual void onEnter();

	void setMainLayerBtnGrey();

	void setIsTouchScene(bool _isTouch);
public:
	Widget     *_widget;
	ImageView  *sp1; 
	ImageView  *sp2; 
	ImageView  *sp3; 
	ImageView  *sp4; 
	Armature   *_finger;

	bool       _touchedEnable;
	bool       _hasFinger;
	bool       _ispause;
	string     _direction;
	Rect       _rect;
	string     _content;
	string     _contentTitle;
	string     _currentID;
	bool       _pauseFreshMan;

private:
	float      _time;
};


class FirstScene : public FreshManManager
{
public:
	static FirstScene* create(const char* id);
	void initView(const char* id);
	void fitZone(const char* id);
};

class SecondScene : public FreshManManager
{
public:
	static SecondScene* create(const char* id);
	void initView(const char* id);
	void fitZone(const char* id);
};

class ThirdScene : public FreshManManager
{
public:
	static ThirdScene* create(const char* id);
	void initView(const char* id);
	void fitZone(const char* id);
};

class HeroScene : public FreshManManager
{
public:
	static HeroScene* create(const char* id);
	void initView(const char* id);
	void fitZone(const char* id);
};

class UpgradeScene : public FreshManManager
{
public:
	static UpgradeScene* create(const char* id);
	void initView(const char* id);
	void fitZone(const char* id);
};


#endif