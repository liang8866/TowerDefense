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
	targetAddr=0;//Ĭ�ϵ���0
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
	_states=STATES_DEFAULT;//������Ĭ�ϵ�
	setIsEvent106_Object(false);

	_isRun=false;//�����Ƿ����ܣ�Ĭ����ֹͣ��

	_curTargetType=-1;//Ĭ���� -1 �������
	frozenArm=nullptr;
	_isFly = NO;
	if (_selfType==ENEMY_ITEM13||_selfType==ENEMY_ITEM14||_selfType==ENEMY_ITEM15||_selfType==ENEMY_ITEM16)//���Ƿ��е��˵�
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



//��������
void enemyObjects::setProperty(int fnum)
{
	//��ʱ�õ�����
	_maxHp=400;
	_hp=_maxHp;
	_atk=60;
	_dfe=20;
	_lev=1;
	_atkLen=_armature->getContentSize().width/2*_armature->getScale();
	_moveSpeedScale=1;//����Ĭ�ϵ��ƶ����ٶ��ʵ�
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


//��������������غ��� 
void enemyObjects::CreateArmature(const std::string& filename,const std::string&name ){
	
isArmature=true;//����Ϊtrue
ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
_armature = Armature::create(name);   
_armature->getAnimation()->setAnimationScale((float)0.8);

//_armature->setScale(0.8);����Ҫ�����������ˣ��Ѿ��ڵ�������������С����0.8
_armature->setPosition(0,0);

this->addChild(_armature,1);
this->load();
if (_selfType==ENEMY_ITEM20){

	_armature->getAnimation()->setAnimationScale(0.5f);
}

if (_selfType==ENEMY_ITEM16)//������̫���ˣ���С
{
	_armature->setScale(0.5/0.8); 
	
}
}


//���������Ĵ����¼�
void enemyObjects::CreateListenerTouch_Armature(){

	
	 listener = EventListenerTouchOneByOne::create();//����һ����������
	 CC_SAFE_RETAIN(listener);
	 //3.0 �����ֱ����touchBegan���������ʵ�ִ��룬����������ȥдһ��touchBegan�ĺ���
    listener->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Armature*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    Size s = target->getContentSize();
	Rect rect = Rect(0, 0, s.width*target->getScale(), s.height*target->getScale());

    if (rect.containsPoint(locationInNode)){//�жϴ������Ƿ���Ŀ��ķ�Χ��
	
		return true;
	}
    else{
       	  return false;
	}   };
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener ,_armature);  
	
}

//�����ɾ�������¼���
void enemyObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
}

//����֡����

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
      �ƶ�
********************************/

void enemyObjects::load(){
	ttAddr=0;
	_states=STATES_LOAD;
	targetAddr=0;
	_armature->getAnimation()->play("load",-1,1);

	
}
void enemyObjects::Run()//�����ܲ�
{
	ttAddr=0;
	_isRun=true;
	_states=STATES_RUN;
	_armature->getAnimation()->play("run",-1,1);
	_armature->getAnimation()->setSpeedScale((float)0.8);
	
	
	
}

