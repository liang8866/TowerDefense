#include "Ui_Prompt.h"
#include "audioPlayer.h"


Ui_Prompt::Ui_Prompt():
mButtonRight(NULL),
mButtonLeft(NULL),
mTitle(NULL),
mContent(NULL),
_listener(NULL),
m_obj_r(NULL),
m_obj_l(NULL),
m_callfunc_r(NULL),
m_callfunc_l(NULL),
m_connect_r(NULL),
m_callbackTag(0)
{

}

Ui_Prompt::~Ui_Prompt()
{
	mButtonRight  = NULL;
	mButtonLeft   = NULL;
	mTitle        = NULL;
	mContent      = NULL;
	_listener     = NULL;
	m_obj_r       = NULL;
	m_obj_l       = NULL;
	m_callfunc_r  = NULL;
	m_callfunc_l  = NULL;
}


bool Ui_Prompt::init()
{
	if (!Widget::init())
		return false;

	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Prompt/GameUI0.plist","Ui_Prompt/GameUI0.png");
	m_widget     = GUIReader::getInstance()->widgetFromJsonFile("Ui_Prompt/Ui_Prompt.ExportJson");
	mButtonRight = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_right"));
	mButtonLeft  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_left"));
	mTitle       = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"prompt_titile"));
	mContent     = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"prompt_content"));

	mButtonRight->addTouchEventListener(this,toucheventselector(Ui_Prompt::eventTouchButton));
	mButtonLeft ->addTouchEventListener(this,toucheventselector(Ui_Prompt::eventTouchButton));
	mButtonRight->setPressedActionEnabled(true);
	mButtonLeft ->setPressedActionEnabled(true);

	this->addChild(m_widget);
	m_widget->setTouchEnabled(true);
	m_widget->setTouchPriority(TOUCH_UI_PRIORITY-50);
	fitScreen();

	return true;
}

void Ui_Prompt::onEnter()
{
	Widget::onEnter();
}
void Ui_Prompt::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Prompt/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Prompt/GameUI0.png");
	Widget::onExit();
}

void Ui_Prompt::initWithOneButton(const char* content , const char* title , const char* buttonTitle)
{
	do{
		if (content){
			CC_BREAK_IF(!mContent);
			mContent->setText(content);
		}
		if (title){
			CC_BREAK_IF(!mTitle);
			mTitle->setText(title);
		}
		
		CC_BREAK_IF(!mButtonLeft);
		//removeChild(mButtonLeft,true);
		mButtonLeft->removeFromParentAndCleanup(true);
		CC_BREAK_IF(!mButtonRight);
		mButtonRight->setPositionX(480);

		if (buttonTitle){
			mButtonRight->setTitleText(buttonTitle);
		}

	} while (0);
}

void Ui_Prompt::initWithTwoButtons(const char* content , const char* title , const char* rbuttonTitle , const char* lbuttonTitle)
{
	do{
		if (content){
			CC_BREAK_IF(!mContent);
			mContent->setText(content);
			//mContent->setTextAreaSize(Size(400,200));
		}
		if (title){
			CC_BREAK_IF(!mTitle);
			mTitle->setText(title);
		}
		if (rbuttonTitle){
			CC_BREAK_IF(!mButtonRight);
			mButtonRight->setTitleText(rbuttonTitle);
		}
		if (lbuttonTitle){
			CC_BREAK_IF(!mButtonLeft);
			mButtonLeft->setTitleText(lbuttonTitle);
		}
	} while (0);
}

void Ui_Prompt::initWithRetry(const char* content , const char* title , const char*  rbuttonTitle , const char* lbuttonTitle )
{
	mButtonRight->getChildByName("Image_35")->setVisible(false);
	mButtonRight->getChildByName("retry")->setVisible(true);
	initWithTwoButtons(content ,  title  , rbuttonTitle , lbuttonTitle);
}
void Ui_Prompt::close()
{
	removeFromParentAndCleanup(true);
}

void Ui_Prompt::eventTouchButton(Ref* sender, TouchEventType type)
{
	if (type==TOUCH_EVENT_ENDED){
		Button *btn = static_cast<Button*>(sender);
		if (strcmp(btn->getName(),mButtonRight->getName())==0){
			this->removeChild(m_widget,true);
			if (m_callbackTag != 0)
			{
				if (m_obj_r && m_connect_r){
					(m_obj_r->*m_connect_r)(m_callbackTag);
					 m_callbackTag = 0;
				}
			} else {
				if (m_obj_r && m_callfunc_r){
					(m_obj_r->*m_callfunc_r)();
				}
			}
			audioPlayer::getInstance() -> playEffect("S2.mp3");
		}else if (strcmp(btn->getName(),mButtonLeft->getName())==0){
			this->removeChild(m_widget,true);
			if (m_obj_l && m_callfunc_l){
				(m_obj_l->*m_callfunc_l)();
			}

			audioPlayer::getInstance() -> playEffect("S2.mp3");
		}
		close();
	}
}


void Ui_Prompt::setRightSelector(Ref *obj , SEL_COMM callfunc)
{
	m_obj_r      = obj;
	m_callfunc_r = callfunc;
}
void Ui_Prompt::setRightConnectSelector(Ref *obj , SEL_CONNECT callfunc,int tag)
{
	m_obj_r      = obj;
	m_connect_r = callfunc;
	m_callbackTag = tag;
}

void Ui_Prompt::setLeftSelector(Ref *obj , SEL_COMM callfunc)
{
	m_obj_l      = obj;
	m_callfunc_l = callfunc;
}

void Ui_Prompt::fitScreen()
{

	Widget *wg = static_cast<Widget*>(Helper::seekWidgetByName(m_widget,"Panel_frame"));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	wg->setScale(_scale);
	wg->setPosition(Point(visibleSize.width/2  - wg->getSize().width/2*_scale ,
						  visibleSize.height/2 - wg->getSize().height/2*_scale 
		));

	Widget *bg = static_cast<Widget*>(Helper::seekWidgetByName(m_widget,"Panel_mask"));
	bg->setScaleY(visibleSize.height/640);
}
