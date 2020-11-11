#pragma once
#include "GameObjects.h"
#include "SceneData.h"
#include "StoreyData.h"
#include "Window.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "effectsObject.h"
#include "PlistManager.h"
#include "headstoneObjects.h"
#include "EventObject_106.h"

USING_NS_CC;

GameObjects::GameObjects(void):
_canAtkFlyObj(false),
_armature(nullptr)
{

}

GameObjects::~GameObjects(void)
{
	
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, PAUSEKEY);
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, RESUMEKEY);
	

}

void GameObjects::myPauseAllAction(Ref *obj){

	
	if(!_children.empty()&&isPlaySkill==NO)
	{
		for(int i=0 ; i < _children.size(); i++ )
		{
			Node*node = _children.at(i);
			node->pause();
		}
	}


	if (_armature!=nullptr)
	{
		_armature->getAnimation()->pause();
		_armature->pause();
	}
 	if (_actionManager!=nullptr)
 	{
 		_actionManager->pauseTarget(this);
 	}
 	if (_scheduler!=nullptr)
 	{
 		_scheduler->pauseTarget(this);
 	}
 	if (_eventDispatcher!=nullptr)
 	{
 		_eventDispatcher->pauseEventListenersForTarget(this);
 	}
 	

	NotificationCenter::sharedNotificationCenter()->postNotification(UInextPAUSEKEY, NULL);


	//������Ϸ��ͣ��ʶ
	UserRecord::getInstance()->setIsGamePause(true);
}
void GameObjects::myResumeAllAction(Ref *obj){

	
	if(!_children.empty()&&isPlaySkill==NO)
	{
		for(int i=0 ; i < _children.size(); i++ )
		{
			Node*node = _children.at(i);
			node->resume();
		}
	}

	if (this->getActionManager()!=nullptr)
	{
		this->getActionManager()->resumeTarget(this);
	}
	if (this->getScheduler()!=nullptr)
	{
		this->getScheduler()->resumeTarget(this);
	}
	if (this->getEventDispatcher()!=nullptr)
	{
		this->getEventDispatcher()->resumeEventListenersForTarget(this);
	}


 	if (_armature!=nullptr)
 	{
 		_armature->getAnimation()->resume();
		_armature->resume();
 	}

	NotificationCenter::sharedNotificationCenter()->postNotification(UInextRESUMEKEY, NULL);

	//������Ϸ��ͣ��ʶ
	UserRecord::getInstance()->setIsGamePause(false);

}

void GameObjects::pauseMyActionsAndSchedulers()
{
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->pause();
	}
	if (_actionManager!=nullptr)
	{
		_actionManager->pauseTarget(this);
	}
	if (_scheduler!=nullptr)
	{
		_scheduler->pauseTarget(this);
	}
	if (_eventDispatcher!=nullptr)
	{
		_eventDispatcher->pauseEventListenersForTarget(this);
	}
}

void GameObjects::resumeMyActionsAndSchedulers()
{
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->resume();
	}
	if (this->getActionManager()!=nullptr)
	{
		this->getActionManager()->resumeTarget(this);
	}
	if (this->getScheduler()!=nullptr)
	{
		this->getScheduler()->resumeTarget(this);
	}
	if (this->getEventDispatcher()!=nullptr)
	{
		this->getEventDispatcher()->resumeEventListenersForTarget(this);
	}
}

bool GameObjects::init()
{
    //////////////////////////////
    // 1. super init first
	if ( !Sprite::init() )
    {
        return false;
    }
	_isSpecialColorFlag=false;
	isStart=NO;//��ʼ����NO
    _CRI=0;//�����ʳ�ʼ��Ϊ0
	_CRI_damage=0.2;//�����˺�
	_isSelect=false;
	_isFly=NO;
	_maxlev=5;
	_isFrozen=NO;
	_isSpeedDown=NO;
	_addHPAddr=0;
	_isAtkTower3Flag=NO;
	criarm=nullptr;
	_isIntimidate=NO;
	_storeydata=nullptr;
	_isBackWind=NO;
	setNextFloorNum(-1);
	setBatch(-1);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameObjects::myPauseAllAction),PAUSEKEY,NULL);
	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(GameObjects::myResumeAllAction),RESUMEKEY,NULL);
    return true;
}

void GameObjects::setobjectType(int t){
	_objectType=t;
}

int GameObjects::getobjectType()
{
	return _objectType;
}
void GameObjects::setSelfType(int tt){
	_selfType=tt;
}
int GameObjects::getSelfType(){
	return _selfType;
}


