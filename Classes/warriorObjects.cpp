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
	CruisingDirectX=1;//Ѳ�߷���Ĭ��������
	_states=STATES_DEFAULT;//������Ĭ�ϵ�
	isCruising=true;
	_isRun=false;//�����Ƿ����ܣ�Ĭ����ֹͣ��
	PrePosX=0;//��¼��������
	firstTimeFlag=false;//��һ�ηŽ�����ʱ���
	left_offset=0.0f;
	right_offset=0.0f;

	_skill2_buffValue=0;
	_skill3_buffValue=0;

	mSleepLoopEffectId = 0;

	if (_selfType==WAR_ITEM3)//���ĸ��Ƿ��е�
	{
	     _isFly=YES;
	}
    this->CreateArmature(filename,name);
	this->setProperty(1);
	_storeydata=nullptr;
	CreateListenerTouch_Armature();
	this->schedule(schedule_selector(warriorObjects::warriorupdate), (float)1.0/60);
	//this->l_createCAtkLenObject();//������Ƭ����������

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

  
 
//��������
void warriorObjects::setProperty(int fnum)
{
	////��ʱ�õ�����
	
	_maxHp=200;
	_hp=_maxHp;
	_atk=50;
	_dfe=10;
	_lev=UserRecord::getInstance()->getWarLevelBySelfType(_selfType);
	_atkLen=_armature->getContentSize().width/2*_armature->getScale();
	_moveSpeedScale=(float)0.8;//����Ĭ�ϵ��ƶ����ٶ���
	
	updateLevel(_lev);

 	// �Ƽ�����ʿ����������
 	float _level1 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 1);
 	float _level3 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 3);
 	float _level5 = UserRecord::getInstance()->getUpgradesItemIsSelect("tech", "item4", 5);
	float _value = 1.0 - _level1 - _level3 - _level5;
	int _valueInt = (int)floor(_value*1000);
	float _valueFloat = _valueInt/1000.0f;
	_upgradeMoney = _upgradeMoney * _valueFloat;
	if (_lev!=1)//˵���������˵�
	{
		reSetProperty(_lev);
	}
}
 void warriorObjects::reSetProperty(int mLv){//������ʱ��������������

	 updateLevel(mLv);

	 // �Ƽ�����ʿ����������
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
		 ArmatureDataManager::getInstance()->addArmatureFileInfo(ijson);  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
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

	
	
		 if (_selfType==WAR_ITEM1)//���̵�
		 {
			 audioPlayer::getInstance()->playEffect("S16.mp3");
		 }
		 if (_selfType==WAR_ITEM2)//Զ�̵�
		 {
			 audioPlayer::getInstance()->playEffect("S17.mp3");
		 }
		 if (_selfType==WAR_ITEM3)//���е�
		 {
			 audioPlayer::getInstance()->playEffect("S18.mp3");
		 }
		 if (_selfType==WAR_ITEM4)//�Ա���
		 {
			 audioPlayer::getInstance()->playEffect("S19.mp3");
		 }

	
 }

