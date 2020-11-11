#include "towerObjects.h"
#include "BulletObjects.h"
#include "SceneData.h"
#include "UserRecord.h"
USING_NS_CC;



towerObjects::towerObjects(void):
attak_fly(noAttack),
paodanTime(0),
checkHpTime(0)
{
	_canAtkFlyObj = false;
	_fly_target =nullptr;
}

towerObjects::~towerObjects(void)
{
	_armature=nullptr;
	_states  = STATES_DEFAULT;
	RemoveListenerTouch();
}

void towerObjects::onExit()
{
	Node::onExit();
	this->unscheduleAllSelectors();
}

towerObjects* towerObjects::create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
	towerObjects *sprite = new towerObjects();
	if (sprite && sprite->init())
	{
		sprite-> towerObjectsInit(filename,name,otype,_l);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool towerObjects:: towerObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
	_objectType= TYPE_TOWER;
	_selfType  = otype;
	_target    = nullptr;
	_states    = STATES_LOAD;
	_oname     = name;
	_oFileName = filename;
	isHalf     = false;
	loadNum    = 1;
	_tower3_selectNum=0;

	_boss2_skill3_of_Tower=0.0;
	this->_maxHp = 100;
	this->_hp = 100;
	this->_atk = 40;
	this->_dfe = 1;
	this->_lev = 1;//等级
	this->_atkLen = 200;//攻击距离
	this->_gap = 1; //攻击间隔
	this->_floorNum = 1;

	if (_lev<=1&&_selfType==TOWER_ITEM3)
	{
		_tower3_selectNumMax=3;
	}

	setProperty();
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);
	_armature = Armature::create(name);
	if (_selfType == TOWER_ITEM1 || _selfType == TOWER_ITEM2 || _selfType == TOWER_ITEM5){
		_armature->getAnimation()->play("load");
		if (_selfType ==TOWER_ITEM1 ){
			_canAtkFlyObj = true;
		}
		
	}else if (_selfType == TOWER_ITEM3)
	{
		_armature->getAnimation()->play("load1");
		_states = STATES_LOCK;

	}else if (_selfType == TOWER_ITEM4){
		_armature->getAnimation()->play("load1");

	}



// 已经在导出骨骼动画的时候做了缩放处理
// 	if (_selfType != TOWER_ITEM5 )
// 		_armature->setScale(0.85f);

	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(towerObjects::onFrameEvent));
	this->addChild(_armature,1);
	this->schedule(schedule_selector(towerObjects::logicUpdate),0.1);
	this->CreateListenerTouch_Sprite();
	this->l_createCAtkLenObject();//创建透明底片攻击范围显示框
	this->createDaodan();
	return true;
}

void towerObjects::upgradeObjectAnimation(int level){//升级的动画播放
	
	if (level>getmaxlev()){
	
		return;
	}
	this->dragObjectSmoke();

	if (_selfType==TOWER_ITEM4){
		if (_states == STATES_LOAD){
			_armature->getAnimation()->play(StringUtils::format("load%d",level));
		}else if (_states == STATES_LOCK){
			_armature->getAnimation()->play(StringUtils::format("cd%d",level));
		}else if (_states == STATES_FIGHTING){
			_armature->getAnimation()->play(StringUtils::format("load%d",level));
		}
		return;
	}
	
	size_t _pos = _oname.find_last_of("_");
	if (strcmp(_oname.substr(_pos+1).c_str(),"a")==0){
		_states    = this->_states;
		_oname     = _oname.substr(0,_pos+1)+"b";
		_oFileName = _oname + ".ExportJson";
	}else if(strcmp(_oname.substr(_pos+1).c_str(),"b")==0){
		_states    = this->_states;
		_oname     = _oname.substr(0,_pos+1)+"c";
		_oFileName = _oname + ".ExportJson";
	}else{
		return;
	}

	isHalf     = false;
	loadNum    = 1;

	this->unschedule(schedule_selector(towerObjects::logicUpdate));
	_armature->removeFromParentAndCleanup(true);
	_armature=nullptr;

	ArmatureDataManager::getInstance()->addArmatureFileInfo(_oFileName);
	_armature = Armature::create(_oname);
	if (_selfType==1){
		if (this->_states == STATES_LOAD){
			_armature->getAnimation()->play("load");
		}else if (this->_states == STATES_FIGHTING){
			if (attak_fly == floorAttack || attak_fly == floorAndSkyAttack )
			{
					Firing();
					
			}

		}else if (this->_states == STATES_READY){
			_armature->getAnimation()->play("ready",-1,0);
		}
	}else if (_selfType==2){
		if (this->_states == STATES_LOCK){
			_armature->getAnimation()->play("end",-1,0);
		}else{
			_states = STATES_LOAD;
			_armature->getAnimation()->play("load");
		}
	}else if (_selfType==3){
		_states = STATES_LOCK;
	}else if (_selfType==5){
		if (this->_states == STATES_LOAD){
			_armature->getAnimation()->play("load");
		}else if (this->_states == STATES_FIGHTING){
			Firing();
			
		}else if (this->_states == STATES_READY){
			_armature->getAnimation()->play("ready",-1,0);
		}
	}


// 已经在导出骨骼动画的时候做了缩放处理
// 	if (_selfType != TOWER_ITEM5 )
// 		_armature->setScale(0.85f);

	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(towerObjects::onFrameEvent));
	this->addChild(_armature);

	this->createDaodan();
	this->schedule(schedule_selector(towerObjects::logicUpdate),0.1f);

	
}