void enemyObjects::Fight(){//����ս��
	_states=STATES_FIGHTING;//����ս��
	_armature->getAnimation()->play("atk",-1,1);
	_armature->getAnimation()->setAnimationScale((float)0.9);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(enemyObjects::onFrameEvent)); 
	int FindNum=0;



}   
void enemyObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	//CCLOG(" enemy (%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	if (strcmp(evt.c_str(),"load1finish")==0)//������
	{
		if (sheepArmature!=nullptr)
		{  
			
			sheepArmature->getAnimation()->play("load2",-1,-1);
		}
		
	}

	if (strcmp(evt.c_str(),"trap_finish")==0)//���������
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

				if (obj->getobjectType()==TYPE_BOSS&&obj->getSelfType()==BOSS_ITEM4)//���Ӣ�۵ģ��з����˺�
				{
				   float hurt=_atk*obj->getskill3_buffValue();//�����˺���ֵ
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

    ����ʱ�� �����߼��ж������˵�AI ����
 ����AI��������AI�����߼���ǰ��һֱ���������²㣬
 ǰ������������ʿ������������BOSSĿ�꣬������Ŀ��
 ����֮ս����ֱ���Է�HP=0����������������ִ��ǰ��
 �����ǰ����Ŀ���Ҹ�Ŀ��û����ս��״̬ʱ����ǰ
 ����Ŀ�����ս��������ǰ�������б��󷽵ĵ��˹���
 ��ֹͣǰ����󷽵��˽���ս�����ظ�ս���߼�������
 ����ʱ����1����1���ԣ���һ�����˱��ҷ���λ������
 ����BOSS���������������˾ͻ����ǰ�������ٺ���
 ���Ĵ˵�λ������

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

	if (isStart==YES&&_isIntimidate==YES&&_hp>0)//��С�����ź������ܵ�
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
		
		   this->reduceHpEffect(_hp);//���Լ���Ѫ����Ϊ0����ɱɾ��*/
// 			Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
// 			Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
// 			_curBackGroundWnd->removeStoreyObjectFormStoreyIndex(getfloorNum(), this);
			}
	}


	if (isStart==YES&&_isIntimidate==NO)//�����ڿյ�ʱ��
	{

		//����Ǳ���֣�����Ѫ��С�ڰٷ�֮��ʮ
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

		
		bool isFind=false;//���ñ����Ƿ����Ѱ�ҵ�
		int isFindTower3=NO;
		GameObjects *targetObj=nullptr;
		int nearestX=0;//����ж��Ŀ��Ļ���¼�����һ��Ŀ�������һ��
		
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
			
				if (m_isScoffFlag==true&&optype==TYPE_BOSS&&obj->getSelfType()==BOSS_ITEM4)//�Ƿ񱻳����ˣ��Ǻ�
				{
					targetObj=obj;
					isFind=true;//��ʾѰ�ҵ�
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
					if (optype==TYPE_WARRIOR&&obj->getSelfType()==WAR_ITEM4)//�ж��Ƿ����Ա�С��
					{
						isBombWar=true;
					}

					/*****************************************************************************************
					 
					 ****************************************************************************************/
					if (_states!=STATES_FIGHTING&&isBombWar==false){//������ӵķ�������--------
						float thisPosX=this->getPosition().x;
						float objPosX=obj->getPosition().x;
						int tempNearestX=fabs(obj->getPositionX()-this->getPositionX());//������
					
						if (_isFly==NO&&obj->getobjectType()==TYPE_TOWER&&obj->getSelfType()==TOWER_ITEM3&&obj->gethp()>0&&objPosX>thisPosX)//�����⼸����������ڷ�����3�Ǹ������������˴��
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
								isFind=true;//��ʾѰ�ҵ�
								isFindTower3==YES;
								nearestX=tempNearestX;//������
								if (obj->getlocationIndx()==1||_atkLenOption==ATKLEN_NEAR)
								{
									targetObj->settargetAddr((uintptr_t)this);
								}
								
							}
						}

						if (obj->gettargetAddr()==(uintptr_t)this&&isFindTower3==NO){//���ܻ��Ƕ��սʿ��ѡ�е��ǵ��ˣ���������ҲҪ���ж�
							
							if ((_isFly==NO&&obj->getisFly()==NO)||(_isFly==YES&&obj->getisFly()==YES)||(_isFly==YES&&obj->getSelfType()==MAGIC_BOSS5SKILL)){
								if (nearestX==0){//��ʾֻ��һ��
									targetObj=obj;
									isFind=true;//��ʾѰ�ҵ�
									nearestX=tempNearestX;//������
								}
								else{//��ʾ���еڶ���
									if (nearestX>tempNearestX){//���ǰһ��Ŀ�����������ڵĻ�,
										targetObj=obj;
										isFind=true;//��ʾѰ�ҵ�	
										nearestX=tempNearestX;//������
										
									}
									
								}
							}
						}




					}
					
				}

			}
		}

		bool isSameTarget=true;//��ֹ�����߶�
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

		if (isFind==true&&targetObj!=nullptr&&_states!=STATES_FIGHTING&&targetObj->getisStart()==YES&&isSameTarget==true)//���������Ŀ��Ļ�,������û��ս��״̬��ʱ��
		{
		
			float thisPosX=this->getPosition().x;
			float objPosX=targetObj->getPosition().x;
			float tempAtklen=_atkLen;
			if (_isFly==YES) //��������Ƿ��еĻ�������Ҫ��������Ҫ��Ч���ܴ�
			{
				tempAtklen=_armature->getContentSize().width/2*_armature->getScale();
			}

			bool isOk=false;
			if (targetObj->getobjectType()==TYPE_TOWER&&(targetObj->getSelfType()==TOWER_ITEM3||targetObj->getSelfType()==TOWER_ITEM2||targetObj->getSelfType()==TOWER_ITEM4 ||
				targetObj->getSelfType()==TOWER_ITEM5))//�ڶ���������������ܵ�
			{
				isOk=true;
			}else{
				if (targetObj->getstates()==STATES_FIGHTING)
				{
					isOk=true;
				}
				if (targetObj->getobjectType()==TYPE_WARRIOR&&(targetObj->getSelfType()==WAR_ITEM1||targetObj->getSelfType()==WAR_ITEM5)&&_atkLenOption==ATKLEN_REMOTE)//��������Զ�̵�ħ��С�����Է��ǽ��̵�սʿ�Ļ�
				{
					isOk=true;
				}
			}

			float dist = fabs( thisPosX- objPosX );
			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
							 targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
							 LEN_SCALE;

			if (targetObj->getobjectType()==TYPE_TOWER&&targetObj->getSelfType()==TOWER_ITEM3)//��ֹ����̫��ȫ��Χ���������߶�
			{
				safeZone=70;
			}

			if (dist>=safeZone && dist-safeZone<=tempAtklen &&isOk==true)//˵���ڹ�����Χ��
			{
				
				targetAddr=(uintptr_t)targetObj;
				_curTargetType=targetObj->_objectType;//��ס����Ŀ�������
				this->Fight();//ս����
				
				//��ǰһ���
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



		if (isFind==false)//û�ҵ���ʱ��....................................................
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
					if (_isFly==YES) //��������Ƿ��еĻ�������Ҫ��������Ҫ��Ч���ܴ�
					{
						tempAtklen=_armature->getContentSize().width/2*_armature->getScale();
					}

					if (obj->getobjectType()==TYPE_TOWER&&obj->getSelfType()==TOWER_ITEM3)//��ֹ����̫���µ��������߶�
					{

						safeZone=70;
					}

					if ( ( dist-safeZone > tempAtklen || dist<safeZone ) &&obj->gethp()>0)
					{
						this->Run();
					
						
					}
				}

			}

			if (_states==STATES_FIGHTING&&targetAddr==0&&_states!=STATES_RUN)//���û�ҵ��ұ�����ս��
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

