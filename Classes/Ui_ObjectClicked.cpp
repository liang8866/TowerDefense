#include "Ui_ObjectClicked.h"
#include "SceneData.h"
#include "towerObjects.h"
#include "warriorObjects.h"
#include "UserRecord.h"
#include "headstoneObjects.h"
#include "WindowManager.h"
#include "Ui_HeroAltar.h"
#include "Connect.h"
#include "audioPlayer.h"

using namespace std;
using namespace network;

Ui_ObjectClicked::Ui_ObjectClicked():m_widget(NULL),
mObj(NULL),m_CallFunc(NULL),m_selected_Btn(NULL)
{

}

Ui_ObjectClicked::~Ui_ObjectClicked()
{
	m_selected_Btn = NULL;
	resumeObjStatus();

	NotificationCenter::getInstance()->removeObserver(this,SELECTEDUIREMOVE);
}


bool Ui_ObjectClicked::init()
{
	if (!Widget::init())
		return false;

	mHeroIsGoldReviveMode = false;

	bool Ret = false;
	do{
		m_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_ObjectClicked/Ui_ObjectClicked.ExportJson");
		CC_BREAK_IF(!m_widget);
		m_widget->setPosition(Director::getInstance()->getVisibleOrigin());
		this->addChild(m_widget);
		
		auto _listener = EventListenerTouchOneByOne::create();   //创建一个触摸监听  
		_listener->setSwallowTouches(true);                 //设置是否吞噬触摸 

		_listener->onTouchBegan = CC_CALLBACK_2(Ui_ObjectClicked::onTouchBegan, this);
		_listener->onTouchMoved = CC_CALLBACK_2(Ui_ObjectClicked::onTouchMoved, this);
		_listener->onTouchEnded = CC_CALLBACK_2(Ui_ObjectClicked::onTouchEnded, this);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener,m_widget);
	
		scheduleUpdate();

		//添加广播
		NotificationCenter::getInstance()->addObserver( this,
														callfuncO_selector(Ui_ObjectClicked::removeSelf),
														SELECTEDUIREMOVE,
														NULL);

		Ret=true;
		
	}while (0);

	return Ret;
}

void Ui_ObjectClicked::removeSelf(Ref *obj)
{
	if ( (uintptr_t)obj == (uintptr_t)mObj ){
		mObj = nullptr;
		
		this->removeFromParentAndCleanup(true);
		
	}
}


void Ui_ObjectClicked::clickedSoldier(Ref *obj , Point pos)
{
	do{
		GameObjects *pSender = static_cast<GameObjects*>(obj);

		CC_BREAK_IF(!m_widget);
		Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
		CC_BREAK_IF(!lay);
		pos.y += 120;
		pos.x -= Director::getInstance()->getVisibleOrigin().x;
		lay->setPosition(pos);

		Button* reviveBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Revive"));
		CC_BREAK_IF(!reviveBtn);
		reviveBtn->removeFromParentAndCleanup(true);
		Size visibleSize = Director::getInstance()->getVisibleSize();
		float hscale = visibleSize.height/640;
		lay->setScale(hscale);

		float tHeight=0;
		if (hscale<1.0)
		{
			tHeight=100*(1.0-hscale+0.1);
		}
		this->setPositionY(this->getPositionY()-tHeight);


		Button* upGradeBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Up"));
		CC_BREAK_IF(!upGradeBtn);
		upGradeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::soldierUpGradeBtnCallFuc));

		Button* removeBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Close"));
		CC_BREAK_IF(!removeBtn);
		removeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::soldierRemoveBtnCallFuc));
		removeBtn->setPositionY(removeBtn->getPositionY()-40);
		removeBtn->runAction(Sequence::create(MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));

	
		TextBMFont *upGradePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_up"));
		TextBMFont *sellOutPrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_return"));
		upGradePrice->setText(String::createWithFormat("%d",pSender->getupgradeMoney())->getCString());
		sellOutPrice->setText(String::createWithFormat("%d",pSender->getsellOutMoney())->getCString());


		upGradeBtn->setTouchEnabled(true);

		if (UserRecord::getInstance()->getSweet()<pSender->getupgradeMoney()){
			upGradePrice->setColor(Color3B::RED);
		}

		if (pSender->getlev() >= pSender->getmaxlev())
		{
			upGradeBtn->setTouchEnabled(false);
			upGradeBtn->removeFromParentAndCleanup(true);
			upGradeBtn=NULL;

			Widget *maxBtn = Helper::seekWidgetByName(m_widget,"Button_Max");
			maxBtn->setVisible(true);

			maxBtn->setPositionY(maxBtn->getPositionY()-40);
			maxBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}else{
			upGradeBtn->setPositionY(upGradeBtn->getPositionY()-40);
			upGradeBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}

	}while(0);
}


