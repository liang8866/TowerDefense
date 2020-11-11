#pragma once

#include "AndroidStartup.h"
#include "startScene.h"
USING_NS_CC;

Scene* AndroidStartup::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease objec*t
	AndroidStartup *layer=AndroidStartup::create();
	// add layer as a child to scene
	scene->addChild(layer);



	// return the scene
	return scene;
}
AndroidStartup::AndroidStartup(void)
{
	
}

AndroidStartup::~AndroidStartup(void)
{


}
// on "init" you need to initialize your instance

bool AndroidStartup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	 visibleSize = Director::getInstance()->getVisibleSize();
	Sprite* lunach=Sprite::create("andriodLaunch/Default.png");
	lunach->setPosition(visibleSize.width/2,visibleSize.height/2);
	this->addChild(lunach);
	auto action = Sequence::create(
		DelayTime::create(1.5),
		CallFunc::create([=](){
			Scene* pTScene = startScene::createScene();
			TransitionFade *transitionfadeScene=TransitionFade::create(0.3,pTScene);
			Director::sharedDirector()->replaceScene(transitionfadeScene);
	}),NULL);

	this->runAction(action);
	
	
	return true;
}

