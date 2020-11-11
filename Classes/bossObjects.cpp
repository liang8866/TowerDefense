#include "bossObjects.h"
#include "SceneData.h"
#include "WindowManager.h"
#include "ballObjects.h"
#include "magicObjects.h"
USING_NS_CC;
bossObjects::bossObjects(void)
{
	
}
bossObjects::~bossObjects(void)
{
	RemoveListenerTouch();
	releaseMoveListener();
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, RESETBOSSSKILL);
	//主界面英雄头像隐藏
	if (getisSelect()){
		Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
		if (mainLayer)
			mainLayer->hideHeroInfo();
	}
}
// on "init" you need to initialize your instance


bool bossObjects::bossObjectsInit(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{
	
	_MisPixel=0;
	targetAddr=0;
	_selfType=otype;
	_layer=_l;
	cdleftTime=0.0;//计算技能冷却时间的
	_objectType=TYPE_BOSS;
	isArmature=false;// 默认设置为false 
	_states=STATES_LOAD;//设置是默认的
	_armature=nullptr;
	_isSelect=false;//默认没选中
	isPlaySkill=NO;
	this->CreateArmature(filename,name);//创建骨骼动画。。。。。。。。。。。。。


	_selectFrame = nullptr;

	mIsMouseMoving = false;		// 是否鼠标点击移动
	mMouseMovePos = Point(0,0);	// 是否鼠标点击移动到的目的地

	this->setProperty();//设置属性

	this->CreateListenerTouch_Armature();

	this->schedule(schedule_selector(bossObjects::bossupdate),(float)1.0/60);
	this->l_createCAtkLenObject();//创建透明底片攻击范围显示框

	_mouseListener = nullptr;
	_moveListener = nullptr;

	NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(bossObjects::reSetAttribute),RESETBOSSSKILL,NULL);


	if (_selfType==BOSS_ITEM1)
	{
		audioPlayer::getInstance()->playEffect("S21.mp3");
	}
	if (_selfType==BOSS_ITEM2)
	{
		audioPlayer::getInstance()->playEffect("S22.mp3");
	}
	if (_selfType==BOSS_ITEM3)
	{
		audioPlayer::getInstance()->playEffect("S23.mp3");
	}
	if (_selfType==BOSS_ITEM4)
	{
		audioPlayer::getInstance()->playEffect("S24.mp3");
	}
	if (_selfType==BOSS_ITEM5)
	{
		audioPlayer::getInstance()->playEffect("S25.mp3");
	}


	return true;
}

bossObjects* bossObjects::create(const std::string& filename,const std::string&name,int otype,cocos2d::Layer *_l)
{

	bossObjects *sprite = new bossObjects();
	if (sprite && sprite->init())
	{
		sprite-> bossObjectsInit(filename,name,otype,_l);
		sprite->autorelease();

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}


void bossObjects::reSetAttribute(Ref *f)//重新获取属性
{
	__String bossName=StringUtils::format("boss%d",_selfType);//获取技能表的
	auto _haveData = UserRecord::getInstance()->getHeroData(_selfType);
	_lev= _haveData.level;
	updateLevel(_lev);

	//进阶的增益属性
	int star = _haveData.star;
	_hp+=_maxLevelHp*0.1*star;
	_atk+=_maxLevelAtk*0.1*star;
	_CRI+=0.01*star;
	_dfe+=0.01*star;
	
	this->getBossSkill();

	if (_selfType==BOSS_ITEM1)
	{

	if (_bossSkillOneLevel<= bossSkillOne_Level_1)
	{
		SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("td_boss1_skill1_level1.plist");
	}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
	{
		SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("td_boss1_skill1_level2.plist");
	}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
	{
		SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("td_boss1_skill1_level3.plist");
	}
	}

}
//设置属性
void bossObjects::setProperty()
{
	
		SpriteFrameCache* cache =  SpriteFrameCache::getInstance();
		cache->addSpriteFramesWithFile("bossBaseImage.plist");//添加箭头的
		cache->addSpriteFramesWithFile("td_boss_common.plist");//技能通用
	
	if (_selfType==BOSS_ITEM1){// boss1

		cache->addSpriteFramesWithFile("td_boss1_baseEffect.plist");//基本攻击特效。。。。。。
		
		this->createAnimationForSomeBody("td_bossEffects1_01_",5,this,Point(1,0),-1);//这个时候是脚下面的转盘动画
		
	}
	if (_selfType==BOSS_ITEM2)//英雄儿
	{
		cache->addSpriteFramesWithFile("td_boss2_skillEffect.plist");//技能特效
	}

	if (_selfType==BOSS_ITEM3)
	{
	
		cache->addSpriteFramesWithFile("td_boss3_baseEffect.plist");//基本攻击特效。。。。。。
		cache->addSpriteFramesWithFile("td_boss3_skillEffect.plist");//技能攻击特效。。。。。。

	}

	if (_selfType==BOSS_ITEM4)
	{
		cache->addSpriteFramesWithFile("td_boss4_fennu.plist");//基本攻击特效。。。。。。
		cache->addSpriteFramesWithFile("td_boss4_baseeffect.plist");//基本攻击特效。。。。。。
	}

	if (_selfType==BOSS_ITEM5)
	{
		cache->addSpriteFramesWithFile("td_boss5_effect.plist");//基本攻击特效。
	}



   this->reSetAttribute(nullptr);
	
}


void bossObjects::createAnimationForSomeBody(const char*FrameName,int max,Node *parent,Point pos,int zd){
	
	Sprite *sp=Sprite::createWithSpriteFrameName(__String::createWithFormat("%s%02d.png",FrameName,1)->getCString());
	sp->setPosition(pos);
	parent->addChild(sp,zd);

	Animate *ani=this->CreateAnimation(FrameName,max);
	
	RepeatForever *repeat = RepeatForever::create(ani);  
	sp->runAction(repeat);
	
}
Animate* bossObjects::CreateAnimation(const char*FrameName,int max){

	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.08f);
	Animate *animate = Animate::create(animation);

	return animate;

}



void bossObjects::Load(){
	
	_states=STATES_LOAD;

	_armature->getAnimation()->play("load",-1,1);
	targetAddr=0;
	
	this->BackHp();//回血啦。。

	if (_selfType==BOSS_ITEM4)
	{
		_xiaohai->getAnimation()->play("load",-1,1);
	
		if (isChangeFlag==YES)
		{
			
			this->stopActionByTag(1836);
			auto delay1=DelayTime::create(2.0f);
			auto ddlay = Sequence::create(
				delay1,
				CallFuncN::create(CC_CALLBACK_0(bossObjects::comebackXiaohai,this)),
				NULL);
			ddlay->setTag(1836);
			this->runAction(ddlay);
		}
		
	}


}

void bossObjects::comebackXiaohai(){
	
	if (_states==STATES_LOAD)
	{
		this->Boss4ChangeEffect();
		
	}else{
		this->stopActionByTag(1836);
	}
		
	
}

void bossObjects::Run()//正在跑步
{
	
	_isRun=true;
	_states=STATES_RUN;
	_armature->getAnimation()->play("run",-1,1);
	
	if (_selfType==BOSS_ITEM4)
	{
		_xiaohai->getAnimation()->play("run",-1,1);
	}

}