//��������������غ��� 
void warriorObjects::CreateArmature(const std::string& filename,const std::string&name ){
	
	
	if (_armature!=nullptr)
	{
		_armature->removeFromParentAndCleanup(true);
		_armature=nullptr;
	}
	
	isArmature=true;//����Ϊtrue
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
	_armature = cocostudio::Armature::create(name);   
	_armature->setPosition(0,0);
	//���Ŵ����Ѿ��ڵ�����������ʱ��������ͬ���������
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


//���������Ĵ����¼�
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
//�����ɾ�������¼���
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

		// ����
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
		{

			if ( UserRecord::getInstance()->getIsGamePause() && !UserRecord::getInstance()->getIsFreshMan()  ){
				return true;
			}

			//test
			 std::string MoveID=_armature->getAnimation()->getCurrentMovementID();
			

			clickedEffect();
			
			scheduleOnce(schedule_selector(warriorObjects::clickedCallFunc),0.4f);
			
			

			if (_selfType==WAR_ITEM1)//���̵�
			{
				audioPlayer::getInstance()->playEffect("S16.mp3");
			}
			if (_selfType==WAR_ITEM2)//Զ�̵�
			{
				audioPlayer::getInstance()->playEffect("S17.mp3");
			}
			if (_selfType==WAR_ITEM3)//���е�
			{
				audioPlayer::getInstance()->playEffect("S18.mp3");
			}
			if (_selfType==WAR_ITEM4)//�Ա���
			{
				audioPlayer::getInstance()->playEffect("S19.mp3");
			}
			if (_selfType==WAR_ITEM5)//����
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

		this->MakeEnemyNoAttkMe();//�õ���ֹͣ����


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

		if (thisPosX<left_mid)//��һ����
	    {
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*4*50.0+20;
		}
		else if (thisPosX>=left_mid&&thisPosX<moveWidthCenterX)//�ڶ�����
		{
			if (rands>0)
			{	CruisingDirectX=-1;
			left_offset=CCRANDOM_0_1()*4*30.0+25;
			}else{
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*5*40.0+20;
			}
		}
	else if (thisPosX>=moveWidthCenterX&&thisPosX<right_mid)//��������
		{
			if (rands>0)
			{	CruisingDirectX=-1;
			   left_offset=CCRANDOM_0_1()*5*40.0+60;
			}else{
				CruisingDirectX=1;
				right_offset=CCRANDOM_0_1()*3*40.0+20;
			}
		}
		else if (thisPosX>=right_mid)//��������
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
	}else if(_selfType==WAR_ITEM5)//���ŵ�ʱ�����loading����
	{
		
		int nIdx=CCRANDOM_0_1()*3+1;
		_armature->getAnimation()->play(__String::createWithFormat("load%d",nIdx)->getCString(),-1,1);
	}
	if (isStart==YES)
	{
	   //�������Ա���Ľڵ㣬������ںͱ�������ͬ�Ļ������λ���ڸ����Ļ���Ҫ�Ͻ��뿪
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
	this->BackHp();//��Ѫ������
	

	

	
}


void warriorObjects::Run()//�����ܲ�
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

	ctNum=0;//����
	_isRun=true;
	_states=STATES_RUN;
	_armature->getAnimation()->play("run",-1,1);
	
}

void warriorObjects::Fight(){//����ս��

	
	_states=STATES_FIGHTING;//����ս��
	
	if (_selfType==WAR_ITEM4)//�Ա�����
	{   
		audioPlayer::getInstance()->playEffect("S34.mp3");//�Ա���Ч
		for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�6
		{
			GameObjects *obj=(GameObjects*)sp;
			if (obj->getobjectType()==TYPE_ENEMY&&obj->getisFly()!=YES)
			{
				float dist = fabs( getPositionX() - obj->getPositionX());

				if (dist<=60)
				{
					float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS1);//Ӣ��3�ı������ܶ�սʿ������Ӱ��
					 float cri=this->findBossSkillBuffValue(BOSS_ITEM5,PASSIVESKILLS2);//Ӣ��5�ڶ����������ܶ�սʿ��������Ӣ��
					  obj->setReduceHp(_atkOption,(buffValue+1)*_atk,_CRI,_CRI_damage);
					 
				}
			}
		}

		this->CreateAnimationForDeathObject(Point(0,0));

	}else if (_selfType==WAR_ITEM5)
	{
		_armature->getAnimation()->play("atk1",-1,0);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
		audioPlayer::getInstance()->playEffect("S70.mp3");//��������
	}
	
	else{
		_armature->getAnimation()->play("atk",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(warriorObjects::onFrameEvent));
	}

		
}