void enemyObjects::bossAtkEffect(Point BossPos){//��ʾ��Ч������������������
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
	for (Ref *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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


void enemyObjects::FrozenMyself(float dt,bool isFirst){//������
	this->load();
	isStart=NO;
	_isFrozen=YES;
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->pause();
	}

	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_trap_020.png","td_trap_020.plist","td_trap_02.ExportJson");  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
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


void enemyObjects::speedDown(float t,float buffValue){//�����ٵ�ʱ��

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


//������ȥ��Ч��
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

int enemyObjects::GetWarAtkMeNum()//�����2��С���ڹ������ˣ��Ͳ����ٹ�������
{
	int num=0;
	for (Object *sp:_storeydata->getObjectData())//ѭ�������ӽڵ�
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


void enemyObjects::boss2SkillEffect(float f){//������ʱ��

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
void enemyObjects::boss2skillCallBack(Ref *sender){//�ص�����

	boss2Effct->removeFromParent();
	this->Run();
	isStart=YES;
	if (_armature!=nullptr)
	{
		_armature->getAnimation()->resume();
	}
}

//������ļ��ܵ�ʵ��
void enemyObjects::boss4SkillEeffect(float f)//f �������ʱ��
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
void enemyObjects::boss4SkillEffectCallback(Ref *sender)//������Ļص�����
{
	scoff->removeFromParent();
	this->load();
	m_isScoffFlag=false;
}

//©����һ��
void enemyObjects::checkIsEvent1(GameObjects *obj){//����Ƿ�������©����һ���
	if (obj->getobjectType()==TYPE_EVENT1)//���������©������һ��Ķ���
	{
		// �⼸�����Ͳ�©��ȥ
		if (this->getSelfType() == ENEMY_ITEM4 || this->getSelfType() == ENEMY_ITEM8 || this->getSelfType() == ENEMY_ITEM12 || this->getSelfType() == ENEMY_ITEM16 || this->getSelfType() == ENEMY_ITEM20
			|| this->getSelfType() == ENEMY_ITEM21)
		{
			return;
		}
		event1Object *eObj=static_cast<event1Object*>(obj);

		float x1= this->getPosition().x + this->getArmature()->getContentSize().width/2;	
		float x2= obj->getPosition().x + obj->getArmature()->getContentSize().width/2;
		float dist=fabs(x1-x2);
		if (eObj->getm_CanDownFlag()==true&&dist<2.0)//����ǿ��Ե���ȥ�Ļ�
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
				// ��������Ȧ����
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
		// ���Ž�����ɾ���Լ�
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

//�����˴��͵Ķ���ħ��

void enemyObjects::checkIsMagicTrans(GameObjects *obj){//����Ƿ�������ħ�����͵�
	if (obj->getobjectType()==TYPE_MAGIC)
	{
		float x1=this->getPosition().x-60;
		float x2=obj->getPosition().x;
		float dist=fabs(x1-x2);
		magicObjects *magic=static_cast<magicObjects*>(obj);
		if (magic->getSelfType()==MAGIC_TRANSMIT&&magic->getotherType()==TRANSMIT_IN&&dist<3.0&&magic->getCountNum()<magic->getmaxNum())//����Ǵ��͵Ļ�
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
	
		if (magic->getSelfType()==MAGIC_TRANSMIT&&magic->getCountNum()>=magic->getmaxNum()&&magic->getisDel()==NO)//���������Ŀ�Ļ���ɾ��
		{
			//magic->delayKeepHereCallback(nullptr);//ɾ��

		  magic->setisDel(YES);
			magic->delayKeepHere(1.0);
		
			StoreyData* _groundData = _scenedata->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_GROUND);//����Ƿ���½���ϵ�

			for (Ref *sp:_groundData->getObjectData())//ѭ�������ӽڵ�
			{
				GameObjects *obj=(GameObjects*)sp;
				if (obj->getobjectType()==TYPE_MAGIC)
				{
					
						magicObjects *magic1=static_cast<magicObjects*>(obj);
					if (magic1->getSelfType()==MAGIC_TRANSMIT&&magic1->getotherType()==TRANSMIT_OUT)
					{
						//magic1->delayKeepHereCallback(nullptr);//ɾ��
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

//���С����
void enemyObjects::changeToSheep(float time){//�Ȱѵ���͸���ȵ��ͣ�Ȼ����load �����ö�ʱ�� 
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
void enemyObjects::createSheepArmature(){//��������
	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_Magic20.png","td_Magic20.plist","td_Magic2.ExportJson");  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
	sheepArmature = Armature::create("td_Magic2");   
	sheepArmature->getAnimation()->setAnimationScale((float)0.8);
	sheepArmature->getAnimation()->play("load1",-1,0);
	this->addChild(sheepArmature);
	sheepArmature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(enemyObjects::onFrameEvent)); 
}
void enemyObjects::backEnemyEffect(){//������֮ǰ��һ��
	effectsObject* effects = effectsObject::create();
	effects->setPosition(Point(this->getPositionX(),this->getPositionY()+25));
	effects->sheepEffect();
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,this->getZOrder()+1);

}
void enemyObjects::backEnemy(){//��ص���
	this->removeChild(sheepArmature,true);
	sheepArmature=nullptr;
	isStart=YES;
	this->load();
	_armature->setVisible(true);
}
// Event106�¼���Я������ħ���Ĺ���
void enemyObjects::setEnemyObjectEvent106()
{
	_armature->setColor(Color3B(59, 255, 89));
	setIsEvent106_Object(true);
}


//С������������С������֮��ı���
void enemyObjects::xiaogoufawei(int mlv){

	isStart=NO;
	this->MakeWarToLoad();//�ô��ҵ�սʿ��ͣ��
	load();//���loadingһ��
	 _isIntimidate=YES;
	_lev=mlv;	
	auto action = Sequence::create(
			DelayTime::create(0.8),
			CallFunc::create(CC_CALLBACK_0(enemyObjects::xiaogoufaweiBackRun, this)),
			NULL);
	this->runAction(action);


	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_war_05_kongxia.ExportJson");  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
	Armature *arm = Armature::create("td_war_05_kongxia");   
	arm->setPosition(0,_armature->getContentSize().height*_armature->getScale()-20);
	this->addChild(arm,1);
     arm->getAnimation()->play("play",-1,1);

}

void enemyObjects::xiaogoufaweiBackRun(){
	
	 this->Run();
	 isStart=YES;
}

//���������ܵ�AI
void enemyObjects::newAiForBackRun(){
	
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
			float thisPosX=this->getPositionX();
			float moveWidthCenterX=_swg->getPositionX();

			
			if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&_states!=STATES_FIGHTING&&obj->getisFrozen()==NO&&
				obj->getisIntimidate()==NO)//Ѱ��һ�����ˣ�û�������߲���ս����ʱ��,����ͬһ��֮��
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

				
					int tempNearestX=dist - safeZone;//������

					if (nearestX==0)//����ǵ���0��ʱ��Ѱ�ҵ���һ��Ŀ��
					{  
						if (obj->getisFly()==NO&&obj->getstates()!=STATES_FIGHTING){
							isFind=true;//��ʾѰ�ҵ�
							nearestX=tempNearestX;//������
							targetObj=obj;//��ȡһ��Ŀ��
							targetAddr=(uintptr_t)targetObj;
						}

					}else {//����Ļ���˵���ǻ��еڶ���Ŀ���˻��߶��Ŀ��,����Ƕ��Ŀ��Ļ���Ѱ��һ��û����ս��״̬�ĵ��˽��й��������뵱ǰĿ�����
						
						if (obj->getisFly()==NO&&obj->getstates()!=STATES_FIGHTING)
						{
							if (nearestX>tempNearestX)
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


	if (isFind==true&&targetAddr!=0&&_states!=STATES_FIGHTING&&targetObj!=nullptr)//���������Ŀ��Ļ�,������û��ս��״̬��ʱ��
	{
		float thisPosX=this->getPosition().x;
		float objPosX=targetObj->getPosition().x;
		float tempAtkLen=_atkLen;

		float dist = fabs( thisPosX- objPosX );
		float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
			targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
			LEN_SCALE;

		float movespeed=_moveSpeedScale+1.0;
		if (dist>=safeZone &&  dist-safeZone<=tempAtkLen )//˵���ڹ�����Χ��
		{
				
			this->Fight();//ս����
			
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




	if (isFind==false)//���ûѰ�ҵ�
	{

		if (_states==STATES_FIGHTING&&targetAddr==0)//���û�ҵ��ұ�����ս��
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