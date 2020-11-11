#include "Ui_ProgressBar.h"
USING_NS_CC;

Ui_ProgressBar::Ui_ProgressBar(void)
{
}
Ui_ProgressBar::~Ui_ProgressBar(void)
{
}
Ui_ProgressBar* Ui_ProgressBar::createUI()
{
	Ui_ProgressBar* newWidget = new Ui_ProgressBar();
	if (newWidget && newWidget->init())
	{
		newWidget->initUI();
		newWidget->autorelease();
		return newWidget;
	}
	CC_SAFE_DELETE(newWidget);
	return nullptr;
}
bool Ui_ProgressBar::initUI()
{
	if (!Widget::init()){
		return false;
	}
	setProgressValue(0);
	_mHeadstoneObject = nullptr;

	mMainWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("ProgressBar/ProgressBar.ExportJson"));
	this->addChild(mMainWidget, 1);

	mProgressBar	= static_cast<LoadingBar*>(Helper::seekWidgetByName(mMainWidget, "ProgressBar"));
	mPanelBack		= static_cast<ImageView*>(Helper::seekWidgetByName(mMainWidget, "ProgressBar_bk"));
	mTextValue		= static_cast<TextBMFont*>(Helper::seekWidgetByName(mMainWidget, "BitmapLabel_Value"));
	mTextValue->setVisible(false);
	this->schedule(schedule_selector(Ui_ProgressBar::updatePercent),(float)1.0/60);
	return true;
}
// 设置进度
void Ui_ProgressBar::setProgressBarPercent(int _percent)
{
	setProgressValue(_percent);
	mProgressBar->setPercent(_percent);	
}
// 取进度
int Ui_ProgressBar::getProgressBarPercent()
{
	setProgressValue(mProgressBar->getPercent());
	return getProgressValue();
}
// 开始复活动画
void Ui_ProgressBar::beginReviveProgressToPercent(headstoneObjects* _obj, int _beginPercent)
{
	_mHeadstoneObject = _obj;
	if(!_mHeadstoneObject)
		return;
	// 先设置到当前位置
	setProgressBarPercent(_beginPercent);

	setEndTime(15);

	// 计算剩下的时间
	float _baseStep = _EndTime / 100;
	int _lastParent = 100 - _beginPercent;
	float _lastTime = _baseStep * _lastParent;
	// 从当前位置到结束
	auto _action = Sequence::create(
		LoadingTo::create(_lastTime, 100),
		CallFuncN::create(CC_CALLBACK_1(Ui_ProgressBar::beginProgressToPercentCallBack, this)),
		NULL);
	mProgressBar->runAction(_action);
}
// 倒计时结束回调
void Ui_ProgressBar::beginProgressToPercentCallBack(Ref* sender)
{
	if(!_mHeadstoneObject)
		return;
	_mHeadstoneObject->reviveHeroByCostGold();	// 复活英雄
}

// 设置所有墓碑(进度条)暂停。非暂停
void Ui_ProgressBar::isPauseAction(bool _isBool)
{
	if(!mProgressBar)
		return;
	if (_isBool)
	{
		mProgressBar->pause();
		mProgressBar->pauseSchedulerAndActions();
	}else
	{
		mProgressBar->resume();
		mProgressBar->resumeSchedulerAndActions();
	}
}

// 更新
void Ui_ProgressBar::updatePercent(float _time)
{
	mTextValue->setText(StringUtils::format("%d%%", getProgressBarPercent()));
}