#pragma once
#include "GameConfig.h"
#include "GameObjects.h"
#include "WindowManager.h"
#include "Ui_MessageBox.h"
#include "UserRecord.h"

typedef enum
{
	Coolie_TYPE_EMPTY,
	Coolie_TYPE_USED,
	Coolie_TYPE_DISABLE
}CoolieType;
class Coolie : public Sprite
{
public:
	 ~Coolie(void);
	bool init(CoolieType t);
	static Coolie* create(CoolieType t);
	CoolieType getType();
	void setType(CoolieType t);
	std::string getTypeImage(CoolieType t);
	void logicUpdate(float dt);
private:
	void myPauseAllAction(Ref *obj);
	void myResumeAllAction(Ref *obj);


	CoolieType _type;
	CC_SYNTHESIZE(int , _iIndex, iIndex);
};

class AltarObject : public Widget
{
public:
	~AltarObject();
	virtual bool init();
	CREATE_FUNC(AltarObject);
	void logicUpdate(float dt);
	void setProperty();
	void createTouchEvent();
	void onTouchEnded(Touch* touch,Event* event);
	void animationCall(Ref *obj);
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //帧事件回调
	virtual void onExit();
	int _interval;
	int _duration;
	int _cdTime;
    Sprite* _head;
	Sprite* _body;
	Vector<Coolie*> _coolies;
	Vector<Armature*> AnimateToDel;
	EventListenerTouchOneByOne* _listener;
	void eventTouchButton(Ref* sender, TouchEventType type);
	int money;

	Widget* _dialog;
	Widget *getDialog();//获取Dialog
	CC_SYNTHESIZE(int,_isSelect,isSelect);//选择状态

	void updateValue();		// 取升级消耗糖果


	void myPauseAllAction(Ref *obj);
	void myResumeAllAction(Ref *obj);

	void propsAddProduct(float dt);
	void resumeProduct(float dt);
	bool _isSpeed;

	Armature* _armature;
private:
	CoolieType _coolieData[8];
	int _employeeCount; //当前已使用的工位数量
	int _employeeEmpty;//当前空的工位数量
	float _curOneCostPrice; //当前购买工位的价格
	int _curCoolieCount; //当前工位数量
	int _curCoolieOutPut; //当前单个工位产出效率
	unsigned int _clickCount;

	Widget* _widget;
};

