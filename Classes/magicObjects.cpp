#include "magicObjects.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "audioPlayer.h"

USING_NS_CC;

magicObjects::magicObjects(void)
{
	
}

magicObjects::~magicObjects(void)
{
	audioPlayer::getInstance()->stopEffect(effectId);
}
// on "init" you need to initialize your instance

bool magicObjects::magicObjectsInit(const std::string& filename,const std::string&name,int otype)
{
    
	targetAddr=0;
	_objectType=TYPE_MAGIC;
	_selfType=otype;
	keepTime=1000;
	CountNum=0;
	_states=STATES_DEFAULT;//设置是默认的
	isCruising=true;
	firstTimeFlag=false;//第一次放进来的时候的
	left_offset=0.0f;
	right_offset=0.0f;
	_isRun=false;//设置是否在跑，默认是停止的
	_storeydata=nullptr;
	
	isDel=NO;
	this->CreateArmature(filename,name);
	
	this->setProperty();//属性。。。。。
	

	float  dt=1.0/10;
	if (_selfType==MAGIC_ATK)//如果是魔法攻击类技能的话
	{
		 dt=1.0;
		this->magicupdate(0);
	}
	if (_selfType==MAGIC_GODZILLA)//如果是哥斯拉的话
	{
		dt=1.0/60;
	}
	if (_selfType==MAGIC_BOSS5SKILL)
	{
		dt=1.0/60;
	}

	this->schedule(schedule_selector(magicObjects::magicupdate), dt);

	this->delayKeepHere(keepTime);//存在的持续时间

    return true;
}

 magicObjects* magicObjects::create(const std::string& filename,const std::string&name,int otype)
  {

	  magicObjects *sprite = new magicObjects(); 
	  if (sprite && sprite->init())
	  {
		  sprite-> magicObjectsInit(filename,name,otype);
		  sprite->autorelease();
		 
		  return sprite;
	  }
	  CC_SAFE_DELETE(sprite);
	  return nullptr;
  }

 void magicObjects::delayKeepHere(float t)//设置多少秒后调用删除
 {
	 keepTime=t;
	 this->stopAllActions();
	 auto delay=DelayTime::create(keepTime);
	 auto ddlay = Sequence::create(
		 delay,
		 CallFuncN::create(CC_CALLBACK_1(magicObjects::delayKeepHereCallback,this)),
		 NULL);
	 this->runAction(ddlay);
 }
 void magicObjects::delayKeepHereCallback(Ref *f)//回调
 {

	 if (_selfType==MAGIC_GODZILLA)
	 {
		 this->dragObjectSmoke();
	 }

	GameObjects *obj=static_cast<GameObjects*>(f);
	
	Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
	Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
	_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
		
 }
 

 void magicObjects::dragObjectSmoke(){


	 effectsObject* effects = effectsObject::create();
	 effects->setPosition(Point(this->getPositionX(),this->getPositionY()+25));
	 effects->sheepEffect();
	 auto parent = static_cast<Widget*>(this->getParent());
	 parent->addChild(effects,this->getZOrder()+1);

	 if (_selfType==MAGIC_GODZILLA)
	 {
		  audioPlayer::getInstance()->playEffect("S20.mp3");
	 }
 }



 void magicObjects::chuansongmenEffect(){

	 SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
	 cache->addSpriteFramesWithFile("td_magic3_effect.plist");

	 effectsObject* effects = effectsObject::create();
	 effects->setPosition(Point(this->getPositionX()+5,this->getPositionY()));
	 effects->magic3Effect();
	 auto parent = static_cast<Widget*>(this->getParent());
	 parent->addChild(effects,this->getZOrder()+1);
 	
	 float w1=_armature->getContentSize().width/2;

	 this->setScale(0.3);
	 float w2=_armature->getContentSize().width/2;
	 float ft=0.8f;
	 auto sca=ScaleTo::create(ft,1.0);
	 auto move=MoveBy::create(ft,Point(-50,-20));
	 auto spa=Spawn::create(sca,move,NULL);

	 auto dely=DelayTime::create(1.0);
	 auto action = Sequence::create(spa,
		 CallFuncN::create(CC_CALLBACK_0(magicObjects::chuansongmenEffectcallback, this)),
		 dely,
		 NULL);
	 this->runAction(action);



 }

 void magicObjects::chuansongmenEffectcallback(){

	  SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
	  cache->removeSpriteFrameByName("td_magic3_effect.plist");
 }