void GameObjects::setstates(int t){
	_states=t;
}
int GameObjects::getstates(){
	return _states;
}

void GameObjects::setfloorNum(int f){
	_floorNum=f;
}
int GameObjects::getfloorNum(){
	return _floorNum;
}



int GameObjects::gettargetAddr(){
	return targetAddr;
}




void GameObjects::setlocationIndx(int i){

	locationIndx=i;
}
int GameObjects::getlocationIndx(){
	return locationIndx;
}

void GameObjects::setReduceHp(int m_atkOption,int m_atk,float cri,float criDamge){

	
	float hurt=0;
	float tempDef=_dfe;
	if (_objectType==TYPE_WARRIOR)//���������սʿ
	{
		float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS2);//Ӣ��3�ĵڶ����������ܶ�սʿ������Ӱ��
		tempDef+=tempDef*buffValue;
	}

	if (_objectType==TYPE_TOWER)//��������Ƿ�����
	{
		float buffValue=this->findBossSkillBuffValue(BOSS_ITEM5,PASSIVESKILLS1);//Ӣ��5�ĵ�һ���������ܶ�սʿ������Ӱ��
		tempDef+=tempDef*buffValue;
		
	}


	if (m_atkOption==ATK_OPTION1)
	{
		if (_dfeOption==DFE_OPTION1)
		{
			hurt=m_atk-tempDef*m_atk;
		}else{
			hurt=m_atk;
		}

	}else if (m_atkOption==ATK_OPTION2)
	{
		if (_dfeOption==DFE_OPTION2)
		{
			hurt=m_atk-tempDef*m_atk;
		}else{
			hurt=m_atk;
		}
	}
	else
	{
		hurt=m_atk;
	}
	
	
	   int ItCri=cri*100.0;//���������
	
		int ram=CCRANDOM_0_1()*100;
	    if (cri>0&&ram<=ItCri)//�м�����
	    {
			
	 	   hurt+=hurt;
		   this->criEffect();//������Ч
	   }else{
	 	   hurt=hurt;
	    }
	this->reduceHpEffect(hurt);

}
void GameObjects::reduceHpEffect(int t){//��ʾ��Ѫ����ֵ�������ƶ�����͸��
	if (_hp>0)//��ֹ=0ʱ��2�ο�Ѫ�����
	{

	/*audioPlayer::getInstance()->playEffect("S28.mp3");*/
   int hurt=t;

	_hp-=hurt;
	if (_hp<0){
		_hp=0;
	}

	this->changeColorWithAtk();//��ɫ�仯����ʾ�ܹ���
	
	// ��������.....
	if (_hp <= 0)
	{
		//�������������ѡ��״̬����ȡ��UI_ObjectClicked
        NotificationCenter::sharedNotificationCenter()->postNotification(SELECTEDUIREMOVE, this);
		auto _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());
		auto _curFloorNum = this->getfloorNum();
		auto _curPostion = this->getPosition();
		auto _curHeroSeltType = this->getSelfType();
		auto _curHeroCostGold = this->getboss_reviveGold();
		auto _curHeroCostSoul = this->getboss_reviveSoul();
		this->CreateAnimationForDeathObject(Point(0,0));

		if (_objectType==TYPE_ENEMY)
		{
			if (_isAtkTower3Flag==YES&&targetAddr!=0)
			{

				GameObjects *pObj=nullptr;
				for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
				{
					GameObjects *obj=(GameObjects*)sp;
					if ((uintptr_t)obj==targetAddr&&obj->getobjectType()!=TYPE_ENEMY&&obj->gethp()>0)
					{
						pObj=obj;
						break;
					}
				}
				if (pObj!=nullptr&&pObj->getobjectType()==TYPE_TOWER&&pObj->getSelfType()==TOWER_ITEM3)
				{
					pObj->settower3_selectNum(pObj->gettower3_selectNum()-1);
					
				}
			}
		}

		if (_objectType == TYPE_BOSS)
		{
			bossObjects* _curBossObject = static_cast<bossObjects*>(this);
			if (_curBossObject)
			{
				_curBossObject->RemoveSelectFrame();
				//_curBossObject->releaseMoveListener();
				// ������״̬д���ļ�
				UserRecord::getInstance()->setHeroKeyValue(this->getSelfType(), _HeroDataKey_Index_isDead, YES);
				UserRecord::getInstance()->setHeroKeyValue_Float(this->getSelfType(), _HeroDataKey_Index_reviviePosX, _curPostion.x);

				// ����һ��Ĺ��
				GameObjects* _object = static_cast<GameObjects*>(headstoneObjects::create());
				if (_object)
				{
					if (_curStorey && _curStorey->mItem)
					{	
						_object->setTag(55);
						_object->setfloorNum(_curFloorNum);
						_curStorey->mItem->addChild(_object, 8);
						SceneData::getInstance()->addStoreyObject(_curStorey->getUiIndex(), _object);
						_object->setPositionX(_curPostion.x);
 						_object->setPositionY(40);
						auto _curHeadstoneObject = static_cast<headstoneObjects*>(_object);
						_curHeadstoneObject->setHeroSelfType(_curHeroSeltType);
						_curHeadstoneObject->setHeroCostMoney(_curHeroCostGold, _curHeroCostSoul);
					}
				}
			}
			
			this->settargetAddr(0);
			this->MakeEnemyNoAttkMe();
		}
	}

 }



}
void GameObjects::labelForCallback(Ref* pSender)//ɾ������
{
	this->removeChild(static_cast<LabelTTF*>(pSender),true);

}

