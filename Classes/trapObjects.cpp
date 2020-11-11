#include "trapObjects.h"
#include "bossObjects.h"
#include "WindowManager.h"
USING_NS_CC;

trapObjects::trapObjects(void)
{
	
}

trapObjects::~trapObjects(void)
{
	RemoveListenerTouch();

}
// on "init" you need to initialize your instance
 trapObjects* trapObjects::create(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame)
  {

	  trapObjects *sprite = new trapObjects();
	  if (sprite && sprite->init())
	  {
		  sprite->trapObjectsInit(filename,name,otype,_fileFrame);
		  sprite->autorelease();
		  return sprite;
	  }
	  CC_SAFE_DELETE(sprite);
	  return nullptr;
  }

  
bool trapObjects::trapObjectsInit(const std::string& filename,const std::string&name,int otype,const std::string& _fileFrame)
{

	
	targetAddr=0;
	_objectType=TYPE_TRAP;
	_selfType=otype;//是属于那种的陷阱
    _filenameFrame=_fileFrame;
	_floorNum=1;
	_aktFlag=YES;
	_states=STATES_DEFAULT;//设置是默认的
	_lev=1;
	_armature=nullptr;
	_touchFlag=YES;

	this->setProperty();//创建的时候的默认第等级一
	this->CreateArmature(filename,name);
	
	_storeydata=nullptr;
	CreateListenerTouch_Armature();
	float dt=1.0;
	 if (_selfType==TRAP_IETM1)//攻击类型
	 {
		 dt=0.5;
		
	 }
	 if (_selfType==TRAP_IETM2)//固定目标 即冰冻
	 {
		 dt=1.0/20;
	 }
	 if (_selfType==TRAP_IETM5)//减速类型的
	 {
		 dt=1.0;
	 }
	 if (_selfType==TRAP_IETM4)//弹飞上去的
	 {
		 dt=1.0/20.0;
	 }
	this->schedule(schedule_selector(trapObjects::trapupdate), dt);
	

	return true;
}

//设置属性
void trapObjects::setProperty()
{
	////暂时用的属性
	updateLevel(_lev);
	_hp=10;

	// 科技树，陷阱升级减少
	float _level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item3", 1);
	float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item3", 3);
	float _level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item3", 5);
	float _value = 1.0 - _level1 - _level3 - _level5;
	int _valueInt = (int)floor(_value*1000);
	float _valueFloat = _valueInt/1000.0f;
	_upgradeMoney = _upgradeMoney * _valueFloat;
}
void trapObjects::upgradeObjectAnimation(){//升级的动画播放
	this->dragObjectSmoke();

	
		 std::string ijson;
		 std::string iname;
		 if (_lev==1)
		 {
			 ijson= __String::createWithFormat("%s%s.ExportJson",_filenameFrame.c_str(),"a")->getCString();
			 iname= __String::createWithFormat("%s%s",_filenameFrame.c_str(),"a")->getCString();
		 }
		 if (_lev==2)
		 {
			 ijson= __String::createWithFormat("%s%s.ExportJson",_filenameFrame.c_str(),"b")->getCString();
		   iname= __String::createWithFormat("%s%s",_filenameFrame.c_str(),"b")->getCString();
		 }
		 if (_lev==3)
		 {
			 ijson= __String::createWithFormat("%s%s.ExportJson",_filenameFrame.c_str(),"c")->getCString();
			 iname= __String::createWithFormat("%s%s",_filenameFrame.c_str(),"c")->getCString();
		 }
		
		 this->CreateArmature(ijson,iname);

		 setProperty();
}

void trapObjects::dragObjectSmoke(){

	ArmatureDataManager::getInstance()->addArmatureFileInfo("dragObjectSmoke0.png","dragObjectSmoke0.plist","dragObjectSmoke.ExportJson");
	dragsmokeArm = Armature::create("dragObjectSmoke");

	if (_selfType==TRAP_IETM1)
	{
		dragsmokeArm->setPosition(0,20);
	}
	if (_selfType==TRAP_IETM2)
	{
		dragsmokeArm->setPosition(0,20);
	}
	if (_selfType==TRAP_IETM3)
	{
		dragsmokeArm->setPosition(0,20);
	}
	if (_selfType==TRAP_IETM5)
	{
		dragsmokeArm->setPosition(0,30);
	}
	
	this->addChild(dragsmokeArm,2);
	dragsmokeArm->getAnimation()->play("Animation2",-1,0);
	dragsmokeArm->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(trapObjects::onFrameEvent));
}

