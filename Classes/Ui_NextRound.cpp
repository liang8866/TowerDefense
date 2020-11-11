#include "Ui_NextRound.h"
#include "WindowManager.h"
#include "UserRecord.h"
#include "StoreyData.h"
#include "Event1Object.h"

static const int boxEnemyRound = 6;

Ui_NextRound::Ui_NextRound():roundWidget(NULL),
	roundBtn(NULL),progress(NULL),clickedCnt(0)
{

}

Ui_NextRound::~Ui_NextRound()
{
	NotificationCenter::getInstance()->removeObserver(this,UInextPAUSEKEY);
	NotificationCenter::getInstance()->removeObserver(this, UInextRESUMEKEY);
	NotificationCenter::getInstance()->removeObserver(this, FORCESTART);
}


bool Ui_NextRound::init()
{
	if (!Widget::init())
		return false;

	roundWidget = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_NextRound/Ui_NextRound.ExportJson"));
	roundWidget->setAnchorPoint(Point(0.5f,0.5f));

	roundBtn = static_cast<Button*>(Helper::seekWidgetByName(roundWidget,"Button_Next_Round"));
	roundBtn -> addTouchEventListener(this,toucheventselector(Ui_NextRound::evetRoundButtonCallFunc)); 

	auto sp = Sprite::createWithSpriteFrameName("image/UI_zjm_022.png");
	progress = ProgressTimer::create(sp);
	progress->setType(ProgressTimer::Type::RADIAL);
	//progress->setReverseProgress(true);
	progress->setPosition(Point(sp->getContentSize().width/2,sp->getContentSize().height/2));
	roundBtn->addChild(progress,100);

	this->addChild(roundWidget);
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float hscale = visibleSize.height/640;
	roundWidget->setScale(hscale);


	_timeFlag = false;
	_timeCount = 1;
	_disTime = 5;
	
	//roundWidget->setVisible(false);	

	//setFirstRound(true);

	auto user = UserRecord::getInstance();
	int _sectionNum = user->getSectionNum();
	int _sceneNum = user->getSceneNum();
	this->schedule(schedule_selector(Ui_NextRound::update),1.0);

	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	Value currentSection(_sectionNum);
	Value currentScene(_sceneNum);
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	int monsterCount = monster->count();
	UserRecord::getInstance()->setMaxRound(monsterCount);
	show(5);

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_NextRound::myPauseSchedule),UInextPAUSEKEY,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_NextRound::myResumeSchedule),UInextRESUMEKEY,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Ui_NextRound::freshmanEnemyComeIn),FORCESTART,NULL);
	

	return true;
}
void Ui_NextRound::update(float dt)
{

	/*_timeCount++;
	if (_timeCount == _disTime)
	{

	auto user = UserRecord::getInstance();
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	Value currentSection(user->getSectionNum());
	Value currentScene(user->getSceneNum());
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	Value currentRound(user->getRound());
	__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
	__String* disTime  = dynamic_cast<__String*>(round->objectForKey("disTime"));
	this->show(disTime->intValue());
	_disTime = disTime->intValue();


	_timeCount = 1;



	}*/
	
	//auto user = UserRecord::getInstance();
	//if (user->getRound() >= user->getMaxRound() )
	//{
	//	return;
	//}
	//if (_timeCount+1 == DEFAULT_START_TIME && !user->getIsGameStart())
	//{
	//	user->setIsGameStart(true);
	//}
	//if (_timeCount == 1)
	//{
	//	if (!user->getIsGameStart())
	//	{
	//		setFirstRound(false);
	//		this->show(_disTime);
	//	} else {
	//		//取下一波兵数据
	//		__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	//		Value currentSection(user->getSectionNum());
	//		Value currentScene(user->getSceneNum());
	//		__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	//		__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	//		__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	//		__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	//		Value currentRound(user->getRound());
	//		__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
	//		__String* disTime  = dynamic_cast<__String*>(round->objectForKey("disTime"));
	//		this->show(disTime->intValue());
	//		_disTime = disTime->intValue();
	//	}
	//}
	//_timeCount++;
	//if (_timeCount == _disTime)
	//{
	//	user->setRound(user->getRound()+1);
	//	_timeCount = 1;
	//	_timeFlag = false;
	//}

	
}
void Ui_NextRound::show(float dt)
{
	do{
		CC_BREAK_IF(!roundWidget);
		roundWidget->setVisible(true);
		CC_BREAK_IF(!roundBtn);
		roundBtn->setTouchEnabled(true);
		CC_BREAK_IF(!progress);
		progress->setPercentage(0);
		progress->setVisible(true);

		progress->runAction(Sequence::create(
			ProgressFromTo::create(dt,0,100),
			CallFunc::create([=](){
				enemyAttack();
				hide();
			}),
			NULL));

			if (UserRecord::getInstance()->getIsFreshMan())
			{
				CCLOG("UInextPAUSEKEY");
				NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);
			}
	}while(0);
}