cocostudio::Armature* GameObjects::getArmature(){

	return _armature;
}
void GameObjects::setisSelect(bool f){
	_isSelect=f;
}
bool GameObjects::getisSelect(){
	return  _isSelect;
}

void  GameObjects::sethp(int h)
{
	_hp=h;
	if (_hp>_maxHp)
	{
		_hp=_maxHp;
	}
}
int GameObjects::gethp(){
	return _hp;
}


void GameObjects::changeColorWithAtk(){
	if (_isSpecialColorFlag==false)
	{
	
	if (_armature!=nullptr)
	{
		_armature->setColor(Color3B(102,102,102));
	}
	
	auto delay=DelayTime::create(0.15);
	auto ddlay = Sequence::create(
		delay,
		CallFuncN::create(CC_CALLBACK_1(GameObjects::changeBackColor,this)),
		NULL);
	this->runAction(ddlay);
	}
}
 
void GameObjects::changeBackColor(Ref *f){
	if (_armature!=nullptr)
	{
		enemyObjects* _curObject = static_cast<enemyObjects*>(this);
		if (_curObject && _curObject->getobjectType() == TYPE_ENEMY && _curObject->getIsEvent106_Object())
		{
			_curObject->setEnemyObjectEvent106();
		}else
		{
			_armature->setColor(Color3B(255,255,255));
		}
	}
}


void GameObjects::setisStart(int b){
	
	isStart=b;
	if (isStart==YES)
	{
		if (_floorNum!=SCENE_STOREY_GROUND_INDEX||_floorNum!=SCENE_STOREY_GROUND)
		{
			_scenedata = SceneData::getInstance();
			if (_scenedata)
			{
				_storeydata  = _scenedata->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
				if (_storeydata)
				{
					_swg=_storeydata->getMovementWidget();
				}
			}



			

			if (_objectType==TYPE_TRAP||_objectType==TYPE_TOWER||_objectType==TYPE_WARRIOR)
			{
				float moveWidthCenterX=_swg->getPositionX();
				float thisPosX=this->getPositionX();
				float leftp=(moveWidthCenterX-_swg->getSize().width/2);
				float rightp=(moveWidthCenterX+_swg->getSize().width/2);
				
				if (thisPosX<(moveWidthCenterX-_swg->getSize().width/2)||thisPosX>(moveWidthCenterX+_swg->getSize().width/2))
				{
					
					Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
					Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
					_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
					Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
					mainLayer->FindObjToNoCd(_objectType,_selfType);
					
				}


				if (_storeydata->getObjectData().size() > 0)
				{
					for (Ref *sp:_storeydata->getObjectData())
					{
						GameObjects *obj=(GameObjects*)sp;

 						if ((uintptr_t)obj!=(uintptr_t)this&&obj->getlocationIndx()==this->getlocationIndx()&&(obj->getobjectType()==TYPE_TRAP||obj->getobjectType()==TYPE_TOWER)
							&&(_objectType==TYPE_TRAP||_objectType==TYPE_TOWER))
							{
							
							Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
							Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
							_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
							Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
							mainLayer->FindObjToNoCd(_objectType,_selfType);

							return;
						}
					}
				}
			}





		}
	}

	
	
		


}
int GameObjects::getisStart(){
	return isStart;
}
Animate* GameObjects::CreateAnimation(const char*FrameName,int max){

	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.1f);
	Animate *animate = CCAnimate::create(animation);

	return animate;

}
//����������ʾ
void GameObjects::CreateAnimationForDeathObject(Point pos){
	
	auto action = Sequence::create(
		FadeOut::create((float)0.2),
		CallFuncN::create( CC_CALLBACK_1(GameObjects::DeathObjectCallBack, this, false)),
		NULL);

	this->_armature->runAction(action);
	if (_selfType==TRAP_IETM3&&_objectType==TYPE_TRAP)
	{
		this->setOpacity(0);
		auto effects = effectsObject::create();
		effects->setPosition(Point(this->getPositionX(),this->getPositionY()));
		effects->Trap03_bomb();
		auto parent = static_cast<Widget*>(this->getParent());
		parent->addChild(effects, 8);
	}
	else if (_selfType==WAR_ITEM4&&_objectType==TYPE_WARRIOR)
	{
		this->setOpacity(0);
		auto effects = effectsObject::create();
		effects->setPosition(Point(this->getPositionX(),this->getPositionY()));
		effects->War04_bomb();
		auto parent = static_cast<Widget*>(this->getParent());
		parent->addChild(effects, 8);

	}else{
		auto effects = effectsObject::create();
		effects->setPosition(Point(this->getPositionX(),this->getPositionY()-10));
		effects->Death(this->getobjectType());
		auto parent = static_cast<Widget*>(this->getParent());
		parent->addChild(effects, 8);
		if (this->getisIntimidate()==YES)//����ǵ��˵Ļ����ܳ��ź���ʾ��������
		{
			effects->setOpacity(0);
		}
	}
	
	//����ֳɹ������ص�λ����
	if (_selfType==ENEMY_ITEM21 && _objectType==TYPE_ENEMY){
		UserRecord::getInstance()->setBoxEnemyDieMethod(1);
	}
}

