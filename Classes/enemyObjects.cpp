#include "enemyObjects.h"
#include "effectsObject.h"
#include "Event1Object.h"
#include "magicObjects.h"
USING_NS_CC;


enemyObjects::enemyObjects(void)
	:_clippingEnabled(false),_leftDoorPoint(Point::ZERO),_rightDoorPoint(Point::ZERO),_moveDirection("right")
{
	
}

enemyObjects::~enemyObjects(void)
{
	
	
}
// on "init" you need to initialize your instance

bool enemyObjects:: enemyObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
    //////////////////////////////
	_MisPixel=0;
	targetAddr=0;//默认等于0
	_objectType=TYPE_ENEMY;
	_selfType=otype;
	_layer=_l;
	_targetSelect=NO;
	_oname=name;
	_oFileName=filename;
	ttAddr=0;
	_lev=1;
	_GuangQuanArmature = nullptr;
	m_isScoffFlag=false;
	_isIntimidate=NO;
	_states=STATES_DEFAULT;//设置是默认的
	setIsEvent106_Object(false);

	_isRun=false;//设置是否在跑，默认是停止的

	_curTargetType=-1;//默认是 -1 方便计算
	frozenArm=nullptr;
	_isFly = NO;
	if (_selfType==ENEMY_ITEM13||_selfType==ENEMY_ITEM14||_selfType==ENEMY_ITEM15||_selfType==ENEMY_ITEM16)//这是飞行敌人的
	{
		_isFly=YES;
	}

	this->CreateArmature(filename,name);
	this->setProperty(1);
	
	this->schedule(schedule_selector(enemyObjects::enemyupdate),(float) 1.0/60);
    return true;
}



enemyObjects* enemyObjects::create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
	enemyObjects *sprite = new enemyObjects();
    if (sprite && sprite->init())
    {
		sprite-> enemyObjectsInit(filename,name,otype,_l);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}



//设置属性
void enemyObjects::setProperty(int fnum)
{
	//暂时用的属性
	_maxHp=400;
	_hp=_maxHp;
	_atk=60;
	_dfe=20;
	_lev=1;
	_atkLen=_armature->getContentSize().width/2*_armature->getScale();
	_moveSpeedScale=1;//设置默认的移动的速度率敌
	_floorNum=fnum;

	if (_selfType==20){
		_atkLen=_armature->getContentSize().width/2*_armature->getScale()+200;
		_moveSpeedScale = 0.4f;
	}
	updateLevel(_lev);
	
	               
}

void enemyObjects::setMoveDirection(string direction)
{
	if (_moveDirection==direction)
		return;

	_moveDirection = direction;
	if (strcmp("right",direction.c_str())==0){
		_armature->setRotationY(0);
	}else if ( strcmp("left",direction.c_str())==0 ){
		_armature->setRotationY(180);
	}
}

string enemyObjects::getMoveDirection()
{
	return _moveDirection;
}


//创建骨骼动画相关函数 
void enemyObjects::CreateArmature(const std::string& filename,const std::string&name ){
	
isArmature=true;//设置为true
ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
_armature = Armature::create(name);   
_armature->getAnimation()->setAnimationScale((float)0.8);

//_armature->setScale(0.8);不必要在这里缩放了，已经在导出骨骼动画缩小到了0.8
_armature->setPosition(0,0);

this->addChild(_armature,1);
this->load();
if (_selfType==ENEMY_ITEM20){

	_armature->getAnimation()->setAnimationScale(0.5f);
}

if (_selfType==ENEMY_ITEM16)//飞行王太大了，缩小
{
	_armature->setScale(0.5/0.8); 
	
}
}


//骨骼动画的触摸事件
void enemyObjects::CreateListenerTouch_Armature(){

	
	 listener = EventListenerTouchOneByOne::create();//创建一个触摸监听
	 CC_SAFE_RETAIN(listener);
	 //3.0 后可以直接在touchBegan后添加它的实现代码，而不用特意去写一个touchBegan的函数
    listener->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Armature*>(event->getCurrentTarget());//获取的当前触摸的目标
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width*target->getScale(), s.height*target->getScale());

    if (rect.containsPoint(locationInNode)){//判断触摸点是否在目标的范围内
	
		return true;
	}
    else{
       	  return false;
	}   };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener ,_armature);  
	
}

//这个是删除触摸事件的
void enemyObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
}

//序列帧动画

Animate* enemyObjects::CreateAnimation(const char*FrameName,int max){

	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.08f);
	Animate *animate = Animate::create(animation);

	return animate;

}

/*******************************
      移动
********************************/

void enemyObjects::load(){
	ttAddr=0;
	_states=STATES_LOAD;
	targetAddr=0;
	_armature->getAnimation()->play("load",-1,1);

	
}
void enemyObjects::Run()//正在跑步
{
	ttAddr=0;
	_isRun=true;
	_states=STATES_RUN;
	_armature->getAnimation()->play("run",-1,1);
	_armature->getAnimation()->setSpeedScale((float)0.8);
	
	
	
}