void bossObjects::Fight_Begin(){//正在战斗
	
	_states=STATES_FIGHTING;//正在战斗


if (_selfType==BOSS_ITEM1)//超人的话是感叹号
{
	
	Sprite *gantanhao=Sprite::createWithSpriteFrameName("GanTanHao.png");
	gantanhao->setPosition(_armature->getContentSize().width/2,_armature->getContentSize().height*_armature->getScale());
	this->addChild(gantanhao,3);
	gantanhao->setScale((float)0.2);
	float t=(float)0.20;
	auto sca=CCScaleTo::create(t,0.8);
	auto move=MoveBy::create(t,Point(0,20));
	auto delay=CCDelayTime::create((float)0.2);
	auto spa=Spawn::create(sca,move,NULL);
	auto action = Sequence::create(spa,delay,CallFuncN::create(CC_CALLBACK_1(bossObjects::gantanhaoCallback, this)),NULL);
	gantanhao->runAction(action);
}

if (_selfType==BOSS_ITEM2)
{
	_armature->getAnimation()->play("zhunbei",-1,1);
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
}
	
if (_selfType==BOSS_ITEM3 ||_selfType==BOSS_ITEM5)
{
	this->Fight_Mid();
}
	


if (_selfType==BOSS_ITEM4&&isChangeFlag==NO)//超人的话是感叹号
{

	Sprite *gantanhao=Sprite::createWithSpriteFrameName("GanTanHao.png");
	gantanhao->setPosition(_xiaohai->getContentSize().width/2,_xiaohai->getContentSize().height*_xiaohai->getScale());
	this->addChild(gantanhao,3);
	gantanhao->setScale((float)0.2);
	float t=(float)0.20;
	auto sca=CCScaleTo::create(t,0.8);
	auto move=MoveBy::create(t,Point(0,20));
	auto delay=CCDelayTime::create((float)0.2);
	auto spa=Spawn::create(sca,move,NULL);
	auto action = Sequence::create(spa,delay,CallFuncN::create(CC_CALLBACK_1(bossObjects::gantanhaoCallback, this)),NULL);
	gantanhao->runAction(action);
}else{
	this->Fight_Mid();
}

}
void bossObjects::gantanhaoCallback(Object *psender){

	Sprite *sender=static_cast<Sprite *>(psender);
	sender->removeFromParentAndCleanup(true);
	this->Fight_Mid();

}


 void bossObjects::bossCreateBall(GameObjects *tempObj,int etype){

	 int d=D_LEFT;
	 if (tempObj->getPositionX()>this->getPositionX())
	 {
		 d=D_RIGHT;
	 }
	 ballObjects*ball=ballObjects::create(targetAddr,tempObj->getPosition(),etype,_floorNum,d);
	 ball->setb_cri(_CRI);
	 ball->setb_cri_damage(_CRI_damage);

	 if (tempObj->getPositionX()>this->getPositionX())//对象在右边
	 {
		 ball->setPosition(this->getPositionX()+_armature->getContentSize().width/2*_armature->getScale()-10,_armature->getContentSize().height/2*_armature->getScale()-10);

	 }else//对象在左边
	 {
		 ball->setPosition(this->getPositionX()-_armature->getContentSize().width/2*_armature->getScale()+10,_armature->getContentSize().height/2*_armature->getScale()-10);
	 }
	 ball->setb_atk(_atk);
	 ball->setb_atkOption(_atkOption);
	 ball->setB_shooterPos(this->getPosition());
	 Widget *w=static_cast<Widget*>(this->getParent());
	 w->addChild(ball, this->getZOrder());

 }

