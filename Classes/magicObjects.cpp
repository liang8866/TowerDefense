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
	_states=STATES_DEFAULT;//������Ĭ�ϵ�
	isCruising=true;
	firstTimeFlag=false;//��һ�ηŽ�����ʱ���
	left_offset=0.0f;
	right_offset=0.0f;
	_isRun=false;//�����Ƿ����ܣ�Ĭ����ֹͣ��
	_storeydata=nullptr;
	
	isDel=NO;
	this->CreateArmature(filename,name);
	
	this->setProperty();//���ԡ���������
	

	float  dt=1.0/10;
	if (_selfType==MAGIC_ATK)//�����ħ�������༼�ܵĻ�
	{
		 dt=1.0;
		this->magicupdate(0);
	}
	if (_selfType==MAGIC_GODZILLA)//����Ǹ�˹���Ļ�
	{
		dt=1.0/60;
	}
	if (_selfType==MAGIC_BOSS5SKILL)
	{
		dt=1.0/60;
	}

	this->schedule(schedule_selector(magicObjects::magicupdate), dt);

	this->delayKeepHere(keepTime);//���ڵĳ���ʱ��

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

 void magicObjects::delayKeepHere(float t)//���ö���������ɾ��
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
 void magicObjects::delayKeepHereCallback(Ref *f)//�ص�
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


//��������
void magicObjects::setProperty()
{
	
	
	auto userData = UserRecord::getInstance();

	if (_selfType==MAGIC_ATK)//������ħ������
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

	//����ʱ���
	if (_selfType==MAGIC_TRANSMIT)//����ħ�����ܵĳ���ʱ��
	{
		keepTime=12.0;
		maxNum=8;//һ��ʼ�����4��


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


	if (_selfType==MAGIC_GODZILLA)//���Ǹ�˹���Ļ�
	{
		_maxHp=8000;
		_atkOption=3;
		_atk=800;
		_dfe=0.5;
		_dfeOption=1;
		_CRI=0;
		_CRI_damage=0.0;

		_atkLen=100;
		_moveSpeedScale=1.0;//����Ĭ�ϵ��ƶ����ٶ��ʵ�
		keepTime=20.0;//����20��

	

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


//��������������غ��� 
void magicObjects::CreateArmature(const std::string& filename,const std::string&name ){
	

ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
_armature = cocostudio::Armature::create(name);   
_armature->setPosition(0,0);
this->addChild(_armature,1);
//���� ������_armature->setScale(0.8);�ڵ�������ʱ���Ѿ�����
if (_selfType==MAGIC_ATK)//����ǹ���ħ���Ļ��Ͳ���
{
	_armature->getAnimation()->play("play",-1,1);
}
if (_selfType==MAGIC_BOSS5SKILL)
{
	_armature->setScale(1.25);
}
if (_selfType==MAGIC_GODZILLA)//��˹��
{
	this->Load();//����״̬
	
}
if (_selfType==MAGIC_BOSS5SKILL)//�����BOSS�ų����ľ���Ļ�
{
	_armature->getAnimation()->play("call",-1,0);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(magicObjects::onFrameEvent));
}

}

//���2����Ҫ�Ǵ��͵�
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
		//this->penHuo();//��𶯻�
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
		//�������Ա���Ľڵ㣬������ںͱ�������ͬ�Ļ������λ���ڸ����Ļ���Ҫ�Ͻ��뿪
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

}
void magicObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	 // CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	
	if (strcmp(evt.c_str(),"startfinish")==0)//���Ž���
	{
		this->Load();
	}

	  if (strcmp(evt.c_str(),"reducehp")==0)//��Ѫ��ʱ��
	  {		
		  
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//ûѰ�ҵ�
		  { 
			  this->Load(); 

		  }else{//Ѱ������������,ҲҪ����
			  tempObj->setReduceHp(_atkOption,_atk,0,0);
			
			  if (_selfType==MAGIC_GODZILLA)
			  {
				  for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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
	  if (strcmp(evt.c_str(),"begin")==0)//�ڹ�����ʼʱ��
	  {		
		  GameObjects *tempObj=this->FindObject(targetAddr);
		  if (tempObj==nullptr)//ûѰ�ҵ�
		  { 
			  this->Load(); 
			
		  }
	  }

	  if (strcmp(evt.c_str(),"begin_finish")==0)//��ʼ����
	  {		
		  this->Fight_Mid();//�л�������״̬
	  }
	  if (strcmp(evt.c_str(),"end_finish")==0)//��������
	  {		
		  this->Load();//�л���load
	  }

}  




/*********************************************

    ����ʱ�� �����߼��ж�
	ʿ��AI����ʿ��AI�����߼����ڲ���Ĳ��ھ��䣬
	���е��˽��뾯�䷶Χ����֮����ս����ֱ��˫
	��һ��HP=0������������������һĿ�꣨����ж�
	��Ŀ����ѡ��û�н���ս����Ŀ�꣩���ظ�ս���߼�

************************************************/

void magicObjects::magicupdate(float dt)
{
	

	if (isStart==YES)//�����ڿյ�ʱ��
	{
		bool isFind=false;//�Ƿ�Ѱ�ҵ�Ŀ��
		GameObjects *targetObj=nullptr;
		int nearestX=0;//����ж��Ŀ��Ļ���¼�����һ��Ŀ�������һ��
		
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
					
					if (_selfType==MAGIC_ATK&&fabs(this->getPositionX()-obj->getPositionX())<_atkLen)//ħ�����ܹ�����
					{
						obj->setReduceHp(_atkOption,_atk,_CRI,_CRI_damage);
					}

					if ((_selfType==MAGIC_GODZILLA||_selfType==MAGIC_BOSS5SKILL)&&_states!=STATES_FIGHTING)//����Ǹ�˹���Ļ������Ǿ���Ļ�
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
						int tempNearestX=dist - safeZone;//������
						isFind=true;//��ʾѰ�ҵ�
						if (nearestX==0)//����ǵ���0��ʱ��Ѱ�ҵ���һ��Ŀ��
						{ 
						 
								nearestX=tempNearestX;//������
								targetObj=obj;//��ȡһ��Ŀ��
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


		//�µĴ�����Ҫ�����Զ��֧ԮӢ���ͷų����� �ɳ�ȥ�ͷɻ���
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
			for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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

			

			if (dist>=safeZone &&  dist-safeZone<=tempAtkLen &&thisPosX>leftposX&&thisPosX<rightposX)//˵���ڹ�����Χ��
			{

				this->Fight_Mid();//��ʼս����
				
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




		if (isFind==false)//���ûѰ�ҵ�
		{
			
			if (_states==STATES_FIGHTING&&targetAddr==0)//���û�ҵ��ұ�����ս��
			{
				this->Load();
			}else if (_states==STATES_RUN)
			{

				this->setPosition(this->getPosition().x+_moveSpeedScale*CruisingDirectX,this->getPosition().y);
				
				float moveWidthCenterX=_swg->getPositionX();
				float thisPosX=this->getPositionX();

				if (CruisingDirectX>0)//���ұ���
				{
					_armature->setRotationY(180);

				}else if (CruisingDirectX<=0)//�������
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
	for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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