void enemyObjects::Fight(){//正在战斗
	_states=STATES_FIGHTING;//正在战斗
	_armature->getAnimation()->play("atk",-1,1);
	_armature->getAnimation()->setAnimationScale((float)0.9);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(enemyObjects::onFrameEvent)); 
	int FindNum=0;



}   
void enemyObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	//CCLOG(" enemy (%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	if (strcmp(evt.c_str(),"load1finish")==0)//变成羊的
	{
		if (sheepArmature!=nullptr)
		{  
			
			sheepArmature->getAnimation()->play("load2",-1,-1);
		}
		
	}

	if (strcmp(evt.c_str(),"trap_finish")==0)//冰冻陷阱的
	{
		frozenArm->getAnimation()->stop();
		frozenArm->removeAllChildrenWithCleanup(true);
		this->Run();
		isStart=YES;
		_isFrozen=NO;
		if (_armature!=nullptr)
		{
			_armature->getAnimation()->resume();
		}
	}

	if (strcmp(evt.c_str(),"reducehp")==0)
	{
		if (_selfType==ENEMY_ITEM20){
			this->enymyItem20AttackEffect(0.5f);
		}else if (_selfType==ENEMY_ITEM17 || _selfType==ENEMY_ITEM18 || _selfType==ENEMY_ITEM19){
			this->enymyItem17to19AttackEffect(0.5f);
		}
	}

	if (strcmp(evt.c_str(),"reducehp")==0)
	{
		GameObjects *obj=FindObject(targetAddr);
		if (obj==nullptr)//
		{
			this->load();
		    
		}else{
			if (obj->gethp()>0)
			{
				
				obj->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);

				if (obj->getobjectType()==TYPE_BOSS&&obj->getSelfType()==BOSS_ITEM4)//肉盾英雄的，有反弹伤害
				{
				   float hurt=_atk*obj->getskill3_buffValue();//反弹伤害的值
				   this->reduceHpEffect(hurt);
				   
				}

			}
		}
	}


	if (strcmp(evt.c_str(),"begin")==0){

		GameObjects *obj=FindObject(targetAddr);
		if (obj==nullptr)
		{
           this->load();
		  
		}
	}

}  





