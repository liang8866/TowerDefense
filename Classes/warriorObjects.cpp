#include "warriorObjects.h"
#include "bossObjects.h"
#include "ballObjects.h"
#include "audioPlayer.h"
USING_NS_CC;


warriorObjects::warriorObjects(void)
{
	
}

warriorObjects::~warriorObjects(void)
{
	RemoveListenerTouch();
}
// on "init" you need to initialize your instance

bool warriorObjects::warriorObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
    
	_MisPixel=0;
	targetAddr=0;
	_objectType=TYPE_WARRIOR;
	_selfType=otype;
	_layer=_l;
	_floorNum=1;
	CruisingDirectX=1;//巡逻方向默认是向右
	_states=STATES_DEFAULT;//设置是默认的
	isCruising=true;
	_isRun=false;//设置是否在跑，默认是停止的
	PrePosX=0;//记录行走坐标
	firstTimeFlag=false;//第一次放进来的时候的
	left_offset=0.0f;
	right_offset=0.0f;

	_skill2_buffValue=0;
	_skill3_buffValue=0;

	mSleepLoopEffectId = 0;

	if (_selfType==WAR_ITEM3)//第四个是飞行的
	{
	     _isFly=YES;
	}
    this->CreateArmature(filename,name);
	this->setProperty(1);
	_storeydata=nullptr;
	CreateListenerTouch_Armature();
	this->schedule(schedule_selector(warriorObjects::warriorupdate), (float)1.0/60);
	//this->l_createCAtkLenObject();//创建底片。。。。。

    return true;
}

  warriorObjects* warriorObjects::create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
  {

	  warriorObjects *sprite = new warriorObjects();
	  if (sprite && sprite->init())
	  {
		  sprite-> warriorObjectsInit(filename,name,otype,_l);
		  sprite->autorelease();
		 
		  return sprite;
	  }
	  CC_SAFE_DELETE(sprite);
	  return nullptr;
  }

  
 
//设置属性
void warriorObjects::setProperty(int fnum)
{
	////暂时用的属性
	
	_maxHp=200;
	_hp=_maxHp;
	_atk=50;
	_dfe=10;
	_lev=UserRecord::getInstance()->getWarLevelBySelfType(_selfType);
	_atkLen=_armature->getContentSize().width/2*_armature->getScale();
	_moveSpeedScale=(float)0.8;//设置默认的移动的速度率
	
	updateLevel(_lev);

 	// 科技树，士兵升级减少
 	float _level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 1);
 	float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 3);
 	float _level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 5);
	float _value = 1.0 - _level1 - _level3 - _level5;
	int _valueInt = (int)floor(_value*1000);
	float _valueFloat = _valueInt/1000.0f;
	_upgradeMoney = _upgradeMoney * _valueFloat;
	if (_lev!=1)//说明升级过了的
	{
		reSetProperty(_lev);
	}
}
 void warriorObjects::reSetProperty(int mLv){//升级的时候重新设置属性

	 updateLevel(mLv);

	 // 科技树，士兵升级减少
	 float _level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 1);
	 float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 3);
	 float _level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 5);
	 float _value = 1.0 - _level1 - _level3 - _level5;
	 int _valueInt = (int)floor(_value*1000);
	 float _valueFloat = _valueInt/1000.0f;
	 _upgradeMoney = _upgradeMoney * _valueFloat;

	 std::string ijson;
	 std::string iname;
	
		 if (_lev==1)
		 {
			 if (_selfType==WAR_ITEM1)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_01_","a")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_01_","a")->getCString();
			 }
			 if (_selfType==WAR_ITEM2)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_02_","a")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_02_","a")->getCString();
			 }
			 if (_selfType==WAR_ITEM3)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_03_","a")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_03_","a")->getCString();
			 }
			 if (_selfType==WAR_ITEM4)
			 {
			 }
			 if (_selfType==WAR_ITEM5)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_05_","a")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_05_","a")->getCString();
			 }
			 
		 }
		 if (_lev==2)
		 {

			 if (_selfType==WAR_ITEM1)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_01_","b")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_01_","b")->getCString();
			 }
			 if (_selfType==WAR_ITEM2)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_02_","b")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_02_","b")->getCString();
			 }
			 if (_selfType==WAR_ITEM3)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_03_","b")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_03_","b")->getCString();
			 }
			 if (_selfType==WAR_ITEM4)
			 {
			 }
			 if (_selfType==WAR_ITEM5)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_05_","b")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_05_","b")->getCString();
			 }
			 
		 }
		 if (_lev==3)
		 {
			 if (_selfType==WAR_ITEM1)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_01_","c")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_01_","c")->getCString();
			 }
			 if (_selfType==WAR_ITEM2)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_02_","c")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_02_","c")->getCString();
			 }
			 if (_selfType==WAR_ITEM3)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_03_","c")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_03_","c")->getCString();
			 }
			 if (_selfType==WAR_ITEM4)
			 {
			 }
			 if (_selfType==WAR_ITEM5)
			 {
				 ijson= __String::createWithFormat("%s%s.ExportJson","td_war_05_","c")->getCString();
				 iname= __String::createWithFormat("%s%s","td_war_05_","c")->getCString();
			 }
			
		 }




		 	 std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
			
			
		 if (_selfType==WAR_ITEM5||_selfType==WAR_ITEM1||_selfType==WAR_ITEM2||_selfType==WAR_ITEM3)
		 {
		
		
		 if (_armature!=nullptr)
		 {
			 _armature->removeFromParentAndCleanup(true);
			 _armature=nullptr;
		 }
		 ArmatureDataManager::getInstance()->addArmatureFileInfo(ijson);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
		 _armature = cocostudio::Armature::create(iname);   
		 _armature->setPosition(0,0);
		
		 this->addChild(_armature,1);

		 }


		 if (_selfType==WAR_ITEM5)
		 {
			 if (strcmp(MoveID.c_str(),"run")==0)
			 {
				 this->load();
				 isStart=YES;
				
			 }else if (strcmp(MoveID.c_str(),"load1")==0||strcmp(MoveID.c_str(),"load2")==0||strcmp(MoveID.c_str(),"load3")==0)
			 {
				 _armature->getAnimation()->play(MoveID,-1,1);
				 isStart=YES;
				
			 } else 
			 {
				 int sIdx=CCRANDOM_0_1()*2+1;
				
				 _armature->getAnimation()->play(__String::createWithFormat("sleep%d",sIdx)->getCString(),-1,1);
			 }
		 }
		if (_selfType==WAR_ITEM1||_selfType==WAR_ITEM2||_selfType==WAR_ITEM3)
		{
			this->load();
			
		}
		  
		std::string MoveID2=_armature->getAnimation()->getCurrentMovementID();

		
		
	


  }
 void warriorObjects::dragObjectSmoke(){

	
	
		 if (_selfType==WAR_ITEM1)//近程的
		 {
			 audioPlayer::getInstance()->playEffect("S16.mp3");
		 }
		 if (_selfType==WAR_ITEM2)//远程的
		 {
			 audioPlayer::getInstance()->playEffect("S17.mp3");
		 }
		 if (_selfType==WAR_ITEM3)//飞行的
		 {
			 audioPlayer::getInstance()->playEffect("S18.mp3");
		 }
		 if (_selfType==WAR_ITEM4)//自爆的
		 {
			 audioPlayer::getInstance()->playEffect("S19.mp3");
		 }

	
 }

