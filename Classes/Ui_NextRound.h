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


	float GetlessX();//�����ǰ½���ϻ��е��˵Ļ��������һ�����Ļ��û�ߵĻ�����ȡ��

	void freshmanEnemyComeIn(Ref *obj);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
private:

	Widget*          roundWidget;
	Button*          roundBtn;
	ProgressTimer*      progress;

	int              clickedCnt;   //�������
	int				_timeCount;
	int				_disTime;
	bool			_timeFlag;

};



#endif // !__UI__NEXTROUND_H__
