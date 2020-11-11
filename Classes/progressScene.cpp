#pragma once

#include "progressScene.h"
#include "Ui_Ready.h"

USING_NS_CC;

Scene* progressScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease objec*t
	progressScene *layer=progressScene::create();
	// add layer as a child to scene
	scene->addChild(layer);



	// return the scene
	return scene;
}
progressScene::progressScene(void)
{

}

progressScene::~progressScene(void)
{
}
// on "init" you need to initialize your instance

bool progressScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}

	Ui_Ready *_ready = Ui_Ready::create();
	addChild(_ready);

	return true;
}