//设置属性
void magicObjects::setProperty()
{
	
	
	auto userData = UserRecord::getInstance();

	if (_selfType==MAGIC_ATK)//攻击的魔法技能
	{
		_atk=600;
		_atkLen=250;
		_atkOption=1;
		_CRI=0;
		_CRI_damage=0.0;
		keepTime=6.0;

		/*int _curItem1SelectIndex = userData->getUpgradesItemLevel("magic", "item1");

		float addAtkPer=0.0;
		float addLenPer=0.0;
		if (_curItem1SelectIndex==1)
		{
		addAtkPer=0.2;
		}
		if (_curItem1SelectIndex==2)
		{  
		addAtkPer=0.2;
		addLenPer=0.1;
		}
		if (_curItem1SelectIndex==3)
		{
		addAtkPer=0.4;
		addLenPer=0.1;
		}
		if (_curItem1SelectIndex==4)
		{
		addAtkPer=0.6;
		addLenPer=0.1;
		}
		if (_curItem1SelectIndex==5)
		{
		addAtkPer=0.6;
		addLenPer=0.2;
		}
		_atk=_atk*(1+addAtkPer);
		_atkLen=_atkLen*(1+addLenPer);*/
	}

	//持续时间的
	if (_selfType==MAGIC_TRANSMIT)//传送魔法技能的持续时间
	{
		keepTime=12.0;
		maxNum=8;//一开始最大是4个


		//int _curItemSelectIndex = userData->getUpgradesItemLevel("magic", "item3");
		////int _curItemSelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item3");

		//if (_curItemSelectIndex>=1&&_curItemSelectIndex<3)
		//{
		//	maxNum+=1;
		//}

		//if (_curItemSelectIndex>=3&&_curItemSelectIndex<4)
		//{
		//	maxNum+=(1+1);
		//}

		//if (_curItemSelectIndex>=5)
		//{
		//	maxNum+=(1+1+2);
		//}
	}


	if (_selfType==MAGIC_GODZILLA)//如是哥斯拉的话
	{
		_maxHp=8000;
		_atkOption=3;
		_atk=800;
		_dfe=0.5;
		_dfeOption=1;
		_CRI=0;
		_CRI_damage=0.0;

		_atkLen=100;
		_moveSpeedScale=1.0;//设置默认的移动的速度率敌
		keepTime=20.0;//持续20秒

	

		//int _curItemSelectIndex = userData->getUpgradesItemLevel("magic", "item4");
		////int _curItemSelectIndex = PlistManager::getInstance()->getUpgradesSelect("magic", "item4");
		//
		//if (_curItemSelectIndex==1)
		//{
		//	_atk=(1.0+0.2);
		//}
		//if (_curItemSelectIndex==2)
		//{
		//	_atk=(1.0+0.2)*_atk;
		//	_maxHp=(1.0+0.3)*_maxHp;

		//}
		//if (_curItemSelectIndex==3)
		//{
		//	_atk=(1.0+0.2)*_atk;
		//	_maxHp=(1.0+0.3)*_maxHp;
		//}
		//if (_curItemSelectIndex==4)
		//{
		//	_atk=(1.0+0.2+0.3)*_atk;
		//	_maxHp=(1.0+0.3)*_maxHp;
		//}
		//if (_curItemSelectIndex==5)
		//{
		//	_atk=(1.0+0.2+0.3)*_atk;
		//	_maxHp=(1.0+0.3)*_maxHp;
		//	keepTime+=5.0;
		//}

		_hp=_maxHp;
	}

	
}


//创建骨骼动画相关函数 
void magicObjects::CreateArmature(const std::string& filename,const std::string&name ){
	

ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
_armature = cocostudio::Armature::create(name);   
_armature->setPosition(0,0);
this->addChild(_armature,1);
//本来 这是缩_armature->setScale(0.8);在导出来的时候已经缩了
if (_selfType==MAGIC_ATK)//如果是攻击魔法的话就播放
{
	_armature->getAnimation()->play("play",-1,1);
}
if (_selfType==MAGIC_BOSS5SKILL)
{
	_armature->setScale(1.25);
}
if (_selfType==MAGIC_GODZILLA)//哥斯拉
{
	this->Load();//待机状态
	
}
if (_selfType==MAGIC_BOSS5SKILL)//如果是BOSS放出来的精灵的话
{
	_armature->getAnimation()->play("call",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));
}

}

//这个2个主要是传送的
void magicObjects::playTransmitIn(){
	_armature->getAnimation()->play("in",-1,1);
	this->chuansongmenEffect();
}
void magicObjects::playTransmitOut(){
	_armature->getAnimation()->play("out",-1,1);
	this->chuansongmenEffect();
}