//创建骨骼动画相关函数 
void warriorObjects::CreateArmature(const std::string& filename,const std::string&name ){
	
	
	if (_armature!=nullptr)
	{
		_armature->removeFromParentAndCleanup(true);
		_armature=nullptr;
	}
	
	isArmature=true;//设置为true
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	_armature = cocostudio::Armature::create(name);   
	_armature->setPosition(0,0);
	//缩放处理已经在导出骨骼动画时候做出等同下面的缩放
// 	if (_selfType == WAR_ITEM1)
// 	{
// 		_armature->setScale(1.0);
// 	}else
// 	{
// 		_armature->setScale(0.8);
// 	}

	this->addChild(_armature,1);
	this->load();
	
	
	

} 


//骨骼动画的触摸事件
void warriorObjects::CreateListenerTouch_Armature()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(warriorObjects::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(warriorObjects::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(warriorObjects::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_WARRIOR_ZORDER); 
}
//这个是删除触摸事件的
void warriorObjects::RemoveListenerTouch()
{
	listener->release();
	_eventDispatcher->removeEventListener(listener);  
	listener = nullptr;
}
bool warriorObjects::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{

	bool isCliced = SceneData::getInstance()->isExistObjSelect();

	if (_storeydata!=nullptr && isCliced==false && _hp>0){
		Point _touchPos		= _touch->getLocation();
		Size _curSize		= this->getArmature()->getContentSize()*this->getArmature()->getScale();
		Point _touchNodePos = this->convertToNodeSpace(_touchPos);
		Point _worldPoint   = Director::getInstance()->getRunningScene()->convertToWorldSpace(_touchPos);
		Point _originPoint  = Director::getInstance()->getVisibleOrigin();

		auto _distance = abs(_worldPoint.y - _originPoint.y);

		Point _curPos;
		_curPos.x = 0 - _curSize.width/2;
		_curPos.y = 0;
		Rect _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height);

		// 点中
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
		{

			if ( UserRecord::getInstance()->getIsGamePause() && !UserRecord::getInstance()->getIsFreshMan()  ){
				return true;
			}

			//test
			 std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
			

			clickedEffect();
			
			scheduleOnce(schedule_selector(warriorObjects::clickedCallFunc),0.4f);
			
			

			if (_selfType==WAR_ITEM1)//近程的
			{
				audioPlayer::getInstance()->playEffect("S16.mp3");
			}
			if (_selfType==WAR_ITEM2)//远程的
			{
				audioPlayer::getInstance()->playEffect("S17.mp3");
			}
			if (_selfType==WAR_ITEM3)//飞行的
			{
				audioPlayer::getInstance()->playEffect("S18.mp3");
			}
			if (_selfType==WAR_ITEM4)//自爆的
			{
				audioPlayer::getInstance()->playEffect("S19.mp3");
			}
			if (_selfType==WAR_ITEM5)//狗狗
			{
				std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
				if (strcmp(MoveID.c_str(),"sleep")!=0 && strcmp(MoveID.c_str(),"sleep1")!=0 && strcmp(MoveID.c_str(),"sleep2")!=0)
				{
					audioPlayer::getInstance()->playEffect("S70.mp3");
				}
				
				
			}
			return true;
		}
	}
	return false;
} 

void warriorObjects::clickedCallFunc(float dt)
{
	if (_hp>0)
	{
		if (WindowManager::getInstance()->mGameLayer->getChildByTag(250)){
			WindowManager::getInstance()->mGameLayer->removeChildByTag(250);
		}
		_isSelect=true;
		isStart=NO;

		if (_selfType==WAR_ITEM5)
		{
			this->WarStatesLoad();
		}else
		{
			this->load();
			
		}

		this->MakeEnemyNoAttkMe();//让敌人停止攻击


		pauseSchedulerAndActions();
		Ui_ObjectClicked *objClickedUI = Ui_ObjectClicked::create();
		objClickedUI->setTag(250);
		WindowManager::getInstance()->mGameLayer->addChild(objClickedUI,50);
		objClickedUI->clickedSoldier(this,convertToWorldSpace(getArmature()->getPosition()));
		objClickedUI->setSelector(this,comm_selector(warriorObjects::resumeObjScheduler));


		this->startTint();

		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}
}