void bossObjects::Fight_Mid(){//正在战斗


	if (_selfType==BOSS_ITEM1)
	{
		_armature->getAnimation()->play("atk_mid",-1,1);
		if (_isSpecialColorFlag==true)
		{
			//要保持颜色和速度播放
			_armature->setColor(SKILLCOLOR);
			float speedscale=_skill1_buff_atkScale+1.0;
			_armature->getAnimation()->setSpeedScale(speedscale);//设置速度
		}
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
		bool isfind=false;
		for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
		{
			GameObjects *obj=(GameObjects*)sp;
			if ((uintptr_t)obj==targetAddr)
			{
				isfind=true;
				//这里应该显示 打击的特效
				break;
			}
		}

		if (isfind==false)//是否寻找到
		{
			CCLOG("isfind==false");
			this->Load();
			targetAddr=0;
		}
	}
	
	if (_selfType==BOSS_ITEM2||_selfType==BOSS_ITEM3||_selfType==BOSS_ITEM5)//近程攻击的,治疗的
	{
		
		_armature->getAnimation()->play("atk",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
		
	}
	

	if ( _selfType == BOSS_ITEM4)//如果是英雄4的话
	{
		_armature->getAnimation()->play("atk",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
		if (isChangeFlag==NO)
		{
			this->Boss4ChangeEffect();//变身
		}
		
	}

	
}


void bossObjects::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	//CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);
	
	if (_selfType==BOSS_ITEM4)
	{                                         
		if (strcmp(evt.c_str(),"talkfinish")==0)
		{ 	
			isStart=YES;
			this->Load();
		   isPlaySkill=NO;
		}
	}

	
	if (_selfType==BALL_BOSS3)//治疗英雄的
	{
		if (strcmp(evt.c_str(),"addhp")==0)
		{ 	
			
			this->boss3SkillAction();

		}
	}



	if (_selfType==BOSS_ITEM2)
	{
		if (strcmp(evt.c_str(),"readyFinish")==0)
		{ 	
			this->Fight_Mid();
		}
		if (strcmp(evt.c_str(),"skillatk")==0)
		{ 	
			this->boss2FindObject();
		}
		if (strcmp(evt.c_str(),"skillend")==0)
		{ 	
			this->Load();
			isStart=YES;
			isPlaySkill=NO;
		}
	}


	if (_selfType==BOSS_ITEM5)//召唤精灵
	{
		if (strcmp(evt.c_str(),"callstart")==0)
		{ 	
			
			Window* _curWindow = WindowManager::getInstance()->getWindow("BackGround");
			Ui_MainBack* _curBackGroundWnd = static_cast<Ui_MainBack*>(_curWindow);
			_curBackGroundWnd->CreateBoss5jingling(this);
		}
		if (strcmp(evt.c_str(),"callfinish")==0)
		{ 	
			_armature->getAnimation()->stop();
			this->Load();
			isStart=YES;
			isPlaySkill=NO;
		}

	}


	if (_selfType==BOSS_ITEM1)
	{
		
		if (strcmp(evt.c_str(),"m_begin")==0||strcmp(evt.c_str(),"m_reducehp")==0)
		{ 	
			GameObjects *tpobj=FindObject(targetAddr);

			if (tpobj!=nullptr)
			{
				if (this->getPositionX()>tpobj->getPositionX())
				{
					_armature->setRotationY(0);
				}else{
					_armature->setRotationY(180);
				}

				if (tpobj->gethp()<=0)
				{
					this->Load();
					targetAddr=0;
				}else{
					 this->bossCreateBall(tpobj,BALL_BOSS1);//创建子弹
					 audioPlayer::getInstance()->playEffect("S46.mp3");	//超人的攻击声音
				}
			}else if (tpobj==nullptr)
			{
				this->Load();
				targetAddr=0;
			}
		}
	}else{

		if (strcmp(evt.c_str(),"begin")==0)
		{
			GameObjects *tempObj=this->FindObject(targetAddr);
			if (tempObj==nullptr)//没寻找到
			{ 
				this->Load();
				
			}
		}


		if (strcmp(evt.c_str(),"reducehp")==0)
		{
			GameObjects *tempObj=this->FindObject(targetAddr);
			if (tempObj==nullptr)//没寻找到
			{ 
				this->Load();
				
			}else{//寻找到
             
				if (this->getPositionX()>tempObj->getPositionX())
				{
					_armature->setRotationY(0);
				}else{
					_armature->setRotationY(180);
				}

			   if (_selfType==BOSS_ITEM3)
			   {
				    this->bossCreateBall(tempObj,BALL_BOSS3);//创建子弹
					audioPlayer::getInstance()->playEffect("S48.mp3");	
			   }else if (_selfType==BOSS_ITEM5)
			   {
				    this->bossCreateBall(tempObj,BALL_BOSS5);//创建子弹
					audioPlayer::getInstance()->playEffect("S50.mp3");
			   }
			   else{
				     tempObj->setReduceHp(this->_atkOption,this->_atk,_CRI,_CRI_damage);
					 if (_selfType==BOSS_ITEM2)
					 {
						audioPlayer::getInstance()->playEffect("S47.mp3");
					 } if (_selfType==BOSS_ITEM4)
					 {
						audioPlayer::getInstance()->playEffect("S49.mp3");
					 }
			   }

			}
		}
	}

}


//创建骨骼动画相关函数 
void bossObjects::CreateArmature(const std::string& filename,const std::string&name )
{

	
	isChangeFlag=NO;
	isArmature=true;//设置为true
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	_armature = Armature::create(name);   
	_armature->setPosition(0,0);

	if (_selfType==BOSS_ITEM2)
	{
		//_armature->setScale(1.0/0.7);
	}
	 if (_selfType==BOSS_ITEM4)
	{
		_armature->setVisible(false);//创建了，但是不可见
		ArmatureDataManager::getInstance()->addArmatureFileInfo("td_boss_04_xiaohai0.png","td_boss_04_xiaohai0.plist","td_boss_04_xiaohai.ExportJson");  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
		_xiaohai = Armature::create("td_boss_04_xiaohai");   
		_xiaohai->setPosition(0,0);
		_xiaohai->setScale(1.2);
		this->addChild(_xiaohai,1);
		
	}
	this->addChild(_armature,1);
	this->Load();
}


void bossObjects::CreateBoss4Armature1()//变成胖子
{
	
	isChangeFlag=YES;
	_armature->setVisible(true);
	
	if (_selectFrame)
	{
		_selectFrame->setPosition(0,_armature->getContentSize().height*_armature->getScale()+15);
	}
	
	
}

void bossObjects::CreateBoss4Armature2()//变回小孩
{
	
	isChangeFlag=NO;
	_xiaohai->setVisible(true);

	if (_selectFrame)
	{
		_selectFrame->setPosition(0,_xiaohai->getContentSize().height+15);
	}
}

void  bossObjects::Boss4ChangeEffect(){


	effectsObject* effects = effectsObject::create();
	effects->setPosition(Point(this->getPositionX(),this->getPositionY()+25));
	effects->Boss4ChangeEffect();
	auto parent = static_cast<Widget*>(this->getParent());
	parent->addChild(effects,this->getZOrder()+1);

	if (isChangeFlag==NO)//这次变成胖子
	{
		_xiaohai->setVisible(false);
		auto delay1=DelayTime::create(0.5f);
		auto ddlay = Sequence::create(
			delay1,
			CallFuncN::create(CC_CALLBACK_0(bossObjects::CreateBoss4Armature1,this)),
			NULL);
		this->runAction(ddlay);

	}else//这次是变成小孩
	{
		_armature->setVisible(false);
		auto delay1=DelayTime::create(0.6f);
		auto ddlay = Sequence::create(
			delay1,
			CallFuncN::create(CC_CALLBACK_0(bossObjects::CreateBoss4Armature2,this)),
			NULL);
		this->runAction(ddlay);
	}
	


}



//箭头上下移动
void bossObjects::CreateSelectFrame(){
	if (Director::getInstance()->getRunningScene()->getChildByTag(250)){
		Director::getInstance()->getRunningScene()->removeChildByTag(250);
	}
	if (!_selectFrame)
	{
		_selectFrame=Sprite::createWithSpriteFrameName("JianTou.png");
		if (_selfType==BOSS_ITEM1||_selfType==BOSS_ITEM5)
		{
				_selectFrame->setPosition(0,_armature->getContentSize().height*_armature->getScale()+15);
		}else{
			_selectFrame->setPosition(0,_armature->getContentSize().height*_armature->getScale());
		}
		
		this->addChild(_selectFrame,2);
		//_selectFrame->setScale((float)0.5);//箭头太大了，缩小了
		auto mb1=MoveBy::create((float)0.3,Point(0,5));
		auto mb2=MoveBy::create((float)0.3,Point(0,-5));
		auto action = Sequence::create(mb1,mb2,NULL);
		RepeatForever *repeat = RepeatForever::create(action); 
		_selectFrame->runAction(repeat);

		if (_selfType==BOSS_ITEM4&&isChangeFlag==NO)
		{
			_selectFrame->setPosition(0,_xiaohai->getContentSize().height+15);
		}
	}
	_selectFrame->setVisible(true);
	
	_isSelect=true;



	// 创建鼠标移动监听
	//createMouseListener();

	createMoveListener();
	WindowManager::getInstance()->setSceneViewEnable(true, this->getfloorNum());
	auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());
	if (_curStoreyData)
	{
		_curStoreyData->setIsCanMove(true);
	}

	//主界面英雄头像显示
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	mainLayer->hideMagics();
	if (mainLayer){
		mainLayer->showHeroInfo(this->getSelfType(),this->getlev(),this);
		mainLayer->setHeroHp(this->gethp(),this->getmaxHp());
	}	
}

void  bossObjects::RemoveSelectFrame()
{
	if (_selectFrame)
	{
		_selectFrame->setVisible(false);
	}
	_isSelect=false;


	// 销毁鼠标移动监听
	//releaseMouseListener();
	releaseMoveListener();
	WindowManager::getInstance()->setSceneViewEnable(false, -1);
	//WindowManager::getInstance()->setSceneViewEnable(true, this->getfloorNum());
	auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum());
	if (_curStoreyData)
	{
		_curStoreyData->setIsCanMove(false);
	}

	//主界面英雄头像隐藏
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	if (mainLayer)
		mainLayer->hideHeroInfo();
}

//骨骼动画的触摸事件
void bossObjects::CreateListenerTouch_Armature()
{
	listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	
	CC_SAFE_RETAIN(listener);
	listener->onTouchBegan = CC_CALLBACK_2(bossObjects::onTouchBegan, this);  
	listener->onTouchMoved = CC_CALLBACK_2(bossObjects::onTouchMoved, this);  
	listener->onTouchEnded = CC_CALLBACK_2(bossObjects::onTouchEnded, this);  
	_eventDispatcher->addEventListenerWithFixedPriority(listener, TOUCH_BOSS_ZORDER); 
}