/*********************************************

    主定时器 各种逻辑判定，敌人的AI 描述
 敌人AI——敌人AI基本逻辑，前进一直进攻到最下层，
 前进中搜索所有士兵，防御塔或BOSS目标，当发现目标
 后与之战斗，直至对方HP=0结束，结束后优先执行前进
 命令，当前方有目标且该目标没有在战斗状态时则与前
 方新目标进行战斗，如在前进过程中被后方的敌人攻击
 则停止前进与后方敌人进行战斗。重复战斗逻辑。敌人
 进攻时采用1吸引1策略，当一个敌人被我方单位（兵，
 塔，BOSS）吸引后，其他敌人就会继续前进，不再和我
 方的此单位纠缠。

************************************************/
void enemyObjects::delayStart(){
	isStart=YES;
	ttAddr=0;
}
void enemyObjects::enemyupdate(float dt)
{


	if (isStart==NO&&ttAddr!=0&&_hp>0&&_armature!=nullptr)
	{
		
		setMoveDirection("right");
		this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
	}

	if (isStart==YES&&_isIntimidate==YES&&_hp>0)//被小狗恐吓后往后跑的
	{
		if (_lev<=2)
		{
			setMoveDirection("left");
			this->setPosition(this->getPosition().x-(_moveSpeedScale+1.0),this->getPosition().y);
		}
		if(_lev==3){
			this->newAiForBackRun();
		}


		if (this->getPositionX()+_armature->getContentSize().width/4  < this->getLeftDoorPoint().x ){
			isStart=NO;
		
		   this->reduceHpEffect(_hp);//把自己的血减掉为0，自杀删掉*/
// 			Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
// 			Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
// 			_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
			}
	}


	if (isStart==YES&&_isIntimidate==NO)//不等于空的时候
	{

		//如果是宝箱怪，而且血量小于百分之三十
		if (_selfType == ENEMY_ITEM21 && _hp<=0.3*_maxHp){
			//_armature->setRotationY(180);
			if (getMoveDirection()=="right"){
				if (!getChildByTag(10000)){
					this->load();

					Sprite* sp = Sprite::create("GanTanHao005.png");
					sp->setPosition(Point(-80,_armature->getContentSize().height-50));
					sp->setFlipX(true);
					sp->setTag(10000);
					this->addChild(sp,10);
	
					sp->runAction(Sequence::create(FadeIn::create(0.2f),DelayTime::create(1.0f),FadeOut::create(0.2f),CallFunc::create([=](){
						  setMoveDirection("left");
					}),NULL));
				}
				
				return;
			}

			this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
			if (_states!=STATES_RUN)
			{
				this->load();
				this->Run();
			}

			return;
		}

		
		bool isFind=false;//设置变量是否可以寻找到
		int isFindTower3=NO;
		GameObjects *targetObj=nullptr;
		int nearestX=0;//如果有多个目标的话记录最近的一个目标最近的一个
		
		int DoorAddr=0;
		int DoorPosX=0;
		
		_storeydata = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());

		if (!_storeydata)
			return;
		if (_storeydata->getObjectData().size() > 0)
		{
			_storeydata->sortAllChildren();

			for (Ref *sp:_storeydata->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;

				int optype=obj->getobjectType();
				if (optype==TYPE_DOOR)
				{
					DoorAddr=(uintptr_t)obj;
					DoorPosX=obj->getPositionX();
				}

				this->checkIsMagicTrans(obj);
				this->checkIsEvent1(obj);
			
				if (m_isScoffFlag==true&&optype==TYPE_BOSS&&obj->getSelfType()==BOSS_ITEM4)//是否被嘲讽了，呵呵
				{
					targetObj=obj;
					isFind=true;//表示寻找到
					break;
				}

				bool isGollis=false;
				if (optype==TYPE_MAGIC)
				{
					magicObjects *migc=(magicObjects*)(obj);
					if (migc->getSelfType()==MAGIC_GODZILLA||migc->getSelfType()==MAGIC_BOSS5SKILL)
					{
						isGollis=true;
						
					}
				}

				
				float thisPosX=this->getPositionX();
				float moveWidthCenterX=_swg->getPositionX();
				if ((optype==TYPE_WARRIOR||optype==TYPE_TOWER||optype==TYPE_BOSS||isGollis==true||optype==TYPE_ENEMY)&&thisPosX>(moveWidthCenterX-_swg->getSize().width/2-50)&&_floorNum==obj->getfloorNum()
					&&obj->getisStart()==YES&&obj->gethp()>0)
				{
					bool isBombWar=false;
					if (optype==TYPE_WARRIOR&&obj->getSelfType()==WAR_ITEM4)//判断是否是自爆小兵
					{
						isBombWar=true;
					}

					/*****************************************************************************************
					 
					 ****************************************************************************************/
					if (_states!=STATES_FIGHTING&&isBombWar==false){//针对胖子的防御塔的--------
						float thisPosX=this->getPosition().x;
						float objPosX=obj->getPosition().x;
						int tempNearestX=fabs(obj->getPositionX()-this->getPositionX());//距离差距
					
						if (_isFly==NO&&obj->getobjectType()==TYPE_TOWER&&obj->getSelfType()==TOWER_ITEM3&&obj->gethp()>0&&objPosX>thisPosX)//下面这几句代码是用于防御塔3那个胖子吸引敌人打的
						{
							float ttdis=100;
							if ((obj->getlocationIndx()==2||obj->getlocationIndx()==3)&&_atkLenOption!=ATKLEN_NEAR)
							{
								ttdis=270;
								
							}if (_atkLenOption==ATKLEN_NEAR)
							{
								ttdis=70;
							}

							if (obj->gettower3_selectNum()<obj->gettower3_selectNumMax()&&tempNearestX<=ttdis)//80
							{
								targetObj=obj;
								targetAddr=(uintptr_t)obj;
								isFind=true;//表示寻找到
								isFindTower3==YES;
								nearestX=tempNearestX;//距离差距
								if (obj->getlocationIndx()==1||_atkLenOption==ATKLEN_NEAR)
								{
									targetObj->settargetAddr((uintptr_t)this);
								}
								
							}
						}

						if (obj->gettargetAddr()==(uintptr_t)this&&isFindTower3==NO){//可能会是多个战士都选中的是敌人，所以这里也要做判断
							
							if ((_isFly==NO&&obj->getisFly()==NO)||(_isFly==YES&&obj->getisFly()==YES)||(_isFly==YES&&obj->getSelfType()==MAGIC_BOSS5SKILL)){
								if (nearestX==0){//表示只有一个
									targetObj=obj;
									isFind=true;//表示寻找到
									nearestX=tempNearestX;//距离差距
								}
								else{//表示还有第二个
									if (nearestX>tempNearestX){//如果前一个目标的相差距离大于的话,
										targetObj=obj;
										isFind=true;//表示寻找到	
										nearestX=tempNearestX;//距离差距
										
									}
									
								}
							}
						}




					}
					
				}

			}
		}

		bool isSameTarget=true;//防止来回走动
      if (ttAddr!=0&&targetObj!=nullptr&&_curTargetType!=TYPE_DOOR)
      {
		  if (ttAddr!=(uintptr_t)targetObj)
		  {
			  isSameTarget=false;
			  isStart=NO;
			  auto delay=DelayTime::create(2.0);
			  auto ddlay = Sequence::create(
				  delay,
				  CallFuncN::create(CC_CALLBACK_0(enemyObjects::delayStart,this)),
				  NULL);
			  this->runAction(ddlay);
			
		  }
      }

		if (isFind==true&&targetObj!=nullptr&&_states!=STATES_FIGHTING&&targetObj->getisStart()==YES&&isSameTarget==true)//如果发现有目标的话,且自身没有战斗状态的时候
		{
		
			float thisPosX=this->getPosition().x;
			float objPosX=targetObj->getPosition().x;
			float tempAtklen=_atkLen;
			if (_isFly==YES) //如果本身是飞行的话，打门要进点距离打，要不效果很挫
			{
				tempAtklen=_armature->getContentSize().width/2*_armature->getScale();
			}

			bool isOk=false;
			if (targetObj->getobjectType()==TYPE_TOWER&&(targetObj->getSelfType()==TOWER_ITEM3||targetObj->getSelfType()==TOWER_ITEM2||targetObj->getSelfType()==TOWER_ITEM4 ||
				targetObj->getSelfType()==TOWER_ITEM5))//第二，三防御塔是肉盾的
			{
				isOk=true;
			}else{
				if (targetObj->getstates()==STATES_FIGHTING)
				{
					isOk=true;
				}
				if (targetObj->getobjectType()==TYPE_WARRIOR&&(targetObj->getSelfType()==WAR_ITEM1||targetObj->getSelfType()==WAR_ITEM5)&&_atkLenOption==ATKLEN_REMOTE)//如果本身的远程的魔法小兵而对方是近程的战士的话
				{
					isOk=true;
				}
			}

			float dist = fabs( thisPosX- objPosX );
			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
							 targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
							 LEN_SCALE;

			if (targetObj->getobjectType()==TYPE_TOWER&&targetObj->getSelfType()==TOWER_ITEM3)//防止胖子太大安全范围导致来回走动
			{
				safeZone=70;
			}

			if (dist>=safeZone && dist-safeZone<=tempAtklen &&isOk==true)//说明在攻击范围内
			{
				
				targetAddr=(uintptr_t)targetObj;
				_curTargetType=targetObj->_objectType;//记住攻击目标的类型
				this->Fight();//战斗啦
				
				//向前一点打
				if (targetObj->getobjectType()==TYPE_TOWER&&targetObj->getSelfType()==TOWER_ITEM3&&(targetObj->getlocationIndx()==2||targetObj->getlocationIndx()==3)&&_atkLenOption!=ATKLEN_NEAR)
				{
					this->setPositionX(this->getPositionX()+5.0);
				}

				if (thisPosX>objPosX)
				{
					//_armature->setRotationY(180);
					setMoveDirection("left");
					
				}else{
					//_armature->setRotationY(0);
					setMoveDirection("right");
				}
			}

			if (targetObj->getobjectType()!=TYPE_DOOR){

				if ( (dist-safeZone>tempAtklen))
				{
					if (thisPosX>objPosX)
					{
						
						//_armature->setRotationY(180);
						setMoveDirection("left");
						this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
						if (_states!=STATES_RUN)
						{
							this->Run();
						}
						
					}
					else{
					
						this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
						//_armature->setRotationY(0);
						setMoveDirection("right");
						if (_states!=STATES_RUN){
							this->Run();
						}
					}   

				}
				else if(dist<safeZone)
				{
 					if (thisPosX>objPosX)
					{
					//CCLOG("dt=%f type=%d target=%d dist=%f safeZone=%f 55555555555",dt,targetObj->getobjectType(),(int)targetObj,dist,safeZone);
						//_armature->setRotationY(0);
						setMoveDirection("right");
						this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
						if (_states!=STATES_RUN)
						{
							this->Run();
							
						}
					}
					else{
						ttAddr=(uintptr_t)(targetObj);
						this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
						setMoveDirection("left");
						if (_states!=STATES_RUN){
							this->Run();
							
						}
					}   
				}

			}
		}



		if (isFind==false)//没找到的时候....................................................
		{		
			if (_states==STATES_RUN&&(targetAddr==0||targetAddr==DoorAddr)){
				this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
				
				if ((int)_armature->getRotationY()==180)
				{
					setMoveDirection("right");
				}
			}

			if (_states==STATES_RUN&&targetAddr!=0)
			{
				this->load();
			}
			
			if (_states==STATES_FIGHTING&&targetAddr!=0 && targetAddr!=DoorAddr)
			{
				GameObjects *obj=FindObject(targetAddr);
				if (obj!=nullptr&&obj->getArmature()!=nullptr)
				{
					float thisPosX=this->getPosition().x;
					float objPosX=obj->getPosition().x;
					float dist = fabs( thisPosX- objPosX );
					float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
									 obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale() +
									 LEN_SCALE;
					float tempAtklen=_atkLen;
					if (_isFly==YES) //如果本身是飞行的话，打门要进点距离打，要不效果很挫
					{
						tempAtklen=_armature->getContentSize().width/2*_armature->getScale();
					}

					if (obj->getobjectType()==TYPE_TOWER&&obj->getSelfType()==TOWER_ITEM3)//防止胖子太大导致敌人来回走动
					{

						safeZone=70;
					}

					if ( ( dist-safeZone > tempAtklen || dist<safeZone ) &&obj->gethp()>0)
					{
						this->Run();
					
						
					}
				}

			}

			if (_states==STATES_FIGHTING&&targetAddr==0&&_states!=STATES_RUN)//如果没找到且本身在战斗
			{
				this->Run();
				
				if ((int)_armature->getRotationY()==180)
				{
					//_armature->setRotationY(0);
					setMoveDirection("right");
				}
			}
			else if (_states==STATES_LOAD&&targetAddr==0&&_states!=STATES_RUN){
				this->Run();
				
				if ((int)_armature->getRotationY()==180)
				{
					//_armature->setRotationY(0);
					setMoveDirection("right");
				}
			}
		}
	}
}