void  warriorObjects::onTouchMoved(Touch *touch, Event *unused_event)  
{  
	
}  

void  warriorObjects::onTouchEnded(Touch *touch, Event *unused_event)  
{  
	  
}
void warriorObjects::beginWithCruising(float dt){
	
	//if (_states==STATES_LOAD){
		_isRun=true;
		float moveWidthCenterX=_swg->getPositionX();

		float left_end=moveWidthCenterX-_swg->getContentSize().width/2;
		float left_mid= moveWidthCenterX-_swg->getContentSize().width/4;

		float right_mid=moveWidthCenterX+_swg->getContentSize().width/4;
		float right_end=moveWidthCenterX+_swg->getContentSize().width/2;

    

		float thisPosX=this->getPositionX();
		int rands=CCRANDOM_0_1()*2;

		if (thisPosX<left_mid)//第一区间
	    {
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*4*50.0+20;
		}
		else if (thisPosX>=left_mid&&thisPosX<moveWidthCenterX)//第二区间
		{
			if (rands>0)
			{	CruisingDirectX=-1;
			left_offset=CCRANDOM_0_1()*4*30.0+25;
			}else{
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*5*40.0+20;
			}
		}
	else if (thisPosX>=moveWidthCenterX&&thisPosX<right_mid)//第三区间
		{
			if (rands>0)
			{	CruisingDirectX=-1;
			   left_offset=CCRANDOM_0_1()*5*40.0+60;
			}else{
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*3*40.0+20;
			}
		}
		else if (thisPosX>=right_mid)//第四区间
		{
				CruisingDirectX=-1;
		     	left_offset=CCRANDOM_0_1()*4*50.0+60;
			
		}

	
	
	if (thisPosX<=(moveWidthCenterX-_swg->getSize().width/2))
	{
		this->setPosition(moveWidthCenterX-_swg->getSize().width/2+2.0,this->getPositionY());
	}else if (thisPosX>=(moveWidthCenterX+_swg->getSize().width/2))
	{
		this->setPosition(moveWidthCenterX+_swg->getSize().width/2-2.0,this->getPositionY());
	}

		this->Run();
		

}

void warriorObjects::load(){

	if (_selfType==WAR_ITEM5)
	{
		if (_states==STATES_FIGHTING)
		{
			return;
		}
		std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
		if (strcmp(MoveID.c_str(),"atk1")==0||strcmp(MoveID.c_str(),"sleep")==0||strcmp(MoveID.c_str(),"sleep1")==0
			||strcmp(MoveID.c_str(),"sleep2")==0||strcmp(MoveID.c_str(),"sleep2")==0)
		{
			return;
		}
	}




	_isRun=false;
	_states=STATES_LOAD;
	targetAddr=0;

	if (_selfType!=WAR_ITEM5)
	{
		_armature->getAnimation()->play("load",-1,1);
	}else if(_selfType==WAR_ITEM5)//播放的时候随机loading动作
	{
		
		int nIdx=CCRANDOM_0_1()*3+1;
		_armature->getAnimation()->play(__String::createWithFormat("load%d",nIdx)->getCString(),-1,1);
	}
	if (isStart==YES)
	{
	   //遍历所以本层的节点，如果存在和本类型相同的话，如果位置在附近的话就要赶紧离开
		left_offset=0.0f;
		right_offset=0.0f;

		if (_selfType!=WAR_ITEM5)
		{
			bool is_find=this->FindSameTypeObject();
			if (is_find)
			{
				this->scheduleOnce(schedule_selector(warriorObjects::beginWithCruising), 0.1);

			}else
			{
				this->scheduleOnce(schedule_selector(warriorObjects::beginWithCruising),  1.0);

			}
		}else if (_selfType==WAR_ITEM5)
		{
			this->beginWithCruising(0.0);
		}

		
	
	}
	this->BackHp();//回血啦。。
	

	

	
}


void warriorObjects::Run()//正在跑步
{
	if (_selfType==WAR_ITEM5)
	{
		if (_states==STATES_FIGHTING)
		{
			return;
		}
		std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
		if (strcmp(MoveID.c_str(),"atk1")==0||strcmp(MoveID.c_str(),"sleep")==0||strcmp(MoveID.c_str(),"sleep1")==0
			||strcmp(MoveID.c_str(),"sleep2")==0||strcmp(MoveID.c_str(),"sleep2")==0)
		{
			return;
		}
	}

	ctNum=0;//计数
	_isRun=true;
	_states=STATES_RUN;
	_armature->getAnimation()->play("run",-1,1);
	
}

void warriorObjects::Fight(){//正在战斗

	
	_states=STATES_FIGHTING;//正在战斗
	
	if (_selfType==WAR_ITEM4)//自爆兵的
	{   
		audioPlayer::getInstance()->playEffect("S34.mp3");//自爆音效
		for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点6
		{
			GameObjects *obj=(GameObjects*)sp;
			if (obj->getobjectType()==TYPE_ENEMY&&obj->getisFly()!=YES)
			{
				float dist = fabs( getPositionX() - obj->getPositionX());

				if (dist<=60)
				{
					float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS1);//英雄3的被动技能对战士攻击有影响
					 float cri=this->findBossSkillBuffValue(BOSS_ITEM5,PASSIVESKILLS2);//英雄5第二个被动技能对战士暴击率有英雄
					  obj->setReduceHp(_atkOption,(buffValue+1)*_atk,_CRI,_CRI_damage);
					 
				}
			}
		}

		this->CreateAnimationForDeathObject(Point(0,0));

	}else if (_selfType==WAR_ITEM5)
	{
		_armature->getAnimation()->play("atk1",-1,0);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
		audioPlayer::getInstance()->playEffect("S70.mp3");//狗狗攻击
	}
	
	else{
		_armature->getAnimation()->play("atk",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
	}

		
}



