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
   virtual bool initUI(); //自定义的初始化
public:
	CC_SYNTHESIZE(int,		_ProgressValue, ProgressValue);	// 当前进度值
	CC_SYNTHESIZE(float,	_EndTime, EndTime);				// 结束时间, 默认30秒

public:
	void setProgressBarPercent(int _percent);				// 设置进度
	int getProgressBarPercent();

	// 复活
	headstoneObjects* _mHeadstoneObject;
	void beginReviveProgressToPercent(headstoneObjects* _obj, int _beginPercent);	// 开始动画
	void beginProgressToPercentCallBack(Ref* sender);		// 结束动画
	void isPauseAction(bool _isBool);
	void updatePercent(float _time);
public:
	Widget*			mMainWidget;

	LoadingBar*		mProgressBar;
	ImageView*		mPanelBack;
	TextBMFont*		mTextValue;

	
};
#endif