void Ui_ObjectClicked::clickedTower(Ref *obj ,Point pos)
{
	do{
		GameObjects *pSender = static_cast<GameObjects*>(obj);
		int _level = pSender->getlev();
		int max_level = pSender->getmaxlev();
		CC_BREAK_IF(!m_widget);
		Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
		CC_BREAK_IF(!lay);

		pos.x -= Director::getInstance()->getVisibleOrigin().x;
		pos.y += 150;
		lay->setPosition(pos);
	
		
		Size visibleSize = Director::getInstance()->getVisibleSize();
		float hscale = visibleSize.height/640;
		lay->setScale(hscale);

		 float tHeight=0;
		if (hscale<1.0)
		{
			 tHeight=100*(1.0-hscale+0.1);
		}
	  // CCLOG("hscale=%f",hscale);
		this->setPositionY(this->getPositionY()-tHeight);

		Button* reviveBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Revive"));
		CC_BREAK_IF(!reviveBtn);
		reviveBtn->removeFromParentAndCleanup(true);

		Button* upGradeBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Up"));
		CC_BREAK_IF(!upGradeBtn);
		upGradeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::towerUpGradeBtnCallFuc));

		Button* removeBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Close"));
		CC_BREAK_IF(!removeBtn);
		removeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::towerRemoveBtnCallFuc));
		removeBtn->setPositionY(removeBtn->getPositionY()-40);
		removeBtn->runAction(Sequence::create(MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));

		TextBMFont *upGradePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_up"));
		TextBMFont *sellOutPrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_return"));

		upGradePrice->setText(String::createWithFormat("%d",pSender->getupgradeMoney())->getCString());
		sellOutPrice->setText(String::createWithFormat("%d",pSender->getsellOutMoney())->getCString());
	

		upGradeBtn->setTouchEnabled(true);

		upGradeBtn->setTouchPriority(-9);
		removeBtn ->setTouchPriority(-9);

		if (UserRecord::getInstance()->getSweet()<pSender->getupgradeMoney()){
			upGradePrice->setColor(Color3B::RED);
		}

		if (_level>=max_level){
			upGradeBtn   ->removeFromParentAndCleanup(true);
			upGradeBtn   = NULL;

			Widget *maxBtn = Helper::seekWidgetByName(m_widget,"Button_Max");
			maxBtn->setVisible(true);

			maxBtn->setPositionY(maxBtn->getPositionY()-40);
			maxBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}else{
			upGradeBtn->setPositionY(upGradeBtn->getPositionY()-40);
			upGradeBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}
	
	}while(0);
}