void warriorObjects::FlyFlyFight(){

	GameObjects *tempObj=this->FindObject(targetAddr);
	float alllen=_atkLen;
	_states=STATES_FIGHTING;//正在战斗
	if (tempObj!=nullptr)
	{

	if (fabs(tempObj->getPositionX()-this->getPositionX())<(alllen))
	{
		float moveX=0;
		_armature->getAnimation()->play("run",-1,1);
		if (this->getPositionX()>tempObj->getPositionX())
		{
			_armature->setRotationY(180);
			moveX=alllen-fabs(tempObj->getPositionX()-this->getPositionX());
		}else{
			_armature->setRotationY(0);
			moveX=fabs(tempObj->getPositionX()-this->getPositionX())-alllen;
		}
		isStart=NO;
		auto move=MoveBy::create(0.15,Point(moveX,0));
		auto action = Sequence::create(
			move,
			CallFuncN::create(CC_CALLBACK_1(warriorObjects::FlyFlyCallback, this)),
			NULL);
		this->runAction(action);
	}else{
		_armature->getAnimation()->play("atk_fly",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
	}

	}
}


void warriorObjects::FlyFlyCallback(Ref *psend){
	
	isStart=YES;
	GameObjects *tempObj=this->FindObject(targetAddr);
	if (tempObj!=nullptr)
	{
		if (this->getPositionX()>tempObj->getPositionX())
		{
			_armature->setRotationY(0);
		}else{
			_armature->setRotationY(180);
		}
		_armature->getAnimation()->play("atk_fly",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
	}
}

void warriorObjects::SleepWar(){//睡眠
	
	if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"sleep1")==0)
	{
		_armature->getAnimation()->play("sleep2",-1,1);
	}
	else if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"sleep2")==0)
	{
		_armature->getAnimation()->play("sleep1",-1,1);
	}
	

	
}
void warriorObjects::CdbackLoad(){//CD冷却结束后变回load

	auto action = Sequence::create(
		DelayTime::create(20.0),
		CallFunc::create([=](){
			int sIdx=CCRANDOM_0_1()*3+1;
			_states=STATES_LOAD;
			_armature->getAnimation()->play(__String::createWithFormat("load%d",sIdx)->getCString(),-1,1);
			if (mSleepLoopEffectId > 0)
			{
				audioPlayer::getInstance()->stopEffect(mSleepLoopEffectId);//停止狗狗睡觉
				mSleepLoopEffectId = 0;
			}
			
	   }),
        DelayTime::create(0.4f),
		CallFunc::create([=](){
			isStart=YES;
	   }),
		CallFunc::create(CC_CALLBACK_0(warriorObjects::load, this)),
		NULL);
	this->runAction(action);
}


 void warriorObjects::WarStatesLoad(){//状态判断，用于升级和被点击的时候

	 std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
	

	 if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"run")==0)
	 {
		 this->load();
	 }
	
	 if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load1")==0||strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load2")==0||strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load3")==0)
		{
			 		 
		this->load();
		}


	 std::string MoveID2=_armature->getAnimation()->getCurrentMovementID();
	

 }


void warriorObjects::upgradeObjectAnimation(){//播放升级的显示的动画

	SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("upgradeSmoke.plist","upgradeSmoke.png");

	 Sprite* upgradeSprite;
	upgradeSprite=Sprite::createWithSpriteFrameName("td_upgrade_object_smoke_01.png");
	upgradeSprite->setPosition(0,_armature->getContentSize().height/2);
	this->addChild(upgradeSprite,2);

	Animate *ani=CreateAnimation("td_upgrade_object_smoke_",8);

	auto action = Sequence::create(
		ani,
		CallFuncN::create(CC_CALLBACK_1(warriorObjects::upgradeObjectAnimationCallfun, this)),
		NULL);

	upgradeSprite->runAction(action);
	
}

void warriorObjects::upgradeObjectAnimationCallfun(Ref *psender){

	Sprite*p=(Sprite *)psender;
	p->removeFromParentAndCleanup(YES);
}



void warriorObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	 // CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	
	if (_selfType==WAR_ITEM5)
	{
		if (strcmp(evt.c_str(),"atk1_end")==0)//第一个的
		{
			isStart=NO;
			
			this->CdbackLoad();//冷却时间
// 			_armature->getAnimation()->play("sleep",-1,0);
// 			_armature->getAnimation()->setSpeedScale(3.0);
// 			_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));

			int sIdx=CCRANDOM_0_1()*2+1;
			_armature->getAnimation()->play(__String::createWithFormat("sleep%d",sIdx)->getCString(),-1,1);
			_armature->getAnimation()->setSpeedScale(1.0);
			auto action = Sequence::create(
				DelayTime::create(5.0+CCRANDOM_0_1()*5),
				CallFunc::create(CC_CALLBACK_0(warriorObjects::SleepWar, this)),
				NULL);
			this->runAction(action);
			mSleepLoopEffectId = audioPlayer::getInstance()->playEffect("S71.mp3", true);//狗狗睡觉. 循环播放
		}

		if (strcmp(evt.c_str(),"atk_run")==0)//敌人要开始跑了
		{
			this->findIsBackRunEmeny();//.....计算寻找附近要被恐吓的敌人
		}
		if (strcmp(evt.c_str(),"begin_sleep_end")==0)
		{
// 			int sIdx=CCRANDOM_0_1()*2+1;
// 			_armature->getAnimation()->play(__String::createWithFormat("sleep%d",sIdx)->getCString(),-1,1);
// 			_armature->getAnimation()->setSpeedScale(1.0);
// 			auto action = Sequence::create(
// 				DelayTime::create(5.0+CCRANDOM_0_1()*5),
// 				CallFunc::create(CC_CALLBACK_0(warriorObjects::SleepWar, this)),
// 				NULL);
// 			this->runAction(action);
// 			mSleepLoopEffectId = audioPlayer::getInstance()->playEffect("S71.mp3", true);//狗狗睡觉. 循环播放
		}
	}
	  if (strcmp(evt.c_str(),"reducehp")==0)
	  {
		 
			  GameObjects *tempObj=this->FindObject(targetAddr);
			  if (tempObj==nullptr)//没寻找到
			  { 
				  this->load();
				
				   
			  }else{//寻找到
				  if (tempObj->getPositionX()>this->getPositionX())
				  {
					  _armature->setRotationY(180);
				  }else
				  {
					   _armature->setRotationY(0);
				  }

				  this->War2Shooter(tempObj);//发射子弹或者扣血
			
		  }
		 
	  }

	   if (strcmp(evt.c_str(),"begin")==0){
		  
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//没寻找到
		  { 
			  this->load(); 
			  
		  }else{//寻找到

			  if (_isFly==YES&&tempObj->getisFly()==NO)
			  {
				  audioPlayer::getInstance()->playEffect("S32.mp3");//飞行兵对陆地敌人
				  Point pos=Point(this->getPositionX(),this->getPositionY());
				  Point movePos=Point(tempObj->getPositionX(),tempObj->getPositionY()+tempObj->getArmature()->getContentSize().height/2*0.7);
				  this->flyWarBombCreate2(pos,movePos,tempObj->getZOrder()+1);
			  }
		  }

	   }




}  




/*********************************************

    主定时器 各种逻辑判定
	士兵AI——士兵AI基本逻辑，在部署的层内警戒，
	当有敌人进入警戒范围内与之进行战斗，直至双
	方一方HP=0结束，结束后搜索下一目标（如果有多
	个目标则选择没有进入战斗的目标），重复战斗逻辑

************************************************/

