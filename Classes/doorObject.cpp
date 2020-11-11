#pragma once
#include "doorObject.h"
#include "WindowManager.h"

doorObject* doorObject::create(DoorType doorType)
{
	doorObject* pRet = new doorObject();
	if (pRet && pRet->init(doorType)){
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

bool doorObject::init(DoorType doorType)
{
	string file;
	if (doorType==InsideDoor){
		file = "image/Men.png";
	}else if (doorType==OutDoor){
		file = "image/Men_a.png";
	}else{
		CCAssert(0,"doorType can't be null");
	}

	this->initWithFile(file);
	this->getTexture()->setAliasTexParameters();

	_objectType = TYPE_DOOR;
	_selfType = doorType;
	CurrentAction = nullptr;

	this->setProperty();

	return true;
}

void doorObject::reduceHpEffect(int t)
{
	
	UserRecord* user =  UserRecord::getInstance();
	Window* _curWindow = WindowManager::getInstance()->getWindow("Ui_MainLayerWnd");
	Ui_MainLayer* _mainLayer = static_cast<Ui_MainLayer*>(_curWindow);

	bool isFresh= UserRecord::getInstance()->getIsFreshMan();
	if (isFresh==true)
	{
		user->setHP(user->getHP());
	}else{
		 
		int rNum=1;
		if (t==ENEMY_ITEM4||t==ENEMY_ITEM8||t==ENEMY_ITEM12||t==ENEMY_ITEM16||t==ENEMY_ITEM20)
		{
			rNum=6;
		}
		user->setHP(user->getHP()-rNum);
	}
	
	if (user->getHP() <= 0 )
	{
		user->setHP(0);
		WindowManager::getInstance()->createGameOverFrame(false);
		return;
	} 
	_mainLayer->setLifeNum(user->getHP());
	
	_mainLayer->SetAtkDoorWarning();//打门的时候的警报红色

	/*if (CurrentAction == nullptr)
	{
		auto scaleby = ScaleBy::create(0.1, 0.9);
		auto actionbyback = scaleby->reverse();
		CurrentAction =  this->runAction(Sequence::create(
			scaleby,
			actionbyback,
			CallFuncN::create(this, callfuncN_selector(doorObject::ActionDone)),
			NULL));
	} 
	 _hp-=t;
	 if (this->_hp <= 0)
	 {
		 auto gamelayer =  WindowManager::getInstance()->getGameLayer();
		 LabelTTF* doorboold = dynamic_cast<LabelTTF*>(gamelayer->getChildByTag(1314));
		 doorboold->setString(String::createWithFormat("%03d/300",0)->getCString());
		 CCLOG("GAME OVER!");

		 
	 } else 
	 {
		 auto gamelayer =  WindowManager::getInstance()->getGameLayer();
		 LabelTTF* doorboold = dynamic_cast<LabelTTF*>(gamelayer->getChildByTag(1314));
		 doorboold->setString(String::createWithFormat("%03d/300",(int)_hp)->getCString());
	 }
*/
	 
}

void doorObject::setProperty() 
{

	this->_maxHp = 20;
	auto user = UserRecord::getInstance();
	this->_hp = user->getHP();
	this->_atk = 00;
	this->_dfe = 0;
	this->_lev = 1;//等级
	this->_atkLen = 500;//攻击距离
	this->isStart = YES;
}

void doorObject::ActionDone(Node* psend)
{
	CurrentAction = nullptr;
}