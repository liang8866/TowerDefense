#include "Ui_MessageBox.h"
#include "levelScene.h"
#include "progressScene.h"
#include "SceneData.h"

/*
std::string WC2UT(const wchar_t* buf) 
{ 
	
	int len = WideCharToMultiByte(CP_UTF8, 0, buf, -1, NULL, 0, NULL, NULL); 
	std::vector<char> utf8(len); 
	WideCharToMultiByte(CP_UTF8, 0, buf, -1, &utf8[0], len, NULL, NULL); 

	return std::string(&utf8[0]); 
} 
*/

Ui_MessageBox::Ui_MessageBox(void):_listener(NULL)
{

}

Ui_MessageBox::~Ui_MessageBox(void)
{
	_eventDispatcher->removeEventListener(_listener);
	CC_SAFE_RELEASE(_listener);
	_listener=NULL;
}


// 初始化
bool Ui_MessageBox::init()
{
	if (!Widget::init())
		return false;


	// 加载json
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MessageBox/GameUI0.plist","MessageBox/GameUI0.png");
	mMainWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("MessageBox/MessageBox.ExportJson");
	if (nullptr == mMainWidget)
		return false;


	mButtonEnter		= static_cast<Button*>(findChild("Button_NewGame"));
	mButtonCencel		= static_cast<Button*>(findChild("Button_Exit"));
	mMusicBtn		= static_cast<Button*>(findChild("Button_music"));
	mEffectBtn       = static_cast<Button*>(findChild("Button_effect"));
	mCloseBtn        = static_cast<Button*>(findChild("closeBtn"));
	mButtonEnter ->addTouchEventListener(this, toucheventselector(Ui_MessageBox::eventTouchButton));
	mButtonCencel->addTouchEventListener(this, toucheventselector(Ui_MessageBox::eventTouchButton));
	mMusicBtn    ->addTouchEventListener(this, toucheventselector(Ui_MessageBox::eventTouchButton));
	mEffectBtn   ->addTouchEventListener(this, toucheventselector(Ui_MessageBox::eventTouchButton));
	mCloseBtn    ->addTouchEventListener(this, toucheventselector(Ui_MessageBox::eventTouchButton));
	mButtonEnter ->setPressedActionEnabled(true);
	mButtonCencel->setPressedActionEnabled(true);
	mMusicBtn    ->setPressedActionEnabled(true);
	mEffectBtn   ->setPressedActionEnabled(true);
	mCloseBtn    ->setPressedActionEnabled(true);


	disable_icon_music  = static_cast<ImageView*>(findChild("disable_icon_music")); ;
	disable_icon_effect = static_cast<ImageView*>(findChild("disable_icon_effect"));;
	userData= UserRecord::getInstance();
	if (userData->getmusicLock()==YES){//说明是锁着，不播放音乐啊
		disable_icon_music->setVisible(true);
	}

	if (userData->geteffectLock()==YES)
	{
		disable_icon_effect->setVisible(true);
	}

	this->addChild(mMainWidget);

	mMainWidget->setTouchEnabled(true);
	mMainWidget->setTouchPriority(TOUCH_UI_PRIORITY-1);
	fitScreen();

	showMessageBox();

	return true;
}

void Ui_MessageBox::onEnter()
{
	Widget::onEnter();
}

void Ui_MessageBox::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("MessageBox/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("MessageBox/GameUI0.png");
	Widget::onExit();
}

// 查找子控件
Widget* Ui_MessageBox::findChild(const char* _widgetName)
{
	if (!mMainWidget)
		return nullptr;
	Widget* _thisChild = Helper::seekWidgetByName(mMainWidget, _widgetName);
	if (nullptr == _thisChild)
		return nullptr;
	return _thisChild;
}


bool Ui_MessageBox::showMessageBox()
{
	this->pauseGame(0);
	//scheduleOnce(schedule_selector(Ui_MessageBox::pauseGame),0.3f);
	return true;
}

bool Ui_MessageBox::closeMessageBox()
{
	//Director::getInstance()->resume();
	NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, NULL);
	// 恢复暂停
	audioPlayer::getInstance()->resumeAllMusicEffect();

	setPause_StoreyObject(false);
	removeFromParentAndCleanup(true);
	return true;
}

