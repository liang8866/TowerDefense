#include "Ui_Warning.h"

Ui_Warning::Ui_Warning()
{
}
Ui_Warning::~Ui_Warning()
{
}


Ui_Warning* Ui_Warning::createUI()
{
	Ui_Warning* newWidget = new Ui_Warning();
	if (newWidget && newWidget->init())
	{
		newWidget->initUI();
		newWidget->autorelease();
		return newWidget;
	}
	CC_SAFE_DELETE(newWidget);
	return nullptr;
}
bool Ui_Warning::initUI()
{
	if (!Widget::init())
		return false;
	mWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_Warning/Ui_Warning.ExportJson"));
	mImageBack = static_cast<ImageView*>(Helper::seekWidgetByName(mWidget,"Image_Back"));
	mImageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(mWidget,"Image_Icon"));
	mLoadingBar = static_cast<LoadingBar*>(Helper::seekWidgetByName(mWidget,"LoadingBar_Title"));
	mLoadingBar->setVisible(false);
	Sprite* _sp = Sprite::createWithSpriteFrameName("image/UI_zjm_027.png");
	mProgress = ProgressTimer::create(_sp);
	mWidget->addChild(mProgress, 3);
	mProgress->setPosition(mLoadingBar->getPosition());
	this->addChild(mWidget);

	// 感叹号来回缩放
	float _scale1 = 1.2f;
	float _scale2 = 1.00f;
	auto _act1 = ScaleTo::create(0.5f, _scale1, _scale1);
	auto _act2 = ScaleTo::create(0.5f, _scale2, _scale2);
	auto action = Sequence::create(_act1, _act2, NULL);
	RepeatForever *repeat = RepeatForever::create(action); 
	mImageIcon->runAction(repeat);
	return true;
}

void Ui_Warning::beginProgress(float _time)
{
	this->setVisible(true);
	mProgress->setVisible(true);
	mProgress->setPercentage(0);

	auto _act1 = ProgressFromTo::create(_time, 0, 100);
	auto _actions = Sequence::create(
			_act1,	
			CallFuncN::create(CC_CALLBACK_1(Ui_Warning::progressFinishCallBack, this)),
			NULL);
	mProgress->runAction(_actions);
}
void Ui_Warning::progressFinishCallBack(Ref* _sender)
{
	endProgress();
}
void Ui_Warning::endProgress()
{
	this->removeFromParentAndCleanup(true);
}
void Ui_Warning::setPause(bool _isBool)
{
	if(!mProgress)
		return;
	if (_isBool)
	{
		mProgress->pause();
		mProgress->pauseSchedulerAndActions();
	}else
	{
		mProgress->resume();
		mProgress->resumeSchedulerAndActions();
	}
}
