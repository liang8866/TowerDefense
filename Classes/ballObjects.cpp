
#include "ballObjects.h"
#include "SceneData.h"
#include "StoreyData.h"
USING_NS_CC;

ballObjects::ballObjects(void){

}
ballObjects::~ballObjects(void){
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, PAUSEKEY);
	NotificationCenter::sharedNotificationCenter()->removeObserver(this, RESUMEKEY);
}


 void ballObjects::myPauseAllAction(Ref *obj){
 
 	this->pauseSchedulerAndActions();
 	
 }
 void ballObjects::myResumeAllAction(Ref *obj){
 
 	this->resumeSchedulerAndActions();
 	
 }
ballObjects* ballObjects::create(int tagAddr,Point tagPos,int Type,int floorNum,int derect){
	ballObjects *sprite = new ballObjects();
	 std::string bspriteFrameName;
	if (Type==BALL_WAR02)
	{
		bspriteFrameName="td_war_02_zidan.png";
	}
	if (Type==BALL_BOSS1)
	{
		bspriteFrameName="td_boss_01_zidan.png";
	}
	if (Type==BALL_BOSS3)
	{
		bspriteFrameName="td_boss3_feixing.png";
	}
	if (Type==BALL_BOSS5)
	{
		bspriteFrameName="td_boss_05_gongjixingxing.png";
	}

	if ( sprite && sprite->initWithSpriteFrameName(bspriteFrameName))
	{
		sprite-> ObjectsInit(tagAddr,tagPos,Type,floorNum,derect);
		sprite->autorelease();

		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}


 bool ballObjects::ObjectsInit(int tagAddr,Point tagPos,int Type,int floorNum,int derect){ //自定义的初始化
	
	 b_targetAddr=tagAddr;
	 b_targetPos=tagPos;
	 b_type=Type;
	 b_direct=derect;
	 moveLineX=(float)7.0;
	 if (Type==BALL_WAR02)
	 {
		 moveLineX=(float)7.0;
	 }
	 if (Type==BALL_BOSS1||BALL_BOSS3||BALL_BOSS5)
	 {
		  moveLineX=(float)10.0;
	 }
	 this->schedule(schedule_selector(ballObjects::myupdate),(float) 1.0/60);//定时器
	SceneData* _scenedata = SceneData::getInstance();
	 _storeydata  = _scenedata->getSceneDataMapStoreyFormStoreyIndex(floorNum);

	 NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ballObjects::myPauseAllAction),PAUSEKEY,NULL);
	 NotificationCenter::getInstance()->addObserver(this,callfuncO_selector(ballObjects::myResumeAllAction),RESUMEKEY,NULL);

	 return true;
}


 void  ballObjects::myupdate(float dt){


	 if (FindGameObject()==false)//说明碰撞了
	 {

		
		 if (b_direct==D_LEFT)//向左
		 {
			 if (this->getPositionX()>b_targetPos.x)
			 {
				 this->setPosition(this->getPositionX()-moveLineX,this->getPositionY());
			 }else{
				 this->unschedule(schedule_selector(ballObjects::myupdate));
				 this->removeFromParentAndCleanup(true);
			 }

		 }
		 else if (b_direct==D_RIGHT)//向右
		 {
			 this->setRotationY(180);
			 if (this->getPositionX()<b_targetPos.x)
			 {
				  this->setPosition(this->getPositionX()+moveLineX,this->getPositionY());
			 }else{
				 this->unschedule(schedule_selector(ballObjects::myupdate));
				 this->removeFromParentAndCleanup(true);
			 }
		 }

	 }

 }


 Animate* ballObjects::CreateAnimation(const char*FrameName,int max){

	 Vector<SpriteFrame*> aniFrames(max);
	 char str[100] = {0};
	 for(int i = 1; i <= max; i++)
	 {
		 SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(__String::createWithFormat("%s%d.png",FrameName,i)->getCString());
		 aniFrames.pushBack(frame);
	 }
	 Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.08f);
	 Animate *animate = Animate::create(animation);

	 return animate;

 }

bool ballObjects::FindGameObject(){

	bool f=false;

	 if (!_storeydata)
		 return false;

	 for (Object *sp:_storeydata->getObjectData())//循环遍历子节点
	 {

		 GameObjects *obj=(GameObjects*)sp;
		 if ((obj->getobjectType()==TYPE_ENEMY)&&(uintptr_t)obj==b_targetAddr)
		 {

			 if (fabs(obj->getPositionX()-b_shooterPos.x)<60)
			 {
				 f=true;
				 obj->setReduceHp(b_atkOption,b_atk,b_cri,b_cri_damage);
				 // obj->setReduceHp(b_atkOption,b_atk,1.0,b_cri_damage);
				 this->unschedule(schedule_selector(ballObjects::myupdate));
				 if (obj->getstates()==STATES_RUN)
				 {
					 if (b_direct==D_LEFT)
					 {
						 this->setPosition(obj->getPositionX()+10,this->getPositionY());
					 }else if (b_direct==D_RIGHT)
					 {
						 this->setPosition(obj->getPositionX()-10,this->getPositionY());
					 }
				 }else{
					 this->setPosition(obj->getPositionX(),this->getPositionY());
				 }
				 this->bombEffect();//爆炸啦
				 break;

			 }else
			 {
				 
				 float x1=this->getPositionX();
				 float x2=obj->getPositionX();

				 float temLen=5.0;
				 if (b_type==BALL_BOSS1||b_type==BALL_BOSS3)
				 {
					 temLen=15.0;
				 }

				 if (fabs(x1-x2)<temLen||(b_direct==D_LEFT&&(x1<x2))||(b_direct==D_RIGHT&&(x1>x2)))
				 {
					 f=true;
					 obj->setReduceHp(b_atkOption,b_atk,b_cri,b_cri_damage);
					 // obj->setReduceHp(b_atkOption,b_atk,1.0,b_cri_damage);
					 this->unschedule(schedule_selector(ballObjects::myupdate));
					 if (obj->getstates()==STATES_RUN)
					 {
						 if (b_direct==D_LEFT)
						 {
							 this->setPosition(obj->getPositionX()+10,this->getPositionY());
						 }else if (b_direct==D_RIGHT)
						 {
							 this->setPosition(obj->getPositionX()-10,this->getPositionY());
						 }
					 }else{
						 this->setPosition(obj->getPositionX(),this->getPositionY());
					 }

					  this->bombEffect();//爆炸啦
					 break;
				 }
				 




			 }

		 }
	 }
	 return f;

 }



void ballObjects::bombEffect(){
	
	char*nameFrame;
	int max=0;
	if (b_type==BALL_WAR02)
	{
		nameFrame="td_war_02_jz";
		max=6;
	}
	if (b_type==BALL_BOSS1)
	{
		nameFrame="td_boss_01_jizhong_0";
		max=4;
	}

	if (b_type==BALL_BOSS3)
	{
		nameFrame="td_boss3_base_0";
		max=8;
	}

	if (b_type==BALL_BOSS5)
	{
		nameFrame="td_boss5_effect_0";
		max=5;
	}
	Animate *animate = this->CreateAnimation(nameFrame,max);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(ballObjects::bombEffectCallback, this)),
		NULL);
	
 	this->runAction(action);

}
void ballObjects::bombEffectCallback(Object *p){
	this->removeFromParentAndCleanup(true);
}