void GameObjects::DeathObjectCallBack(Ref *psender, bool _isOutDeath){
	int _curBatch = -1;
	if (this->getobjectType() == TYPE_ENEMY)
	{
		// ����������Ч
		audioPlayer::getInstance()->playEffect("S26.mp3");
		// ȡ��ǰ����
		_curBatch = this->getBatch();
		// ȡ��ǰ�����Ƿ���event106 Я��ħ��������С��
		enemyObjects* _enemyObject = static_cast<enemyObjects*>(this);
		if (_enemyObject && !_isOutDeath)
		{
			if (_enemyObject->getIsEvent106_Object())
			{
				// ��106�¼�������� ����һ��ħ����
				EventObject_106* _eventObject = EventObject_106::createEvent();
				if (_eventObject)
				{
					int _curFloor = _enemyObject->getfloorNum();
					auto _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_curFloor);
					if (_curStorey && _curStorey->mItem)
					{		
						_eventObject->setTag(66);
						_eventObject->setfloorNum(_curFloor);
						_curStorey->mItem->addChild(_eventObject, 9);
						SceneData::getInstance()->addStoreyObject(_curStorey->getUiIndex(), _eventObject);

						Widget* _leftDoor = _curStorey->getLeftDoor();
						float _leftDoorX = _leftDoor->getPositionX();
						float _newX = _enemyObject->getPositionX();
						// ����������⣬�������ڴ���
						if (_enemyObject->getPositionX() < _leftDoorX)
						{
							_newX = _leftDoorX + _leftDoor->getSize().width;
						}
						_eventObject->setPositionX(_newX);
						_eventObject->showAction();
						audioPlayer::getInstance()->playEffect("S15.mp3");
					}
				}
			}
		}
	}
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);

	//CCLOG("getRound()=%d,getMaxRound()=%d,isClearAllMonster()=%d",UserRecord::getInstance()->getRound(), UserRecord::getInstance()->getMaxRound(),SceneData::getInstance()->isClearAllMonster());
	if (!UserRecord::getInstance()->getIsFreshMan()){
		if (UserRecord::getInstance()->getRound()== UserRecord::getInstance()->getMaxRound() && SceneData::getInstance()->isClearAllMonster()){

			WindowManager::getInstance()->createGameOverFrame(true);
		}
		// ����Ƿ� event2 �¼���ѿ���
		if (_curBatch >= 0)
		{
			if (SceneData::getInstance()->isFinishBatch(_curBatch))
			{
				// ����Ƿ� event2 �¼���ѿ���
				_curBackGroundWnd->doEvent2(_curBatch);
			}
		}
	}else if (SceneData::getInstance()->isClearAllMonster()){
		UserRecord::getInstance()->setFreshManMutex(false);
	}
}