//创建骨骼动画相关函数 
void trapObjects::CreateArmature(const std::string& filename,const std::string&name ){

	if (_armature!=nullptr)
	{
		_armature->removeFromParentAndCleanup(true);
		_armature=nullptr;
	}
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	_armature = cocostudio::Armature::create(name);   
	_armature->setPosition(0,0);
	this->addChild(_armature,1);

	/*_armature->setScale(0.8);*/ //这个已经在导出来的时候处理过缩小大屏幕0.8了
   if (_selfType==TRAP_IETM1)//攻击类型的
   {
	   	_armature->getAnimation()->play("play",-1,1);
		//已经在导出骨骼动画的时候缩小到0.9了
		//_armature->setScale(0.9);
	if (_lev==1)
	{
		_armature->setPosition(0,5);
	}
	if (_lev==2)
	{
		_armature->setPosition(0,5);
	}

	this->Load();
	
   } 
   if (_selfType==TRAP_IETM2)//固定目标陷阱
   {
	   if (_aktFlag==NO)//说明在冷却中
	   {
		      _armature->getAnimation()->play("play",-1,1);
	   }else{
		   _armature->getAnimation()->play("load",-1,1);
		   _aktFlag=YES;
	   }
   }
	if (_selfType==TRAP_IETM3)//手动引爆陷阱
	{
		_armature->getAnimation()->play("play",-1,1);
		if (_aktFlag==NO)//说明正在冷却时间中
		{
			_armature->getAnimation()->play("remain",-1,1);
		}
	}
	if (_selfType==TRAP_IETM4)//弹飞敌人上到上一层的
	{
		//有可能会是存在升级了的时候
		
		_armature->getAnimation()->play("load",-1,1);

	
		if (_aktFlag==NO)//说明正在冷却时间中
		{
			_armature->getAnimation()->play("atkcd",-1,1);
		}
		
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(trapObjects::onFrameEvent));
	}
	if (_selfType==TRAP_IETM5)//减速陷阱
	{
		_armature->getAnimation()->play("play",-1,1);
	}

}

void trapObjects::actionForTrap4_LOAD()//待机状态
{
	_armature->getAnimation()->play("load",-1,1);
}
void trapObjects::actionForTrap4_ATK()//攻击状态
{
	_armature->getAnimation()->play("atk",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(trapObjects::onFrameEvent));
}
void trapObjects::actionForTrap4_ATKCD()//攻击完的状态
{
	_armature->getAnimation()->play("atkcd",-1,1);

}
void trapObjects::actionForTrap4_BACK()//返回到待机的状态
{
	_armature->getAnimation()->play("end",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(trapObjects::onFrameEvent));
}

void trapObjects::Load(){
	_states=STATES_LOAD;
	if (_selfType==TRAP_IETM1&&strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"load")!=0)
	{
		_armature->getAnimation()->play("load",-1,1);
		audioPlayer::getInstance()->pauseEffect(trap1_SoundId);
	}
}
void trapObjects::Fighting()
{
	_states=STATES_FIGHTING;
	if (_selfType==TRAP_IETM1&&strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"play")!=0)
	{
		_armature->getAnimation()->play("play",-1,1);
		
		//trap1_SoundId=audioPlayer::getInstance()->playEffect("S41.ogg",true);//是否循环
		//trap1_SoundId=audioPlayer::getInstance()->playEffect("S41.ogg",true);//是否循环
	}

}


void  trapObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	// CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);

	if (strcmp(evt.c_str(),"smokeFinish")==0)
	{
		dragsmokeArm->getAnimation()->stop();
		dragsmokeArm->removeAllChildrenWithCleanup(true);
	}
	
	if (strcmp(evt.c_str(),"finish1")==0)//攻击结束
	{
		this->actionForTrap4_ATKCD();
		
	}
	if (strcmp(evt.c_str(),"finish2")==0)//返回结束
	{
		this->actionForTrap4_LOAD();
		_aktFlag=YES;
		
	}


}