void towerObjects::dragObjectSmoke(){
	ArmatureDataManager::getInstance()->addArmatureFileInfo("dragObjectSmoke0.png","dragObjectSmoke0.plist","dragObjectSmoke.ExportJson");
	dragsmokeArm = Armature::create("dragObjectSmoke");
	dragsmokeArm->setPosition(0,20);
	this->addChild(dragsmokeArm,2);
	dragsmokeArm->getAnimation()->play("Animation2",-1,0);
	dragsmokeArm->getAnimation()->setFrameEventCallFunc(CC_CALLBACK_0(towerObjects::onFrameEvent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
}

void towerObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	//CCLOG("=======================================tower = %s",evt.c_str());
	if (strcmp(evt.c_str(),"smokeFinish")==0)
	{
		dragsmokeArm->getAnimation()->stop();
		dragsmokeArm->removeAllChildrenWithCleanup(true);

	}else if(strcmp(evt.c_str(),"reducehp")==0)
	{
		GameObjects *obj=_target;
		if (obj!=nullptr)//
		{
			if (obj->gethp()>0)
			{
				obj->setReduceHp(_atkOption,_atk+_atk*_boss2_skill3_of_Tower,_CRI,_CRI_damage);
			}
			if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAttack ){
				Firing();
				
			}else if ( _states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAndSkyAttack ){
				Firing();
			}
		}else{
			_target=nullptr;
			if (this->_states==STATES_FIGHTING){
				attak_fly = TowerAttackType::noAttack;
				this->_states = STATES_READY;
				scheduleOnce(schedule_selector(towerObjects::resumeStatus),5.0f);
			}
		}
					
	}else if(strcmp(evt.c_str(),"ready_finish")==0){
		if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAttack ){
			Firing();
		
		}else if ( _states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAndSkyAttack ){
			Firing();
			
		}
		this->createDaodan();
	}else if(strcmp(evt.c_str(),"end_finish")==0){
		if (_states == STATES_LOAD){
			load();
		}
	}else if(strcmp(evt.c_str(),"arrow_finish")==0){

	}else if (strcmp(evt.c_str(),"atk_finished")==0){
	
	}
}


//触摸事件
void towerObjects::CreateListenerTouch_Sprite()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(towerObjects::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(towerObjects::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(towerObjects::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_WARRIOR_TOWER); 
}
//这个是删除触摸事件的
void  towerObjects::RemoveListenerTouch()
{
	if (listener){
		_eventDispatcher->removeEventListener(listener);  
		CC_SAFE_RELEASE(listener);
		listener = nullptr;
	}
}

bool towerObjects::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{

	bool isCliced = SceneData::getInstance()->isExistObjSelect();


	if (isStart==YES && isCliced==false&&_hp>0)
	{
		Point _touchPos		= _touch->getLocation();
		Size _curSize		= this->getArmature()->getContentSize()*_armature->getScale();
		Point _touchNodePos = this->convertToNodeSpace(_touchPos);
		Point _worldPoint   = Director::getInstance()->getRunningScene()->convertToWorldSpace(_touchPos);
		Point _originPoint  = Director::getInstance()->getVisibleOrigin();

		auto _distance = abs(_worldPoint.y - _originPoint.y);

		Point _curPos;
		float reduceX=0.0;
		
		

		_curPos.x = 0 - _curSize.width/2;
		_curPos.y = 0;
		Rect _curRect;
		if (_selfType==TOWER_ITEM1)
		{
			 _curRect = Rect(_curPos.x+10, _curPos.y, _curSize.width-10, _curSize.height);
		}else{
			 _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height);
		}
	

		Rect dd = this->getArmature()->getBoundingBox();
			// 点中
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight() )
		{
			clickedEffect();
			
			
			if (WindowManager::getInstance()->mGameLayer->getChildByTag(250)){
				WindowManager::getInstance()->mGameLayer->removeChildByTag(250);
			}
			
			Ui_ObjectClicked *objClickedUI = Ui_ObjectClicked::create();
			objClickedUI->setTag(250);
			WindowManager::getInstance()->mGameLayer->addChild(objClickedUI,50);
			objClickedUI->clickedTower(this,convertToWorldSpace(getArmature()->getPosition()));
			objClickedUI->setSelector(this,comm_selector(towerObjects::resumeObjScheduler));
			//this->l_atklenObjectShow();
			this->startTint();
			//showLevel();
			_isSelect=true;
			return true;
		}
	}
	return false;
} 

void  towerObjects::onTouchMoved(Touch *touch, Event *unused_event)  
{  
	 
}  

void  towerObjects::onTouchEnded(Touch *touch, Event *unused_event)  
{  
	
}