std::string  GameObjects::getoName(){

	return _oname;
}
std::string GameObjects::getoFileName(){
	return _oFileName;
}




void GameObjects::BackHp(){//3.0���ص� updateBackHp()
	
	if (_objectType!=TYPE_ENEMY)
	{
	  
		auto delay=DelayTime::create(BackHpTime);
		auto ddlay = Sequence::create(
			delay,
			CallFuncN::create(CC_CALLBACK_1(GameObjects::updateBackHp,this)),
			NULL);
		this->runAction(ddlay);

		
	}
	

}
void GameObjects::updateBackHp(Ref*f)//��Ѫ�ġ�������ÿ��ص�һ��isBackHp()
{
	if ((_states==STATES_RUN||_states==STATES_LOAD)&&targetAddr==0)//��������ڴ���״̬�Ļ�
	{   
		this->unschedule(schedule_selector(GameObjects::isBackHp));
		this->schedule(schedule_selector(GameObjects::isBackHp),BackHpTime);
	}
}
void GameObjects::isBackHp(float dt)//�ж��Ƿ����Ѫ��������
{
	if ((_states==STATES_RUN||_states==STATES_LOAD)&&targetAddr==0)
	{  
		if (_hp<_maxHp&&_hp>0)//���û����Ѫ��ʱ��,��ֹ�����˵�Ӣ��Ҳ��Ѫ
		{
			_hp+=SECOND_BACK_BLOOD;//��ս����ʱ��Ļ�Ѫ��
			
			if (_hp>_maxHp)//�����˱��뻻��������Ѫ
			{
				_hp=_maxHp;
				this->unschedule(schedule_selector(GameObjects::isBackHp));//Ѫ��������ͣ
			}
		}

	}else{
		
		this->unschedule(schedule_selector(GameObjects::isBackHp));
		
	}
}



void GameObjects::l_createCAtkLenObject(){//����������Χ�ĵ�Ƭ

	
// 		l_atklenObject1=Sprite::create("atklen.png");
// 		l_atklenObject2=Sprite::create("atklen.png");
// 
// 		float contWidth=l_atklenObject1->getContentSize().width;
// 		float contHeight=l_atklenObject1->getContentSize().width;
// 		l_atklenObject1->setScaleX(_atkLen/contWidth);
// 		l_atklenObject1->setScaleY(_armature->getContentSize().width/contHeight*this->getScale());
// 		l_atklenObject2->setScaleX(_atkLen/contWidth);
// 		l_atklenObject2->setScaleY(_armature->getContentSize().width/contHeight*this->getScale());
// 		
// 		l_atklenObject1->setPosition(-_atkLen/2,_armature->getContentSize().height/5*2);
// 		l_atklenObject2->setPosition(_atkLen/2,_armature->getContentSize().height/5*2);
// 		this->addChild(l_atklenObject1,-3);
// 		this->addChild(l_atklenObject2,-3);
// 		l_atklenObject2->setFlippedX(true);//����
// 		this->l_atklenObjectHide();
	
	
}
void GameObjects::l_atklenObjectHide(){//���ع�����Χ�ĵ�Ƭ

		
// 		l_atklenObject1->setVisible(false);
// 		l_atklenObject2->setVisible(false);
	
}
void GameObjects::l_atklenObjectShow(){//��ʾ������Χ�ĵ�Ƭ
	
// 	l_atklenObject1->setVisible(true);
// 	l_atklenObject2->setVisible(true);
// 	this->l_atklenUpdateSize();

}

void GameObjects::l_atklenUpdateSize(){


//  	float myposx=this->getPositionX();
//  	float w=_swg->getSize().width/2;
//  	float CenterX=_swg->getPositionX();
//  	float letfW=CenterX-w-25;
//  	float rightW=CenterX+w+20;
//  	float tempLen1=_atkLen;
//  	float tempLen2=_atkLen;
//  	if (myposx>(letfW)&&myposx<(letfW+_atkLen))//����������Χ֮�ڵģ����
//  	{
//  		float contWidth=l_atklenObject1->getContentSize().width;
//  		tempLen1=myposx-letfW;
//  		l_atklenObject1->setScaleX((tempLen1)/contWidth);
//  		l_atklenObject1->setPosition(-tempLen1/2,_armature->getContentSize().height/5*2);
//  	}
//  	if (myposx<(rightW)&&myposx>(rightW-_atkLen))//����������Χ֮�ڵģ��ұ�
//  	{
//  		float contWidth=l_atklenObject2->getContentSize().width;
//  		tempLen2=rightW-myposx;
//  
//  		l_atklenObject2->setScaleX((tempLen2)/contWidth);
//  		l_atklenObject2->setPosition(tempLen2/2,_armature->getContentSize().height/5*2);
//  	}

}