void warriorObjects::FlyFlyFight(){

	GameObjects *tempObj=this->FindObject(targetAddr);
	float alllen=_atkLen;
	_states=STATES_FIGHTING;//����ս��
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

void warriorObjects::SleepWar(){//˯��
	
	if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"sleep1")==0)
	{
		_armature->getAnimation()->play("sleep2",-1,1);
	}
	else if (strcmp(_armature->getAnimation()->getCurrentMovementID().c_str(),"sleep2")==0)
	{
		_armature->getAnimation()->play("sleep1",-1,1);
	}
	

	
}
void warriorObjects::CdbackLoad(){//CD��ȴ��������load

	auto action = Sequence::create(
		DelayTime::create(20.0),
		CallFunc::create([=](){
			int sIdx=CCRANDOM_0_1()*3+1;
			_states=STATES_LOAD;
			_armature->getAnimation()->play(__String::createWithFormat("load%d",sIdx)->getCString(),-1,1);
			if (mSleepLoopEffectId > 0)
			{
				audioPlayer::getInstance()->stopEffect(mSleepLoopEffectId);//ֹͣ����˯��
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


 void warriorObjects::WarStatesLoad(){//״̬�жϣ����������ͱ������ʱ��

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


void warriorObjects::upgradeObjectAnimation(){//������������ʾ�Ķ���

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
		if (strcmp(evt.c_str(),"atk1_end")==0)//��һ����
		{
			isStart=NO;
			
			this->CdbackLoad();//��ȴʱ��
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
			mSleepLoopEffectId = audioPlayer::getInstance()->playEffect("S71.mp3", true);//����˯��. ѭ������
		}

		if (strcmp(evt.c_str(),"atk_run")==0)//����Ҫ��ʼ����
		{
			this->findIsBackRunEmeny();//.....����Ѱ�Ҹ���Ҫ�����ŵĵ���
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
// 			mSleepLoopEffectId = audioPlayer::getInstance()->playEffect("S71.mp3", true);//����˯��. ѭ������
		}
	}
	  if (strcmp(evt.c_str(),"reducehp")==0)
	  {
		 
			  GameObjects *tempObj=this->FindObject(targetAddr);
			  if (tempObj==nullptr)//ûѰ�ҵ�
			  { 
				  this->load();
				
				   
			  }else{//Ѱ�ҵ�
				  if (tempObj->getPositionX()>this->getPositionX())
				  {
					  _armature->setRotationY(180);
				  }else
				  {
					   _armature->setRotationY(0);
				  }

				  this->War2Shooter(tempObj);//�����ӵ����߿�Ѫ
			
		  }
		 
	  }

	   if (strcmp(evt.c_str(),"begin")==0){
		  
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//ûѰ�ҵ�
		  { 
			  this->load(); 
			  
		  }else{//Ѱ�ҵ�

			  if (_isFly==YES&&tempObj->getisFly()==NO)
			  {
				  audioPlayer::getInstance()->playEffect("S32.mp3");//���б���½�ص���
				  Point pos=Point(this->getPositionX(),this->getPositionY());
				  Point movePos=Point(tempObj->getPositionX(),tempObj->getPositionY()+tempObj->getArmature()->getContentSize().height/2*0.7);
				  this->flyWarBombCreate2(pos,movePos,tempObj->getZOrder()+1);
			  }
		  }

	   }




}  




/*********************************************

    ����ʱ�� �����߼��ж�
	ʿ��AI����ʿ��AI�����߼����ڲ���Ĳ��ھ��䣬
	���е��˽��뾯�䷶Χ����֮����ս����ֱ��˫
	��һ��HP=0������������������һĿ�꣨����ж�
	��Ŀ����ѡ��û�н���ս����Ŀ�꣩���ظ�ս���߼�

************************************************/

void warriorObjects::warriorupdate(float dt)
{
	if (isStart==YES&&firstTimeFlag==false)
	{
		firstTimeFlag=true;
		//�������Ա���Ľڵ㣬������ںͱ�������ͬ�Ļ������λ���ڸ����Ļ���Ҫ�Ͻ��뿪
		bool is_find=this->FindSameTypeObject();
		if (is_find==true)
		{ 
			this->unschedule(schedule_selector(warriorObjects::beginWithCruising));//ȡ����ʱ��
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

	if (_layer!=nullptr&&isStart==YES)//�����ڿյ�ʱ��
	{

		bool isFind=false;//�Ƿ�Ѱ�ҵ�Ŀ��
		GameObjects *targetObj=nullptr;

		int nearestX=0;//����ж��Ŀ��Ļ���¼�����һ��Ŀ�������һ��
		bool isFight=false;//�Ƿ���ս��	 
		
		if (!_storeydata)
			return;
		if (_storeydata->getObjectData().size() > 0)
		{
			for (Ref *sp:_storeydata->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;
				
				if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&_states!=STATES_FIGHTING&&obj->getisFrozen()==NO&&
					obj->getisIntimidate()==NO&&obj->getisBackWind()==NO)//Ѱ��һ�����ˣ�û�������߲���ս����ʱ��,����ͬһ��֮��
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
							continue;//���Ծ�Ӣ���˺ͱ������Ч
						}


						int tempNearestX=dist - safeZone;//������
						
						if (nearestX==0)//����ǵ���0��ʱ��Ѱ�ҵ���һ��Ŀ��
						{  
							if (obj->getisFly()==YES&&(_selfType==WAR_ITEM3))//��������Ƿ��еĵ��˵Ļ���Ҫ���б��ſ��Թ���
							{
								isFind=true;//��ʾѰ�ҵ�
								nearestX=tempNearestX;//������
								targetObj=obj;//��ȡһ��Ŀ��
								targetAddr=(uintptr_t)targetObj;
							}else if (obj->getisFly()==NO){
								isFind=true;//��ʾѰ�ҵ�
								nearestX=tempNearestX;//������
								targetObj=obj;//��ȡһ��Ŀ��
								targetAddr=(uintptr_t)targetObj;
							}

						}else {//����Ļ���˵���ǻ��еڶ���Ŀ���˻��߶��Ŀ��,����Ƕ��Ŀ��Ļ���Ѱ��һ��û����ս��״̬�ĵ��˽��й��������뵱ǰĿ�����
							
							if (_isFly==YES)//����������Ƿ��б��Ļ�
							{
								if (targetObj->getisFly()==NO&&obj->getisFly()==YES)//��� �ʼ��ȡ���Ǹ����Ƿ��е��˵Ļ�������ǰ������Ƿ��е��˵Ļ�������ѡȡ���е���
								{	
									isFind=true;//��ʾѰ�ҵ�
									targetObj=obj;
									nearestX=tempNearestX;
									targetAddr=(uintptr_t)targetObj;

								}else if ((targetObj->getisFly()==NO&&obj->getisFly()==NO)||(targetObj->getisFly()==YES&&obj->getisFly()==YES))//�����ǰ��ǰ��ѡ��Ķ����Ƿ��е��˵Ļ����߶��Ƿ��е���
								{

									if (nearestX>tempNearestX)//���ǰһ��Ŀ�����������ڵĻ�,���������
									{
										if (targetObj->getstates()==STATES_FIGHTING)
										{
											isFind=true;//��ʾѰ�ҵ�
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;

										}
									else if (targetObj->getstates()!=STATES_FIGHTING)
									{
										if (obj->getstates()==STATES_FIGHTING&&obj->getcurTargetType()==TYPE_DOOR)
										{
											isFind=true;//��ʾѰ�ҵ�
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}
									}

									}else{//����Ǵ��ڵĻ�

										if (targetObj->getstates()==STATES_FIGHTING&&targetObj->getcurTargetType()!=TYPE_DOOR&&obj->getstates()!=STATES_FIGHTING)
										{
												isFind=true;//��ʾѰ�ҵ�
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}

									}
								}
							}else{//��������Ƿ��б��Ļ�,ҲҪ������Է��Ƿ��Ƿ��б��ͱ����Ƿ���
								if (obj->getisFly()==YES)//����Է��Ƿ��е��˵Ļ�qie
								{
								}else{

									if (_selfType==WAR_ITEM2||_selfType==WAR_ITEM4)//�ڶ�����Զ�̹����ĺͱ�ը�ģ�Ӧ���Ǵ������Ŀ���
									 {
										if (nearestX>tempNearestX)
										{
											isFind=true;//��ʾѰ�ҵ�
											targetObj=obj;
											targetAddr=(uintptr_t)targetObj;
											nearestX=tempNearestX;
										}
									}else{

										if (nearestX>tempNearestX)//���ǰһ��Ŀ�����������ڵĻ�,���������
										{
											if (targetObj->getstates()==STATES_FIGHTING)
											{
												isFind=true;//��ʾѰ�ҵ�
												targetObj=obj;
												targetAddr=(uintptr_t)targetObj;
												nearestX=tempNearestX;

											}
											else if (targetObj->getstates()!=STATES_FIGHTING)
											{
												if (obj->getstates()==STATES_FIGHTING&&obj->getcurTargetType()==TYPE_DOOR)
												{
														isFind=true;//��ʾѰ�ҵ�
													targetObj=obj;
													targetAddr=(uintptr_t)targetObj;
													nearestX=tempNearestX;
												}else if (obj->getstates()!=STATES_FIGHTING)
												{
														isFind=true;//��ʾѰ�ҵ�
													targetObj=obj;
													targetAddr=(uintptr_t)targetObj;
													nearestX=tempNearestX;
												}
											}

										}else{//�����һ��ѡȡ��Ŀ����ڵ����ϴ�ѡ���Ŀ��ľ���Ļ�

											if (targetObj->getstates()==STATES_FIGHTING&&targetObj->getcurTargetType()!=TYPE_DOOR&&obj->getstates()!=STATES_FIGHTING)
											{
												isFind=true;//��ʾѰ�ҵ�
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



		//���δѰ������Ŀ�꣬��ȫ���򣨵����������ص������е��˱��ܣ����Լ�ͣ�µȴ������ܳ���ȫ����add by xiewei��
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

		if (_isFly==YES&&targetAddr!=0)//�����Ƕ��ӵ�������
		{
			GameObjects *tempTarObj=FindObject(targetAddr);
			if (tempTarObj!=nullptr&&tempTarObj->getisFly()==NO)//˵��Ѱ�ҵ��Ķ����Ƿ��еĲ���
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


		//���Ѱ�ҵ�Ŀ���ˣ���������ǽ�սС���Ļ�������һ���жϣ����ǲ��ܶ������2����սС����һ������

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


		




		if (isFind==true&&targetAddr!=0&&_states!=STATES_FIGHTING&&targetObj!=nullptr)//���������Ŀ��Ļ�,������û��ս��״̬��ʱ��
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
			if (_selfType==WAR_ITEM4)//�Ա�����
			{
				tempAtkLen=4.0;
				movespeed+=1.5;
			}

			if (dist>=safeZone &&  dist-safeZone<=tempAtkLen &&thisPosX>leftposX&&thisPosX<rightposX)//˵���ڹ�����Χ��
			{

				if (_isFly==YES)//������Ƿ��б��Ļ�
				{
					targetObj->settargetSelect(YES);
				}
				if (_isFly==YES&&targetObj->getisFly()==YES)
				{
					this->FlyFlyFight();//����Ƿ��б�����е��˵Ļ�
				}else{
					this->Fight();//ս����
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


		//ǿ�Ʒ��б���
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

		
		


		if (isFind==false)//���ûѰ�ҵ�
		{
			
			if (isCruising==true&&_states==STATES_LOAD)//��ֹ��һ�ηŽ�ȥ��ʱ�򲻵���
			{
				isCruising=false;
				this->unschedule(schedule_selector(warriorObjects::beginWithCruising));//ȡ����ʱ��
				this->scheduleOnce(schedule_selector(warriorObjects::beginWithCruising),  CCRANDOM_0_1()*3*2.0f);
			}

			if (_states==STATES_FIGHTING&&targetAddr==0)//���û�ҵ��ұ�����ս��
			{
				this->load();
				
			}else if (_states==STATES_RUN&&_isRun==true)
			{

				ctNum++;
				if (ctNum==50)
				{
					ctNum=0;
					for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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

				if (CruisingDirectX>0)//���ұ���
				{
					_armature->setRotationY(180);
					
				}else if (CruisingDirectX<=0)//�������
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

void warriorObjects::flyWarBombCreate1(Point pos,int zord)//�������б��ı�ը
{
	
	effectsObject* effects = effectsObject::create();
	effects->setPosition(pos);
	effects->bottleExplod();
	effects->setRotationY(_armature->getRotationY());
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,zord);
	effects->setScale(0.8);
}

void warriorObjects::flyWarBombCreate2(Point pos,Point movePos, int zord)//�������б��ı�ը
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
	for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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

	for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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


void warriorObjects::War2Shooter(GameObjects *tempObj){//�����ӵ�
	if (_selfType==WAR_ITEM2)//�ڶ�����Զ�̹�����
	{
		audioPlayer::getInstance()->playEffect("S30.mp3");//�ڶ���Զ�̱��Ĵ򶷵���Ч

		int d=D_LEFT;
		if (tempObj->getPositionX()>this->getPositionX())
		{
			d=D_RIGHT;
		}
		ballObjects*ball=ballObjects::create(targetAddr,tempObj->getPosition(),BALL_WAR02,_floorNum,d);
		ball->setb_cri(_CRI);
		ball->setb_cri_damage(_CRI_damage);

		if (tempObj->getPositionX()>this->getPositionX())//�������ұ�
		{
			ball->setPosition(this->getPositionX()+_armature->getContentSize().width*_armature->getScale()-10,_armature->getContentSize().height/2*_armature->getScale()-7);

		}else//���������
		{
			ball->setPosition(this->getPositionX()-_armature->getContentSize().width*_armature->getScale()+10,_armature->getContentSize().height/2*_armature->getScale()-7);
		}
		ball->setb_atk(_atk);
		ball->setb_atkOption(_atkOption);
		ball->setB_shooterPos(this->getPosition());
		Widget *w=static_cast<Widget*>(this->getParent());
		w->addChild(ball, 8);

	}else{
		float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS1);//Ӣ��3�ı������ܶ�սʿ������Ӱ��
		float cri=this->findBossSkillBuffValue(BOSS_ITEM5,PASSIVESKILLS2);//Ӣ��5�ڶ����������ܶ�սʿ��������Ӣ��
	
		tempObj->setReduceHp(_atkOption,_atk*(1+buffValue),_CRI+cri,_CRI_damage);


		if (_selfType==WAR_ITEM1)
		{
			audioPlayer::getInstance()->playEffect("S29.mp3");//��ս����
		}
	
		if (_selfType==WAR_ITEM3)
		{
			audioPlayer::getInstance()->playEffect("S31.mp3");//���б��Է��е���
		}
		
		}
		
	}

void warriorObjects::findIsBackRunEmeny(){//Ѱ�ұ����ŵĵ���

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

	
	for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�,�ȸ㶨������Ǹ�ѡ�е�
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
	
	for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�,�ȸ㶨������Ǹ�ѡ�е�
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