void towerObjects::logicUpdate(float dt) 
{
	if (isStart==YES)
	{

	_target = this->getClosesTarget();
	
	paodanTime++;
	checkHpTime++;
	
	if (!_armature)
		return;

	//检测血量
	if (checkHpTime>=20){
		checkHpTime=0;
		checkHp();
	}

	if (_states == STATES_LOCK)
		return;
	
	//对陆
	if (_target != nullptr)
	{
		this->_currentTargetPoint = _target->getPosition();
		if (_states == STATES_LOAD){
			
			this->_states = STATES_FIGHTING;
			attak_fly = TowerAttackType::floorAttack;
			if (_selfType!=TOWER_ITEM4)
				_armature->getAnimation()->play("ready",-1,0);
			else{
				Firing();
			}
		}else if (_states == STATES_READY){
			this->_states = STATES_FIGHTING;
			attak_fly = TowerAttackType::floorAttack;
			this->createDaodan();
			Firing();
		}else if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::skyAttack){
			attak_fly = TowerAttackType::floorAndSkyAttack;
			Firing();
		}
	} 

	//对空
	if (paodanTime>=10 && _lev>=3){
		paodanTime=0;
		_fly_target = getFlyEnemy();
		if (_canAtkFlyObj){
	
			if (_fly_target && _fly_target->_hp>0){
				this->_currFlyTargetPoint = _fly_target->getPosition();
				if (_states == STATES_LOAD){
					this->_states = STATES_FIGHTING;
					attak_fly = TowerAttackType::skyAttack;
					_armature->getAnimation()->play("ready",-1,0);
				}else if (_states == STATES_READY){
					this->_states = STATES_FIGHTING;
					this->createDaodan();
					attak_fly = TowerAttackType::skyAttack;
				}else if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAttack){
					attak_fly = TowerAttackType::floorAndSkyAttack;
				}else if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::skyAttack){
					FlyFiring();
				}else if (_states == STATES_FIGHTING && attak_fly == TowerAttackType::floorAndSkyAttack){
					FlyFiring();
				}
			}
		}
		
		checkDirection();
	
	}
	
	if ( _target == nullptr &&  _fly_target == nullptr ) {
		if (_states == STATES_LOAD || _states == STATES_READY || _selfType==TOWER_ITEM4 || _selfType==TOWER_ITEM5 )
			return;
		
		attak_fly = TowerAttackType::noAttack;
		this->_states = STATES_READY;
		scheduleOnce(schedule_selector(towerObjects::resumeStatus),5.0f);
	}else if ( _target == nullptr ){
		if (attak_fly == TowerAttackType::floorAndSkyAttack){
			attak_fly = TowerAttackType::skyAttack;
		}else {
			attak_fly = TowerAttackType::skyAttack;
		}
	}else if ( _fly_target == nullptr ){
		if (attak_fly == TowerAttackType::floorAndSkyAttack){
			attak_fly = TowerAttackType::floorAttack;
		}else{
			attak_fly = TowerAttackType::floorAttack;
		}
	}


	}


}

void towerObjects::resumeStatus(float dt)
{

	if (this->_states != STATES_READY)
		return;

	if (!_armature)
		return;

	this->_states = STATES_LOAD;

	if (_selfType==1)  //塔1，恢复动作
	{
		_armature->getAnimation()->play("end",-1,0);
		this->createDaodan();
	}
}

void towerObjects::attackFlyObj()
{

}

GameObjects* towerObjects::getClosesTarget()
{
	
	auto sdata = SceneData::getInstance();
	StoreyData* Storey  = sdata->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
	if (Storey == nullptr )
	{
		return nullptr;
	}

	if (_target != nullptr)
	{
		//判断对象是否还存在
		for(GameObjects* obj:Storey->mObjectData){

			if (obj == _target && _target->_hp>0 && _target->getisFly() == NO)
			{
				//避免两个肉盾，吸引同一个敌人
				if (_selfType==TOWER_ITEM3 && obj->gettargetAddr()!=(uintptr_t)this)
					continue;

				   //防止逃跑的时候还打
					if (_target->getisIntimidate()==YES) {
						_target = nullptr;
						targetAddr = 0;
						return _target;
					}
		
					//防止被电风扇吹的时候还打
					if (_target->getisBackWind()==YES) {
						_target = nullptr;
						targetAddr = 0;
						return _target;
					}

				this->targetAddr= (uintptr_t) _target;
				return _target;
					 
			}
		}
		_target = nullptr;
		targetAddr =0;
	}
	
	
	int currentTarget = 0;
	GameObjects *targetObj=nullptr;
	this->targetAddr =0;

	float moveWidthCenterX=_swg->getPositionX();
	float thisPosX=this->getPositionX();
	float sWidth=_swg->getSize().width;
	for(GameObjects* obj:Storey->mObjectData){
		if (_swg==nullptr)
			return nullptr;

		float dist = fabs( thisPosX- obj->getPositionX());
		if (obj->getobjectType()==TYPE_ENEMY  && obj->gethp() > 0 && obj->getisFly() == NO&&obj->getisIntimidate()==NO&&obj->getisBackWind()==NO&&
			obj->getPositionX()<(moveWidthCenterX+sWidth/2)&&obj->getPositionX()>(moveWidthCenterX-sWidth/2) )
		{

			//累计防御塔胖子被人打了的个数
			if (_selfType==TOWER_ITEM3&&obj->gettargetAddr()==(uintptr_t)(this)&&obj->getstates()==STATES_FIGHTING&&obj->getisAtkTower3Flag()==NO)
			{ 
				if (obj->gethp()>0)
				{
					_tower3_selectNum++;
					obj->setisAtkTower3Flag(YES);
				}
				
			}
			if (_selfType==TOWER_ITEM1&&obj->getPositionX()<120)
			{
				continue;
			}
			
			float dist = fabs( this->getPositionX() - obj->getPositionX() );
			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
							 obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale() + LEN_SCALE;

			if (dist < safeZone || dist-safeZone > this->getatkLen()) {
				continue;
			}

			//tower1,1级只能对前
			if ( _selfType==TOWER_ITEM1 && _lev==1 && this->getPositionX()<obj->getPositionX() ){
				continue;
			}

			//tower4,只能对前
			if ( _selfType==TOWER_ITEM4 && this->getPositionX()<obj->getPositionX() ){
				continue;
			}

			//tower5,只能对前
			if ( _selfType==TOWER_ITEM5 && this->getPositionX()<obj->getPositionX() ){
				continue;
			}


			if (currentTarget == 0)
			{
				currentTarget = dist-safeZone;
				targetObj=obj;
			} else
			{
				if (currentTarget > dist-safeZone) {
						targetObj=obj;
						currentTarget = dist-safeZone;
				}
			}
		}
	}
	if (targetObj == nullptr)
	{
		this->targetAddr=0;
	} else {
		this->targetAddr= (uintptr_t) targetObj;
	}

	return targetObj;
}

