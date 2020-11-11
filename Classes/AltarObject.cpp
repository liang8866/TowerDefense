#include "AltarObject.h"
#include "UIUpgrades.h"
#include "PlistManager.h"
#include "audioPlayer.h"

USING_NS_CC;
#define  PLAYATK "PLAYATK"
AltarObject::~AltarObject(void)
{
	NotificationCenter::getInstance()->removeObserver(this, PAUSEKEY);
	NotificationCenter::getInstance()->removeObserver(this, RESUMEKEY);
}
bool AltarObject::init()
{
	if (!Widget::init())
	{
		return false;
	}
	_isSelect=NO;
	this->setProperty();
	_dialog = nullptr;
	money = 1000;
	_employeeCount = 0;
	_employeeEmpty = 0;
	_clickCount = 0;
	_interval = 0;
	_duration = 0;
	_isSpeed  = false;
	_cdTime = PlistManager::getInstance()->getConfigTypeCString("coolie","cdTime").floatValue();
	_curCoolieOutPut = PlistManager::getInstance()->getConfigTypeCString("coolie","output").intValue();
	_curOneCostPrice = PlistManager::getInstance()->getConfigTypeCString("coolie","price").floatValue();

	UserRecord* user =  UserRecord::getInstance();
	_curCoolieCount = user->getCoolieCount(); 

	updateValue();
	this->schedule(schedule_selector(AltarObject::logicUpdate),1.0f);

	ArmatureDataManager::getInstance()->addArmatureFileInfo("scene_tanguoji0.png","scene_tanguoji0.plist","scene_tanguoji.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	_armature = Armature::create("scene_tanguoji");
	this->addChild(_armature,1);
	_armature->getAnimation()->play("load");
	_armature->setAnchorPoint(Point::ANCHOR_MIDDLE_BOTTOM);
	this->setContentSize(Size(202,253));
	_armature->setPosition(Point(202/2,0));
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(AltarObject::onFrameEvent));


	Widget* seatWarp = Widget::create();
	seatWarp->setPosition(Point(22,201));
	this->addChild(seatWarp);
	for (int i=1;i<= 8;++i)
	{
		CoolieType ct = Coolie_TYPE_DISABLE;
		if (i<=_curCoolieCount)
		{
			ct = Coolie_TYPE_EMPTY;
		}

		if (i == 1)
		{
			ct = Coolie_TYPE_USED;
		}
		auto coolie = Coolie::create(ct);
		coolie->setiIndex(i);
		seatWarp->addChild(coolie);
		coolie->setPosition((i-1)*22,24);
		_coolies.pushBack(coolie);
		if (ct == Coolie_TYPE_USED)
		{
			_employeeCount++;
		} else if(ct == Coolie_TYPE_EMPTY)
		{
			_employeeEmpty++;
		}
	}

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(AltarObject::myPauseAllAction),PAUSEKEY,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(AltarObject::myResumeAllAction),RESUMEKEY,NULL);
	
	
	return true;
}
void AltarObject::onFrameEvent(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0){
		_armature->getAnimation()->play("load");
	} 
}
void AltarObject::animationCall(Ref* obj)
{
	std::string aName = _armature->getAnimation()->getCurrentMovementID();
	if (aName == "load")
	{
		_armature->getAnimation()->play("atk",-1,0);
	} 
}


Widget *AltarObject::getDialog(){
	
	Widget* tdialog=(Widget*)this->getChildByTag(1318);
	return tdialog;
}//获取Dialog