void enemyObjects::bossAtkEffect(Point BossPos){//显示特效。。。。。。。。。
	Sprite *sp=Sprite::createWithSpriteFrameName(CCString::createWithFormat("%s%02d.png","td_bossEffects1_02_",1)->getCString());
	sp->setPosition(0,_armature->getContentSize().height/3);
	this->addChild(sp,2);
	Animate *ani=this->CreateAnimation("td_bossEffects1_02_",6);
	auto action = Sequence::create(ani,CallFuncN::create(CC_CALLBACK_1(enemyObjects::bossAtkEffectCallback, this)),NULL);
	sp->runAction(action);
	
}
void enemyObjects::bossAtkEffectCallback(Ref *psend){

	Sprite *psobject=dynamic_cast<Sprite*>(psend);
	this->removeChild(psobject,true);
	
}

void enemyObjects::enymyItem20AttackEffect(float dt)
{
	Point pos = getPosition();
	bool hasAttackObj=false;
	if (!_storeydata)
		return;

	for (Ref *sp:_storeydata->getObjectData())
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((uintptr_t)obj==targetAddr)
		{
			pos = Point(obj->getPositionX()+obj->getContentSize().width/2,
						obj->getPositionY()+obj->getContentSize().height/2);
			hasAttackObj=true;
			break;
		}
	}
	if (!hasAttackObj)
		return;

	Point beginPos = Point(getPositionX(),_armature->getContentSize().height*0.8);
	double angleNum = pos.x>beginPos.x? abs(atan((pos.y-beginPos.y)/(pos.x-beginPos.x))*180/3.1415927):
										180-abs(atan((pos.y-beginPos.y)/(pos.x-beginPos.x))*180/3.1415927)	;
	
	pos =  Point(pos.x - 115*cos(angleNum*3.1415927/180),
				 pos.y + 115*sin(angleNum*3.1415927/180));
	
	auto effects = effectsObject::create();
	effects->setAnchorPoint(Point(0,0.5f));
	effects->setPosition(beginPos);
	effects->setRotation(angleNum);
	effects->enemyItem20AttackEffects();
	effects->runAction(MoveTo::create(0.5f,pos));
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,10);
}