void Ui_ObjectClicked::clickedTrap(Ref *obj, Point pos)
{
	do{
		GameObjects *pSender = static_cast<GameObjects*>(obj);

		CC_BREAK_IF(!m_widget);
		Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
		CC_BREAK_IF(!lay);
		
		pos.x -= Director::getInstance()->getVisibleOrigin().x;

		if (pSender->getSelfType() == TRAP_IETM2)
		{
			pos.y += 100;
		}else if (pSender->getSelfType() == TRAP_IETM3)
		{
			pos.y += 150;
		}else if (pSender->getSelfType() == TRAP_IETM5)
		{
			pos.y += 150;
		}
		else
		{
			pos.y += 100;
		}
		
		lay->setPosition(pos);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		float hscale = visibleSize.height/640;
		lay->setScale(hscale);

		float tHeight=0;
		if (hscale<1.0)
		{
			tHeight=100*(1.0-hscale+0.1);
		}
		this->setPositionY(this->getPositionY()-tHeight);






		Button* reviveBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Revive"));
		CC_BREAK_IF(!reviveBtn);
		reviveBtn->removeFromParentAndCleanup(true);

		Button* upGradeBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Up"));
		CC_BREAK_IF(!upGradeBtn);
		upGradeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::trapUpGradeBtnCallFuc));

		Button* removeBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Close"));
		CC_BREAK_IF(!removeBtn);
		removeBtn->addTouchEventListener(this,toucheventselector(Ui_ObjectClicked::trapRemoveBtnCallFuc ));
		removeBtn->setPositionY(removeBtn->getPositionY()-40);
		removeBtn->runAction(Sequence::create(MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));

		TextBMFont *upGradePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_up"));
		TextBMFont *sellOutPrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_return"));
		upGradePrice->setText(String::createWithFormat("%d",pSender->getupgradeMoney())->getCString());
		sellOutPrice->setText(String::createWithFormat("%d",pSender->getsellOutMoney())->getCString());


		upGradeBtn->setTouchEnabled(true);

		if (UserRecord::getInstance()->getSweet()<pSender->getupgradeMoney()){
			upGradePrice->setColor(Color3B::RED);
		}

		if (pSender->getlev() >= pSender->getmaxlev())
		{
			upGradeBtn->setTouchEnabled(false);
			upGradeBtn->removeFromParentAndCleanup(true);
			upGradeBtn=NULL;

			Widget *maxBtn = Helper::seekWidgetByName(m_widget,"Button_Max");
			maxBtn->setVisible(true);

			maxBtn->setPositionY(maxBtn->getPositionY()-40);
			maxBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}else{
			upGradeBtn->setPositionY(upGradeBtn->getPositionY()-40);
			upGradeBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));
		}

	}while(0);
}


void Ui_ObjectClicked::clickedDeathHero(Ref *obj ,Point pos, int selftype, int costSweet, int costSoul, bool isGoldReviveMode) 
{
	do{
		GameObjects *pSender = static_cast<GameObjects*>(obj);
		int _level = pSender->getlev();
		int max_level = pSender->getmaxlev();
		CC_BREAK_IF(!m_widget);
		Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
		CC_BREAK_IF(!lay);
		
		// 处理iPad 零点偏移量
		pos.x = pos.x - Director::getInstance()->getVisibleOrigin().x;
		pos.y = pos.y + 80;
		lay->setPosition(pos);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		float hscale = visibleSize.height/640;
		lay->setScale(hscale);
		mObj = obj;
		mHeroHeadstoneObject = obj;
		mHeroReviveSweet = costSweet;
		mHeroReviveSoul = costSoul;
		mHeroIsGoldReviveMode = isGoldReviveMode;
		mHeroReviveStoreyIndex = pSender->getfloorNum();
		mHeroReviveSelfType = selftype;
		mHeroRevivePos = pos;

		Button* sellOutBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Close"));
		CC_BREAK_IF(!sellOutBtn);
		sellOutBtn->removeFromParentAndCleanup(true);

		Button* reviveBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Revive"));
		CC_BREAK_IF(!reviveBtn);
		TextBMFont *revivePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_revive"));
		revivePrice->setText(String::createWithFormat("%d", costSweet)->getCString());
		reviveBtn->addTouchEventListener(this, toucheventselector(Ui_ObjectClicked::deathHeroReviveGoldBtnCallFuc));
		reviveBtn->setVisible(true);
		reviveBtn->setTouchEnabled(true);
		Widget* _imageIconGold = reviveBtn->getChildByName("Image_Icon");	// -- 显示图标
		if (_imageIconGold)
		{
			_imageIconGold->setVisible(true);
		}
		if (UserRecord::getInstance()->getSweet() < mHeroReviveSweet)
		{
			reviveBtn->setTouchEnabled(false);
			revivePrice->setColor(Color3B::RED);
		}
		Button* upGradeBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Up"));
		CC_BREAK_IF(!upGradeBtn);
		TextBMFont *upGradePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_up"));
		upGradePrice->setText(String::createWithFormat("%d", costSoul)->getCString());
		upGradeBtn->addTouchEventListener(this, toucheventselector(Ui_ObjectClicked::deathHeroReviveSoulBtnCallFuc));
		upGradeBtn->setVisible(true);
		upGradeBtn->setTouchEnabled(true);
		Widget* _imageSoulGold = upGradeBtn->getChildByName("Image_Icon");	// -- 显示图标
		if (_imageSoulGold)
		{
			_imageSoulGold->setVisible(true);
		}
		if (UserRecord::getInstance()->getHeroSoul() < mHeroReviveSoul)
		{
			upGradeBtn->setTouchEnabled(false);
			upGradePrice->setColor(Color3B::RED);
		}
		upGradeBtn->setTouchPriority(-9);
		reviveBtn ->setTouchPriority(-9);

		if (mHeroIsGoldReviveMode)
		{
			reviveBtn->setGrey();
		}else
		{
			reviveBtn->removeGrey();
		}
		reviveBtn->setPositionY(reviveBtn->getPositionY()-40);
		reviveBtn->runAction(Sequence::create(MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));

		upGradeBtn->setPositionY(upGradeBtn->getPositionY()-40);
		upGradeBtn->runAction(Sequence::create(DelayTime::create(0.02f),MoveBy::create(0.05f,Point(0,50)),MoveBy::create(0.05f,Point(0,-10)),NULL));

	}while(0);
}