//骨骼动画的触摸事件
void trapObjects::CreateListenerTouch_Armature()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(trapObjects::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(trapObjects::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(trapObjects::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_TRAP_ZORDER); 
}
bool trapObjects::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{
	if (_touchFlag==YES && _hp>0)
	{

// 		auto _curData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());
// 		if (!_curData)
// 			return false;
// 		bossObjects *obj = static_cast<bossObjects*>(_curData->getHero());
// 		bool isCliced = false;
// 		if (obj){
// 			isCliced = obj->getisSelect();
// 		}

		bool isCliced = SceneData::getInstance()->isExistObjSelect();
		if (isStart==YES && isCliced==false){
			Point _touchPos		= _touch->getLocation();
			Size _curSize		= this->getArmature()->getContentSize();
			Point _touchNodePos = this->convertToNodeSpace(_touchPos);
			Point _worldPoint   = Director::getInstance()->getRunningScene()->convertToWorldSpace(_touchPos);
			Point _originPoint  = Director::getInstance()->getVisibleOrigin();

			auto _distance = abs(_worldPoint.y - _originPoint.y);


			Point _curPos;
			_curPos.x = 0 - _curSize.width/2;
			_curPos.y = 0;

			Rect btnRect,_curRect;

			if (_selfType==TRAP_IETM3&&_aktFlag==YES)//点击爆炸的陷阱的
			{
				btnRect=Rect(_curPos.x, _curSize.height/2, _curSize.width, _curSize.height/2);
			  _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height/2);
			 
			}else{
				 _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height);
				 if (_selfType==TRAP_IETM4)//如果是弹飞上去的，高度放高点
				 {
					  _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height+25);
				 }
				 if (_selfType==TRAP_IETM2)//如果是固定目标的陷阱
				 {
					 _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height+30);
				 }

			}

			
		 if (_selfType==TRAP_IETM3)//手动爆炸的陷阱
		 {
			 if (btnRect.containsPoint(_touchNodePos)&&_distance>100)
			 {
				 this->BombWithTrap3();
				 audioPlayer::getInstance()->playEffect("S43.mp3");//爆炸的生效

			 }
		 }


			


			// 点中
			if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
			{

				clickedEffect();
				if (WindowManager::getInstance()->mGameLayer->getChildByTag(250)){
					WindowManager::getInstance()->mGameLayer->removeChildByTag(250);

				}

				isStart=NO;
				_isSelect=true;
				Ui_ObjectClicked *objClickedUI = Ui_ObjectClicked::create();
				objClickedUI->setTag(250);
				WindowManager::getInstance()->mGameLayer->addChild(objClickedUI,50);
				objClickedUI->clickedTrap(this,convertToWorldSpace(getArmature()->getPosition()));
				objClickedUI->setSelector(this,comm_selector(trapObjects::resumeObjScheduler));

				this->startTint();

				showLevel();
				return true;
			}



		}

	}
	
	return false;
} 

void  trapObjects::onTouchMoved(Touch *touch, Event *unused_event)  
{  
	//CCLOG("bbb");  
}  

void  trapObjects::onTouchEnded(Touch *touch, Event *unused_event)  
{  
	//CCLOG("ccc");  
}


void trapObjects::resumeObjScheduler(){

	if (UserRecord::getInstance()->getIsGameOver()){
		return;
	} 

	isStart=YES;
	_isSelect=false;
	//this->l_atklenObjectHide();//隐藏攻击范围底片的
	hideLevel();

	this->stopTint();
}



//这个是删除触摸事件的
void  trapObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
}

/***

主定时器
***/