// 按钮点击事件
void Ui_MessageBox::eventTouchButton(Ref* sender, TouchEventType type)
{
	if (type ==TOUCH_EVENT_ENDED)
	{
		Button* nowButton = static_cast<Button*>(sender);
		if (nowButton)
		{
			if (nowButton->getName() == mButtonEnter->getName())
			{
				// 清空临时魔法
				std::map<int, int>::iterator iter;
				iter = UserRecord::getInstance()->mTemporaryMagic.begin();
				while (iter != UserRecord::getInstance()->mTemporaryMagic.end())
				{
					if (iter->second > 0)
					{
						// 清空实时数据
						UserRecord::getInstance()->alterGoodsCount(Value(iter->first).asString(), -iter->second);
						// 清空临时数据
						iter->second = 0;
					}
					++iter;
				}
				UserRecord::getInstance()->clearTemporaryMagic();
				audioPlayer::getInstance()->stopAllEffects();
				nowButton->setTouchEnabled(false);
				//Director::getInstance()->resume();
				NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, NULL);

				Scene* pTScene = ReadyScene::createScene();
				TransitionFade *transitionfadeScene=TransitionFade::create(0.8f,pTScene);
				Director::getInstance()->replaceScene(transitionfadeScene);
			} 
			else if (nowButton->getName() == mButtonCencel->getName())
			{	
				// 清空临时魔法
				std::map<int, int>::iterator iter;
				iter = UserRecord::getInstance()->mTemporaryMagic.begin();
				while (iter != UserRecord::getInstance()->mTemporaryMagic.end())
				{
					if (iter->second > 0)
					{
						// 清空实时数据
						UserRecord::getInstance()->alterGoodsCount(Value(iter->first).asString(), -iter->second);
						// 清空临时数据
						iter->second = 0;
					}
					++iter;
				}
				UserRecord::getInstance()->clearTemporaryMagic();
				audioPlayer::getInstance()->stopAllEffects();
				nowButton->setTouchEnabled(false);
				//Director::getInstance()->resume();
				NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, NULL);
				Scene* pTScene = levelScene::createScene();
				TransitionFade *transitionfadeScene=TransitionFade::create(0.8f,pTScene);
				Director::getInstance()->replaceScene(transitionfadeScene);
			}else if (nowButton->getName() == mMusicBtn->getName()){
				musicLock();
			}else if (strcmp(nowButton->getName(),mEffectBtn->getName())==0){
				effectLock();
			}else if (strcmp(nowButton->getName(),mCloseBtn->getName())==0){
				closeMessageBox();
			}
		}

	}
}


void Ui_MessageBox::pauseGame(float dt)
{
	//Director::getInstance()->pause();
	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY, (Ref*)1);
	// 开始暂停
	setPause_StoreyObject(true);
}

void Ui_MessageBox::musicLock()
{
	if (disable_icon_music->isVisible()){
		disable_icon_music->setVisible(false);
		userData->setMusicLockValue(NO);
		userData->resetMuiscPlay();
	}else{
		disable_icon_music->setVisible(true);
		userData->setMusicLockValue(YES);
		audPlayer->stopBackgroundMusic();
	}
}


void Ui_MessageBox::effectLock()
{
	if (disable_icon_effect->isVisible()){
		disable_icon_effect->setVisible(false);
		userData->setEffectLockValue(NO);
		audPlayer->resumeAllEffects();
	}else{
		disable_icon_effect->setVisible(true);
		userData->setEffectLockValue(YES);
		audPlayer->stopAllEffects();
	}
}

void Ui_MessageBox::fitScreen()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Widget *wg = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"ImageView_Back"));
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	wg->setScale(_scale);
	wg->setPosition(Point(originPt.x+visibleSize.width/2 - wg->getSize().width/2*_scale,
						  originPt.y+visibleSize.height/2 - wg->getSize().height/2*_scale
						  ));

	Widget *bg = static_cast<Widget*>(Helper::seekWidgetByName(mMainWidget,"Panel_mask"));
	bg->setScaleY(visibleSize.height/640);
    bg->setScaleX(visibleSize.width/960);
    bg->setPosition(originPt);

	bg->setTouchEnabled(true);
	bg->setTouchPriority(TOUCH_UI_PRIORITY);
}


// 设置层上的某些物件 暂停
void Ui_MessageBox::setPause_StoreyObject(bool _isBool)
{
	for (int i = 0; i < SceneData::getInstance()->getSceneDataMap().size(); ++i)
	{
		StoreyData* thisStoreyData = SceneData::getInstance()->getSceneDataMapStorey(i);
		if (thisStoreyData && thisStoreyData->mItem)
		{
			// 暂停Event1 渗透魔法
			thisStoreyData->setPause_AllEvent1(_isBool);
			// 暂停墓碑
			thisStoreyData->isPauseHeadstone(_isBool);
			// 暂停感叹号
			thisStoreyData->setPause_GanTanHao(_isBool);
		}
	}
}