// ���µȼ�����
// _level:		�ȼ�
bool GameObjects::updateLevel(int _level)
{
	if (this->getobjectType() == TYPE_BOSS)
	{
		_maxlev = TYPE_BOSS_MAX_LEVEL;
	}else
	{
		_maxlev = TYPE_ENEMY_MAX_LEVEL;
	}
	if (_level > _maxlev)
	{
		return false;
	}
	int _typeIndex = this->getSelfType();	// _typeIndex:	�����͵ı���,����war1,war2,war3		


	PlistManager* _curPlistMgr = PlistManager::getInstance();
	
	// ȡ��������
	__String _curTypeDataName = _curPlistMgr->getTypeDataName(this->getobjectType());
	// ȡ������������
	__String _curTypeIndexName = _curPlistMgr->getTypeName(this->getobjectType(), _typeIndex);

	// *******************
	// Common��������
	// *******************
	_lev = _level;	// level
	_maxHp = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "maxHp").intValue();					// hp
	_hp=_maxHp;//���¸�ֵ��Ѫ��
	

	_maxLevelAtk = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), 40, "atk").intValue();						// base
	_maxLevelHp =_curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), 40, "maxHp").floatValue();					// base

	if (getobjectType()!=TYPE_TOWER && getobjectType()!=TYPE_TRAP){
		int getMoveScaleType= _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "movespeed").intValue();
		_moveSpeedScaleOption=getMoveScaleType;
		if (getMoveScaleType==MOVESPEED_FAST)
		{
			_moveSpeedScale=(float)MOVESPEED_FAST_VALUE;
			_armature->getAnimation()->setAnimationScale(1.2);

		}else if (getMoveScaleType==MOVESPEED_SLOW)
		{
			_moveSpeedScale=(float)MOVESPEED_SLOW_VALUE;
			_armature->getAnimation()->setAnimationScale(0.9);
		}else if (getMoveScaleType==MOVESPEED_MID)
		{
			_moveSpeedScale=(float)MOVESPEED_MID_VALUE;
			_armature->getAnimation()->setAnimationScale(1.05);
		}
		
		if (_objectType!=TYPE_ENEMY)
		{
			float mutileSpeed=CCRANDOM_0_1()*10;
			_moveSpeedScale+=mutileSpeed*0.012;
		}else{
			float mutileSpeed=CCRANDOM_0_1()*10;
			_moveSpeedScale+=mutileSpeed*0.008;
		}
		

	}
	// _atkLen = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atk").floatValue();				// atk len ��˵�Ǿɵ�
	_unlockMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "unlockMoney").intValue();		// unlockMoney
	_callMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "callMoney").intValue();			// callMoney
	_upgradeMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "upgradeMoney").intValue();	// UpgradeMoney
	_atkLenOption = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkLenOption").intValue();	// atkLenOption
	
	if (_atkLenOption==ATKLEN_REMOTE)//Զ�̵�
    {
		_atkLen=LEN_REMOTE;

	} else if (_atkLenOption==ATKLEN_MID)//�г�
	{
		_atkLen=LEN_MID;
		
	}else if (_atkLenOption==ATKLEN_NEAR)//����
	{
		_atkLen=LEN_NEAR;
		if (_objectType==TYPE_TRAP)
		{
			_atkLen=70;
		}
	}else{
		_atkLen=0;
	}
	


	_atkOption = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkOption").intValue();			// _atkOption
	_atk = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atk").intValue();						// atk	
	_atkSpeed = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkSpeed").intValue();			// atkSpeed	
	_dfe = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "dfe").floatValue();					// dfe	
	_dfeOption = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "dfeOption").intValue();			// dfeOption
	_cdTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "cdTime").floatValue();				// CdTime	

	if (_objectType==TYPE_WARRIOR)
	{
// 		CCLOG("MAXHP=%f,unlockMoney=%d,_callMoney=%d,_upgradeMoney=%d,_sellOutMoney=%d,_atkLenOption=%d,_atkOption=%d,_atk=%d,_atkSpeed=%f,_dfe=%f,_cdTime=%f",
// 			_maxHp,_unlockMoney,_callMoney,_upgradeMoney,_sellOutMoney,_atkLenOption,_atkOption,_atk,_atkSpeed,_dfe,_cdTime);
// 			
		
	}
	

	// *******************
	// ��������
	// *******************
	switch (this->getobjectType())
	{
	case TYPE_BOSS:		// Ӣ��
		//_exp = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();					// exp
		//_skillNum = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();				// skillNum
		//_skillCDTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();			// skillCDTime
		//_skillKeepTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();		// skillKeepTime

		_boss_reviveGlod = 200;	//_curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "reviveGold").intValue();		// reviveGold
		_boss_reviveSoul = 8;	//_curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "reviveSoul").intValue();		// reviveSoul
		break;
	case TYPE_ENEMY:	// ����

		break;
	case TYPE_TRAP:		// ����
		_isGround = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();			// isGround
		_sellOutMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "sellOutMoney").intValue();	// SellOutMoney
	   _trap_atkCDTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "trap_CDTime").floatValue();
	   _trap_buffValue = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "trap_buffValue").floatValue();
	   _trap_keepTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "trap_keepTime").floatValue();
	   _trap_targetNum = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "trap_targetNum").floatValue();
	
	 

						// atkTime
		break;
	case TYPE_TOWER:	// ������
		_tower_fly_atk=_curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "tower_fly_atk").intValue();
		_isGround = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "isGround").intValue();			// isGround
		_sellOutMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "sellOutMoney").intValue();	// SellOutMoney
		_tower_atkCDTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "tower_cdTime").floatValue();
		// _tower_isDebuff = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkTime").intValue();		// tower_isDebuff
		// _tower_debuffTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkTime").intValue();		// tower_debuffTime
		// _tower_triggerOdds = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkTime").intValue();	// tower_triggerOdds
		// _tower_triggerTime = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "atkTime").intValue();	// tower_triggerTime
		//_tower_upgradeMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "tower_upgradeMoney").intValue();
		break;
	case TYPE_WARRIOR:	// սʿ
		_sellOutMoney = _curPlistMgr->getAttributeCCString(_curTypeDataName.getCString(), _curTypeIndexName.getCString(), _level, "sellOutMoney").intValue();	// SellOutMoney
		
		break;
	case TYPE_NPC:		// Npc
		break;
	case TYPE_DOOR:		// ��
		break;
	case TYPE_ALTAR:	// ��̳
		break;
	default:
		break;
	}
	return true;
}