GameObjects *towerObjects::getFlyEnemy()
{
	StoreyData* Storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
	if (Storey == nullptr )
	{
		return nullptr;
	}

	if (_fly_target != nullptr)
	{
		bool flag = false;
		//判断对象是否还存在
		for(GameObjects* obj:Storey->mObjectData){
			if (obj == _fly_target && _fly_target->getisFly() == YES )
			{
				return _fly_target;
			}
		}
		_fly_target = nullptr;
		fly_target_addr =0;
	}


	int currentFlyTarget = 0;
	GameObjects *flyTargetObj=nullptr;
	this->fly_target_addr =0;
	for(GameObjects* obj:Storey->mObjectData){
		if (obj->getobjectType()==TYPE_ENEMY  && obj->gethp() > 0 && obj->getisFly() == YES )
		{

			float dist = abs(obj->getPositionX()-this->getPositionX());
			if (dist > this->getatkLen()) {
				continue;
			}
			if (currentFlyTarget == 0)
			{
				currentFlyTarget = dist;
				flyTargetObj=obj;
			} else
			{
				if (currentFlyTarget > dist) {
					flyTargetObj=obj;
					currentFlyTarget = dist;
				}
			}
		}
	}
	if (flyTargetObj == nullptr)
	{
		this->fly_target_addr=0;
	} else {
		this->fly_target_addr= (uintptr_t) flyTargetObj;
	}

	return flyTargetObj;
}

//Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex
void towerObjects::Firing(){
	
	switch (_selfType)
	{
	case 1:
		tower1atkEffect();
		break;
	case 2:
		tower2atkEffect();
		break;
	case 3:

		break;
	case 4:
		_states = STATES_FIGHTING;
		scheduleOnce(schedule_selector(towerObjects::tower4atkEffect),TOWER_ITEM4_ATK_DELAY);
		break;
	case 5:
		tower5atkEffect();
		break;
	default:
		break;
	}
}

void towerObjects::FlyFiring()
{
	if (_canAtkFlyObj){

		GameObjects *obj=FindObject(fly_target_addr);
		if (obj!=nullptr)//
		{
			if (obj->gethp()>0)
			{
				
				obj->setReduceHp(_atkOption,_tower_fly_atk*(1+_boss2_skill3_of_Tower),_CRI,_CRI_damage);
			}
		}

		//攻击音效
		audioPlayer::getInstance()->playEffect("S38.mp3");

		string file_name;
		string arma_name;
		if (_lev==1)
		{
			file_name = "td_tower_01_a_daodan.ExportJson";
			arma_name = "td_tower_01_a_daodan";
		}else if (_lev==2){
			file_name = "td_tower_01_b_daodan.ExportJson";
			arma_name = "td_tower_01_b_daodan";
		}else if (_lev==3){
			file_name = "td_tower_01_c_daodan.ExportJson";
			arma_name = "td_tower_01_c_daodan";
		}

		ArmatureDataManager::getInstance()->addArmatureFileInfo(file_name.c_str()); 
		Armature *daodanArmature = cocostudio::Armature::create(arma_name.c_str());
		Point pos = Point(this->getPositionX(),this->getPositionY()+this->getArmature()->getContentSize().height/2);
		daodanArmature->setPosition(pos);
		this->getParent()->addChild(daodanArmature,9);
		daodanArmature->getAnimation()->play("play",-1,1);

		float dis = ccpDistance(this->getPosition(),_currFlyTargetPoint);

		float angel=0;
		if (_currFlyTargetPoint.x<pos.x){
			if (_fly_target->_states == STATES_RUN &&  _currFlyTargetPoint.x + 50 < pos.x)
				_currFlyTargetPoint.x += 50;
			angel = 360-(abs(atan((pos.x-_currFlyTargetPoint.x)/(pos.y-_currFlyTargetPoint.y))*180/3.1415927)) ;
		}else if (_currFlyTargetPoint.x>pos.x){
			angel = abs(atan((pos.x-_currFlyTargetPoint.x)/(pos.y-_currFlyTargetPoint.y))*180/3.1415927) ;
		}
		daodanArmature->setRotation(angel);
		daodanArmature->runAction(Sequence::create(MoveTo::create(dis/600,_currFlyTargetPoint),
												   CallFuncN::create(CC_CALLBACK_1(towerObjects::daodanEffectCallBack,this)),
												   NULL
												 ));
	}
}

