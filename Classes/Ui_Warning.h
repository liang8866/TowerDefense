#ifndef __UI__WARNING_H__
#define __UI__WARNING_H__
#include "GameConfig.h"

class Ui_Warning : public Widget
{
public:
	Ui_Warning(void);
	~Ui_Warning(void);

	//自定义的初始化
	static Ui_Warning* createUI();
	virtual bool initUI();
public:
	void beginProgress(float _time);
	void endProgress();

	void setPause(bool _isBool);

	void progressFinishCallBack(Ref* _sender);
private:
	Widget*			mWidget;
	ImageView*		mImageBack;
	ImageView*		mImageIcon;
	LoadingBar*		mLoadingBar;
	ProgressTimer*	mProgress;
};



#endif // !__UI__NEXTROUND_H__