void Ui_ObjectClicked::soldierUpGradeBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

	/*	Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(true);*/

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}

	if (mObj){
		warriorObjects *_obj = static_cast<warriorObjects*>(mObj);
		if (UserRecord::getInstance()->getSweet()<_obj->getupgradeMoney())
			return;

		UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()-_obj->getupgradeMoney());

		for (int i = 0; i < SceneData::getInstance()->mSceneDataMap.size(); ++i)
		{
			StoreyData* _thisStorey = SceneData::getInstance()->getSceneDataMapStorey(i);
			Vector<GameObjects*> vec;
			vec.clear();
			vec.pushBack(_thisStorey->findObjByType(_obj->getobjectType(),_obj->getSelfType()));

			if (!vec.empty()){
				for (Vector<GameObjects*>::iterator iter= vec.begin();iter!=vec.end();++iter){
					warriorObjects *curObj = static_cast<warriorObjects*>(*iter);
					
					
					curObj->reSetProperty(curObj->getlev()+1);
					curObj->upgradeObjectAnimation();
					
					UserRecord::getInstance()->setWarSelfTypeLevel(curObj->getSelfType(),curObj->getlev());

				
				}
			}
		}
		audioPlayer::getInstance()->playEffect("S12.mp3");
		NotificationCenter::getInstance()->postNotification(CHANGE_UNIT_PRICE,_obj);
	}
	resumeObjStatus();
	if (UserRecord::getInstance()->getIsFreshMan()){
		UserRecord::getInstance()->setFreshManMutex(false);
	}
}

void Ui_ObjectClicked::soldierRemoveBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);


		Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(false);

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}

	
	if (mObj){
		GameObjects *_obj = static_cast<GameObjects*>(mObj);
		UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()+_obj->getsellOutMoney());

		StoreyData* _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_obj->getfloorNum());
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
			_curStorey->mItem->removeChild(_obj);
		}
		mObj = NULL;
		m_CallFunc = NULL;

		audioPlayer::getInstance()->playEffect("S13.mp3");
	}
	this->removeFromParentAndCleanup(true);
}

void Ui_ObjectClicked::towerUpGradeBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

		Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(false);
		
// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}


	if (mObj){
		towerObjects *_obj = static_cast<towerObjects*>(mObj);
		if (_obj->getlev() <= _obj->getmaxlev()-1){
			if (UserRecord::getInstance()->getSweet()<_obj->getupgradeMoney())
				return;

			UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()-_obj->getupgradeMoney());
			_obj->setlev(_obj->getlev()+1);
			_obj->setProperty();
			_obj->upgradeObjectAnimation(_obj->getlev());

			audioPlayer::getInstance()->playEffect("S12.mp3");

			if (_obj->getlev()<=1&&_obj->getSelfType()==TOWER_ITEM3)
			{
				
				_obj->settower3_selectNumMax(3);
			}
			if (_obj->getlev()==2&&_obj->getSelfType()==TOWER_ITEM3)
			{
				_obj->settower3_selectNumMax(5);
			}
			if (_obj->getlev()==3&&_obj->getSelfType()==TOWER_ITEM3)
			{
				_obj->settower3_selectNumMax(8);
			}

		}
	}
	resumeObjStatus();
}

void Ui_ObjectClicked::towerRemoveBtnCallFuc(Ref *pSender,TouchEventType type)
{

	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

		Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(false);

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}


	if (mObj){
		GameObjects *_obj = static_cast<GameObjects*>(mObj);
		UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()+_obj->getsellOutMoney());

		StoreyData* _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_obj->getfloorNum());
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
			_curStorey->mItem->removeChild(_obj);
		}
		mObj = NULL;
		m_CallFunc = NULL;

		audioPlayer::getInstance()->playEffect("S13.mp3");
	}
	this->removeFromParentAndCleanup(true);
}

void Ui_ObjectClicked::trapUpGradeBtnCallFuc(Ref *pSender,TouchEventType type)
{

	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

		Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(false);

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}


	if (mObj){
		trapObjects *_obj = static_cast<trapObjects*>(mObj);
		 
		if (_obj->getlev() <= _obj->getmaxlev()-1){
			if (UserRecord::getInstance()->getSweet()<_obj->getupgradeMoney())
				return;

			UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()-_obj->getupgradeMoney());
			_obj->updateLevel(_obj->getlev()+1);
			_obj->upgradeObjectAnimation();

			audioPlayer::getInstance()->playEffect("S12.mp3");

		}
	}
	resumeObjStatus();

}

void Ui_ObjectClicked::trapRemoveBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

		Text *_text = static_cast<Text*>(Helper::seekWidgetByName(m_widget,"Text_Desc"));
		_text->setText(m_selected_Btn->getName());
		_text->setVisible(false);

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}

	if (mObj){
		GameObjects *_obj = static_cast<GameObjects*>(mObj);
		UserRecord::getInstance()->setSweet(UserRecord::getInstance()->getSweet()+_obj->getsellOutMoney());

		StoreyData* _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_obj->getfloorNum());
		if (_curStorey)
		{
			_curStorey->removeObject(_obj);
			_curStorey->mItem->removeChild(_obj);
		}
		mObj = NULL;
		m_CallFunc = NULL;

		audioPlayer::getInstance()->playEffect("S13.mp3");
	}
	this->removeFromParentAndCleanup(true);
}

void Ui_ObjectClicked::heroReviveBtnCallFunc(Ref *pSender,TouchEventType type)
{

}

void Ui_ObjectClicked::heroReviveSoonBtnCallFunc(Ref *pSender,TouchEventType type)
{

}

void Ui_ObjectClicked::deathHeroReviveGoldBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED)
		return;
	//CCLOG("click >> deathHeroReviveGoldBtnCallFuc");
	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);