void towerObjects::load()
{
	if (_armature){
		if (_selfType==1){
			_armature->getAnimation()->play("load",-1,1);
		}else if (_selfType==3){
				if (loadNum == 1)
					loadNum = 2;
				else 
					loadNum = 1;

				_armature->getAnimation()->play(StringUtils::format("load%d",loadNum),-1,0);
			
		}
	}
}

void towerObjects::tower1atkEffect()
{
	if (!_target)
		return;

	Point  origin; 
	Point  endPoint;
	Point  anchor;
	string direct;
	Point  targetPos = _currentTargetPoint;

	if (targetPos.x<=this->getPositionX()){
		_armature->getAnimation()->play("atk_pre",-1,0);
		origin = Point(getPositionX()-_armature->getContentSize().width/2,getPositionY()+_armature->getContentSize().height/2-5);
		anchor = Point(1,0.5f);
		direct = "left";
		if (this->getlev() == 1)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 4;
		}else if (this->getlev() == 2)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 1;
		}else if (this->getlev() == 3)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 9;
		}
	}else{
		_armature->getAnimation()->play("atk_back",-1,0);
		origin= Point(getPositionX()+_armature->getContentSize().width/2,getPositionY()+_armature->getContentSize().height/2-5);
		anchor = Point(0,0.5f);
		direct = "right";

		if (this->getlev() == 1)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 4;
		}else if (this->getlev() == 2)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 1;
		}else if (this->getlev() == 3)
		{
			origin.y = getPositionY()+_armature->getContentSize().height/2 - 11;
		}
	}
	targetPos.y = endPoint.y = origin.y;
	 
 	Sprite *bullet = Sprite::create("bullet.png");
 	bullet->setPosition(origin);
	bullet->setPositionY(bullet->getPositionY()+3.0);
 	bullet->setAnchorPoint(anchor);
 	this->getParent()->addChild(bullet,8);
	
 	float length = bullet->getContentSize().width;
	float _scale;
	float dt;
	float moveDistance;

	if (strcmp(direct.c_str(),"left")==0 && origin.x -targetPos.x<0){
		dt    = 0;
		bullet->removeFromParentAndCleanup(true);
		bullet=NULL;
	}else if (strcmp(direct.c_str(),"left")==0 && origin.x -targetPos.x>=0){
		_scale = length>abs(origin.x -targetPos.x) ? abs(origin.x - targetPos.x)/length:1;
		length *= _scale;

		endPoint.x = targetPos.x + length;

		dt = (origin.x - endPoint.x)/500>0? (origin.x - endPoint.x)/500:0;

 		if ( _target && _target->_states == STATES_RUN && length>0.5f){
 			endPoint.x += 40; 
			targetPos.x += 40;
 		}
	}else if (strcmp(direct.c_str(),"right")==0 && origin.x -targetPos.x>0){
		dt    = 0;
		bullet->removeFromParentAndCleanup(true);
		bullet=NULL;
	}else if (strcmp(direct.c_str(),"right")==0 && origin.x -targetPos.x<=0){
		_scale = length>abs(origin.x -targetPos.x) ? abs(origin.x - targetPos.x)/length:1;
		length *= _scale;

		endPoint.x = targetPos.x - length;

		dt = (endPoint.x - origin.x)/500>0? (endPoint.x - origin.x)/500:0;


		if ( _target && _target->_states == STATES_RUN && length>0.5f){
			endPoint.x -= 30; 
			targetPos.x -= 30;
		}
	}

	//攻击音效
	audioPlayer::getInstance()->playEffect("S37.mp3");

	if (bullet){
		//bullet->setScaleX(_scale);
		bullet->setScale(0.6f);
		bullet->runAction(Sequence::create(MoveTo::create(dt,endPoint),
										   CallFuncN::create(CC_CALLBACK_1(towerObjects::tower1atkEffectCallBack,this)),
			                               NULL	
			                              ));
	}
	
	if (!_target)
		return;
	
	auto effects = effectsObject::create();
	effects->setAnchorPoint(Point::ANCHOR_MIDDLE);
	effects->setPosition(targetPos);
	effects->towerItem1AttackEffects(dt);
	this->getParent()->addChild(effects,10);
}

void towerObjects::tower1atkEffectCallBack(Node *pSender)
{
	do{
		CC_BREAK_IF(!pSender);
		pSender->removeFromParentAndCleanup(true);
	}while (0);
}

void towerObjects::tower2atkEffect()
{
	if (_target->getPositionX()<=this->getPositionX()){
		_armature->getAnimation()->play("atk",-1,0);
	}else
	{
		_armature->getAnimation()->play("atk_1",-1,0);
	}
	//攻击音效
	audioPlayer::getInstance()->playEffect("S39.mp3");

	this->_states = STATES_LOCK;

	_target = getClosesTarget();
	if (!_target){
		this->_states = STATES_LOAD;
		return;
	}

	CCLOG("=========hp=%d,tak=%d",_target->gethp(),this->getatk());
	if (_target->gethp()<=this->getatk())
	{
		Point endPos = Point(getPositionX()-15,getPositionY()+15);
		float _scale = MAX(60/_target->getArmature()->getContentSize().width,80/_target->getArmature()->getContentSize().height);
		_target->runAction(ScaleTo::create(0.5f,_scale,_scale));
		_target->runAction(MoveTo::create(0.5f,endPos) );
		_target->runAction(Sequence::create(RotateBy::create(0.5f,20),
			CallFuncN::create(CC_CALLBACK_1(towerObjects::targetHide,this)),
			NULL));
	}else{
		_target->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);
	}

	this -> runAction(Sequence::create(DelayTime::create(1.0f),
									   CallFuncN::create(CC_CALLBACK_1(towerObjects::tower2atkEffectCallBack,this)),
									   NULL));
}