void enemyObjects::enymyItem17to19AttackEffect(float dt)
{
	Point pos = getPosition();
	bool hasAttackObj=false;
	if (!_storeydata)
		return;

	for (Ref *sp:_storeydata->getObjectData())
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((uintptr_t)obj==targetAddr)
		{
			pos = Point(obj->getPositionX()+obj->getContentSize().width/2,
						obj->getPositionY()+obj->getContentSize().height/2);
			hasAttackObj=true;

			if (obj->getobjectType()==TYPE_WARRIOR && obj->getSelfType()==WAR_ITEM1)
			{
				pos.x -=25;
				pos.y +=25;
			}
			break;
		}
	}
	if (!hasAttackObj)
		return;

	Point beginPos = Point(getPositionX(),_armature->getContentSize().height*0.7f);
	double angleNum = pos.x>beginPos.x? abs(atan((pos.y-beginPos.y)/(pos.x-beginPos.x))*180/3.1415927):
		180-abs(atan((pos.y-beginPos.y)/(pos.x-beginPos.x))*180/3.1415927)	;
	
	auto effects = effectsObject::create();
	effects->setAnchorPoint(Point(0.5f,0.5f));
	effects->setPosition(beginPos);
	effects->setRotation(angleNum);
	effects->enemyItem17AttackEffects();
	effects->runAction(MoveTo::create(0.5f,pos));
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,10);
}

const std::string enemyObjects::getFileName(){

	return _filename;
}
const std::string enemyObjects::getName(){

	return _name;
}

GameObjects * enemyObjects::FindObject(int addr){

	GameObjects *pObj=nullptr;
	for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((uintptr_t)obj==addr&&obj->gethp()>0)
		{
			if (_isIntimidate==YES&&obj->getisIntimidate()==NO)
			{
				pObj=obj;
				break;
			}
			if (_isIntimidate==NO)
			{
				pObj=obj;
				break;
			}
			
		}
	}
	return pObj;
}


void enemyObjects::FrozenMyself(float dt,bool isFirst){//被冰冻
	this->load();
	isStart=NO;
	_isFrozen=YES;
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->pause();
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_trap_020.png","td_trap_020.plist","td_trap_02.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	frozenArm = cocostudio::Armature::create("td_trap_02");   
	frozenArm->setPosition(0,-4);
	frozenArm->setScale(1.0);
	this->addChild(frozenArm,10);
	frozenArm->getAnimation()->play("frozen",-1,1);

	effectsObject* ring = effectsObject::create();
	ring->setPosition(Point(0,frozenArm->getContentSize().height/2));
	frozenArm->addChild(ring,10);

	effectsObject* born = effectsObject::create();
	born->setPosition(Point(0,frozenArm->getContentSize().height/2));
	frozenArm->addChild(born,11);

	ring->trap02_ring_effect();
	born->trap02_born_effect();

	ring->setScale(0.8f);
	born->setScale(0.8f);
	if (!isFirst){
		ring->setVisible(false);
		born->setScale(0.7f);
	}

	if (_selfType==ENEMY_ITEM4)
	{
		frozenArm->setScale(1.8);
	}
	if (_selfType==ENEMY_ITEM6||_selfType==ENEMY_ITEM7)
	{
		frozenArm->setScale(1.4);
	}
	if (_selfType==ENEMY_ITEM8)
	{
		frozenArm->setScale(2.0);
	}

	if (_selfType==ENEMY_ITEM12)
	{
		frozenArm->setScale(1.5);
	}
	if (_selfType==ENEMY_ITEM20)
	{
		frozenArm->setScale(2.0);
	}


	auto delay=DelayTime::create(dt);
	auto ddlay = Sequence::create(
		delay,
		CallFuncN::create(CC_CALLBACK_1(enemyObjects::FrozenBomb,this)),
		NULL);
	this->runAction(ddlay);

}
void enemyObjects::FrozenBomb(Ref*psender){
	frozenArm->getAnimation()->play("bomb",-1,0);
	frozenArm->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(enemyObjects::onFrameEvent));
}


void enemyObjects::speedDown(float t,float buffValue){//被减速的时候

	_isSpecialColorFlag=true;
	CCTintTo *tint=CCTintTo::create(0.1,69,253,253);
	auto delay=DelayTime::create(t);
	auto ddlay = Sequence::create(tint,
		delay,
		CallFuncN::create(CC_CALLBACK_1(enemyObjects::speedDownCallback,this)),
		NULL);
	_armature->runAction(ddlay);

	pre_movespeed=_moveSpeedScale;
	_isSpeedDown=YES;
	_moveSpeedScale=(1.0-buffValue)*_moveSpeedScale;
	_armature->getAnimation()->setSpeedScale(1.0-buffValue);

}

void  enemyObjects::speedDownCallback(Ref*r){
	_isSpeedDown=NO;
	_moveSpeedScale=pre_movespeed;
	_armature->getAnimation()->setSpeedScale(1.0);
	CCTintTo *tint=CCTintTo::create(0.1,255,255,255);
	_armature->runAction(tint);
	_isSpecialColorFlag=false;
}


//被弹上去的效果
void enemyObjects::trap4FlyUp(){
	this->load();
	_armature->getAnimation()->stop();
	auto moveup=MoveBy::create(0.2f,Point(0,200));
	auto act = Sequence::create(
		moveup,
		CallFuncN::create(CC_CALLBACK_1(enemyObjects::trap4FlyUpCallback,this)),
		NULL);
	this->runAction(act);
}
void enemyObjects::trap4FlyUpCallback(Ref *r){

	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->toUpStorey(this);

}

int enemyObjects::GetWarAtkMeNum()//如果有2个小兵在攻击我了，就不能再攻击我了
{
	int num=0;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((obj->getobjectType()==TYPE_WARRIOR&&obj->getSelfType()==WAR_ITEM1)&&obj->gettargetAddr()==(uintptr_t)this&&obj->getstates()==STATES_FIGHTING)
		{
			num++;
		}
	}
	return num;
}