void warriorObjects::warriorupdate(float dt)
{
	if (isStart==YES&&firstTimeFlag==false)
	{
		firstTimeFlag=true;
		//遍历所以本层的节点，如果存在和本类型相同的话，如果位置在附近的话就要赶紧离开
		bool is_find=this->FindSameTypeObject();
		if (is_find==true)
		{ 
			this->unschedule(schedule_selector(warriorObjects::beginWithCruising));//取消定时器
			left_offset=CCRANDOM_0_1()*4*20.0+60;
			right_offset=CCRANDOM_0_1()*4*20.0+10;
			this->Run();
		}

	}

	if (_selfType==WAR_ITEM3&&isStart==YES&&_states==STATES_FIGHTING)
	{
		if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"run")==0)
		{
			load();
		}
	}

	//if (_selfType==WAR_ITEM3)
	/*CCLOG("(this=%d,_states=%d ,floor=%d,moveId=%s)",this,_states,_floorNum,_armature->getAnimation()->getCurrentMovementID().c_str());*/

	if (_layer!=nullptr&&isStart==YES)//不等于空的时候
	{

		bool isFind=false;//是否寻找到目标
		GameObjects *targetObj=nullptr;

		int nearestX=0;//如果有多个目标的话记录最近的一个目标最近的一个
		bool isFight=false;//是否在战斗	 
		
		if (!_storeydata)
			return;
		if (_storeydata->getObjectData().size() > 0)
		{
			for (Ref *sp:_storeydata->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;
				
				if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&_states!=STATES_FIGHTING&&obj->getisFrozen()==NO&&
					obj->getisIntimidate()==NO&&obj->getisBackWind()==NO)//寻找一个敌人，没死亡或者不在战斗的时候,且在同一层之内
				{
					float moveWidthCenterX=_swg->getPositionX();
					float thisPosX=this->getPositionX();
					if (thisPosX>(moveWidthCenterX-_swg->getSize().width/2)&&obj->getPositionX()<(moveWidthCenterX+_swg->getSize().width/2)&&obj->getPositionX()>(moveWidthCenterX-_swg->getSize().width/2))
					{
						float dist = fabs( thisPosX- obj->getPositionX());
						float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
										 obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale() +
										 LEN_SCALE;

						if (_isFly==YES && obj->getisFly()== NO){
							safeZone = 0;
						}else if (dist<safeZone){
							continue;
						} 

						if (_selfType==WAR_ITEM5&&(obj->getSelfType()==ENEMY_ITEM4||obj->getSelfType()==ENEMY_ITEM8||obj->getSelfType()==ENEMY_ITEM12||
							obj->getSelfType()==ENEMY_ITEM16||obj->getSelfType()==ENEMY_ITEM20||obj->getSelfType()==ENEMY_ITEM21))
						{
							continue;//狗对精英敌人和宝箱怪无效
						}


						int tempNearestX=dist - safeZone;//距离差距
						
						if (nearestX==0)//如果是等于0的时候，寻找到第一个目标
						{  
							if (obj->getisFly()==YES&&(_selfType==WAR_ITEM3))//如果敌人是飞行的敌人的话，要飞行兵才可以攻击
							{
								isFind=true;//表示寻找到
								nearestX=tempNearestX;//距离差距
								targetObj=obj;//获取一个目标
								targetAddr=(uintptr_t)targetObj;
							}else if (obj->getisFly()==NO){
								isFind=true;//表示寻找到
								nearestX=tempNearestX;//距离差距
								targetObj=obj;//获取一个目标
								targetAddr=(uintptr_t)targetObj;
							}

						}else {//否则的话，说明是还有第二个目标了或者多个目标,如果是多个目标的话，寻找一个没进在战斗状态的敌人进行攻击，且离当前目标最近
							
							if (_isFly==YES)//如果本身你是飞行兵的话
							{
								if (targetObj->getisFly()==NO&&obj->getisFly()==YES)//如果 最开始获取的那个不是飞行敌人的话，而当前的这个是飞行敌人的话，优先选取飞行敌人
								{	
									isFind=true;//表示寻找到
									targetObj=obj;
									nearestX=tempNearestX;
									targetAddr=(uintptr_t)targetObj;

								}else if ((targetObj->getisFly()==NO&&obj->getisFly()==NO)||(targetObj->getisFly()==YES&&obj->getisFly()==YES))//如果当前和前面选择的都不是飞行敌人的话或者都是飞行敌人
								{

									if (nearestX>tempNearestX)//如果前一个目标的相差距离大于的话,分三种情况
									{
										if (targetObj->getstates()==STATES_FIGHTING)
										{
											isFind=true;//表示寻找到
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;

										}
									else if (targetObj->getstates()!=STATES_FIGHTING)
									{
										if (obj->getstates()==STATES_FIGHTING&&obj->getcurTargetType()==TYPE_DOOR)
										{
											isFind=true;//表示寻找到
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}
									}

									}else{//如果是大于的话

										if (targetObj->getstates()==STATES_FIGHTING&&targetObj->getcurTargetType()!=TYPE_DOOR&&obj->getstates()!=STATES_FIGHTING)
										{
												isFind=true;//表示寻找到
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}

									}
								}
							}else{//如果本身不是飞行兵的话,也要分情况对方是否是飞行兵和本身是否是
								if (obj->getisFly()==YES)//如果对方是飞行敌人的话qie
								{
								}else{

									if (_selfType==WAR_ITEM2||_selfType==WAR_ITEM4)//第二个是远程攻击的和爆炸的，应该是打最近的目标的
									 {
										if (nearestX>tempNearestX)
										{
											isFind=true;//表示寻找到
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}
									}else{

										if (nearestX>tempNearestX)//如果前一个目标的相差距离大于的话,分三种情况
										{
											if (targetObj->getstates()==STATES_FIGHTING)
											{
												isFind=true;//表示寻找到
												targetObj=obj;
												targetAddr=(uintptr_t)targetObj;
												nearestX=tempNearestX;

											}
											else if (targetObj->getstates()!=STATES_FIGHTING)
											{
												if (obj->getstates()==STATES_FIGHTING&&obj->getcurTargetType()==TYPE_DOOR)
												{
														isFind=true;//表示寻找到
													targetObj=obj;
													targetAddr=(uintptr_t)targetObj;
													nearestX=tempNearestX;
												}else if (obj->getstates()!=STATES_FIGHTING)
												{
														isFind=true;//表示寻找到
													targetObj=obj;
													targetAddr=(uintptr_t)targetObj;
													nearestX=tempNearestX;
												}
											}

										}else{//如果这一次选取的目标大于等于上次选择的目标的距离的话

											if (targetObj->getstates()==STATES_FIGHTING&&targetObj->getcurTargetType()!=TYPE_DOOR&&obj->getstates()!=STATES_FIGHTING)
											{
												isFind=true;//表示寻找到
												targetObj=obj;
												targetAddr=(uintptr_t)targetObj;
												nearestX=tempNearestX;
											}

										}
									}

								}


							}

						}
					}
				}
			}
		}



		//如果未寻到攻击目标，安全区域（敌人与自身重叠区）有敌人蹦跑，则自己停下等待敌人跑出安全区（add by xiewei）
		//********************************************************************************************************
		if (targetAddr==0 && this->_states==STATES_RUN){
			for (Ref *sp:_storeydata->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;
				if (obj->getobjectType()==TYPE_ENEMY && obj->_states==STATES_RUN){
					float dist = fabs( this->getPositionX() - obj->getPositionX());
					float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
									 obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale() +
						             LEN_SCALE;

					if (_isFly==YES && obj->getisFly()==NO)
						continue;

					if (dist<safeZone&&_states!=STATES_FIGHTING){
						this->load();
						
						return;
					}
				}
			}
		}
		//********************************************************************************************************

		if (_isFly==YES&&targetAddr!=0)//这里是丢子弹下来的
		{
			GameObjects *tempTarObj=FindObject(targetAddr);
			if (tempTarObj!=nullptr&&tempTarObj->getisFly()==NO)//说明寻找到的对象不是飞行的才行
			{
				float thisPosX=this->getPosition().x;
				float objPosX=tempTarObj->getPosition().x;
				float tempAtkLen=5.0;
				Widget* parent = static_cast<Widget*>(this->getParent());
				effectsObject *myEffObj=static_cast<effectsObject*>(parent->getChildByTag((uintptr_t)this));
				if (fabs(thisPosX-objPosX)>tempAtkLen){
					if (thisPosX>objPosX)
					{	
						_armature->setRotationY(0);
						this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
						if (myEffObj!=nullptr)
						{
							myEffObj->setPositionX(tempTarObj->getPositionX()+3);
						}
					}
					else{	
						this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
						_armature->setRotationY(180);
						if (myEffObj!=nullptr)
						{
							myEffObj->setPositionX(tempTarObj->getPositionX()+3);
						}
					}
				}
			}
		}


		//如果寻找到目标了，如果本身是近战小兵的话，增加一个判断，就是不能多个多于2个近战小兵打斗一个敌人

		if ((_selfType==WAR_ITEM1||_selfType==WAR_ITEM3)&&isFind==true&&targetObj!=nullptr&&_isFly==NO)
		{
			enemyObjects *enemy=static_cast<enemyObjects *>(targetObj);
			if (enemy->GetWarAtkMeNum()>=2)
			{
				isFind=false;
				targetObj=nullptr;
				targetAddr=0;
			
				
			}
		}


		




		if (isFind==true&&targetAddr!=0&&_states!=STATES_FIGHTING&&targetObj!=nullptr)//如果发现有目标的话,且自身没有战斗状态的时候
		{
			float thisPosX=this->getPosition().x;
			float objPosX=targetObj->getPosition().x;
			float tempAtkLen=_atkLen;
			
			float moveWidthCenterX=_swg->getPositionX();
			float leftposX=moveWidthCenterX-_swg->getContentSize().width/2;
			float rightposX=moveWidthCenterX+_swg->getContentSize().width/2;

			float dist = fabs( thisPosX- objPosX );
			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
							targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
							LEN_SCALE;

			if (_isFly==YES&&targetObj->getisFly()!=YES)
			{
				tempAtkLen=5.0;
				safeZone = 0;
			}
			
			float movespeed=_moveSpeedScale;
			if (_selfType==WAR_ITEM4)//自爆兵的
			{
				tempAtkLen=4.0;
				movespeed+=1.5;
			}

			if (dist>=safeZone &&  dist-safeZone<=tempAtkLen &&thisPosX>leftposX&&thisPosX<rightposX)//说明在攻击范围内
			{

				if (_isFly==YES)//如果你是飞行兵的话
				{
					targetObj->settargetSelect(YES);
				}
				if (_isFly==YES&&targetObj->getisFly()==YES)
				{
					this->FlyFlyFight();//如果是飞行兵打飞行敌人的话
				}else{
					this->Fight();//战斗啦
				}

				if (thisPosX>objPosX)
				{
					_armature->setRotationY(0);
					
				}else{
					_armature->setRotationY(180);
					
				}

			}else if ( (dist<safeZone || dist-safeZone>tempAtkLen) &&thisPosX>leftposX&&thisPosX<rightposX)
			{

				if (thisPosX>objPosX)
				{	
					_armature->setRotationY(0);
					
					this->setPosition(this->getPosition().x-movespeed,this->getPosition().y);
					if (_states!=STATES_RUN)
					{
						this->Run();
						
					}
				}

				else 
				{	
					this->setPosition(this->getPosition().x+movespeed,this->getPosition().y);
					_armature->setRotationY(180);
					
					if (_states!=STATES_RUN)
					{
						this->Run();
						
					}

				}
			}
		}


		//强制飞行兵打斗
		if (targetAddr!=0&&_isFly==YES&&_armature!=nullptr&&targetObj!=nullptr)
		{

			if (_states==STATES_FIGHTING&&targetObj->getisFly()==YES&&targetObj->getobjectType()==TYPE_ENEMY&&strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"atk_fly")!=0){
				this->FlyFlyFight();
			}
			float thisPosX=this->getPosition().x;
			float objPosX=targetObj->getPosition().x;
			float dist = fabs( thisPosX- objPosX );
			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
				targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
				LEN_SCALE;
			
		}

		
		


		if (isFind==false)//如果没寻找到
		{
			
			if (isCruising==true&&_states==STATES_LOAD)//防止第一次放进去的时候不调用
			{
				isCruising=false;
				this->unschedule(schedule_selector(warriorObjects::beginWithCruising));//取消定时器
				this->scheduleOnce(schedule_selector(warriorObjects::beginWithCruising),  CCRANDOM_0_1()*3*2.0f);
			}

			if (_states==STATES_FIGHTING&&targetAddr==0)//如果没找到且本身在战斗
			{
				this->load();
				
			}else if (_states==STATES_RUN&&_isRun==true)
			{

				ctNum++;
				if (ctNum==50)
				{
					ctNum=0;
					for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
					{
						GameObjects *obj=(GameObjects*)sp;
						if ((obj->getobjectType()==TYPE_WARRIOR&&obj->getSelfType()==_selfType)&&(uintptr_t)obj!=(uintptr_t)this&&targetAddr==0)
						{

							if (fabs(obj->getPositionX()-this->getPositionX())<50.0f)
							{
								if (obj->getstates()==STATES_RUN&&obj->getCruisingDirectX()==CruisingDirectX)
								{
									this->load();
									
									break;
								}
								
							}
						}
					}
				}

				this->setPosition(this->getPosition().x+_moveSpeedScale*CruisingDirectX,this->getPosition().y);
				Widget *_wg=_storeydata->getMovementWidget();
				float moveWidthCenterX=_wg->getPositionX();
				float thisPosX=this->getPositionX();

				if (CruisingDirectX>0)//向右边走
				{
					_armature->setRotationY(180);
					
				}else if (CruisingDirectX<=0)//向左边走
				{
					_armature->setRotationY(0);
					
				}
				
				if (thisPosX<(moveWidthCenterX-_wg->getSize().width/2+left_offset)||thisPosX>(moveWidthCenterX+_wg->getSize().width/2-right_offset))
				{
					
					this->load();
					
				}
			}


		}

	}

}











