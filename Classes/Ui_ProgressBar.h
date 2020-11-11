#ifndef __UI_PROGRESSBAR_H__
#define __UI_PROGRESSBAR_H__

#include "GameConfig.h"
#include "headstoneObjects.h"
USING_NS_CC;

class Ui_ProgressBar:public Widget
{
public:
	Ui_ProgressBar(void);
   ~Ui_ProgressBar(void);

   static Ui_ProgressBar* createUI();
   virtual bool initUI(); //�Զ���ĳ�ʼ��
public:
	CC_SYNTHESIZE(int,		_ProgressValue, ProgressValue);	// ��ǰ����ֵ
	CC_SYNTHESIZE(float,	_EndTime, EndTime);				// ����ʱ��, Ĭ��30��

public:
	void setProgressBarPercent(int _percent);				// ���ý���
	int getProgressBarPercent();

	// ����
	headstoneObjects* _mHeadstoneObject;
	void beginReviveProgressToPercent(headstoneObjects* _obj, int _beginPercent);	// ��ʼ����
	void beginProgressToPercentCallBack(Ref* sender);		// ��������
	void isPauseAction(bool _isBool);
	void updatePercent(float _time);
public:
	Widget*			mMainWidget;

	LoadingBar*		mProgressBar;
	ImageView*		mPanelBack;
	TextBMFont*		mTextValue;

	
};
#endif