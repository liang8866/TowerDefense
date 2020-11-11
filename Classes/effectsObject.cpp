#pragma once
#include "effectsObject.h"
USING_NS_CC;
void effectsObject::Death(int t)
{
	if (t == TYPE_TOWER) 
	{
		this->TowerDeath();
	} else 
	{
		int max=13;
		Vector<SpriteFrame*> aniFrames(max);
		char str[100] = {0};
		for(int i = 1; i <= max; i++)
		{
			SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(__String::createWithFormat("%s%d.png","death-",i)->getCString());
			aniFrames.pushBack(frame);
		}
		Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
		Animate *animate = Animate::create(animation);
		this->initWithSpriteFrameName("death-1.png");
		auto action = Sequence::create(
			animate,
			CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
			NULL);
		this->setAnchorPoint(Point::ZERO);
		this->setPositionX(this->getPositionX()-this->getContentSize().width/2);
		this->runAction(action);
	}
	
}
void effectsObject::War04_bomb(){

	int max=16;
	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	auto cache = SpriteFrameCache::getInstance();
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("td_war_04_baozha-%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}



	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);

	
	this->setAnchorPoint(Point(0.5,0));
	
	this->setPosition(this->getPositionX()-this->getContentSize().width/2,-30);
	this->runAction(action);

}

void effectsObject::Trap03_bomb(){

	ArmatureDataManager::getInstance()->addArmatureFileInfo("td_trap_03_baozha0.png","td_trap_03_baozha0.plist","td_trap_03_baozha.ExportJson");
	Arm = Armature::create("td_trap_03_baozha");
	Arm->setPosition(0,0);
	this->addChild(Arm,2);
	Arm->setScale(1.25);//在骨骼动画导出来时候被缩小到0.8了
	Arm->getAnimation()->play("bomb",-1,0);
//	Arm->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(effectsObject::onFrameEvent));

	auto seq=Sequence::create(DelayTime::create(1.0),
		CallFunc::create([=](){
		this->removeFromParentAndCleanup(true);	
	}),
		NULL);
	this->runAction(seq);
}