void enemyObjects::visit(Renderer *renderer, const kmMat4 &parentTransform, bool parentTransformUpdated)
{
	bool isFlag=true;
	 
	if ( this->getfloorNum()!= SCENE_STOREY_GROUND && 
		(this->getPositionX()-_armature->getContentSize().width*_armature->getScale()/2  < this->getLeftDoorPoint().x || this->getPositionX()+_armature->getContentSize().width*_armature->getScale()/2 > this->getRightDoorPoint().x)) {
		_beforeVisitCmdScissor.init(_globalZOrder);
		_beforeVisitCmdScissor.func = CC_CALLBACK_0(enemyObjects::onBeforeVisitScissor, this);
		renderer->addCommand(&_beforeVisitCmdScissor);
		Sprite::visit(renderer, parentTransform, parentTransformUpdated);
		_afterVisitCmdScissor.init(_globalZOrder);
		_afterVisitCmdScissor.func = CC_CALLBACK_0(enemyObjects::onAfterVisitScissor, this);

		
		renderer->addCommand(&_afterVisitCmdScissor);
	} else {
		Sprite::visit(renderer, parentTransform, parentTransformUpdated);
	}
}

void enemyObjects::onBeforeVisitScissor()
{       
	   float tw=0,th=0;
		if (_isIntimidate==YES)
		{
			th=80;
		}
		if (_selfType==ENEMY_ITEM4)
		{
			tw=80;
		}
	glEnable(GL_SCISSOR_TEST);
	auto glview = Director::getInstance()->getOpenGLView();
	if (this->getPositionX()-_armature->getContentSize().width*_armature->getScale()/2  < this->getLeftDoorPoint().x ){
		
		Point worldPos = convertToWorldSpace(Point::ZERO);
		glview->setScissorInPoints( 
			worldPos.x - (this->getPositionX()-getLeftDoorPoint().x),
			worldPos.y,
			this->getPositionX()-this->getLeftDoorPoint().x+_armature->getContentSize().width*_armature->getScale()/2+tw,
			_armature->getContentSize().height*_armature->getScale() +th
			);
	} else if (this->getPositionX()+_armature->getContentSize().width*_armature->getScale()/2 > this->getRightDoorPoint().x) {
		Point worldPos = convertToWorldSpace(Point::ZERO);
		glview->setScissorInPoints( 
			worldPos.x-_armature->getContentSize().width*_armature->getScale()/2,
			worldPos.y,
			this->getRightDoorPoint().x-(this->getPositionX()-_armature->getContentSize().width*_armature->getScale()/2+tw),
			_armature->getContentSize().height*_armature->getScale()+th );
	}
}

void enemyObjects::onAfterVisitScissor()
{
	glDisable(GL_SCISSOR_TEST);
}


void enemyObjects::boss2SkillEffect(float f){//被震昏的时间

	this->load();
	isStart=NO;
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->pause();
	}

	boss2Effct=Sprite::createWithSpriteFrameName("td_boss2_skill_vertigo_01.png");
	boss2Effct->setPosition(0,_armature->getContentSize().height/4*3);
	this->addChild(boss2Effct);

	Animate *ani=this->CreateAnimation("td_boss2_skill_vertigo_",8);

	RepeatForever *repeat = RepeatForever::create(ani);  
	boss2Effct->runAction(repeat);

	auto delay=DelayTime::create(f);
	auto ddlay = Sequence::create(
		delay,
		CallFuncN::create(CC_CALLBACK_1(enemyObjects::boss2skillCallBack,this)),
		NULL);
	this->runAction(ddlay);
}
void enemyObjects::boss2skillCallBack(Ref *sender){//回调函数

	boss2Effct->removeFromParent();
	this->Run();
	isStart=YES;
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->resume();
	}
}

//被嘲讽的技能的实现
void enemyObjects::boss4SkillEeffect(float f)//f 被嘲讽的时间
{

	this->load();
	m_isScoffFlag=true;
	
	scoff=Sprite::createWithSpriteFrameName("td_boss4_fennu_01.png");
	scoff->setPosition(0,_armature->getContentSize().height/4*3+20);
	scoff->setScale(0.6);
	this->addChild(scoff);
	
	Animate *ani=this->CreateAnimation("td_boss4_fennu_",4);
	RepeatForever *repeat = RepeatForever::create(ani);  
	scoff->runAction(repeat);

	auto delay=DelayTime::create(f);
	auto ddlay = Sequence::create(
		delay,
		CallFuncN::create(CC_CALLBACK_1(enemyObjects::boss4SkillEffectCallback,this)),
		NULL);
	this->runAction(ddlay);


}
void enemyObjects::boss4SkillEffectCallback(Ref *sender)//被嘲讽的回调函数
{
	scoff->removeFromParent();
	this->load();
	m_isScoffFlag=false;
}

