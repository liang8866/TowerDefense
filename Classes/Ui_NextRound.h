#ifndef __UI__NEXTROUND_H__
#define __UI__NEXTROUND_H__
#define  DEFAULT_START_TIME 5
#include "GameConfig.h"
#include "Connect.h"
class Ui_NextRound : public Widget
{
public:
	Ui_NextRound();
	~Ui_NextRound();

	virtual bool init();
	CREATE_FUNC(Ui_NextRound);

	void hide();
	void show(float dt);

	void evetRoundButtonCallFunc(Ref* sender, TouchEventType type);
	void timerFinishedCallFunc();
	void enemyAttack();
	void update(float dt);

	CC_SYNTHESIZE(bool,_isFirstRound,FirstRound);

	void myPauseSchedule(Ref *obj);
	void myResumeSchedule(Ref *obj);


	float GetlessX();//如果当前陆地上还有敌人的话，。并且还在屏幕外没走的话，获取最

	void freshmanEnemyComeIn(Ref *obj);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
private:

	Widget*          roundWidget;
	Button*          roundBtn;
	ProgressTimer*      progress;

	int              clickedCnt;   //点击计数
	int				_timeCount;
	int				_disTime;
	bool			_timeFlag;

};



#endif // !__UI__NEXTROUND_H__