bool bossObjects::onTouchBegan(Touch* _touch, Event* _unusedEvent)  
{  

	bool isCliced=false;
	GameObjects *tempObj=SceneData::getInstance()->findExistObjSelect();

	if (tempObj!=nullptr)
	{
		isCliced=true;
		if (tempObj==this)
		{
			isCliced=false;
		}
	}

	if (isStart==YES&&isCliced==false)
	{
		Point _touchPos		= _touch->getLocation();
		Size _curSize		= this->getArmature()->getContentSize();
		Point _touchNodePos = this->convertToNodeSpace(_touchPos);
		Point _worldPoint   = Director::getInstance()->getRunningScene()->convertToWorldSpace(_touchPos);
		Point _originPoint  = Director::getInstance()->getVisibleOrigin();

		auto _distance = abs(_worldPoint.y - _originPoint.y);

		Point _curPos;
		_curPos.x = 0 - _curSize.width/2;
		_curPos.y = 0;
		Rect _curRect = Rect(_curPos.x, _curPos.y, _curSize.width, _curSize.height);

		//如果是新手引导，则英雄选中之后，玩家不能自行取消选中状态
		if (UserRecord::getInstance()->getIsFreshMan() && _isSelect){
	
			return true;
		}

		// 点中
		if (_curRect.containsPoint(_touchNodePos) && _distance>WindowManager::getInstance()->getMainLayerBottomHeight())
		{
			if (gethp() <= 0)
			{
				return true;
			}
			//放大效果
			clickedEffect();
			if (_selfType==BOSS_ITEM1)
			{
				audioPlayer::getInstance()->playEffect("S21.mp3");
			}
			if (_selfType==BOSS_ITEM2)
			{
				audioPlayer::getInstance()->playEffect("S22.mp3");
			}
			if (_selfType==BOSS_ITEM3)
			{
				audioPlayer::getInstance()->playEffect("S23.mp3");
			}
			if (_selfType==BOSS_ITEM4)
			{
				if (_armature->isVisible())
				{
					audioPlayer::getInstance()->playEffect("S24_2.mp3");
				}else
				{
					audioPlayer::getInstance()->playEffect("S24.mp3");
				}
			}
			if (_selfType==BOSS_ITEM5)
			{
				audioPlayer::getInstance()->playEffect("S25.mp3");
			}
			//新手引导教学
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
				this->CreateSelectFrame();
				
				return true;
			}

			if (_isSelect==false)
			{
				this->CreateSelectFrame();

			}else{
				this->RemoveSelectFrame();
			}
			return true;
		}else if (_distance>WindowManager::getInstance()->getMainLayerBottomHeight()){
			//判断是否点到选中的英雄所在层
			Widget* _currStoreyPanel = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_floorNum)->mItem;
			Point   _pos  = _currStoreyPanel->convertToNodeSpace(_touchPos);
			Rect    _rect = Rect(0,0,_currStoreyPanel->getSize().width,_currStoreyPanel->getSize().height);

			// 新方式，点击英雄除外，都取消选择
			
			if (_isSelect==true)
			{
				this->RemoveSelectFrame();
			}
		}else{
			if (_isSelect==true)
			{
				this->RemoveSelectFrame();
			}
		}
	}
	return false;
} 

void  bossObjects::onTouchMoved(Touch *touch, Event *unused_event)  
{  
	
}  

void  bossObjects::onTouchEnded(Touch *touch, Event *unused_event)  
{  
}  

//这个是删除触摸事件的
void bossObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
	listener = nullptr;
}


/**
 英雄AI——英雄AI基本逻辑，警戒，当有勇士进入警戒范围内与之进行战斗，
	直至双方一方HP=0结束，结束后搜索下一目标，（都是选择最近的一个目标战斗）
	重复战斗逻辑。如战斗中玩家操作英雄移动则优先执行移动命令，英雄拥有的技能战斗中由玩家
	手动使用，技能的使用优先级最高，打断当前动作优先执行技能释放动作
**/					

//主定时器......................

void bossObjects::bossupdate(float dt){

    //计算技能冷却时间的.................
	if (cdleftTime>0.0)
	{
		cdleftTime-=dt;
		if (cdleftTime<0.0)
		{
			cdleftTime=0.0;
		}
	}


	//更新英雄血条的
	//选中英雄扣血头像表现
	if ( this->getobjectType()==TYPE_BOSS && this->getisSelect() &&isStart==YES){
		Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
		mainLayer->setHeroHp(this->gethp(),this->getmaxHp());
	}

	if (isStart==YES&&mIsMouseMoving==false&&_hp>0)
	{
		
		bool isFind=false;
		GameObjects *targetObj=nullptr;

		int nearestX=0;//如果有多个目标的话记录最近的一个目标最近的一个

		for (Object *sp:_storeydata->getObjectData())
		{
			GameObjects *obj=(GameObjects*)sp;
			if (obj->getobjectType()==TYPE_ENEMY&&_floorNum==obj->getfloorNum()&&_states!=STATES_FIGHTING&& 
				obj->gethp() > 0&&obj->getisFly()!=YES&&obj->getisFrozen()==NO&&obj->getisIntimidate()==NO)//寻找一个敌人，没死亡或者不在战斗的时候,且在同一层之内,和在攻击范围内
			{
				
				float moveWidthCenterX=_swg->getPositionX();
				float thisPosX=this->getPositionX();
				float dist = fabs( thisPosX- obj->getPositionX());


					float tempAtkLen=_atkLen;
					if (_atkLenOption==ATKLEN_NEAR&&obj->getatkLenOption()==ATKLEN_NEAR)
					{					
						tempAtkLen=LEN_MID;
					}else if (_atkLenOption==ATKLEN_NEAR&&obj->getatkLenOption()!=ATKLEN_NEAR)
					{
						tempAtkLen=this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() 
							 							+ obj ->getArmature()->getContentSize().width/2*obj->getArmature()->getScale();
					}

				if (obj->getPositionX()<(moveWidthCenterX+_swg->getSize().width/2)&&obj->getPositionX()>(moveWidthCenterX-_swg->getSize().width/2)&&dist<=tempAtkLen)
				{
					
					
						float tempNearestX=dist;
					if (nearestX==0)//如果是等于0的时候，寻找到第一个目标
					{
						nearestX=tempNearestX;//距离差距
						targetObj=obj;//获取一个目标
						targetAddr=(uintptr_t)targetObj;
						isFind=true;//表示寻找到
						
					}else {//否则的话，说明是还有第二个目标了或者多个目标
						if (tempNearestX<=nearestX)
						{
							targetObj=obj;
							nearestX=tempNearestX;
						    targetAddr=(uintptr_t)targetObj;
							isFind=true;//表示寻找到
						}
					}
				}
			}
		}

		if (isFind==true&&targetObj!=nullptr&&_states!=STATES_FIGHTING&&_armature!=nullptr)//如果寻找到不在跑步的时候和不在战斗的时候,
		{
			
			
			float thisPosX=this->getPosition().x;
			float objPosX=targetObj->getPosition().x;
			
			float dist = fabs( thisPosX- targetObj->getPositionX());

			float tempLen=_atkLen;
			if (_atkLenOption==ATKLEN_NEAR)
			{
				tempLen=this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() 
				+ targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale();
			}

// 			float safeZone = this->getArmature()->getContentSize().width/2*this->getArmature()->getScale() + 
// 							 targetObj ->getArmature()->getContentSize().width/2*targetObj->getArmature()->getScale() +
// 				             LEN_SCALE;
		
			//if (dist>=safeZone && dist-safeZone<=_atkLen)//说明在攻击范围内

		
			if ( dist<=tempLen)//说明在攻击范围内
			{
				this->Fight_Begin();//战斗啦
			
				if (thisPosX>objPosX)
				{ 
					if (_armature!=nullptr)
					{
						_armature->setRotationY(0);
					}
					
				}else{
					if (_armature!=nullptr)
					{
						_armature->setRotationY(180);
					}
					
				}

			}
			
		}

		if (isFind==false)
		{
			if (_states==STATES_FIGHTING&&targetAddr==0)//如果没找到且本身在战斗
			{

				this->Load();
				
				targetAddr=0;
			}
		}

		
	}


	if (mIsMouseMoving==true&&isPlaySkill==NO)
	{
		updateBossMove(dt);

		
	}
	
}


