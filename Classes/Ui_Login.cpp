#include "Ui_Login.h"
#include "levelScene.h"
#include "LoadingScene.h"
#include "audioPlayer.h"
#include "CachManager.h"

Ui_Login::Ui_Login()
{
}
Ui_Login::~Ui_Login()
{
}
bool Ui_Login::init()
{
	if (!Widget::init())
	{
		return false;
	}
	bool ret = true;
	do 
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_Login/GameUI0.plist","Ui_Login/GameUI0.png");
		_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_Login/Ui_Login.ExportJson");
		this->addChild(_widget);
		//play
		_warp = Helper::seekWidgetByName(_widget,"warp");

		Size winSize=Director::getInstance()->getWinSize();
	    playBtn =static_cast<Button*>(Helper::seekWidgetByName(_warp,"play"));
		playBtn->addTouchEventListener(this, toucheventselector(Ui_Login::eventTouchButton));
		playBtn->setTouchEnabled(false);
		playBtn->setVisible(false);
		
		//playBtn->setPosition(Point(playBtn->getPositionX()-7,playBtn->getPositionY()-7));

		auto delay1=DelayTime::create(1.45f);
		auto callback1=CallFuncN::create(CC_CALLBACK_1(Ui_Login::btnComeOut1, this));

		auto seq=Sequence::create(delay1,callback1,NULL);
		playBtn->runAction(seq);


		disable_icon_music  = static_cast<ImageView*>(Helper::seekWidgetByName(_widget,"Image_38")); ;
		disable_icon_effect = static_cast<ImageView*>(Helper::seekWidgetByName(_widget,"audio_close"));;
		mMusicBtn		= static_cast<Button*>(Helper::seekWidgetByName(_widget,"music"));			
		mEffectBtn       = static_cast<Button*>(Helper::seekWidgetByName(_widget,"audio"));
		mMusicBtn ->addTouchEventListener(this, toucheventselector(Ui_Login::eventTouchButton));
		mEffectBtn ->addTouchEventListener(this, toucheventselector(Ui_Login::eventTouchButton));

		
		userData= UserRecord::getInstance();
		if (userData->getmusicLock()==YES){//说明是锁着，不播放音乐啊
			disable_icon_music->setVisible(true);
		}

		if (userData->geteffectLock()==YES)
		{
			disable_icon_effect->setVisible(true);
		}

		//server_panel
	 //	auto server_panel = Helper::seekWidgetByName(_warp,"server_panel");
		//server_panel->setVisible(true);
		//server_panel->setPositionX(server_panel->getPositionX()-1000);
		//btn_close
		//auto btn_close = Helper::seekWidgetByName(_warp,"btn_close");
		//btn_close->addTouchEventListener(this, toucheventselector(Ui_Login::closeServerList));
		//auto changearea =  Helper::seekWidgetByName(_warp,"select_server");
		//changearea->addTouchEventListener(this, toucheventselector(Ui_Login::selectServer));

		UserRecord::getInstance()->setMusicPlay("startScene_Music.mp3");
		fitScreen();
	} while (0);
	return ret;
}

void Ui_Login::onEnter()
{
	Widget::onEnter();
	
}

void Ui_Login::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_Login/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_Login/GameUI0.png");
	Widget::onExit();
}

void Ui_Login::btnComeOut1(Ref*r){

	playBtn->setVisible(true);
	playBtn->setTouchEnabled(true);
	
}

void Ui_Login::btnComeOut2(Ref*r){

}


void Ui_Login::selectServer(Ref* sender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		//auto logo = Helper::seekWidgetByName(_warp,"logo");
		auto play = Helper::seekWidgetByName(_warp,"play");
// 		auto select_server = Helper::seekWidgetByName(_warp,"select_server");
// 		auto server_panel = Helper::seekWidgetByName(_warp,"server_panel");
	
		fadeOutMoveOut(play);
// 		fadeOutMoveOut(select_server);
// 		fadeInMoveIn(server_panel);

		/*server_panel->setPositionX(server_panel->getPositionX()+1000);
		server_panel->setVisible(true);*/
	}
}
void Ui_Login::fadeOutMoveOut(Widget* sender)
{
	auto _action = Sequence::create(
		FadeOut::create(0.2f),
		CallFunc::create([=](){
			sender->setPositionX(sender->getPositionX()-1000);
		}),
		NULL
		);
		sender->runAction(_action);
}