// 
// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}
	// 确认之后开始倒计时
	if (!mHeroHeadstoneObject)
	{
		CCLOG("deathHeroReviveGoldBtnCallFuc >> not Found: mHeroHeadstoneObject");
		return;
	}
	headstoneObjects* _curObject = static_cast<headstoneObjects*>(mHeroHeadstoneObject);
	if (!_curObject)
		return;
	// 消耗金币
	if (UserRecord::getInstance()->getSweet() >= mHeroReviveSweet)
	{
		auto _bossName = StringUtils::format("boss%d", mHeroReviveSelfType);
		// 取等级
		auto _haveData = UserRecord::getInstance()->getHeroData(mHeroReviveSelfType);
		auto _curLevelValue = _haveData.level;
		if(_curLevelValue <= 0)
			_curLevelValue = 0;
		UserRecord::getInstance()->alterSweet(-mHeroReviveSweet);
		// 进入倒计时复活
		_curObject->beginProgressBar(0);
		mObj = nullptr;
		this->removeFromParentAndCleanup(true);
	}else
	{
		Button* reviveBtn  = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Revive"));
		TextBMFont *revivePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_revive"));
		revivePrice->setText(String::createWithFormat("%d", mHeroReviveSweet)->getCString());
		reviveBtn->setTouchEnabled(false);
		revivePrice->setColor(Color3B::RED);
	}
}
void Ui_ObjectClicked::deathHeroReviveSoulBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type != TOUCH_EVENT_ENDED)
		return;
	CCLOG("click >> deathHeroReviveSoulBtnCallFuc");
	if (!m_selected_Btn || m_selected_Btn->getName() != static_cast<Button*>(pSender)->getName() ){
		if (m_selected_Btn){
			ImageView *_has_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
			_has_icon->setVisible(false);

			ImageView *_btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
			_btn_icon->setVisible(true);

			m_selected_Btn->setScale(0.8f);
			m_selected_Btn->runAction(ScaleTo::create(0.05f,1));
		}

		m_selected_Btn = static_cast<Button*>(pSender);
		ImageView *icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("enter_icon"));
		icon->setVisible(true);

		ImageView *btn_icon = static_cast<ImageView*>(m_selected_Btn->getChildByName("btn_icon"));
		btn_icon->setVisible(false);

// 		m_selected_Btn->setScale(0.8f);
// 		m_selected_Btn->runAction(ScaleTo::create(0.05f,1));

		return;
	}

	int _curSoul = UserRecord::getInstance()->getHeroSoul();
	if (mHeroReviveSoul > _curSoul)
		return;
	// 发送成功消息到服务器
	Connect* handle = Connect::getInstance();
	handle->send("c=hero&a=revive", this, httpresponse_selector(Ui_ObjectClicked::onHttpRequestCompleted_ReviveSoul));
}

void Ui_ObjectClicked::deathHeroCancleReviveBtnCallFuc(Ref *pSender,TouchEventType type)
{
	if (type!=TOUCH_EVENT_ENDED)
		return;

	GameObjects* _object = static_cast<GameObjects*>(pSender);
	if (!_object)
		return;
	/*headstoneObjects* _headstoneObject = static_cast<headstoneObjects*>(_object);
	if (!_headstoneObject)
		return;*/
	CCLOG("click >> deathHeroCancleReviveBtnCallFuc");
	//_headstoneObject->objectScheduler();
	this->removeFromParentAndCleanup(true);
}

bool Ui_ObjectClicked::onTouchBegan(Touch *touch, Event *unused_event) 
{
	if (!UserRecord::getInstance()->getIsFreshMan())
		resumeObjStatus();
	return false;
}

void Ui_ObjectClicked::onTouchMoved(Touch *touch, Event *unused_event) 
{

}

void Ui_ObjectClicked::onTouchEnded(Touch *touch, Event *unused_event) 
{

}

void Ui_ObjectClicked::setSelector(Ref* obj,SEL_COMM callfunc)
{
	mObj=obj;
	m_CallFunc = callfunc;
}

void Ui_ObjectClicked::resumeObjStatus()
{
	if (mObj && m_CallFunc)
	{
		(mObj->*m_CallFunc)();
		mObj = NULL;
		m_CallFunc = NULL;
	}
	this->removeFromParentAndCleanup(true);
}