//新的。。。。。。。。。。。。。。。。。
void bossObjects::beginPressBossRun(int dirction){
	mIsMouseMoving = true;	// 开始移动 
	isStart=NO;

	if (_isSelect==true)//如果被选中的时候
	{
		if (isPlaySkill==NO)
		{
			this->Run();//切换成跑步动作
		}
		



		targetAddr=0;    
		
		float moveWidthCenterX=_swg->getPositionX();
		float thisPosX=this->getPositionX();
		float leftposX=(moveWidthCenterX-_swg->getSize().width/2+20);
		float rightposX=(moveWidthCenterX+_swg->getSize().width/2-20);
	
		if (thisPosX<=leftposX)
		{
			this->setPosition(leftposX+5, this->getPosition().y);
		}else if (thisPosX>=rightposX)
		{
		
			this->setPosition(rightposX-5, this->getPosition().y);
		}
		
		_direct=dirction;
		int roat=_armature->getRotationY();

		if (roat==180)//说明转过
		{
			_armature->setRotationY(0);
			if (_selfType==BOSS_ITEM4)
			{
				_xiaohai->setRotationY(0);
			}
		}

		if (_direct == D_LEFT)
		{	
			
		_armature->setRotationY(0);
		if (_selfType==BOSS_ITEM4)
		{
			_xiaohai->setRotationY(0);
		}	

		}else if (_direct == D_RIGHT)
		{
			
			_armature->setRotationY(180);
			if (_selfType==BOSS_ITEM4)
			{
				_xiaohai->setRotationY(180);
			}
		}

		int roat2=_armature->getRotationY();
		

		for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
		{
			GameObjects *obj=(GameObjects*)sp;
			if (obj->gettargetAddr()==(uintptr_t)this)
			{
				
 				enemyObjects*enemy=(enemyObjects*)obj;
				enemy->load();
				

			}

		}

	}
}
//新的。。。。。。。。。。。。。。。。
void bossObjects::endPressBossRun(){

	mIsMouseMoving = false;
	
	int roat=_armature->getRotationY();

	if (isPlaySkill==NO)
	{
		isStart=YES;
		this->Load();
	}
	
	Widget* _curItem = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(this->getfloorNum())->mItem;
	if (_curItem)
	{
		if (_curItem->getChildByTag(SCENE_STOREY_MOUSE_CLICK_TAG))
		{
			_curItem->removeChildByTag(SCENE_STOREY_MOUSE_CLICK_TAG);
		}
	}
}



// 点击移动到目的地
void bossObjects::BeginTouchMoveToPoint(Point _pos)
{
	

}

// 移动结束
void bossObjects::EndTouchMoveToPoint()
{
}


void bossObjects::updateBossMove(float dt)
{

	if (mIsMouseMoving==true){
		
		if (_states==STATES_LOAD)
		{
			this->Run();
			targetAddr=0;
			for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
			{
				GameObjects *obj=(GameObjects*)sp;
				
				if (obj->getobjectType()==TYPE_ENEMY&&obj->gettargetAddr()==(uintptr_t)this)
				{
					enemyObjects*enemy=(enemyObjects*)obj;
					enemy->load();
				}
			}

		}
		
		float moveWidthCenterX=_swg->getPositionX();
		float thisPosX=this->getPositionX();
		float leftposX=(moveWidthCenterX-_swg->getSize().width/2+20);
		float rightposX=(moveWidthCenterX+_swg->getSize().width/2-20);
	
		if (thisPosX>leftposX&&thisPosX<rightposX)
		{
			
			this->setPosition(this->getPosition().x + _moveSpeedScale*_direct, this->getPosition().y);
		}else{
			this->endPressBossRun();
			
		}

	}

}


GameObjects * bossObjects::FindObject(int addr){

	GameObjects *pObj=nullptr;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_ENEMY&&(uintptr_t)obj==addr&&obj->getisFrozen()==NO&&obj->gethp()>0&&obj->getisIntimidate()==NO)
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
	}
	return pObj;
}


void bossObjects::createMouseListener()
{
	if (!_mouseListener)
	{
		_mouseListener = EventListenerMouse::create();
		CC_SAFE_RETAIN(_mouseListener);
		
		_mouseListener->onMouseUp		= CC_CALLBACK_1(bossObjects::onMouseUp, this);  
		_mouseListener->onMouseDown		= CC_CALLBACK_1(bossObjects::onMouseDown, this);  
		_mouseListener->onMouseMove		= CC_CALLBACK_1(bossObjects::onMouseMove, this);  
		_mouseListener->onMouseScroll	= CC_CALLBACK_1(bossObjects::onMouseScroll, this);  
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_mouseListener, this);
	}else
	{
		
		return;
	}
}

void bossObjects::releaseMouseListener()
{
	if (_mouseListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_mouseListener);
		_mouseListener = nullptr;
	}
}
void bossObjects::onMouseUp(Event* _event)  
{  
	EventMouse* _e = (EventMouse*)_event;
	if (_e)
	{
		if (this->getisSelect())
		{
			endPressBossRun();
		}
	}
}  
void bossObjects::onMouseDown(Event* _event)  
{  
	EventMouse* _e = (EventMouse*)_event;
	if (_e)
	{
		// 必需在选中状态
		if (this->getisSelect())
		{
			// 判断下当前点击的方向，是在Boss的左边还是右边
			float _mouseX = _e->getCursorX();
			float _objectX = this->getPositionX();

			
			// 从英雄所谓位置的中心点区分左右
			float _objectConSize = this->getArmature()->getContentSize().width/2;
			if (_mouseX > _objectX)
			{
				if ( _mouseX > _objectX + _objectConSize )
				{
					// 朝右
					beginPressBossRun(D_RIGHT);
				}
			}else if (_mouseX < _objectX)
			{
				if (_mouseX < _objectX - _objectConSize )
				{
					// 朝左
					beginPressBossRun(D_LEFT);
				}
			}
			
		}
	};  
}  
void bossObjects::onMouseMove(Event* _event)  
{  
	EventMouse* _e = (EventMouse*)_event;
	if (_e)
	{
		
	} 
}  
void bossObjects::onMouseScroll(Event* _event)  
{  
	EventMouse* _e = (EventMouse*)_event;
	if (_e)
	{
		
	} 
}  