void Ui_NextRound::evetRoundButtonCallFunc(Ref* sender, TouchEventType type)
{
	if ( type != TOUCH_EVENT_ENDED )
		return;
	
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	if ( mainLayer->getCurrRound() >= mainLayer->getMaxRound() ) {
		return;
	}
		
	clickedCnt += 1;
	
	if ( clickedCnt <2 ) {
		return;
	}
	clickedCnt = 0;
	//点击直接出兵
	enemyAttack();
	hide();
}

void Ui_NextRound::timerFinishedCallFunc()
{
	do{
		clickedCnt = 1;
		evetRoundButtonCallFunc(NULL,TOUCH_EVENT_ENDED);
	}while (0);
}


float Ui_NextRound::GetlessX(){

	
	SceneData *_scenedata = SceneData::getInstance();
	StoreyData*_storeydata  = _scenedata->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_GROUND);
	float tempx=0;
	float mm = -20;
	if (_storeydata->getObjectData().size() > 0)
	{
		for (GameObjects *sp:_storeydata->getObjectData())
		{
			if (sp->getobjectType()==TYPE_ENEMY&&sp->getPositionX()<-50.0)
			{
				if (tempx>-1.0)//第一个
				{
					tempx=sp->getPositionX();
				}else
                {
					tempx=(tempx<sp->getPositionX())?tempx:sp->getPositionX();
				}
		     }

	     }

		mm=tempx-80;
	}
	return mm;
}

void Ui_NextRound::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{

}
void Ui_NextRound::hide()
{
	do{
		CC_BREAK_IF(!roundWidget);
		roundWidget->setVisible(false);
		CC_BREAK_IF(!roundBtn);
		roundBtn->setTouchEnabled(false);
		CC_BREAK_IF(!progress);
		progress->stopAllActions();

		//取下一个CD时间
		UserRecord* user = UserRecord::getInstance();

		if (user->getRound() >= user->getMaxRound() )
		{
			return;
		}



		__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
		Value currentSection(user->getSectionNum());
		Value currentScene(user->getSceneNum());
		Value currentRound(user->getRound());

		__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
		__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
		__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
		__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
		__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
		__String* disTime  = dynamic_cast<__String*>(round->objectForKey("disTime"));

		this->runAction(
			Sequence::create(
			DelayTime::create(0.5f),
			CallFunc::create(CC_CALLBACK_0(Ui_NextRound::show,this,disTime->intValue())),
			NULL));

		
	}while(0);
}