void AltarObject::logicUpdate(float dt)
{

	if (!AnimateToDel.empty())
	{
		Vector<Armature*> temp;
		for (const auto& child : AnimateToDel)  
		{  
			Armature* item = static_cast<Armature*>(child);
			if (!item->getAnimation()->isPlaying())
			{
				temp.pushBack(item);
			}
		} 
		if (!temp.empty())
		{
			for (const auto& child : temp)  
			{  
				Armature* item = static_cast<Armature*>(child);
				this->removeChild(item,true);
				AnimateToDel.eraseObject(item);
			} 
		}
	}

	if (_dialog != nullptr && _employeeCount <= _curCoolieCount)
	{		
		Button* mButtonEnter = static_cast<Button*>(Helper::seekWidgetByName(_dialog, "buy"));
		ui::TextBMFont* priceText = static_cast<ui::TextBMFont*>(mButtonEnter->getChildByName("text_num"));
		ImageView* Image_6 = static_cast<ImageView*>(mButtonEnter->getChildByName("Image_6"));
		if ( UserRecord::getInstance()->getSweet()  < _curOneCostPrice)
		{
			if (!mButtonEnter->_isgrey)
			{
				mButtonEnter->setGrey();
				Image_6->removeGrey();
				priceText->setColor(Color3B::RED);
			}
		}

		if ( UserRecord::getInstance()->getSweet()  >=  _curOneCostPrice)
		{
			if (mButtonEnter->_isgrey && _employeeCount <  _curCoolieCount)
			{
				mButtonEnter->removeGrey();
				priceText->setColor(Color3B(92,31,40));
			}
		}

		/*if (priceText->getColor()==Color3B::RED && UserRecord::getInstance()->getSweet() >=_curOneCostPrice){
			priceText->setColor(Color3B(92,31,40));
		}*/
	}
	
	if (UserRecord::getInstance()->getSweet() >= UserRecord::getInstance()->getMaxSweet())
	{
		return;
	}

	++_duration;
	++_interval;
	if (_isSpeed){
		if (_duration>=_cdTime/3){
			_duration = 0;
			_armature->getAnimation()->play("atk",0,-1);
		}
	}else{
		if (_duration>=_cdTime){
			_duration = 0;
			_armature->getAnimation()->play("atk",0,-1);
		}
	}
	
	if (_interval == _cdTime )
	{
		//播放粮果机动画
		_interval = 0;
		for (int i=1;i<=_employeeCount;++i)
		{
			ArmatureDataManager::getInstance()->addArmatureFileInfo("image/tanguoji_pop0.png","image/tanguoji_pop0.plist","image/tanguoji_pop.ExportJson");
			Armature* _ar = Armature::create("tanguoji_pop");
			_ar->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(AltarObject::onFrameEvent));
			//auto temp = Sprite::createWithSpriteFrameName("image/UI_zjm_23.png");
			this->addChild(_ar);
			_ar->setPositionX(this->getSize().width/2+3);
			this->runAction(
				Sequence::create(
					DelayTime::create(0.2f*i),
					CallFunc::create([=](){
						_ar->getAnimation()->play("Animation1_Copy1",-1,0);
						AnimateToDel.pushBack(_ar);
						UserRecord* user =  UserRecord::getInstance();
						int sweet = user->getSweet();
						int maxSweet = user->getMaxSweet();
						sweet+=_curCoolieOutPut;
						if (sweet > maxSweet)
						{
							sweet = maxSweet;
						}
						user->setSweet(sweet);

						this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
							audioPlayer::getInstance()->playEffect("S15.mp3");	// 草莓汤使用音效
						}),NULL));

						
					}),
				NULL)
				);
			//temp->setPosition(this->getSize().width/2,this->getSize().height/2);
			//ccBezierConfig bezier;
			//bezier.controlPoint_1 =Point(-15,100);
			//bezier.controlPoint_2 = Point(-40, 155);
			//bezier.endPosition = Point(-70,-30);
			//temp->runAction(
			//	Sequence::create(
			//		DelayTime::create(0.2f*i),
			//		Spawn::create(
			//			//MoveTo::create(0.4f,Point(temp->getPositionX(),temp->getPositionY()+110)),
			//			BezierBy::create(1, bezier),
			//			//FadeTo::create(0.4f,100),
			//			NULL
			//		),
			//		CallFunc::create([=](){
			//			//加钱
			//			UserRecord* user =  UserRecord::getInstance();
			//			int sweet = user->getSweet();
			//			int maxSweet = user->getMaxSweet();
			//			if (sweet == maxSweet)
			//			{
			//				temp->removeFromParentAndCleanup(true);
			//				return;
			//			}
			//			
			//			sweet+=_curCoolieOutPut;
			//			if (sweet > maxSweet)
			//			{
			//				sweet = maxSweet;
			//			}
			//			user->setSweet(sweet);

			//			//删除自己
			//			temp->removeFromParentAndCleanup(true);
			//		}),
			//	NULL
			//));
		}
		




	}
}
void AltarObject::setProperty()
{

}
void AltarObject::createTouchEvent()
{
	_listener = EventListenerTouchOneByOne::create();
	CC_SAFE_RETAIN(_listener);
	_listener->setSwallowTouches(false);
	_listener->onTouchBegan = [](Touch* touch, Event* event){
		return true;
	};
	_listener->onTouchEnded = CC_CALLBACK_2(AltarObject::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener,this);
}