void  magicObjects::Run(){
	_states=STATES_RUN;
	_isRun=true;
	_armature->getAnimation()->play("run",-1,1);

	if (_selfType== MAGIC_GODZILLA ){
		//effectId = audioPlayer::getInstance()->playEffect("S35.mp3",true);
	}
}
void  magicObjects::Fight_Begin(){
	
	_states=STATES_FIGHTING;
	_armature->getAnimation()->play("atk_begin",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));

}
void  magicObjects::Fight_Mid(){
	_states=STATES_FIGHTING;

	if (_selfType==MAGIC_GODZILLA)
	{
		audioPlayer::getInstance()->stopEffect(effectId);

		effectId = audioPlayer::getInstance()->playEffect("S36.mp3",true);

		_armature->getAnimation()->play("atk_mid",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));
		//this->penHuo();//喷火动画
	}

	if (_selfType==MAGIC_BOSS5SKILL)
	{
		
		_armature->getAnimation()->play("atk",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));
	}
	


}
void  magicObjects::Fight_End(){
	
	_states=STATES_FIGHTING;
	_armature->getAnimation()->play("atk_end",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));
}
void  magicObjects::Load(){
	
	_isRun=false;
	_states=STATES_LOAD;
	targetAddr=0;
	_armature->getAnimation()->play("load",-1,1);
	this->setisStart(YES);

	if (_selfType==MAGIC_GODZILLA){
		audioPlayer::getInstance()->stopEffect(effectId);
	}
	

	if (isStart==YES&&_selfType==MAGIC_GODZILLA)
	{
		//遍历所以本层的节点，如果存在和本类型相同的话，如果位置在附近的话就要赶紧离开
		left_offset=0.0f;
		right_offset=0.0f;
		
		this->scheduleOnce(schedule_selector(magicObjects::beginWithCruising),  CCRANDOM_0_1()*2.0f);
	}


}

void magicObjects::beginWithCruising(float dt){

	
	if (_states==STATES_LOAD){
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

}
void magicObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	 // CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	
	if (strcmp(evt.c_str(),"startfinish")==0)//播放结束
	{
		this->Load();
	}

	  if (strcmp(evt.c_str(),"reducehp")==0)//减血的时候
	  {		
		  
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//没寻找到
		  { 
			  this->Load(); 

		  }else{//寻找在他附近的,也要受伤
			  tempObj->setReduceHp(_atkOption,_atk,0,0);
			
			  if (_selfType==MAGIC_GODZILLA)
			  {
				  for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
				  {
					  GameObjects *obj=(GameObjects*)sp;

					  if (obj!=tempObj&&obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0&&obj->getisIntimidate()==NO)
					  {
						  float dist=fabs(tempObj->getPositionX()-obj->getPositionX());
						  if (dist<180)
						  {
							  obj->setReduceHp(_atkOption,_atk,0,0);
							 
						  }

					  }
				  }
			  }
			  


		  }


		  

	  }
	  if (strcmp(evt.c_str(),"begin")==0)//在攻击开始时候
	  {		
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//没寻找到
		  { 
			  this->Load(); 
			
		  }
	  }

	  if (strcmp(evt.c_str(),"begin_finish")==0)//开始攻击
	  {		
		  this->Fight_Mid();//切换到攻击状态
	  }
	  if (strcmp(evt.c_str(),"end_finish")==0)//攻击结束
	  {		
		  this->Load();//切换到load
	  }

}  




/*********************************************

    主定时器 各种逻辑判定
	士兵AI——士兵AI基本逻辑，在部署的层内警戒，
	当有敌人进入警戒范围内与之进行战斗，直至双
	方一方HP=0结束，结束后搜索下一目标（如果有多
	个目标则选择没有进入战斗的目标），重复战斗逻辑

************************************************/