void warriorObjects::resumeObjScheduler()
{
	if (!this)
		return;

	if (UserRecord::getInstance()->getIsGameOver()){
		return;
	} 

	_isSelect=false;
	resumeSchedulerAndActions();
	
	if (_selfType!=WAR_ITEM5)
	{
		isStart=YES;
		this->Run();
		
	}else if (_selfType==WAR_ITEM5)
	{
		
		if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"run")==0)
		{
			this->load();
			isStart=YES;
		}

		if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load1")==0||strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load2")==0||strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load3")==0)
		{
			isStart=YES;
			this->load();
		}





	}
	
	

	this->stopTint();
}

void warriorObjects::flyWarBombCreate1(Point pos,int zord)//创建飞行兵的爆炸
{
	
	effectsObject* effects = effectsObject::create();
	effects->setPosition(pos);
	effects->bottleExplod();
	effects->setRotationY(_armature->getRotationY());
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,zord);
	effects->setScale(0.8);
}

void warriorObjects::flyWarBombCreate2(Point pos,Point movePos, int zord)//创建飞行兵的爆炸
{
	
	effectsObject *myEffObj = effectsObject::create();
	myEffObj->setPosition(pos);
	myEffObj->bottleMoveExplod(movePos);
	myEffObj->setRotationY(_armature->getRotationY());
	myEffObj->setTag((uintptr_t)this);
	Widget* parent = static_cast<Widget*>(this->getParent());
	parent->addChild(myEffObj,zord);
	
	myEffObj->setScale(0.8);
	
}