void towerObjects::tower4atkEffect(float dt)
{
	_states = STATES_LOCK;
	audioPlayer::getInstance()->playEffect("S67.mp3");
	_armature->getAnimation()->play(StringUtils::format("atk%d",_lev));

	if (getChildByTag(10000)){
		removeChildByTag(10000);
	}

	effectsObject *effects = effectsObject::create();
	effects->tower4_wind_effect(_lev);
	effects->setTag(10000);
	effects->setPosition(-155,55);
	this->addChild(effects,2);

	tower4AtkedObjs();

	scheduleOnce(schedule_selector(towerObjects::tower4atkCallfunc),TOWER_ITEM4_ATK_LAST);
}

void towerObjects::tower4atkCallfunc(float dt)
{
	if (getChildByTag(10000)){
		removeChildByTag(10000);
	}
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("td_tower_04_feng.plist");

	tower4cd();
}

void towerObjects::targetHide(Node *pSender)
{
	do{
		CC_BREAK_IF(!pSender);
		GameObjects *obj = static_cast<GameObjects*>(pSender);
		auto _curStorey = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(obj->getfloorNum());
		CC_BREAK_IF(!_curStorey);
		if (obj->getSelfType()==ENEMY_ITEM21){
			UserRecord::getInstance()->setBoxEnemyDieMethod(1);
		}
		_curStorey->removeObject(obj);
		pSender->removeFromParentAndCleanup(true);

		//判断游戏是否结束
		if (UserRecord::getInstance()->getRound()== UserRecord::getInstance()->getMaxRound() && SceneData::getInstance()->isClearAllMonster()){
			WindowManager::getInstance()->createGameOverFrame(true);
		}
	}while (0);
}

void towerObjects::tower2atkEffectCallBack(Node *pSender)
{
	bool ret=false;
	do{
		CC_BREAK_IF(!_armature);
	
		Armature *arraw = cocostudio::Armature::create("tower_02_effect");
		Point arrowPos = convertToWorldSpace(getPosition());
		if (getlocationIndx()<= UserRecord::getInstance()->_pixelShift.size())
			arrowPos.x = getPosition().x - UserRecord::getInstance()->_pixelShift.at(getlocationIndx()) + Director::getInstance()->getVisibleOrigin().x;
		arraw->setPosition(arrowPos);
		float sc = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum)->mItem->getScale() * 0.85;
		arraw->setScale(sc);
		arraw->setTag(8088);
		WindowManager::getInstance()->mGameLayer->addChild(arraw,20);
		arraw->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(towerObjects::onFrameEvent));
		
		_armature->getAnimation()->play("end",-1,0);

		if (_lev==1){
			arraw->getAnimation()->play("td_tower_02_a",-1,0);
		}else if (_lev==2){
			arraw->getAnimation()->play("td_tower_02_b",-1,0);
		}else if (_lev==3){
			arraw->getAnimation()->play("td_tower_02_c",-1,0);
		}

		float time  = this->gettower_atkCDTime();
		arraw->runAction(Sequence::create(DelayTime::create(1.0f),CallFunc::create([=](){
																							arraw->setVisible(false);
																				   }),NULL));
		arraw->runAction(Sequence::create(DelayTime::create(time),
										  CallFuncN::create(CC_CALLBACK_1(towerObjects::removeArrow,this)),
									      NULL));

		ret = true;
	}while (0);

	if (!ret)
		this->_states = STATES_LOAD; 
}

void towerObjects::removeArrow(Node *pSender)
{
	bool ret=false;
	do 
	{
		CC_BREAK_IF(!pSender);
		pSender->removeFromParentAndCleanup(true);
		tower2ResumeLoad(2.0f);

		ret=true;
	} while (0);

	if (!ret)
		this->_states = STATES_LOAD;
}

void towerObjects::tower2ResumeLoad(float dt)
{
	if (_armature && this->_states == STATES_LOCK ){
		_armature->getAnimation()->play("load",-1,0);
		this->_states = STATES_LOAD;
	}
}


void towerObjects::createDaodan()
{
	if (_selfType!=1 || _lev<_maxlev )
		return;

	if (_armature && _armature->getChildByTag(101))
		_armature->removeChildByTag(101,true);

	if (this->_states == STATES_LOAD)
		return;

	if (SceneData::getInstance()->isClearAllMonster())//防止没敌人了还会发射
	{
		return;
	}

	string framename="";
	if (_lev==1)
		framename = "td_tower_01_a_daodan.png";
	else if (_lev==2){
		framename = "td_tower_01_b_daodan.png";
	}else {
		framename = "td_tower_01_c_daodan.png";
	}
	Sprite *daodan = Sprite::createWithSpriteFrameName(framename.c_str());

	Point pos = Point(5,_armature->getPositionY()+_armature->getContentSize().height/2);
	if (_lev==2){
		pos.y += 5;
	}
	daodan->setPosition(pos);
	daodan->setTag(101);
	_armature->addChild(daodan,100);

}