//漏到下一层
void enemyObjects::checkIsEvent1(GameObjects *obj){//检查是否碰到了漏到下一层的
	if (obj->getobjectType()==TYPE_EVENT1)//如果碰到了漏洞到下一层的东东
	{
		// 这几种类型不漏下去
		if (this->getSelfType() == ENEMY_ITEM4 || this->getSelfType() == ENEMY_ITEM8 || this->getSelfType() == ENEMY_ITEM12 || this->getSelfType() == ENEMY_ITEM16 || this->getSelfType() == ENEMY_ITEM20
			|| this->getSelfType() == ENEMY_ITEM21)
		{
			return;
		}
		event1Object *eObj=static_cast<event1Object*>(obj);

		float x1= this->getPosition().x + this->getArmature()->getContentSize().width/2;	
		float x2= obj->getPosition().x + obj->getArmature()->getContentSize().width/2;
		float dist=fabs(x1-x2);
		if (eObj->getm_CanDownFlag()==true&&dist<2.0)//如果是可以掉下去的话
		{
			this->downToNextStoreyBegin(obj);
		}
	}
}
void enemyObjects::downToNextStoreyBegin(GameObjects* _eventObj){
	if (!_eventObj)
		return;

	int rom=CCRANDOM_0_1()*100;
	if (rom<30&&_isFly==NO&&_selfType!=ENEMY_ITEM4&&_selfType!=ENEMY_ITEM8&&_selfType!=ENEMY_ITEM12&&_selfType!=ENEMY_ITEM16)
	{
		auto _event1StoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_eventObj->getfloorNum());
		if (_event1StoreyData)
		{	
			if (!_GuangQuanArmature)
			{
				int _nextStoreyIndex = SceneData::getInstance()->getNextStoreyIndex(this->getfloorNum());
				if (_nextStoreyIndex >= SCENE_STOREY_HOME)
				{
					StoreyData* _nextStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_nextStoreyIndex);
					_nextStoreyData->setLineQZVisible(true);
				}
				// 创建个光圈动画
				ArmatureDataManager::getInstance()->addArmatureFileInfo("td_shentou0.png","td_shentou0.plist","td_shentou.ExportJson"); 
				_GuangQuanArmature = cocostudio::Armature::create("td_shentou");  
				_GuangQuanArmature->setTag(133);
				_event1StoreyData->mItem->addChild(_GuangQuanArmature, 13);
				_GuangQuanArmature->setPositionX(_eventObj->getPositionX());
				_GuangQuanArmature->setPositionY(_eventObj->getPositionY());
				_GuangQuanArmature->getAnimation()->play("shine", -1, 0);
				_GuangQuanArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(enemyObjects::onAnimationFrameEvent_GuangQuan));
				isStart=NO;
				auto delay=DelayTime::create(0.2f);
				auto ddlay = Sequence::create(
					delay,
					CallFuncN::create(CC_CALLBACK_1(enemyObjects::downToNextStoreyEnd,this, nullptr)),
					NULL);
				this->runAction(ddlay);
			}
		}
	}
}
void enemyObjects::onAnimationFrameEvent_GuangQuan(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"GuangQuanFinish")==0)
	{
		// 播放结束，删除自己
		if (_GuangQuanArmature)
		{
			_GuangQuanArmature->getAnimation()->stop();
			_GuangQuanArmature->removeFromParentAndCleanup(true);
			_GuangQuanArmature = nullptr;
		}
	}
}
void enemyObjects::downToNextStoreyEnd(Ref*f, Sprite* _sp){
	if (_sp)
	{
		_sp->removeFromParentAndCleanup(true);
	}
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->toDownNextStorey(this);
}

//碰到了传送的东东魔法

void enemyObjects::checkIsMagicTrans(GameObjects *obj){//检查是否碰到了魔法传送的
	if (obj->getobjectType()==TYPE_MAGIC)
	{
		float x1=this->getPosition().x-60;
		float x2=obj->getPosition().x;
		float dist=fabs(x1-x2);
		magicObjects *magic=static_cast<magicObjects*>(obj);
		if (magic->getSelfType()==MAGIC_TRANSMIT&&magic->getotherType()==TRANSMIT_IN&&dist<3.0&&magic->getCountNum()<magic->getmaxNum())//如果是传送的话
		{
			magic->setCountNum(magic->getCountNum()+1);
			isStart=NO;
			_armature->getAnimation()->stop();
			auto delay=DelayTime::create(0.05f);
			float f=0.25f;
			auto fade=FadeOut::create(f);
			auto sca=ScaleBy::create(f,0.6);
			Point endPos = Point(getPositionX(),getPositionY()+15);
			auto mb=MoveTo::create(f,endPos);
			auto spa=Spawn::create(fade,sca,mb,NULL);
			auto action = Sequence::create(
				delay,spa,
				CallFuncN::create(CC_CALLBACK_0(enemyObjects::UpToGroundStorey,this)),
				NULL);
			this->runAction(action);


		}
	
		if (magic->getSelfType()==MAGIC_TRANSMIT&&magic->getCountNum()>=magic->getmaxNum()&&magic->getisDel()==NO)//如果大于数目的话就删除
		{
			//magic->delayKeepHereCallback(nullptr);//删掉

		  magic->setisDel(YES);
			magic->delayKeepHere(1.0);
		
			StoreyData* _groundData = _scenedata->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_GROUND);//这个是放在陆地上的

			for (Ref *sp:_groundData->getObjectData())//循环遍历子节点
			{
				GameObjects *obj=(GameObjects*)sp;
				if (obj->getobjectType()==TYPE_MAGIC)
				{
					
						magicObjects *magic1=static_cast<magicObjects*>(obj);
					if (magic1->getSelfType()==MAGIC_TRANSMIT&&magic1->getotherType()==TRANSMIT_OUT)
					{
						//magic1->delayKeepHereCallback(nullptr);//删掉
						magic1->delayKeepHere(1.0);
						
					}
				}
			}
		}
		
	}
	
}
void enemyObjects::UpToGroundStorey(){
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	
	_curBackGroundWnd->magicTransmitUpToGround(this);

}