GameObjects * warriorObjects::FindObject(int addr){

	GameObjects *pObj=nullptr;
	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((uintptr_t)obj==addr&&obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0&&obj->getisIntimidate()==NO&&obj->getisBackWind()==NO)
		{
			if (_selfType==WAR_ITEM2)
			{
				pObj=obj;
				break;
			}else{
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
			
		}
	}
	return pObj;
}


bool warriorObjects::FindSameTypeObject(){

	bool IsFind=false;

	if (!_storeydata)
		return false;

	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
	{

		GameObjects *obj=(GameObjects*)sp;
		if ((obj->getobjectType()==TYPE_WARRIOR&&obj->getSelfType()==_selfType)&&(uintptr_t)obj!=(uintptr_t)this)
		{
			if (fabs(obj->getPositionX()-this->getPositionX())<5.0f)
			{
				IsFind=true;
				break;
			}
		}
	}
	return IsFind;

}


void warriorObjects::War2Shooter(GameObjects *tempObj){//发射子弹
	if (_selfType==WAR_ITEM2)//第二个是远程攻击的
	{
		audioPlayer::getInstance()->playEffect("S30.mp3");//第二个远程兵的打斗的音效

		int d=D_LEFT;
		if (tempObj->getPositionX()>this->getPositionX())
		{
			d=D_RIGHT;
		}
		ballObjects*ball=ballObjects::create(targetAddr,tempObj->getPosition(),BALL_WAR02,_floorNum,d);
		ball->setb_cri(_CRI);
		ball->setb_cri_damage(_CRI_damage);

		if (tempObj->getPositionX()>this->getPositionX())//对象在右边
		{
			ball->setPosition(this->getPositionX()+_armature->getContentSize().width*_armature->getScale()-10,_armature->getContentSize().height/2*_armature->getScale()-7);

		}else//对象在左边
		{
			ball->setPosition(this->getPositionX()-_armature->getContentSize().width*_armature->getScale()+10,_armature->getContentSize().height/2*_armature->getScale()-7);
		}
		ball->setb_atk(_atk);
		ball->setb_atkOption(_atkOption);
		ball->setB_shooterPos(this->getPosition());
		Widget *w=static_cast<Widget*>(this->getParent());
		w->addChild(ball, 8);

	}else{
		float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS1);//英雄3的被动技能对战士攻击有影响
		float cri=this->findBossSkillBuffValue(BOSS_ITEM5,PASSIVESKILLS2);//英雄5第二个被动技能对战士暴击率有英雄
	
		tempObj->setReduceHp(_atkOption,_atk*(1+buffValue),_CRI+cri,_CRI_damage);


		if (_selfType==WAR_ITEM1)
		{
			audioPlayer::getInstance()->playEffect("S29.mp3");//近战兵的
		}
	
		if (_selfType==WAR_ITEM3)
		{
			audioPlayer::getInstance()->playEffect("S31.mp3");//飞行兵对飞行敌人
		}
		
		}
		
	}

void warriorObjects::findIsBackRunEmeny(){//寻找被恐吓的敌人

	float tLen=150;
	int mCount=2;
	if (_lev==1)
	{
		mCount=1;
		tLen=150;
	}
	if (_lev==2)
	{
		mCount=2;
		tLen=170;
	}
	if (_lev==3)
	{
		mCount=3;
		tLen=200;
	}

	
	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点,先搞定最近的那个选中的
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0&&(uintptr_t)obj==targetAddr)
		{
			enemyObjects *enemy=static_cast<enemyObjects*>(obj);
			enemy->xiaogoufawei(_lev);
			mCount--;
			break;
		}
	}

	float moveWidthCenterX=_swg->getPositionX();
	float thisPosX=this->getPositionX();
	
	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点,先搞定最近的那个选中的
	{
		GameObjects *obj=(GameObjects*)sp;
		float mdist=fabs(this->getPositionX()-obj->getPositionX());
		
		if (obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0&&this->getPositionX()>=obj->getPositionX()&&mdist<=tLen&&mCount>0&&obj->getisFly()==NO&&obj->getisIntimidate()==NO)
		{
			if (obj->getSelfType()!=ENEMY_ITEM4&&obj->getSelfType()!=ENEMY_ITEM8&&obj->getSelfType()!=ENEMY_ITEM12
				&&obj->getSelfType()!=ENEMY_ITEM16&&obj->_selfType!=ENEMY_ITEM20&&obj->_selfType!=ENEMY_ITEM21)
			{
				enemyObjects *enemy=static_cast<enemyObjects*>(obj);
				enemy->xiaogoufawei(_lev);
				mCount--;
			}
		
		}
	}




}