void bossObjects::createMoveListener()
{
	if (!_moveListener)
	{
		_moveListener = EventListenerTouchOneByOne::create();
		CC_SAFE_RETAIN(_moveListener);
		_moveListener->onTouchBegan		= CC_CALLBACK_2(bossObjects::onMoveTouchBegan, this);  
		_moveListener->onTouchMoved		= CC_CALLBACK_2(bossObjects::onMoveTouchMoved, this);  
		_moveListener->onTouchEnded		= CC_CALLBACK_2(bossObjects::onMoveTouchEnded, this);  
		_moveListener->onTouchCancelled	= CC_CALLBACK_2(bossObjects::onMoveTouchCancelled, this);  
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_moveListener, this);
	}else
	{
	
		return;
	}
}

void bossObjects::releaseMoveListener()
{
	if (_moveListener)
	{
		Director::getInstance()->getEventDispatcher()->removeEventListener(_moveListener);
		_moveListener = nullptr;
	}
}

bool bossObjects::onMoveTouchBegan(Touch* _touch, Event* _unusedEvent)  
{  
	// 必需在选中状态
	if (this->getisSelect())
	{
		return true;
		//// 判断下当前点击的方向，是在Boss的左边还是右边 
		//Point _touchPos		= _touch->getLocation();
		//float _mouseX =  _touchPos.x;
		//float _objectX = this->getPositionX();

		//// 从英雄所谓位置的中心点区分左右
		//float _objectConSize = this->getArmature()->getContentSize().width/2;
		//if (_mouseX > _objectX)
		//{
		//	if ( _mouseX > _objectX + _objectConSize )
		//	{
		//		// 朝右
		//		//beginPressBossRun(D_RIGHT);
		//		return true;
		//	}
		//}else if (_mouseX < _objectX)
		//{
		//	if (_mouseX < _objectX - _objectConSize )
		//	{
		//		// 朝左
		//		//beginPressBossRun(D_LEFT);
		//		return true;
		//	}
		//}
	}
	return false;
} 

void  bossObjects::onMoveTouchMoved(Touch *touch, Event *unused_event)  
{  
	 
	
}  

void  bossObjects::onMoveTouchEnded(Touch *touch, Event *unused_event)  
{  
	if (this->getisSelect())
	{
		endPressBossRun();
	}
}  
void  bossObjects::onMoveTouchCancelled(Touch *touch, Event *unused_event)  
{  

}  



void  bossObjects::getBossSkill(){//读取英雄技能属性的

	__String bossName=StringUtils::format("boss%d",_selfType);//获取技能表的
	auto _haveData = UserRecord::getInstance()->getHeroData(_selfType);
	int _skillLevel1 = _haveData.skill1_level;
	int _skillLevel2 = _haveData.skill2_level;
	int _skillLevel3 = _haveData.skill3_level;
	_bossSkillOneLevel=_skillLevel1;
	//CCLOG("skill1=%d,skill2=%d,skill3=%d",_skillLevel1,_skillLevel2,_skillLevel3);
	
	_skill2_buffValue=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),2,_skillLevel2, "skill2_buffValue").floatValue();
	_skill2_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),2,_skillLevel2, "skill2_target").intValue();
	_skill2_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),2,_skillLevel2, "skill2_isGround").intValue();
	_skill2_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),2,_skillLevel2, "skill2_targetNum").intValue();

	_skill3_buffValue=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),3,_skillLevel3, "skill3_buffValue").floatValue();
	_skill3_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),3,_skillLevel3, "skill3_target").intValue();
	_skill3_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),3,_skillLevel3, "skill3_isGround").intValue();
	_skill3_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),3,_skillLevel3, "skill3_targetNum").intValue();

// 	CCLOG("_skill2_buffvalue=%f",_skill2_buffValue);
// 	CCLOG("_skill2_target=%d",_skill2_target);
// 	CCLOG("_skill2_isGround=%d",_skill2_isGround);
// 	CCLOG("_skill2_targetNum=%d",_skill2_targetNum);
// // 
// 	CCLOG("_skill3_buffvalue=%f",_skill3_buffValue);
// 	CCLOG("_skill3_target=%d",_skill3_target);
// 	CCLOG("_skill3_isGround=%d",_skill3_isGround);
// 	CCLOG("_skill3_targetNum=%d",_skill3_targetNum);

	if (_selfType==BOSS_ITEM1)//远程英雄
	{
		
		_skill1_buff_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_isGround").intValue();
	    _skill1_buff_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_targetNum").intValue();
	    _skill1_buff_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "_buff_target").intValue();
		_skill1_buff_atkSpeed=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_atkSpeed").floatValue();
	    _skill1_buff_atkScale=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_atkScale").floatValue();
	    _skill1_skillKeepTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillKeepTime").floatValue();
		_skill1_skillCDTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillCDtime").floatValue();
			
		//远程攻击的英雄的技能1（主动技能）效果是 提升攻击力50%，攻击速度110% 自身
	   //被动技能 2 攻击力增加 只要技能升级了就一直存在
		_atk+=_atk*_skill2_buffValue;//攻击力增加

	   //被动技能3 暴击率增加
		_CRI+=_skill3_buffValue;//暴击率增加的


	}
	if (_selfType==BOSS_ITEM2)//近战英雄
	{
		_skill1_buff_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_isGround").intValue();
		_skill1_buff_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_targetNum").intValue();
		_skill1_buff_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "_buff_target").intValue();
		_skill1_buff_atk=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_atk").intValue();
		_skill1_skillKeepTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillKeepTime").floatValue();
		_skill1_skillCDTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillCDtime").floatValue();
		//震地，震晕敌人（越升级时间越久伤害越大）  等级1——初始
		 
		//被技能2 增加自身的生命值
		_hp+=_hp*_skill2_buffValue;


		 //被动技能3 增加防御塔攻击力
		 //效果在防御塔扣敌人血哪里实现 enemyObject.cpp obj->setReduceHp(_atkOption,_atk+_atk*_boss2_skill3_of_Tower,_CRI,_CRI_diamge); FindObject()中设置_boss2_skill3_of_Tower的值
		
	}
	if (_selfType==BOSS_ITEM3)//治疗英雄的
	{
		_skill1_buff_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_isGround").intValue();
		_skill1_buff_addHp=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_addHp").intValue();
		_skill1_buff_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_targetNum").intValue();
		_skill1_buff_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "_buff_target").intValue();
		_skill1_skillKeepTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillKeepTime").floatValue();
		_skill1_skillCDTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillCDtime").floatValue();
		//生命值增加X%；（越升级加的越高）  等级1——初始


		//被技能2 周围士兵攻击力增加X%
		// 在战士攻击扣对方的血哪里实现 warriorObject.cpp Fight() onFrameEvent() 2个地方 float buffValue=this->findBossSkillBuffValue(BOSS_ITEM3,PASSIVESKILLS1);//英雄3的被动技能对战士攻击有影响
	             

		//被动技能3 周围士兵防御力增加X%（越升级加的越高）
	  // GameObject.cpp  GameObjects::setReduceHp(int m_atkOption,int m_atk,_CRI.CRI_damage) 里面实现

	}
	if (_selfType==BOSS_ITEM4)//肉盾英雄的
	{
        _skill1_buff_isGround=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_isGround").intValue();
		_skill1_buff_targetNum=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "buff_targetNum").intValue();
		_skill1_buff_target=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "_buff_target").intValue();
		_skill1_skillKeepTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillKeepTime").floatValue();
		_skill1_skillCDTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillCDtime").floatValue();
		//嘲讽（越升级可以嘲讽的人越多，时间越长） 

		//被动技能2 护甲增加X%；（越升级加的越高）  等级2
		_dfe+=_skill2_buffValue;

		//被动技能3 反弹伤害的 
		 //在敌人攻击扣对方血哪里实现


	}
	if (_selfType==BOSS_ITEM5)//支援英雄
	{
		_skill1_callMonster_moveSpeedScale=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_moveSpeedScale").intValue();
		_skill1_callMonster_atkSpeed=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_atkSpeed").intValue();
		_skill1_callMonster_atkOption=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_atkLenOption").intValue();
		_skill1_callMonster_dfe=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_dfe").floatValue();
		_skill1_callMonster_dfeOption=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_dfeOption").intValue();
		_skill1_callMonster_atk=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_atk").intValue();
		_skill1_callMonster_atkOption=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_atkOption").intValue();
		_skill1_callMonster_Hp=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "callMonster_Hp").intValue();
		_skill1_skillKeepTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillKeepTime").floatValue();
		_skill1_skillCDTime=PlistManager::getInstance()->getHeroSkill(bossName.getCString(),1,_skillLevel1, "skillCDtime").floatValue();
		//_skill1_skillCDTime=5.0f;
		//召唤一个飞行单位，可对空对地（倒计时消失）越升级攻击力越高时间越久   等级1——初始

		//被动技能2
		//防御塔护甲增加X%； 
		// 在 GameObjects.cpp setReduceHp 实现

		//被动技能3  士兵暴击率增加（越升级加的越高）
	    //在warriorObjects.cpp setReduceHp 之前提前附加的暴击率 实现


	}

	
}