void effectsObject::tower1_paodan_bomb(){
	Vector<SpriteFrame*> aniFrames(14);
	auto cache = SpriteFrameCache::getInstance();
	
	for(int i = 1; i <= 14; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("td_tower_01_baozha1_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	/*this->setScale(0.8);*/
	this->runAction(action);
}

void  effectsObject::onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{ 
	 //CCLOG("(%s) emit a frame event (%s) at frame index (%d).", bone->getName().c_str(), evt.c_str(), currentFrameIndex);

	if (strcmp(evt.c_str(),"finish")==0)
	{
		
//  		Arm->getAnimation()->stop();
//  		Arm->removeAllChildrenWithCleanup(true);
	
// 		auto parent = static_cast<Widget*>(this->getParent());
// 		parent->removeChild(this);
		
		this->removeFromParentAndCleanup(true);
	}
}

void effectsObject::TowerDeath()
{
	int max=11;
	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	auto cache = SpriteFrameCache::getInstance();
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("td_scene_Effect1_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->setAnchorPoint(Point(0.5,0));
	this->setScale(1.5);
	this->setPositionX(this->getPositionX()-this->getContentSize().width/2);
	this->runAction(action);
}
void effectsObject::DeathObjectCallBack(Ref *psender)
{
	
	auto parent = static_cast<Widget*>(this->getParent());
	parent->removeChild(this);
}


void effectsObject::Bullet()
{
	char str[100] = {0};
	Vector<SpriteFrame*> aniFrames(6);
	auto cache = SpriteFrameCache::getInstance();
	for(int i = 1; i <= 6; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("td_bossEffects1_02_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = Sequence::create(
		animate,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->setPositionX(this->getPositionX()-this->getContentSize().width/2);
	this->runAction(action);
}

void effectsObject::addSpeedEffects()
{
	Vector<SpriteFrame*> aniFrames(16);
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("image/tangguoji_addSpeed_effects.plist","image/tangguoji_addSpeed_effects.png");
	for(int i = 1; i <= 16; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("zjm_Effects_update_Cycle_02_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = RepeatForever::create(animate);
	this->runAction(action);
}

void effectsObject::noCDEffects()
{
	Vector<SpriteFrame*> aniFrames(14);
	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFile("image/units_noCD_efftects.plist","image/units_noCD_efftects.png");
	for(int i = 1; i <= 14; i++)
	{
		SpriteFrame *frame = cache->getSpriteFrameByName(String::createWithFormat("zjm_Effects_update_Cycle_01_%02d.png",i)->getCString());
		aniFrames.pushBack(frame);
	}

	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.10f);
	Animate *animate = Animate::create(animation);
	auto action = RepeatForever::create(animate);
	this->runAction(action);
}

void effectsObject::enemyItem20AttackEffects()
{
	Vector<SpriteFrame*> animateFrame(5);
	for (int i=1;i!=6;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_enemy_boss_remote_fly_%02d.png",i)->getCString());
		animateFrame.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(animateFrame, 0.10f);
	Animate *animate = Animate::create(animation);

	Vector<SpriteFrame*> aniFrame(12);
	for (int i=1;i!=13;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_enemy_boss_remote_hit_%02d.png",i)->getCString());
		aniFrame.pushBack(frame);
	}
	Animation *anition = Animation::createWithSpriteFrames(aniFrame, 0.10f);
	Animate *anim = Animate::create(anition);

	auto action = Sequence::create(animate,anim,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
								   NULL);
	this->runAction(action);
}

void effectsObject::enemyItem17AttackEffects()
{
	Vector<SpriteFrame*> ballFrame(7);
	for (int i=1;i!=8;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("shanDianqiu%02d.png",i)->getCString());
		ballFrame.pushBack(frame);
	}
	Animation *ballAnimation = Animation::createWithSpriteFrames(ballFrame, 0.08f);
	Animate   *ballAnimate   = Animate::create(ballAnimation);

	Vector<SpriteFrame*> animateFrame(2);
	for (int i=4;i!=6;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_enemy_boss_remote_fly_%02d.png",i)->getCString());
		animateFrame.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(animateFrame, 0.03f);
	Animate *animate = Animate::create(animation);

	Vector<SpriteFrame*> aniFrame(12);
	for (int i=1;i!=13;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_enemy_boss_remote_hit_%02d.png",i)->getCString());
		aniFrame.pushBack(frame);
	}
	Animation *anition = Animation::createWithSpriteFrames(aniFrame, 0.10f);
	Animate *anim = Animate::create(anition);

	auto action = Sequence::create(ballAnimate,animate,anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->runAction(action);
}

void effectsObject::towerItem1AttackEffects(float dt)
{
	Vector<SpriteFrame*> animateFrame(7);
	for (int i=1;i!=8;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("d_tower_01_c_jizhong_%02d.png",i)->getCString());
		animateFrame.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(animateFrame, 0.03f);
	Animate *animate = Animate::create(animation);

	auto action = Sequence::create(DelayTime::create(dt),animate,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		                           NULL);
	this->setScale(1.0/0.9);
	this->runAction(action);
}

void effectsObject::tower4_wind_effect(int level)
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("td_tower_04_feng.plist","td_tower_04_feng.png");
	Vector<SpriteFrame*> animateFrame(16);
	for (int i=1;i!=17;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_tower_04_feng_%02d.png",i)->getCString());
		animateFrame.pushBack(frame);
	}
	float _time=0.1;
	
	switch (level)
	{
	case 1:
		_time=0.1;
		break;
	case 2:
		_time=0.08;
		break;
	case 3:
		_time=0.06;
		break;
	default:
		break;
	}

	Animation *animation = Animation::createWithSpriteFrames(animateFrame, _time);
	Animate *animate = Animate::create(animation);
	auto action = RepeatForever::create(animate);
	this->setScale(1.0/0.8);						 
	this->runAction(action);
}

void effectsObject::tower5_jiguang_effect()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("td_tower_05_jiguang.plist","td_tower_05_jiguang.png");
	Vector<SpriteFrame*> animateFrame(26);
	for (int i=1;i!=27;++i){
		SpriteFrame *frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_tower_05_%02d.png",i)->getCString());
		frame->getTexture()->setAliasTexParameters();
		animateFrame.pushBack(frame);
	}


	Animation *animation = Animation::createWithSpriteFrames(animateFrame, 0.08f);
	Animate *animate = Animate::create(animation);



	auto action = Sequence::create(animate,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		                           NULL);

	this->runAction(action);
}

void effectsObject::tower5_shexian_effect(float dis)
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("td_tower_05_shexian.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("td_tower_05_shexian.plist","td_tower_05_shexian.png");
	Vector<SpriteFrame*> _frames(8);
	for (int i=1;i!=9;++i){
		SpriteFrame *spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_tower_05_%03d.png",i)->getCString());
		spFrame->getTexture()->setAliasTexParameters();

		Rect _rect = spFrame->getRect();
		spFrame->setRect(Rect(_rect.origin.x+dis,_rect.origin.y,_rect.size.width,_rect.size.height));
		_frames.pushBack(spFrame);
	}

	Animation *_animation = Animation::createWithSpriteFrames(_frames, 0.08f);
	Animate *_animate = Animate::create(_animation);

	auto action = Sequence::create(DelayTime::create(1.28f),_animate,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
								   CallFunc::create([=]{
									   SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("td_tower_05_shexian.plist");
									   TextureCache::getInstance()->removeTextureForKey("td_tower_05_shexian.png");
									}),
		                           NULL);

	this->runAction(action);
}

void effectsObject::tower5_atk_obj_effect()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("tower5_atk_obj_effects.plist","tower5_atk_obj_effects.png");
	Vector<SpriteFrame*> _frames(14);
	for (int i=1;i!=15;++i){
		SpriteFrame *spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_tower_05_shoujitexiao_%02d.png",i)->getCString());
		spFrame->getTexture()->setAliasTexParameters();
		_frames.pushBack(spFrame);
	}

	Animation *_animation = Animation::createWithSpriteFrames(_frames, 0.08f);
	Animate *_animate = Animate::create(_animation);

	auto action = Sequence::create(_animate,
							       CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		                           NULL);

	this->runAction(action);
	audioPlayer::getInstance()->playEffect("S69.mp3");
}

void effectsObject::trap02_ring_effect()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("td_trap2_ice_ring_effect.plist","td_trap2_ice_ring_effect.png");
	Vector<SpriteFrame*> _frames(4);
	for (int i=0;i!=4;++i){
		SpriteFrame *spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_trap2_ice_%02d.png",i)->getCString());
		spFrame->getTexture()->setAliasTexParameters();
		_frames.pushBack(spFrame);
	}

	Animation *_animation = Animation::createWithSpriteFrames(_frames, 0.08f);
	Animate *_animate = Animate::create(_animation);

	auto action = Sequence::create(_animate,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
								   CallFunc::create([=]{
									   SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("td_trap2_ice_ring_effect.plist");
									   TextureCache::getInstance()->removeTextureForKey("td_trap2_ice_ring_effect.png");
								   }),
		                           NULL);


    this->setScale(1.0/0.8);//打包合成大图的时候被压缩到0.8
	this->runAction(action);
}

void effectsObject::trap02_born_effect()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("td_trap2_ice_born_effect.plist","td_trap2_ice_born_effect.png");
	Vector<SpriteFrame*> _frames(8);
	for (int i=0;i!=8;++i){
		SpriteFrame *spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_trap2_ice_%03d.png",i)->getCString());
		spFrame->getTexture()->setAliasTexParameters();
		_frames.pushBack(spFrame);
	}

	Animation *_animation = Animation::createWithSpriteFrames(_frames, 0.08f);
	Animate *_animate = Animate::create(_animation);

	auto action = Sequence::create(_animate,
								   CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		                           CallFunc::create([=]{
									     SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("td_trap2_ice_born_effect.plist");
										 TextureCache::getInstance()->removeTextureForKey("td_trap2_ice_born_effect.png");
								   }),
								   NULL);
//打包的时候已经压缩了到0.7
    this->setScale(1.0/0.7);
	this->runAction(action);
}

void effectsObject::shopBuyGoodsSuccessEffect()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("image/shop_buy_success_effect.plist","image/shop_buy_success_effect.png");
	Vector<SpriteFrame*> _frames(11);
	for (int i=1;i!=8;++i){
		SpriteFrame *spFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(String::createWithFormat("td_buy_success_%02d.png",i)->getCString());
		spFrame->getTexture()->setAliasTexParameters();
		_frames.pushBack(spFrame);
	}

	Animation *_animation = Animation::createWithSpriteFrames(_frames, 0.08f);
	Animate *_animate = Animate::create(_animation);

	auto action = Sequence::create(_animate,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		CallFunc::create([=]{
			SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("image/shop_buy_success_effect.plist");
			TextureCache::getInstance()->removeTextureForKey("image/shop_buy_success_effect.png");
		}),
		NULL);

		this->setScale(1.0/0.8);//合成大图的时候被缩小到了0.8
	this->runAction(action);

}


bool effectsObject::init()
{
	if (!Sprite::init())
	{
		return false;
	}
	return true;
}


Animate* effectsObject::CreateAnimation(const char*FrameName,int max){

	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(__String::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.07f);
	Animate *animate = CCAnimate::create(animation);

	return animate;
}

void effectsObject::bottleExplod(){//飞行兵的爆炸效果,这个是直接爆咋

	Animate *anim = this->CreateAnimation("td_war_fly_hit_",8);
	auto action = Sequence::create(anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->runAction(action);
}
void effectsObject::bottleMoveExplod(Point mbPos)//移动和爆炸
{
	bomb=Sprite::createWithSpriteFrameName(__String::createWithFormat("%s%02d.png","td_war_fly_hit_",1)->getCString());
	bomb->setPosition(0,0);
	this->addChild(bomb);
	 auto move=MoveBy::create(0.20f,Point(0,mbPos.y-this->getPositionY()));
	// auto easeove=CCEaseOut::create(move,2);
	auto action = Sequence::create(
		move,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::moveCallback, this)),
		NULL);
	this->runAction(action);

}
void effectsObject::moveCallback(Ref *psender){
	Sprite *sp=static_cast<Sprite *>(psender);
	bomb->removeFromParent();
	Animate *anim = this->CreateAnimation("td_war_fly_hit_",8);
	auto action = Sequence::create(anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	sp->runAction(action);
	
}

//变成羊的烟雾
void effectsObject::sheepEffect(){
	
	Animate *anim = this->CreateAnimation("td_Magic2_change_",10);
	auto action = Sequence::create(anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->runAction(action);
}

void effectsObject::magic3Effect(){
	Animate *anim = this->CreateAnimation("td_magic3_effect_",15);
	auto action = Sequence::create(anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->runAction(action);

}


void effectsObject::Boss4ChangeEffect()
{
	Animate *anim = this->CreateAnimation("td_boss4_yan_",13);
	auto action = Sequence::create(anim,
		CallFuncN::create(CC_CALLBACK_1(effectsObject::DeathObjectCallBack, this)),
		NULL);
	this->runAction(action);

}