void towerObjects::daodanEffectCallBack(Node *pSender)
{
	do 
	{
		CC_BREAK_IF(!pSender);
		Point pos = pSender->getPosition();
		pos.y +=20;
		

// 		_fly_target = getFlyEnemy();
// 		if (_fly_target)
// 			this->_currFlyTargetPoint = _fly_target->getPosition();

		auto effects = effectsObject::create();
		effects->setPosition(pos);
		effects->tower1_paodan_bomb();
		effects->setScale(0.8f);
		auto parent = static_cast<Widget*>(pSender->getParent());
		parent->addChild(effects, 8);

		pSender->removeFromParentAndCleanup(true);
	} while (0);
}

void towerObjects::checkHp()
{
	if (_selfType==TOWER_ITEM3){
		if (!isHalf && _hp<=0.5*_maxHp)
		{
			isHalf = true;
			_armature->getAnimation()->play("atked1",-1,0);
		}else if (isHalf && _hp<0.5*_maxHp){
			_armature->getAnimation()->play("atked2",-1,0);
		}else if (_hp>0.5*_maxHp){
			isHalf = false;
			load();
		}
	}
}

void towerObjects::checkDirection()
{
	if (_selfType!=TOWER_ITEM1 && _states!=STATES_FIGHTING)
		return;

	if (_fly_target && _armature && _armature->getChildByTag(101)){

		Sprite *paodan = static_cast<Sprite*>( _armature->getChildByTag(101));

		Point pos = Point(this->getPositionX(),this->getPositionY()+this->getArmature()->getContentSize().height/2);
		
		Point temp = _currFlyTargetPoint;
		if (_currFlyTargetPoint.x<pos.x){
			if (_fly_target->_states == STATES_RUN &&  _currFlyTargetPoint.x + 50 < pos.x)
				temp.x += 50;
		}
	
		float angel = abs(atan((pos.x-temp.x)/(pos.y-temp.y))*180/3.1415927) ;
	
		if (_currFlyTargetPoint.x<pos.x){
			if (angel>45){
				paodan->runAction(RotateTo::create(0.3f,45-angel));
			}else{
				paodan->runAction(RotateTo::create(0.3f,45-angel));
			}
		}else{
			paodan->runAction(RotateTo::create(0.3f,45+angel));
		}



	}else if (!_fly_target  && _armature && _armature->getChildByTag(101) ){
		Sprite *paodan = static_cast<Sprite*>( _armature->getChildByTag(101));
		paodan->runAction(RotateTo::create(0.3f,0));
	}
}

void towerObjects::MoveFinished(Node* sender)
{
	Sprite *sprite = (Sprite *)sender;
	auto parent = static_cast<Widget*>(this->getParent());
	parent->removeChild(sprite);
	
}

void towerObjects::tower4cd()
{
	_armature->getAnimation()->play(StringUtils::format("cd%d",_lev));
	scheduleOnce(schedule_selector(towerObjects::tower4resumeLoad),TOWER_ITEM4_CD_TIME);
}