void bossObjects::playCommonSkillEffect(){

	audioPlayer::getInstance()->playEffect("S66.mp3");//通用技能释放前奏音效
	//this->setTouchenabled(true);
	Animate *animate = this->CreateAnimation("td_bossSkill_pre_",7);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1( bossObjects::playCommonSkillEffectCallfunc, this)),
		NULL);

	Sprite *sp=Sprite::createWithSpriteFrameName("td_bossSkill_pre_01.png");
	sp->setPosition(0,_armature->getContentSize().height/2*_armature->getScale());
	sp->setScale(1.0/0.7);
	this->addChild(sp,3);
	sp->runAction(action);
	if (_selfType==BOSS_ITEM3)
	{
		sp->setPosition(sp->getPositionX(),sp->getPositionY()-15);
	}
	if (_selfType==BOSS_ITEM4)
	{
		if (isChangeFlag==NO)//如果已经变身了
		{
		 sp->setPosition(sp->getPositionX(),_xiaohai->getContentSize().height/2);
		}
	}

}
void bossObjects::playCommonSkillEffectCallfunc(Ref *sender){

	Sprite *senSp=static_cast<Sprite*>(sender);
	senSp->removeFromParentAndCleanup(true);

	if (!UserRecord::getInstance()->getIsFreshMan()){
		NotificationCenter::getInstance()->postNotification(RESUMEKEY,  (Ref*)0);
	}

	if (_selfType==BOSS_ITEM1)//boss1的技能
	{
		isStart=NO;//
		this->Load();//
		this->boss1SkillEffect();//
		audioPlayer::getInstance()->playEffect("S53.mp3");//boss1技能释放
	}

	if (_selfType==BOSS_ITEM2)//释放boss技能
	{
		isStart=NO;
		targetAddr=0;

		if (_bossSkillOneLevel<= bossSkillOne_Level_1)
		{
			_armature->getAnimation()->play("atk2",-1,0);
		}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
		{
			_armature->getAnimation()->play("atk3",-1,0);
		}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
		{
			_armature->getAnimation()->play("atk4",-1,0);
			
		}
		
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
			audioPlayer::getInstance()->playEffect("S54.mp3");//boss1技能释放
	}

	if (_selfType==BOSS_ITEM3)//boss3的技能
	{
		isStart=NO;
		this->boss3FindObject();
		audioPlayer::getInstance()->playEffect("S55.mp3");//boss1技能释放
	}

	if (_selfType==BOSS_ITEM4)//英雄4的技能实现
	{
		audioPlayer::getInstance()->playEffect("S56.mp3");//boss1技能释放
		isStart=NO;
		targetAddr=0;
		_states=STATES_FIGHTING;
		
		if (isChangeFlag==YES)//如果已经变身了
		{
			if (_bossSkillOneLevel<= bossSkillOne_Level_1)
			{
				_armature->getAnimation()->play("talk1",-1,0);
			}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
			{
				_armature->getAnimation()->play("talk2",-1,0);
			}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
			{
				_armature->getAnimation()->play("talk3",-1,0);
			}

			_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
			this->boss4FindObject();
		}else{
			this->Boss4ChangeEffect();//先切换到

			auto delay1=DelayTime::create(1.0f);
			auto action = Sequence::create(
				delay1,
				CallFuncN::create(CC_CALLBACK_0( bossObjects::playBoss4Skill,this)),
				NULL);
			this->runAction(action);

		}


	}

	if (_selfType==BOSS_ITEM5)
	{
		audioPlayer::getInstance()->playEffect("S57.mp3");//boss1技能释放
		isStart=NO;
		_armature->getAnimation()->play("call",-1,1);
		_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
	}

}


void bossObjects::playSkillForBoss(){//释放英雄技能

	isStart=NO;
	
	_states=STATES_LOAD;
	_armature->getAnimation()->play("load",-1,1);

	isPlaySkill=YES;//正在释放技能

	if (_selfType==BOSS_ITEM4)
	{
		_xiaohai->getAnimation()->play("load",-1,1);
		if (isChangeFlag==YES)
		{
			this->stopActionByTag(1836);
			auto delay1=DelayTime::create(2.0f);
			auto ddlay = Sequence::create(
				delay1,
				CallFuncN::create(CC_CALLBACK_0(bossObjects::comebackXiaohai,this)),
				NULL);
			ddlay->setTag(1836);
			this->runAction(ddlay);
		}
	}

	if (_selfType==BOSS_ITEM5)
	{
		GameObjects *pObj=nullptr;
		for (Ref *sp:_storeydata->getObjectData())//循环遍历子节点
		{
			GameObjects *obj=(GameObjects*)sp;
			if (obj->getobjectType()==TYPE_MAGIC&&obj->getSelfType()==MAGIC_BOSS5SKILL)
			{
				magicObjects* jingling=static_cast<magicObjects*>(obj);
				jingling->delayKeepHereCallback(nullptr);
			}
		}
	}


	this->playCommonSkillEffect();
}


void bossObjects::playBoss4Skill(){
	if (_bossSkillOneLevel<= bossSkillOne_Level_1)
	{
		_armature->getAnimation()->play("talk1",-1,0);
	}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
	{
		_armature->getAnimation()->play("talk2",-1,0);
	}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
	{
		_armature->getAnimation()->play("talk3",-1,0);
	}
	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));
	this->boss4FindObject();
}


