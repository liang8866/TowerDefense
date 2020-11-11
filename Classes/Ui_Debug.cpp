#include "Ui_Debug.h"

static Ui_Debug* mDebug = nullptr;

Ui_Debug::Ui_Debug()
{

}

Ui_Debug::~Ui_Debug()
{

}

Ui_Debug* Ui_Debug::getInstance()
{
	if (!mDebug)
	{
		mDebug = new Ui_Debug();
		mDebug->init();
		mDebug->autorelease();
		Director::getInstance()->getRunningScene()->addChild(mDebug, 999);
		mDebug->setPosition(Point(-1000, -1000));
		mDebug->setVisible(false);
	}
	return mDebug;
}
void Ui_Debug::destroyInstance()
{
	mDebug->removeFromParentAndCleanup(true);
	CC_SAFE_DELETE(mDebug);
}

bool Ui_Debug::init()
{
	if (!Widget::init())
		return false;
	mWidget = nullptr;
	mButtonClose = nullptr;
	mButtonClear = nullptr;
	mTextInfo = nullptr;
	mScrollView = nullptr;
	mIsRun = false;

	mWidget = GUIReader::getInstance()->widgetFromJsonFile("Ui_DebugLog/Ui_DebugLog.ExportJson");
	this->addChild(mWidget);
	mWidget->setScaleX(Director::getInstance()->getVisibleSize().width/960);
	mWidget->setScaleY(Director::getInstance()->getVisibleSize().height/640);

	mButtonClose = static_cast<Button*>(Helper::seekWidgetByName(mWidget, "Button_Close"));
	mButtonClear = static_cast<Button*>(Helper::seekWidgetByName(mWidget, "Button_Clear"));
	mButtonClose->addTouchEventListener(this, toucheventselector(Ui_Debug::onButtonEvent));
	mButtonClear->addTouchEventListener(this, toucheventselector(Ui_Debug::onButtonEvent));

	mScrollView = static_cast<ui::ScrollView*>(Helper::seekWidgetByName(mWidget, "ScrollView_1"));
	mTextInfo = static_cast<Text*>(Helper::seekWidgetByName(mWidget, "Text_Info"));
	mTextInfo->setText("");
	return true;
}

void Ui_Debug::onEnter()
{
	Widget::onEnter();
}
void Ui_Debug::onExit()
{
	Widget::onExit();
}

void Ui_Debug::setRun(bool _isRun)
{
	mIsRun = _isRun;
	this->setVisible(mIsRun);
	if (mIsRun)
	{
		Point originPt   = Director::getInstance()->getVisibleOrigin();
		Size visibleSize = Director::getInstance()->getVisibleSize();
		this->setPosition(Point(originPt.x, visibleSize.height - 200));
	}else
	{
		this->setPosition(Point(-1000, -1000));
	}
}

void Ui_Debug::onButtonEvent(Ref *pSender , TouchEventType type) 
{
	if (type == TOUCH_EVENT_ENDED)
	{
		Button* _curButton = static_cast<Button*>(pSender);
		if (_curButton)
		{
			if (_curButton->getName() == mButtonClose->getName())
			{
				this->setRun(false);
				this->removeFromParentAndCleanup(true);
				mDebug = nullptr;
			}else if (_curButton->getName() == mButtonClear->getName())
			{
				mTextInfo->setText("");
			}
		}
	}
}
void Ui_Debug::addText(string _text)
{
	if (!mIsRun)
	{
		return;
	}
	string _nowText = mTextInfo->getStringValue();
	if (_nowText.empty())
	{
		mTextInfo->setText(_text);
	}else
	{
		string _newText = StringUtils::format("%s\n%s", _nowText.c_str(), _text.c_str());
		mTextInfo->setText(_newText);
	}
}