void towerObjects::tower4AtkedObjs()
{
	//获取攻击的敌人数量
	int num = getTower4AtkCount();
	//敌人被吹动的距离
	int len;
	switch (_lev)
	{
	case 1:
		len = TOWER_ATK_LEN_NEAR;
		break;
	case 2:
		len = TOWER_ATK_LEN_MID;
		break;
	case 3:
		len = TOWER_ATK_LEN_FAR;
		break;
	default:
		len = TOWER_ATK_LEN_NEAR;
		break;
	}

	StoreyData* Storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
	if (Storey == nullptr )
	{
		return ;
	}

	//吹动对象所在的区域范围
	Rect atkZone = Rect(this->getPositionX()-40-_atkLen,0,_atkLen,107);

	Vector<GameObjects*> atkedEnemy;
	atkedEnemy.clear();
	for(GameObjects* obj:Storey->mObjectData){
		if (obj){
			if ( obj->getPositionX() > atkZone.origin.x && obj->getPositionX()<this->getPositionX()-40 && obj->getisFrozen() == NO && obj->getobjectType()==TYPE_ENEMY){
				atkedEnemy.pushBack(obj);
			}
		}
	}
	
	int idx=0;
	while(!atkedEnemy.empty() && idx<num){
		GameObjects *obj;
		float posX=0;
		for (GameObjects *_obj:atkedEnemy){
			if (_obj && _obj->getPositionX()>posX ){
				obj  = _obj;
				posX = _obj->getPositionX();
			}
		}

		
		//选出最近的对象，并被吹走
		enemyObjects *enemyObj = static_cast<enemyObjects*>(obj);

		//寻找是否有正在打胖子，并取消
		if ( enemyObj->getisAtkTower3Flag()==YES)
		{

			GameObjects *pObj=nullptr;
			for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
			{
				GameObjects *obj=(GameObjects*)sp;
				if ((uintptr_t)obj==enemyObj->gettargetAddr()&&obj->getobjectType()!=TYPE_ENEMY&&obj->gethp()>0)
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


		enemyObj->setisBackWind(YES);//正在被封吹
		enemyObj->load();
		//enemyObj->changeBackColor(NULL);
		enemyObj->getArmature()->getAnimation()->pause();
		enemyObj->setisStart(NO);
		enemyObj->runAction(Sequence::create(MoveBy::create(TOWER_ITEM4_ATK_LAST,Point(-len,0)),CallFunc::create([=](){
																						enemyObj->getArmature()->getAnimation()->resume();
																						enemyObj->setisStart(YES);
																						enemyObj->setisBackWind(NO);
																										
		}),NULL));

		atkedEnemy.eraseObject(obj);
		idx++;
	}
}

void towerObjects::tower4resumeLoad(float dt)
{
	_states = STATES_LOAD;
	_armature->getAnimation()->play(StringUtils::format("load%d",_lev));
}

void towerObjects::setProperty() 
{
	this->updateLevel(_lev);//

	// 科技树，防御塔 防御增加
	float _level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item2", 1);
	float _level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item2", 5);
	_dfe *= (1 + _level1 + _level5);
	// 科技树，防御塔 升级花费减少
	float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item2", 3);
	_upgradeMoney *= (1 - _level3);
}

void towerObjects::resumeObjScheduler()
{
	if (UserRecord::getInstance()->getIsGameOver()){
		return;
	} 

	if (_isSelect){

		_isSelect=false;
		this->l_atklenObjectHide();
		hideLevel();

		this->stopTint();
	}
}

GameObjects *towerObjects::FindObject(int addr)
{
	GameObjects *pObj=nullptr;

	float temp_skill3_buffvalue=0.0;
	if (!_storeydata)
		return pObj;

	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;


		if ((uintptr_t)obj==addr&&obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0&&obj->getisIntimidate()==NO&&obj->getisBackWind()==NO)
		{
			float dist=fabs(this->getPositionX()-obj->getPositionX());
			float tempLen=_atkLen;
			if (_atkLenOption==ATKLEN_NEAR)
			{
				tempLen=this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() 
					+ obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale();
			}
			if (dist<=tempLen)
			{
				pObj=obj;
				break;
			}
		}


		
		if (obj->getobjectType()==TYPE_BOSS&&obj->getSelfType()==BOSS_ITEM2&&obj->gethp()>0)//如果存在boss2的话
		{
			temp_skill3_buffvalue=obj->getskill3_buffValue();//获取英雄赋予的攻击增益
		}

	}

	_boss2_skill3_of_Tower=temp_skill3_buffvalue;
	return pObj;
}

int towerObjects::getTower4AtkCount()
{
	switch (_lev)
	{
	case 1:
		return 3;
		break;
	case 2:
		return 5;
		break;
	case 3:
		return 8;
		break;
	default:
		return 3;
		break;
	}
}

void towerObjects::tower5atkEffect()
{
	audioPlayer::getInstance()->playEffect("S68.mp3");
	_armature->getAnimation()->play("atk",1,0);

	float _x = -85;
	float _y = 52;
	if (_lev==2){
		_x = -95;
		_y = 60;
	}else if (_lev==3){
		_x = -95;
		_y = 60;
	}

	if (getChildByTag(20000))
		return;

	float dis = 0;
	StoreyData* Storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);


	effectsObject* effect1 = effectsObject::create();
	effect1->setPosition(Point(_x,_y));
	effect1->setTag(20000);
	this->addChild(effect1,10);

	effectsObject* effect2 = effectsObject::create();
	effect2->setPosition(Point(_x-320,_y-15));
	this->addChild(effect2,9);

 	if (getPositionX() + effect2->getPositionX() - 320 <Storey->getLeftDoor()->getPositionX()){
	 		dis = Storey->getLeftDoor()->getPositionX() - (getPositionX() + effect2->getPositionX() - 320); 
			effect2->setPositionX(effect2->getPositionX()+dis);
 	}

	effect1->tower5_jiguang_effect();
	effect2->tower5_shexian_effect(dis);

	this->runAction(Sequence::create(DelayTime::create(1.2f),CallFunc::create([=](){
							tower5atkObjsReduceHp();					
					}),NULL));
	this->runAction(Sequence::create(DelayTime::create(2.5f),CallFunc::create([=](){
				tower5cd();
	}),NULL));
}

void towerObjects::tower5atkObjsReduceHp()
{
	StoreyData* Storey  = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
	if (Storey == nullptr )
	{
		return ;
	}

	//攻击对象所在的区域范围
	Rect atkZone = Rect(0,0,this->getPositionX()-_armature->getContentSize().width/2,_armature->getContentSize().height);

	for(GameObjects* obj:Storey->mObjectData){
		if (obj){
			if ( obj->getPositionX() > atkZone.origin.x && obj->getPositionX()<atkZone.size.width && obj->getobjectType()==TYPE_ENEMY && obj->getisFly()==NO ){
				if (obj->gethp()>0)
				{
					obj->setReduceHp(_atkOption,_atk+_atk*_boss2_skill3_of_Tower,_CRI,_CRI_damage);

					effectsObject *effect = effectsObject::create();
					effect->setPosition(Point(10,50));
					obj->addChild(effect,10);
					effect->tower5_atk_obj_effect();
					
				}
			}
		}
	}
}

void towerObjects::tower5cd()
{
	_states = STATES_LOCK;
	if (_armature)
		_armature->getAnimation()->play("end");

	scheduleOnce(schedule_selector(towerObjects::resumeTower5Status),TOWER_ITEM5_CD_TIME);
}

void towerObjects::resumeTower5Status(float dt)
{
	_states = STATES_LOAD;
	_armature->getAnimation()->play("load");
}