void Ui_NextRound::enemyAttack()
{
	//出兵
	UserRecord* user = UserRecord::getInstance();
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");
	Value currentSection(user->getSectionNum());
	Value currentScene(user->getSceneNum());
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	//CCLOG("currentSection %d , currentScene %d, currentRound %d", user->getSectionNum(), user->getSceneNum(), _curRound);
	Value currentRound(user->getRound()+1);
	user->setRound(user->getRound()+1);
	__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
	__Dictionary* enemys = dynamic_cast<__Dictionary*>(round->objectForKey("enemy"));
	Ui_MainBack* mianBack = static_cast<Ui_MainBack*>(WindowManager::getInstance()->getWindow("BackGround"));




	//通知服务器用户进入下一波
	if (!UserRecord::getInstance()->getIsFreshMan())
	{
		auto postDatas = __String::createWithFormat("c=game&a=loop&cid=%d&sid=%d&num=%d",
			UserRecord::getInstance()->getSectionNum(),
			UserRecord::getInstance()->getSceneNum(),
			user->getRound()
			)->getCString();
		Connect* handle = Connect::getInstance();
		handle->send(postDatas,this,httpresponse_selector(Ui_NextRound::onHttpRequestCompleted));
	}

	int _event106_count = 0;
	// 检查是否有事件
	__Dictionary* events = dynamic_cast<__Dictionary*>(round->objectForKey("event"));
	if (events)
	{
		for (int i = 1 ; i <= events->count(); ++i)
		{
			Value _eventKey(i);
			auto _curObject = events->objectForKey(_eventKey.asString());
			if (_curObject)
			{
				// 取事件id
				__String* _objectString = dynamic_cast<__String*>(_curObject);
				int _eventId = _objectString->intValue();
				if (_eventId == 101)
				{
					// 创建渗透陷阱
					event1Object* _object = event1Object::createEvent();
				}else if (_eventId == 106)
				{
					// 106事件:随机出现掉魔法的特殊小兵
					auto _eventPlist = PlistManager::getInstance()->getPlistMap("EventData");
					if (_eventPlist)
					{
						auto _idString = StringUtils::format("%d", _eventId);
						auto _idPlist = PlistManager::getInstance()->getPlist(_eventPlist, _idString.c_str());
						if (_idPlist)
						{
							// 取随机几个
							auto _count = PlistManager::getInstance()->getPlistCString(_idPlist, "count").intValue();
							if (_count > 0)
							{								
								_event106_count = _count;
							}
						}
					}
				}
			}
		}
	}
	
	float posx=this->GetlessX();
	int count = enemys->count();
	int _random = 0;

	// 数量大于0  才开始随机
	if (_event106_count > 0)
	{
		_random = UserRecord::getInstance()->getRandom(count);
	}
	
	for (int i=1;i<=count;++i)
	{
		float distanse=80;
		__Dictionary* item = dynamic_cast<__Dictionary*>(enemys->objectForKey(Value(i).asString()));
		__String* e_type = dynamic_cast<__String*>(item->objectForKey("type"));
		__String* e_num = dynamic_cast<__String*>(item->objectForKey("num"));
		int _type=e_type->intValue();
		if (_type==4||_type==8)
		{
			distanse=120;
		}
		int _count106 = 0;
		if (_event106_count > 0 && _random == i)
		{
			_count106 = _event106_count;
		}
		mianBack->CreateEnemyForStorey(posx,_type,e_num->intValue(), currentRound.asInt(), _count106);
		posx-=distanse*e_num->intValue();
	}
	if (currentRound.asInt()== boxEnemyRound && UserRecord::getInstance()->getBoxEnemySceneBySection(user->getSectionNum()) == user->getSceneNum() ){
		mianBack->CreateEnemyForStorey(posx - 80 , ENEMY_ITEM21 ,1 , currentRound.asInt());
	}
}

void Ui_NextRound::myPauseSchedule(Ref *obj){

	this->pauseSchedulerAndActions();
	progress->pauseSchedulerAndActions();
	roundBtn->setTouchEnabled(false);
}
void Ui_NextRound::myResumeSchedule(Ref *obj){
		this->resumeSchedulerAndActions();
		progress->resumeSchedulerAndActions();
		roundBtn->setTouchEnabled(true);
}

void Ui_NextRound::freshmanEnemyComeIn(Ref *obj)
{
	clickedCnt=1;
	evetRoundButtonCallFunc(roundBtn, TOUCH_EVENT_ENDED);
	
}