//变成小绵羊
void enemyObjects::changeToSheep(float time){//先把敌人透明度调低，然后变成load 不调用定时器 
	isStart=NO;
   this->load();
   _armature->setVisible(false);

   effectsObject* effects = effectsObject::create();
   effects->setPosition(Point(this->getPositionX(),this->getPositionY()+25));
   effects->sheepEffect();
   auto parent = static_cast<Widget*>(this->getParent());
   parent->addChild(effects,this->getZOrder()+1);
  
   auto delay1=DelayTime::create(0.4f);
   auto delay2=DelayTime::create(0.4f);
   auto keepDelay=DelayTime::create(time);
   auto ddlay = Sequence::create(
	   delay1,
	   CallFuncN::create(CC_CALLBACK_0(enemyObjects::createSheepArmature,this)),
	   keepDelay,
	    CallFuncN::create(CC_CALLBACK_0(enemyObjects::backEnemyEffect,this)),
		delay2,
		  CallFuncN::create(CC_CALLBACK_0(enemyObjects::backEnemy,this)),
	   NULL);
   this->runAction(ddlay);

}
void enemyObjects::createSheepArmature(){//创建骨骼
	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_Magic20.png","td_Magic20.plist","td_Magic2.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	sheepArmature = Armature::create("td_Magic2");   
	sheepArmature->getAnimation()->setAnimationScale((float)0.8);
	sheepArmature->getAnimation()->play("load1",-1,0);
	this->addChild(sheepArmature);
	sheepArmature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(enemyObjects::onFrameEvent)); 
}
void enemyObjects::backEnemyEffect(){//变会敌人之前的一步
	effectsObject* effects = effectsObject::create();
	effects->setPosition(Point(this->getPositionX(),this->getPositionY()+25));
	effects->sheepEffect();
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,this->getZOrder()+1);

}
void enemyObjects::backEnemy(){//变回敌人
	this->removeChild(sheepArmature,true);
	sheepArmature=nullptr;
	isStart=YES;
	this->load();
	_armature->setVisible(true);
}
// Event106事件：携带特殊魔法的怪物
void enemyObjects::setEnemyObjectEvent106()
{
	_armature->setColor(Color3B(59, 255, 89));
	setIsEvent106_Object(true);
}


//小狗发威。。被小狗恐吓之后的表情
void enemyObjects::xiaogoufawei(int mlv){

	isStart=NO;
	this->MakeWarToLoad();//让打我的战士暂停下
	load();//变成loading一下
	 _isIntimidate=YES;
	_lev=mlv;	
	auto action = Sequence::create(
			DelayTime::create(0.8),
			CallFunc::create(CC_CALLBACK_0(enemyObjects::xiaogoufaweiBackRun, this)),
			NULL);
	this->runAction(action);


	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_war_05_kongxia.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	Armature *arm = Armature::create("td_war_05_kongxia");   
	arm->setPosition(0,_armature->getContentSize().height*_armature->getScale()-20);
	this->addChild(arm,1);
     arm->getAnimation()->play("play",-1,1);

}

void enemyObjects::xiaogoufaweiBackRun(){
	
	 this->Run();
	 isStart=YES;
}

//用于往回跑的AI
void enemyObjects::newAiForBackRun(){
	
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
			float thisPosX=this->getPositionX();
			float moveWidthCenterX=_swg->getPositionX();

			
			if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&_states!=STATES_FIGHTING&&obj->getisFrozen()==NO&&
				obj->getisIntimidate()==NO)//寻找一个敌人，没死亡或者不在战斗的时候,且在同一层之内
			{
				float moveWidthCenterX=_swg->getPositionX();
				float thisPosX=this->getPositionX();

				float sLd=(moveWidthCenterX-_swg->getSize().width/2+obj->getArmature()->getContentSize().width/2*obj->_armature->getScale()-10.0);
				if (obj->getPositionX()<sLd)
				{
					continue;
				}



				if (thisPosX> obj->getPositionX())
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

				
					int tempNearestX=dist - safeZone;//距离差距

					if (nearestX==0)//如果是等于0的时候，寻找到第一个目标
					{  
						if (obj->getisFly()==NO&&obj->getstates()!=STATES_FIGHTING){
							isFind=true;//表示寻找到
							nearestX=tempNearestX;//距离差距
							targetObj=obj;//获取一个目标
							targetAddr=(uintptr_t)targetObj;
						}

					}else {//否则的话，说明是还有第二个目标了或者多个目标,如果是多个目标的话，寻找一个没进在战斗状态的敌人进行攻击，且离当前目标最近
						
						if (obj->getisFly()==NO&&obj->getstates()!=STATES_FIGHTING)
						{
							if (nearestX>tempNearestX)
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


	if (isFind==true&&targetAddr!=0&&_states!=STATES_FIGHTING&&targetObj!=nullptr)//如果发现有目标的话,且自身没有战斗状态的时候
	{
		float thisPosX=this->getPosition().x;
		float objPosX=targetObj->getPosition().x;
		float tempAtkLen=_atkLen;

		float dist = fabs( thisPosX- objPosX );
		float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
			targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
			LEN_SCALE;

		float movespeed=_moveSpeedScale+1.0;
		if (dist>=safeZone &&  dist-safeZone<=tempAtkLen )//说明在攻击范围内
		{
				
			this->Fight();//战斗啦
			
			if (thisPosX>objPosX)
			{
				_armature->setRotationY(180);

			}else{
				_armature->setRotationY(0);

			}

		}else if ( (dist<safeZone || dist-safeZone>tempAtkLen) )
		{

			if (thisPosX>objPosX)
			{	
				 setMoveDirection("left");

				this->setPosition(this->getPosition().x-movespeed,this->getPosition().y);
				if (_states!=STATES_RUN)
				{
					this->Run();
				}
			}

			else 
			{	
				this->setPosition(this->getPosition().x+movespeed,this->getPosition().y);
				 setMoveDirection("right");

				if (_states!=STATES_RUN)
				{
					this->Run();
				}

			}
		}
	}




	if (isFind==false)//如果没寻找到
	{

		if (_states==STATES_FIGHTING&&targetAddr==0)//如果没找到且本身在战斗
		{
			this->load();
			this->Run();

		}else if (_states==STATES_RUN&&_isRun==true)
		{
			this->setPosition(this->getPosition().x-(_moveSpeedScale+1.0),this->getPosition().y);
			  setMoveDirection("left");
		}
		else if (_states==STATES_LOAD&&targetAddr==0&&_states!=STATES_RUN){
			this->Run();
            setMoveDirection("left");
			
		}


	}










}