void AltarObject::onTouchEnded(Touch* touch,Event* event)
{
	touch->getLocation();
	Rect box  = this->getBoundingBox();
	Point touchd = touch->getLocation();
	Point locationInNode = this->convertToNodeSpace(touch->getLocation());
//	Rect rect = Rect(0, 0, this->_head->getContentSize().width, this->_head->getContentSize().height+this->_body->getContentSize().height);
	if (box.containsPoint(locationInNode)) {
		_isSelect=YES;
		if (_dialog == nullptr)
		{
			_dialog = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("Ui_Sweet_Dialog/Ui_Sweet_Dialog.ExportJson");
			_dialog->setTag(1318);

			_dialog->setAnchorPoint(Point::ANCHOR_MIDDLE);
			_dialog->setPositionX(this->getSize().width/2+this->getAnchorPointInPoints().x);
			_dialog->setPositionY(this->getSize().height/2+this->getAnchorPointInPoints().y+80);
			this->addChild(_dialog,5);
		} else if (nullptr == this->getChildByTag(1318)){
			_dialog->setPosition(Point::ZERO);
			_dialog->setAnchorPoint(Point::ANCHOR_MIDDLE);
			_dialog->setPositionX(this->getSize().width/2+this->getAnchorPointInPoints().x);
			_dialog->setPositionY(this->getSize().height/2+this->getAnchorPointInPoints().y+80);
			this->addChild(_dialog,5);
			_dialog->release();
		}
		Button* mButtonEnter = static_cast<Button*>(Helper::seekWidgetByName(_dialog, "buy"));
		mButtonEnter->setPositionY(mButtonEnter->getPositionY()-40);
		mButtonEnter->runAction(Sequence::create(MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		mButtonEnter->addTouchEventListener(this, toucheventselector(AltarObject::eventTouchButton));
		ui::TextBMFont* priceText = static_cast<ui::TextBMFont*>(mButtonEnter->getChildByName("text_num"));
		ImageView*  Image_6 = static_cast<ImageView*>(mButtonEnter->getChildByName("Image_6"));
		//auto price = PlistManager::getInstance()->getConfigTypeCString("coolie","price");

		priceText->setText( Value(Value(_curOneCostPrice).asInt()).asString());

		//当前工位数是否已满
		if (_employeeCount >= _curCoolieCount)
		{
			mButtonEnter->setGrey();
			priceText->setColor(Color3B(92,31,40));
		} else {
			//当前糖果数是否够
			if (UserRecord::getInstance()->getSweet() < _curOneCostPrice)
			{
				mButtonEnter->setGrey();
				Image_6->removeGrey();
				priceText->setColor(Color3B::RED);
				mButtonEnter->setTouchEnabled(false);
			} else {
				mButtonEnter->removeGrey();
				priceText->setColor(Color3B(92,31,40));
			}
		}
		//此方是解锁新手引导步骤
		UserRecord::getInstance()->setFreshManMutex(false);
	} else {
		_isSelect=NO;
		if (_dialog != nullptr)
		{
			_dialog->retain();
			Widget* mButtonEnter = Helper::seekWidgetByName(_dialog, "buy");
			mButtonEnter->getChildByTag(759)->setVisible(false);
			mButtonEnter->getChildByTag(757)->setVisible(true);
			_clickCount = 0;
			this->removeChild(_dialog,false);
		}
	}
	
}
void AltarObject::eventTouchButton(Ref *pSender, TouchEventType type)
{
	Button* btn = static_cast<Button*>(pSender);
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		
		break;

	case TOUCH_EVENT_MOVED:
		
		break;

	case TOUCH_EVENT_ENDED:

		if (strcmp(btn->getName(),"buy")==0){
			//if (_clickCount == 0)
			//{
			//	btn->getChildByTag(759)->setVisible(true);
			//	btn->getChildByTag(756)->setVisible(false);
			//	//btn->getChildByTag(757)->setVisible(false);
			//	btn->setScale(0);
			//	btn->runAction(ScaleTo::create(0.05f,1));


			//	_clickCount++;
			//} else if (_clickCount == 1)
			{
				
				if (UserRecord::getInstance()->getSweet() < _curOneCostPrice)
				{
					return;
				}

				for (Coolie* sp : _coolies)
				{
					if (sp->getType() == Coolie_TYPE_EMPTY)
					{
						sp->setType(Coolie_TYPE_USED);
						_employeeCount++;
						_employeeEmpty--;
						if (_employeeEmpty <=0)
						{
							Button* mButtonEnter = static_cast<Button*>( Helper::seekWidgetByName(_dialog, "buy")) ;
							mButtonEnter->setTouchEnabled(false);
						}
						UserRecord::getInstance()->alterSweet(-_curOneCostPrice);
						audioPlayer::getInstance()->playEffect("S11.mp3");
						break;
					}
				}

				btn->getChildByTag(759)->setVisible(false);
				btn->getChildByTag(757)->setVisible(true);
				_dialog->retain();
				this->removeChild(_dialog,false);
				//_clickCount= 0;

				_isSelect=NO;

				//此方是解锁新手引导步骤
				if (UserRecord::getInstance()->getIsFreshMan())
					UserRecord::getInstance()->setFreshManMutex(false);
			}
		}
		break;
	case TOUCH_EVENT_CANCELED:
		break;
	default:
		break;
	} 
}
void AltarObject::onExit() 
{ 
	 _eventDispatcher->removeEventListener(_listener);
	 NotificationCenter::getInstance()->removeObserver(this, PLAYATK);
	 Widget::onExit();
} 

void AltarObject::updateValue()
{
	// 科技树，糖果机效率增加
	int _curBaseOutPutValue = PlistManager::getInstance()->getConfigTypeCString("coolie", "output").intValue();
	float _item1_level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item1", 3);
	float _coolieOutPut = _item1_level3;
	_curCoolieOutPut = _curBaseOutPutValue * ( 1 + _coolieOutPut );

	// 科技树，糖果机工位增加
	float _item1_level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item1", 1);
	float _item1_level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item1", 5);
	float _cookieCountValue = _item1_level1 + _item1_level5;
	_curCoolieCount += _cookieCountValue;

	// 科技树，糖果币上限增加
	float _item1_level2 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item1", 2);
	float _item1_level4 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item1", 4);
	float _cookieAddValue = _item1_level2 + _item1_level4;
	int baseMaxSweet = UserRecord::getInstance()->getMaxSweet();
	int _curMaxSweet = baseMaxSweet + _cookieAddValue;
	UserRecord::getInstance()->setMaxSweet(_curMaxSweet);
}

void AltarObject::myPauseAllAction(Ref *obj){
	this->pause();
}
void AltarObject::myResumeAllAction(Ref *obj){
	this->resume();
}


void AltarObject::propsAddProduct(float dt)
{
	//提升产量动画
	_duration = 0;
	_armature->getAnimation()->play("update");

	effectsObject *effect = effectsObject::create();
	effect->setPosition(this->getSize().width/2,this->getSize().height/2-30);
	effect->setTag(12345);
	this->addChild(effect,50);
	effect->addSpeedEffects();

	auto _curBaseOutPutValue = PlistManager::getInstance()->getConfigTypeCString("coolie", "output").intValue();
	_curCoolieOutPut += _curBaseOutPutValue * 2;
	CCLOG("======add=======_curCoolieOutPut=%d",_curCoolieOutPut);
	_isSpeed =true;
	scheduleOnce(schedule_selector(AltarObject::resumeProduct),dt);
}

void AltarObject::resumeProduct(float dt)
{
	if (getChildByTag(12345)){
		getChildByTag(12345)->removeFromParentAndCleanup(true);
	}

	auto _curBaseOutPutValue = PlistManager::getInstance()->getConfigTypeCString("coolie", "output").intValue();
	_curCoolieOutPut -= _curBaseOutPutValue * 2;
	CCLOG("======resume====_curCoolieOutPut=%d",_curCoolieOutPut);
	_isSpeed  = false;
}

Coolie* Coolie::create(CoolieType t)
{
	Coolie* _Coolie = new Coolie();
	if (_Coolie && _Coolie->init(t))
	{
		_Coolie->autorelease();
		return _Coolie;
	}
	CC_SAFE_DELETE(_Coolie);
	return nullptr;
}
Coolie::~Coolie(void)
{
	NotificationCenter::getInstance()->removeObserver(this, PAUSEKEY);
	NotificationCenter::getInstance()->removeObserver(this, RESUMEKEY);
}
bool Coolie::init(const CoolieType t)
{
	if (!Sprite::init())
	{
		return false;
	}
	std::string str = getTypeImage(t);
	if (str.empty())
	{
		return false;
	}
	this->_type = t;
	this->initWithFile(str);
	//this->schedule(schedule_selector(Coolie::logicUpdate),PlistManager::getInstance()->getConfigTypeCString("coolie","cdTime").floatValue());
	//NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Coolie::myPauseAllAction),PAUSEKEY,NULL);
	//NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(Coolie::myResumeAllAction),RESUMEKEY,NULL);
	//
	
	return true;
}



CoolieType Coolie::getType(){
	return _type;
}
void Coolie::setType(CoolieType t)
{
	if (_type != t)
	{
		_type = t;
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/td_candy_lighteffect.plist");
		Vector<SpriteFrame*> aniFrames(12);
		for(int i = 1; i <= 12; i++)
		{
			SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("td_candy_lighteffect_%02d.png",i)->getCString());
			aniFrames.pushBack(frame);
		}
		Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.05f);
		Animate *animate = Animate::create(animation);
		
		auto sp = Sprite::create();
		this->addChild(sp,1);
		this->setLocalZOrder(this->getLocalZOrder()+1);
		sp->setPosition(this->getContentSize().width/2-3,this->getContentSize().height/2);
		auto action = Sequence::create(
			animate,
			CallFunc::create([=](){
				this->removeChild(sp,true);
				Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(getTypeImage(t));
				Rect rect = Rect(0,0,texture->getContentSize().width,texture->getContentSize().height);
				SpriteFrame *frame = SpriteFrame::createWithTexture(texture,rect);
				this->setSpriteFrame(frame);
			}),
			NULL);
		sp->runAction(action);
	}
}
std::string Coolie::getTypeImage(CoolieType t)
{
	std::string str;
	switch(t)
	{
		case Coolie_TYPE_EMPTY:
			str= "image/TangGuoJi_003.png";
			break;
		case Coolie_TYPE_USED:
			str = "image/TangGuoJi_002.png";
			break;
		case Coolie_TYPE_DISABLE:
			str = "image/TangGuoJi_004.png";
			break;
	}
	return str;
}

void Coolie::logicUpdate(float dt)
{
	//加钱
	if (_type == Coolie_TYPE_USED)
	{
		UserRecord* user =  UserRecord::getInstance();
		int sweet = user->getSweet();
		int maxSweet = user->getMaxSweet();
		//CCLOG("%d,%d",sweet,maxSweet);
		if (sweet == maxSweet)
		{
			return;
		}
		__String num = PlistManager::getInstance()->getConfigTypeCString("coolie","output");
		sweet+=num.intValue();
		if (sweet > maxSweet)
		{
			sweet = maxSweet;
		}
		user->setSweet(sweet);
		//NotificationCenter::getInstance()->postNotification(PLAYATK, NULL);
		Window* _curWindow = WindowManager::getInstance()->getWindow("Ui_MainLayerWnd");
		Ui_MainLayer* _mainLayer = static_cast<Ui_MainLayer*>(_curWindow);
		_mainLayer->setSweetNum(user->getSweet(),user->getMaxSweet());
	}
}