void Ui_Login::closeServerList(Ref* sender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		//auto logo = Helper::seekWidgetByName(_warp,"logo");
		auto play = Helper::seekWidgetByName(_warp,"play");
		auto select_server = Helper::seekWidgetByName(_warp,"select_server");
		auto server_panel = Helper::seekWidgetByName(_warp,"server_panel");
		server_panel->setPositionX(server_panel->getPositionX()-1000);
		//fadeInMoveIn(logo);
		fadeInMoveIn(play);
		fadeInMoveIn(select_server);
		
	}
}
void Ui_Login::fadeInMoveIn(Widget* sender)
{
	auto _action = Sequence::create(
		CallFunc::create([=](){	
			sender->setOpacity(0);
			sender->setPositionX(sender->getPositionX()+1000);
			sender->setVisible(true);
		}),
		FadeIn::create(0.3f),
		NULL
		);
	sender->runAction(_action);
}
void Ui_Login::eventTouchButton(Ref* sender, TouchEventType type)
{
	if (type == TouchEventType::TOUCH_EVENT_ENDED)
	{
		Button* nowButton = static_cast<Button*>(sender);
		if (nowButton)
		{
			if (nowButton->getName() == mMusicBtn->getName())
			{
				audioPlayer::getInstance()->playEffect("S1.mp3");//播放点击声音
				if (disable_icon_music->isVisible()){
					disable_icon_music->setVisible(false);
					userData->setMusicLockValue(NO);

					/*UserRecord::getInstance()->resetMuiscPlay();*/
					//UserRecord::getInstance()->setMusicPlay("startScene_Music.mp3");
					this->runAction(Sequence::create(DelayTime::create(0.2f),CallFunc::create([=]()
					{
						UserRecord::getInstance()->setMusicPlay("startScene_Music.mp3");
					}),NULL));

				}else{
					disable_icon_music->setVisible(true);
					userData->setMusicLockValue(YES);
					audioPlayer::getInstance()->stopBackgroundMusic();
				}
			}else if (nowButton->getName() == mEffectBtn->getName())
			{
				audioPlayer::getInstance()->playEffect("S1.mp3");//播放点击声音
				if (disable_icon_effect->isVisible()){
					disable_icon_effect->setVisible(false);
					
					userData->setEffectLockValue(NO);
					audioPlayer::getInstance()->resumeAllEffects();
					
				}else{
					disable_icon_effect->setVisible(true);
					userData->setEffectLockValue(YES);
					audioPlayer::getInstance()->stopAllEffects();
				}
			}else if (nowButton->getName() == playBtn->getName())
			{
				audioPlayer::getInstance()->stopBackgroundMusic();//停止音乐
				audioPlayer::getInstance()->playEffect("S2.mp3");//播放确认声音
				
				Button* btn = dynamic_cast<Button*>(sender);
				btn->setTouchEnabled(false);
				CachManager::getInstance()->cleanCach();

				Scene* pTScene = LoadingScene::createScene();
				LoadingScene*  cl = static_cast<LoadingScene*> (pTScene->getChildByTag(6));
				cl->loadScene(LEVEL_SCENE_TAG);
				TransitionFade* pTranslateScene = TransitionFade::create(0.1f,pTScene);;
				Director::getInstance()->replaceScene(pTranslateScene);
			}
		}
	}
}
void Ui_Login::fitScreen()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	
	float _maxScale = MAX(visibleSize.width/960 , visibleSize.height/640);

// 	auto bg  = Helper::seekWidgetByName(_widget,"bg");
// 	bg->setScale(_maxScale);
// 	
// 	bg->setPosition(Point(
// 		originPt.x+visibleSize.width/2,
// 		originPt.y+visibleSize.height/2));


	float hscale = visibleSize.height/640;
	_warp->setScale(hscale);
	_warp->setPosition(Point(
		originPt.x+visibleSize.width/2-_warp->getSize().width*_warp->getScale()/2,
		originPt.y+visibleSize.height/2-_warp->getSize().height*_warp->getScale()/2
		));

	auto audio_control  = Helper::seekWidgetByName(_widget,"audio_control");
	audio_control->setPosition(
		Point(
		originPt.x,
		visibleSize.height-audio_control->getSize().height
		));

	this->runAction(Sequence::create(DelayTime::create(1.2f),CallFunc::create([=]()
	{
		UserRecord::getInstance()->setMusicPlay("startScene_Music.mp3");
	}),NULL));
}