void  GameObjects::showLevel()
{
// 	Value lv = Value("lv. ");
// 	auto text = lv.asString().append(Value(getlev()).asString());
// 	auto level =  LabelTTF::create(text,"΢���ź�",18);
// 	this->addChild(level);
// 	level->setPosition(0,this->_armature->getContentSize().height*_armature->getScale());
// 	level->setTag(100002);
	
}
void  GameObjects::hideLevel()
{
// 	auto c = this->getChildByTag(100002);
// 	removeChild(c,true);
}



 void GameObjects::MakeEnemyNoAttkMe(){
	
	 for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
	 {
		 GameObjects *obj=(GameObjects*)sp;
		 if (obj->getobjectType()==TYPE_ENEMY&&obj->gettargetAddr()==(uintptr_t)this)
		 {
		         enemyObjects *enemy=static_cast<enemyObjects *>(obj);
				 enemy->load();
				 break;
		 }
	 }

 }
 void  GameObjects::MakeWarToLoad(){//��սʿ��ͣ��
	 for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
	 {
		 GameObjects *obj=(GameObjects*)sp;
		 if (obj->getobjectType()==TYPE_WARRIOR&&obj->gettargetAddr()==(uintptr_t)this&&obj->getSelfType()!=WAR_ITEM5)
		 {
			 warriorObjects *war=static_cast<warriorObjects *>(obj);
			 war->load();
			 break;
		 }
	 }

 }

 float GameObjects::findBossSkillBuffValue(int bossSelfType,int indexSkill){//Ѱ��Ӣ�ۼ��ܵ�buffֵ

	 float tempValue=0;
	 for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
	 {
		 GameObjects *obj=static_cast<GameObjects*>(sp);
		 if (obj->getobjectType()==TYPE_BOSS&&obj->getSelfType()==bossSelfType)
		 {
		  if (indexSkill==PASSIVESKILLS1)
		  {
			  tempValue=obj->getskill2_buffValue();
		  }
		  if (indexSkill==PASSIVESKILLS2)
		  {
			  tempValue=obj->getskill3_buffValue();
		  }

			 break;
		 }
	 }

	 return tempValue;
  }

 void GameObjects::skilladdHpEffect(int t,int skillLev){
	 ArmatureDataManager::getInstance()->addArmatureFileInfo("td_boss3_addhpEffect.ExportJson");  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
	Armature* addhpArm = cocostudio::Armature::create("td_boss3_addhpEffect");   
	 addhpArm->setPosition(0,0);
	 this->addChild(addhpArm,6);
	 
	 if (skillLev<= bossSkillOne_Level_1)
	 {
		 addhpArm->getAnimation()->play("addhpEeffect1",-1,1);
	 }else if (skillLev> bossSkillOne_Level_1&&skillLev<= bossSkillOne_Level_2)
	 {
		 addhpArm->getAnimation()->play("addhpEeffect2",-1,1);
	 }else if (skillLev> bossSkillOne_Level_2)
	 {
		 addhpArm->getAnimation()->play("addhpEeffect3",-1,1);
	 }

	 auto delay=DelayTime::create(1.0*t);
	 auto ddlay = Sequence::create(
		 delay,
		 CallFuncN::create(CC_CALLBACK_1(GameObjects::skilladdHpCallFunc,this)),
		 NULL);
	 addhpArm->runAction(ddlay);


// 	 int max=13;
// 	 Vector<SpriteFrame*> aniFrames(max);
// 	 char str[100] = {0};
// 	 for(int i = 1; i <= max; i++)
// 	 {
// 		 SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("%s%02d.png","td_boss3_skill_",i)->getCString());
// 		 aniFrames.pushBack(frame);
// 	 }
// 	 Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.07f);
// 	 Animate *animate = CCAnimate::create(animation);
// 
// 	// Animate *animate = this->CreateAnimation("td_boss3_skill_",13);
// 	
// // 	 auto action1 = Sequence::create(animate, NULL);
// // 	  auto repeate=CCRepeat::create(action1,t);
// 	 auto action2 = Sequence::create(
// 		animate,
// 		 CallFuncN::create(CC_CALLBACK_1(GameObjects::skilladdHpCallFunc, this)),
// 		 NULL);
//     Sprite *sp=Sprite::createWithSpriteFrameName("td_boss3_skill_01.png");
// 	sp->setPosition(0,_armature->getContentSize().height/2+10);
// 	this->addChild(sp,2);
// 	sp->runAction(action2);

 }

 void GameObjects::skilladdHpCallFunc(Ref *f){

	 Armature*ops=static_cast<Armature*>(f);
	 ops->removeFromParent();
 }


 //������Ч
 void  GameObjects::criEffect(){

	 if (criarm==nullptr)
	 {
		 ArmatureDataManager::getInstance()->addArmatureFileInfo("baojitexiao0.png","baojitexiao0.plist","baojitexiao.ExportJson");  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
		 Armature *criarm = cocostudio::Armature::create("baojitexiao");   
		 criarm->setPosition(0,-25);
		 criarm->getAnimation()->play("cri",-1,0);
		 criarm->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(GameObjects::onFrameEvent));
		 this->addChild(criarm,6);
	 }
	



 }

 void GameObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
 { 
	 //CCLOG(" enemy (%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	 if (strcmp(evt.c_str(),"finish")==0)//������
	 {
		 criarm->stopAllActions();
		 criarm->removeFromParentAndCleanup(true);
		 criarm=nullptr;

	 }
 }

 void  GameObjects::clickedEffect()
 {
	 this->stopActionByTag(1835);
	 this->setScale(1.00);
     auto act=Sequence::create(ScaleBy::create(0.1f,1.1f),ScaleBy::create(0.05f,1.0/1.1f),NULL);
	 act->setTag(1835);
	 this->runAction(act);

 }

 void  GameObjects::startTint()
 {
	 Action *ac = RepeatForever::create(Sequence::create(TintTo::create(1.5f,131,131,131),TintTo::create(1.0f,255,255,255),NULL));
	 ac->setTag(139139);
	 this->getArmature()->runAction(ac);
 }

 void  GameObjects::stopTint()
 {
	if (UserRecord::getInstance()->getIsGameOver()){
		return;
	} 
	this->getArmature()->stopActionByTag(139139);

	enemyObjects* _curObject = static_cast<enemyObjects*>(this);
	if (_curObject && _curObject->getobjectType() == TYPE_ENEMY && _curObject->getIsEvent106_Object())
	{
		_curObject->setEnemyObjectEvent106();
	}else
	{
		this->getArmature()->setColor(Color3B(255,255,255));
	}
 }