// 服务器回调>>>>	英雄之混复活回调
void Ui_ObjectClicked::onHttpRequestCompleted_ReviveSoul(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		// 取等级
		auto _haveData = UserRecord::getInstance()->getHeroData(mHeroReviveSelfType);
		auto _curLevelValue = _haveData.level;
		if(_curLevelValue <= 0)
			_curLevelValue = 0;

		// 扣除
		UserRecord::getInstance()->alterHeroSoul(-mHeroReviveSoul);
		UserRecord::getInstance()->flush();
		Point _newPos;
		_newPos.x = UserRecord::getInstance()->getHeroKeyValue_Float(mHeroReviveSelfType, _HeroDataKey_Index_reviviePosX);
		_newPos.y = 0;
		WindowManager::getInstance()->createHeroToHeroSelect(mHeroReviveStoreyIndex, mHeroReviveSelfType, _newPos, _curLevelValue);

		if (mObj){
			GameObjects *_obj = static_cast<GameObjects*>(mObj);
			// 改变复活状态
			UserRecord::getInstance()->setHeroKeyValue(mHeroReviveSelfType, _HeroDataKey_Index_isDead, NO);
			UserRecord::getInstance()->setHeroKeyValue(mHeroReviveSelfType, _HeroDataKey_Index_revivieTime, NO);

			StoreyData* _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_obj->getfloorNum());
			if (_curStorey)
			{
				_curStorey->removeObject(_obj);
				_curStorey->mItem->removeChild(_obj);
			}
			mObj = NULL;
			m_CallFunc = NULL;
			mHeroHeadstoneObject = nullptr;
		}
		this->removeFromParentAndCleanup(true);
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so ReviveSoul Failure!!!");
	}
}

void Ui_ObjectClicked::update(float fDelta)
{
	do{
		CC_BREAK_IF(!mObj);
		auto _curObject = static_cast<GameObjects*>(mObj);
		Point objPos;
		if (_curObject->getobjectType() == TYPE_HEADSTONE)
		{
			objPos = _curObject->convertToWorldSpaceAR(Point::ZERO);
			CC_BREAK_IF(!m_widget);
			Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
			CC_BREAK_IF(!lay);
			Point _newPos;
			_newPos.x = objPos.x - Director::getInstance()->getVisibleOrigin().x;
			_newPos.y = objPos.y + 80;
			lay->setPosition(_newPos);

			TextBMFont *revivePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_revive"));
			if (revivePrice && revivePrice->getColor()==Color3B::RED)
			{
				int _curSweet = UserRecord::getInstance()->getSweet();
				if (_curSweet >= mHeroReviveSweet)
				{
					revivePrice->setColor(Color3B(92,31,40));
				}
			}
		}else
		{
			auto _gameObj = _curObject->getArmature();
			CC_BREAK_IF(!_gameObj);
			objPos  = _gameObj->convertToWorldSpace(_gameObj->getPosition());
			CC_BREAK_IF(!m_widget);
			Widget *lay = Helper::seekWidgetByName(m_widget,"ImageView_ClickBack");
			CC_BREAK_IF(!lay);
			if (_curObject->getobjectType() == TYPE_WARRIOR)
			{
				objPos.y += 120;
			}else if (_curObject->getobjectType() == TYPE_TOWER)
			{
				objPos.y += 130;
			}else if (_curObject->getobjectType() == TYPE_TRAP)
			{
				if (_curObject->getSelfType() == TRAP_IETM2)
				{
					objPos.y += 100;
				}else if (_curObject->getSelfType() == TRAP_IETM3)
				{
					objPos.y += 150;
				}else if (_curObject->getSelfType() == TRAP_IETM5)
				{
					objPos.y += 150;
				}
				else
				{
					objPos.y += 100;
				}
			}else
			{
				objPos.y += 100;
			}
			objPos.x -= Director::getInstance()->getVisibleOrigin().x;
			objPos.y -= Director::getInstance()->getVisibleOrigin().y;
			lay->setPosition(objPos);
			Button* upGradeBtn = static_cast<Button*>(Helper::seekWidgetByName(m_widget,"Button_Up"));
			TextBMFont *upGradePrice = static_cast<TextBMFont*>(Helper::seekWidgetByName(m_widget,"price_up"));
			if (upGradePrice && upGradePrice->getColor()==Color3B::RED && UserRecord::getInstance()->getSweet() >= _curObject->getupgradeMoney()){
				upGradePrice->setColor(Color3B(92,31,40));
				upGradeBtn->removeGrey();
			}
			if (UserRecord::getInstance()->getSweet() < _curObject->getupgradeMoney())
			{
				if (!upGradeBtn->_isgrey)
				{
					upGradeBtn->setGrey();
					ImageView* ImageView_676_0 = static_cast<ImageView*>(Helper::seekWidgetByName(upGradeBtn,"ImageView_676_0"));
					ImageView_676_0->removeGrey();
				}
			}

		}

		
	}while (0);
}