void bossObjects::boss1SkillEffect(){//英雄1的技能效果


	this->boss1SkillHuoMiao();
}


void   bossObjects::boss1SkillHuoMiao(){//火苗。。。。。。。。。

	const char * p_Name;

	if (_bossSkillOneLevel<= bossSkillOne_Level_1)
	{	
		p_Name="td_boss_01_xunhuan_kb_01_01.png";
	}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
	{
		p_Name="td_boss_01_xunhuan_kb_02_01.png";
	}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
	{
		p_Name="td_boss_01_xunhuan_kb_03_01.png";
	}    

	 	Sprite *sp=Sprite::createWithSpriteFrameName(p_Name);
	 	sp->setPosition(0,_armature->getContentSize().height/2+28);
	 	
		if (_bossSkillOneLevel> bossSkillOne_Level_2)
		{
			this->addChild(sp,2);
		}else{
			this->addChild(sp,-2);
		}

		sp->setTag(2048);
	 	sp->setScale(0.5);
		sp->setOpacity(0);

		auto  sca1=ScaleTo::create(0.1f,1.1);
		auto fade=FadeIn::create(0.1);
		auto spq=Spawn::create(sca1,fade,NULL);

		auto action = Sequence::create(
				 		spq,
			 	 		CallFuncN::create(CC_CALLBACK_1( bossObjects::boss1SkillEffectFunc, this)),
			 	     	NULL);

		sp->runAction(action);
}

void bossObjects::boss1SkillEffectFunc(Ref *f){
 	Sprite *sp=(Sprite*)f;

	const char *p_FrameStr;
	
	int p_max=13;
	if (_bossSkillOneLevel<= bossSkillOne_Level_1)
	{
		p_FrameStr="td_boss_01_xunhuan_kb_01_";
		
	}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
	{
		p_FrameStr="td_boss_01_xunhuan_kb_02_";
		
	}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
	{
		p_FrameStr="td_boss_01_xunhuan_kb_03_";
		
	}

	Animate *animate = this->CreateAnimation(p_FrameStr,p_max);
	RepeatForever *repeat1 = RepeatForever::create(animate);  
	Speed *spd=Speed::create(repeat1,0.7);
	sp->runAction(spd);

	this->Load();
	isStart=YES;
	isPlaySkill=NO;//技能释放完了
	boss1tempAtk=_atk;

	_atk+=_atk*_skill1_buff_atkScale;
	float speedscale=_skill1_buff_atkScale+1.0;
	_armature->getAnimation()->setSpeedScale(speedscale);//设置速度
	auto action = Sequence::create(
		CCDelayTime::create(_skill1_skillKeepTime),
		CallFuncN::create(CC_CALLBACK_1( bossObjects::boss1turnBackSkillBuff, this)),
		NULL);
	this->runAction(action);


}
void bossObjects::boss1turnBackSkillBuff(Ref *f){
	_atk=boss1tempAtk;
	this->removeChildByTag(2048);
	_isSpecialColorFlag=false;
	_armature->setColor(Color3B(255,255,255));
	_armature->getAnimation()->setSpeedScale(1.0);//设置速度
	
}

void bossObjects::boss2FindObject(){///英雄2技能的寻找附近有没敌人。

	int ccount=_skill1_buff_targetNum;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_ENEMY)
		{
			float x1=this->getPositionX();
			float x2=obj->getPositionX();

			if (fabs(x1-x2)<=LEN_MID&&ccount>0)
			{
				enemyObjects *enemy=static_cast<enemyObjects*>(obj);
				enemy->boss2SkillEffect(_skill1_skillKeepTime);
				ccount--;
				enemy->setReduceHp(_atkOption,_skill1_buff_atk,0,0);
			}

		}
	}
}



void bossObjects::boss3SkillAction(){

	
	int t=1;
	int Ind=(int)(_skill1_skillKeepTime*10)/(int)(1.0*10);
	int Mnd=(int)(_skill1_skillKeepTime*10)%(int)(1.0*10);
	if (Mnd>0)
	{
		t=Ind+1;
	}else{
		t=Ind;
	}
	int ccount=_skill1_buff_targetNum;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_WARRIOR||obj->getobjectType()==TYPE_TOWER)
		{
			
			if (boss3count==0)
			{
				if (ccount>0)
				{
					obj->sethp(obj->gethp()+_skill1_buff_addHp/t);
					obj->skilladdHpEffect(t,_bossSkillOneLevel);//显示效果
					ccount--;
					obj->setaddHPAddr((uintptr_t)this);
				}else{
					break;
				}
			}else{
				if (ccount>0&&obj->getaddHPAddr()==(uintptr_t)this)
				{
					obj->sethp(obj->gethp()+_skill1_buff_addHp/t);
					obj->skilladdHpEffect(t,_bossSkillOneLevel);//显示效果
					ccount--;
					obj->setaddHPAddr((uintptr_t)this);
				}else{
					break;
				}
			}
			
		}
	}

	boss3count=1;
}
void bossObjects::boss3FindObject(){
	int t=1;
	int Ind=(int)(_skill1_skillKeepTime*10)/(int)(1.0*10);
	int Mnd=(int)(_skill1_skillKeepTime*10)%(int)(1.0*10);
	if (Mnd>0)
	{
		t=Ind+1;
	}else{
		t=Ind;
	}
	
	
	auto delay=DelayTime::create(1.0*t);
	auto ddlay = Sequence::create(
		delay,
		CallFuncN::create(CC_CALLBACK_0(bossObjects::boss3TimesCallback,this)),
		NULL);
	this->runAction(ddlay);

	boss3count=0;
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_WARRIOR||obj->getobjectType()==TYPE_TOWER)
		{
			obj->setaddHPAddr(0);
	
		}
	}
	
	if (_bossSkillOneLevel<= bossSkillOne_Level_1)
	{
		_armature->getAnimation()->play("addhp1",-1,1);
	}else if (_bossSkillOneLevel> bossSkillOne_Level_1&&_bossSkillOneLevel<= bossSkillOne_Level_2)
	{
		_armature->getAnimation()->play("addhp2",-1,1);
	}else if (_bossSkillOneLevel> bossSkillOne_Level_2)
	{
		_armature->getAnimation()->play("addhp3",-1,1);
	}


	_armature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(bossObjects::onFrameEvent));


}

void bossObjects::boss3TimesCallback(){
	isStart=YES;
	this->Load();
	isPlaySkill=NO;
}


void bossObjects::boss4FindObject(){//英雄4的技能寻找敌人
	
	isPlaySkill=NO;
	int ccount=_skill1_buff_targetNum;
	float x1=this->getPositionX();
	for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	{
		GameObjects *obj=(GameObjects*)sp;
		if (obj->getobjectType()==TYPE_ENEMY)
		{
			float x2=obj->getPositionX();
			if (fabs(x1-x2)<=LEN_MID&&ccount>0)
			{
				enemyObjects *enemy=static_cast<enemyObjects*>(obj);
				enemy->boss4SkillEeffect(_skill1_skillKeepTime);
				ccount--;
			
			}

		}
	}

}