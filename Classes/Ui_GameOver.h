#ifndef __UI_GAMEOVER_H__
#define __UI_GAMEOVER_H__
#include "GameConfig.h"
#include "network/HttpClient.h"
#include "audioPlayer.h"
using namespace std;

class GameReward
{
public:
	int id;
	int count;
};

class Ui_GameOver : public Widget
{
public:
	Ui_GameOver();
	~Ui_GameOver();

	virtual bool init();
	CREATE_FUNC(Ui_GameOver);
	virtual void onEnter() override;
	virtual void onExit() override;

	void winTheGame(float times=2.0f);
	void loseTheGame();
	void boxReward();

	void newChallengesBtnCallBack(Ref* sender, TouchEventType type);   //�ٴ���ս��ť�ص�
	void exitBtnCallBack(Ref* sender, TouchEventType type);            //�˳���ť�ص�
	void nextPostBtnCallBack(Ref* sender, TouchEventType type);        //��һ�ذ�ť�ص�
	void FadeOver();
	void onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex); //֡�¼��ص�

	void createLayer();
	void fitScreen();

	void layerCallback();

	void starAnimation();
	void starAnimationCreate(Widget *widget , int i);

	void showWinButtons();
	//����ص�
	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
	void showError();
	void request();
	void cancelRequest();

	int randomBoxEnemyReward();
private:
	bool _isHttpFinish;//�����Ƿ񷵻�
	bool _isHttpSuccess;//�Ƿ�ɹ�
	bool _isWin;//�Ƿ�ʤ��
	Widget*   m_widget; 
	LayerColor* _warp;
	Armature* _armature;
	Armature* _boxAr;

	bool DelayTimeFlag;
	int star_cnt;
	int starAnimationCount;
	float _scale;

	bool hasBoxWard;

	int randNum; //��������Ľ���
};


#endif