void magicObjects::magicupdate(float dt)
{
	

	if (isStart==YES)//不等于空的时候
	{
		bool isFind=false;//是否寻找到目标
		GameObjects *targetObj=nullptr;
		int nearestX=0;//如果有多个目标的话记录最近的一个目标最近的一个
		
		if (!_storeydata)
			return;
		if (_storeydata->getObjectData().size() > 0)
		{
			for (Ref *sp:_storeydata->getObjectData())
			{
				GameObjects *obj=(GameObjects*)sp;
				
				bool isflySelect=true;
				if (_selfType==MAGIC_BOSS5SKILL||_selfType==MAGIC_GODZILLA)
				{
					isflySelect=true;
				}else if (obj->getisFly()==YES)
				{
					isflySelect=false;
				}

				if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&obj->getisFrozen()==NO&&obj->gethp()>0&&isflySelect==true)
				{
					
					if (_selfType==MAGIC_ATK&&fabs(this->getPositionX()-obj->getPositionX())<_atkLen)//魔法技能攻击的
					{
						obj->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);
					}

					if ((_selfType==MAGIC_GODZILLA||_selfType==MAGIC_BOSS5SKILL)&&_states!=STATES_FIGHTING)//如果是哥斯拉的话或者是精灵的话
					{
                          
		  				float moveWidthCenterX=_swg->getPositionX();
						float thisPosX=this->getPositionX();
 						float dist = fabs( thisPosX- obj->getPositionX());
						float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
							obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale() +
							LEN_SCALE;

						 if (dist<safeZone){
							continue;
						} 
						int tempNearestX=dist - safeZone;//距离差距
						isFind=true;//表示寻找到
						if (nearestX==0)//如果是等于0的时候，寻找到第一个目标
						{ 
						 
								nearestX=tempNearestX;//距离差距
								targetObj=obj;//获取一个目标
								targetAddr=(uintptr_t)(targetObj);
                            
								
						 }else if(nearestX>tempNearestX)
						  {

							targetObj=obj;
							targetAddr=(uintptr_t)(targetObj);
							nearestX=tempNearestX;

						 }

					}

				}
			
			}
		}


		//下的代码主要是针对远程支援英雄释放出来的 飞出去和飞回来
		if (_selfType==MAGIC_BOSS5SKILL&&targetAddr!=0)
		{

			GameObjects *ttarget=this->FindObject(targetAddr);
			if (ttarget!=nullptr)
			{
				if (_selfType==MAGIC_BOSS5SKILL&&ttarget->getisFly()==YES)
				{

					if ((this->getPositionY()+80)<ttarget->getPositionY())
					{
						this->setPosition(this->getPosition().x,this->getPosition().y+4.0);
					}
				}
				else if (_selfType==MAGIC_BOSS5SKILL&&ttarget->getisFly()==NO){

					if ((this->getPositionY()+55)>=ttarget->getPositionY())
					{
						this->setPosition(this->getPosition().x,this->getPosition().y-4.0);
					}
				}
			}
			
		}
		
		if (_selfType==MAGIC_BOSS5SKILL&&targetAddr==0){
			

			GameObjects *boss=nullptr;
			for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
			{
				GameObjects *obj=(GameObjects*)sp;
				if ((uintptr_t)(obj)==itsCallObjectAddr&&obj->getobjectType()==TYPE_BOSS&&obj->gethp()>0)
				{
					boss=obj;
					break;
				}
			}
			if (boss!=nullptr)
			{
				Point pos1=this->getPosition();
				Point pos2=boss->getPosition();
				if ((pos1.x)>(pos2.x-fatherPos.x+2))
				{
						this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
						_armature->setRotationY(0);
						if (boss->getisStart()==YES)
						{
							_armature->setRotationY(0);
						}
						
				}else if((pos1.x)<(pos2.x-fatherPos.x-2))
				{
					this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
					_armature->setRotationY(180);
					if (boss->getisStart()==YES)
					{
						_armature->setRotationY(180);
					}
				}else{
					if (boss->getisStart()==YES)
					{
						_armature->setRotationY(0);
					}
				}

				if ((pos1.y)>(pos2.y+fatherPos.y))
				{
					this->setPosition(this->getPosition().x,this->getPosition().y-2.0);
				}else if ((pos1.y)<(pos2.y-fatherPos.y))
				{
					this->setPosition(this->getPosition().x,this->getPosition().y+2.0);
				}



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

			

			if (dist>=safeZone &&  dist-safeZone<=tempAtkLen &&thisPosX>leftposX&&thisPosX<rightposX)//说明在攻击范围内
			{

				this->Fight_Mid();//开始战斗啦
				
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
				
			    	this->setPosition(this->getPosition().x-_moveSpeedScale,this->getPosition().y);
					
					if (_states!=STATES_RUN)
					{
						this->Run();
					}
				}
				else 
				{	
					this->setPosition(this->getPosition().x+_moveSpeedScale,this->getPosition().y);
					_armature->setRotationY(180);

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
				this->Load();
			}else if (_states==STATES_RUN)
			{

				this->setPosition(this->getPosition().x+_moveSpeedScale*CruisingDirectX,this->getPosition().y);
				
				float moveWidthCenterX=_swg->getPositionX();
				float thisPosX=this->getPositionX();

				if (CruisingDirectX>0)//向右边走
				{
					_armature->setRotationY(180);

				}else if (CruisingDirectX<=0)//向左边走
				{
					_armature->setRotationY(0);

				}

				if (thisPosX<(moveWidthCenterX-_swg->getSize().width/2+left_offset)||thisPosX>(moveWidthCenterX+_swg->getSize().width/2-right_offset))
				{
					this->Load();

				}
			}


		}







	}

}


GameObjects * magicObjects::FindObject(int addr){
	GameObjects *pObj=nullptr;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if ((uintptr_t)obj==addr&&obj->getobjectType()==TYPE_ENEMY&&obj->getisFrozen()==NO&&obj->gethp()>0)
		{
// 			float dist=fabs(this->getPositionX()-obj->getPositionX());
// 			float tempLen=_atkLen;
// 			if (_atkLenOption==ATKLEN_NEAR)
// 			{
// 				tempLen=this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() 
// 					+ obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale();
// 			}
// 			if (dist<=tempLen)
// 			{
				pObj=obj;
				break;
			//}
		
		}

	
	}
	return pObj;

}