void trapObjects::trapupdate(float dt){

	if (isStart==YES)//开始了的时候
	{

		   bool isFind=false;
		   
		   int iCount=0;

			if (!_storeydata)
				return;
			if (_storeydata->getObjectData().size() > 0)
			{
				for (Object *sp:_storeydata->getObjectData())
				{
					GameObjects *obj=(GameObjects*)sp;
					if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&obj->gethp()>0&&obj->getisFly()==NO&&obj->getisFrozen()==NO&&obj->getisIntimidate()==NO)//寻找一个敌人
					{
						float X1=this->getPositionX();
						float X2=obj->getPositionX();
						
						if (_selfType==TRAP_IETM1)//如果是攻击类型的话
						{
							if (fabs(X1-X2)<=(_atkLen+10))
							{
								isFind=true;
								this->Fighting();
								obj->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);
							}
						}
					
						if (_selfType==TRAP_IETM2&&_aktFlag==YES)//如果是冰冻效果的话
						{
							/*CCLOG("_trap_buffValue=%d,%d",(int)(_trap_buffValue),_trap_targetNum);*/

							if (fabs(X1-X2)<=3.0&&iCount<(int)(_trap_targetNum))
							{
								iCount++;
								_aktFlag=NO;
								_armature->getAnimation()->play("play",-1,1);
							 
								float delayTime = 0;
								enemyObjects*enemy=static_cast<enemyObjects*>(obj);
								enemy->FrozenMyself(_trap_keepTime,true);//冰冻起来

								delayTime += 0.4f;
							
								int mDist=100;
								if (_lev==1)
								{
									mDist=150;
								}
								if (_lev==2)
								{
									mDist=200;
								}
								if (_lev==3)
								{
									mDist=250;
								}
								for (Object *sp1:_storeydata->getObjectData())//循环遍历子节点
								{
									GameObjects *obj1=static_cast<GameObjects*>(sp1);
									float distance=fabs(this->getPositionX()-obj1->getPositionX());
									
									if (obj1->getobjectType()==TYPE_ENEMY&&obj1->getisFrozen()==NO&&distance<mDist&&iCount<(int)(_trap_targetNum))
									{
										iCount++;
										enemyObjects*enemy1=static_cast<enemyObjects*>(obj1);
										enemy1->runAction(Sequence::create(DelayTime::create(delayTime),CallFunc::create([=](){
										enemy1->FrozenMyself(_trap_keepTime);
										}),NULL));

										delayTime += 0.4f;
									}
								}

								audioPlayer::getInstance()->playEffect("S42.mp3");//冰冻起来的声音
								auto delay=DelayTime::create(_trap_atkCDTime);
								auto ddlay = Sequence::create(
									delay,
									CallFuncN::create(CC_CALLBACK_1(trapObjects::atkCDtimeBack,this)),
									NULL);
								this->runAction(ddlay);
							}
						}


						if (_selfType==TRAP_IETM4&&_aktFlag==YES)//弹飞上去的
						{
							enemyObjects*enemy=static_cast<enemyObjects*>(obj);
							if (fabs(X1-X2)<=7&&enemy->getisFly()==NO&&enemy->getSelfType()!=ENEMY_ITEM4&&enemy->getSelfType()!=ENEMY_ITEM8&&enemy->getSelfType()!=ENEMY_ITEM12&&enemy->getSelfType()!=ENEMY_ITEM16&&enemy->getSelfType()!=ENEMY_ITEM20&&enemy->getSelfType()!=ENEMY_ITEM21 )
							{
								enemy->trap4FlyUp();
							    _aktFlag=NO;
								audioPlayer::getInstance()->playEffect("S45.mp3");//弹上去的生效
								this->actionForTrap4_ATK();// 播放弹上去的动画
								auto delay=DelayTime::create(_trap_atkCDTime);
								auto ddlay = Sequence::create(
									delay,
									CallFuncN::create(CC_CALLBACK_0(trapObjects::actionForTrap4_BACK,this)),
									NULL);
								this->runAction(ddlay);

							}
						}


						if (_selfType==TRAP_IETM5)//减速的。。。
						{
							enemyObjects*enemy=static_cast<enemyObjects*>(obj);
							if (fabs(X1-X2)<=_atkLen&&enemy->getisSpeedDown()==NO)
							{
								enemy->speedDown(_trap_keepTime,_trap_buffValue);
                              

							}
						}


					}
				}
			}
		
			if(isFind==false&&_selfType==TRAP_IETM1){
				this->Load();
			}


		

	  





	}

}


 void trapObjects::atkCDtimeBack(Ref*psender){

	 	_armature->getAnimation()->play("load",-1,1);
		_aktFlag=YES;

 }

 void trapObjects::BombWithTrap3(){
	 _aktFlag=NO;
	 _armature->getAnimation()->play("remain",-1,1);
	 if (!_storeydata)
		 return;
	 if (_storeydata->getObjectData().size() > 0)
	 {
		 for (Object *sp:_storeydata->getObjectData())
		 {
			 GameObjects *obj=(GameObjects*)sp;
			 if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&obj->getisStart()==YES&&obj->gethp()>0&&obj->getisFly()==NO)//寻找一个敌人
			 {
				 float X1=this->getPositionX();
				 float X2=obj->getPositionX();
				 if (fabs(X1-X2)<=_atkLen&&obj->getisFrozen()==NO)
				 {
					 obj->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);
				 }

			 }

		 }

	 }

	// this->CreateAnimationForDeathObject(Point(0,0));

	 auto effects = effectsObject::create();
	 effects->setPosition(Point(this->getPositionX(),this->getPositionY()));
	 effects->Trap03_bomb();
	 auto parent = static_cast<Widget*>(this->getParent());
	 parent->addChild(effects, 8);

	 //冷却时间
	 auto delay=DelayTime::create(_trap_atkCDTime);
	 auto ddlay = Sequence::create(
		 delay,
		 CallFuncN::create(CC_CALLBACK_0(trapObjects::BombWithTrap3CDCallback,this)),
		 NULL);
	 this->runAction(ddlay);


 }

 void trapObjects::BombWithTrap3CDCallback(){//冷却回调

	 _aktFlag=YES;
	 _armature->getAnimation()->play("play",-1,1);

 }