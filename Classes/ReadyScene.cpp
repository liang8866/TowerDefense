
#pragma once

#include "ReadyScene.h"
#include "Ui_Ready.h"

USING_NS_CC;

Scene* ReadyScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease objec*t
	ReadyScene *layer=ReadyScene::create();
	// add layer as a child to scene
	scene->addChild(layer);



	// return the scene
	return scene;
}
ReadyScene::ReadyScene(void)
{
	CCLOG(" ReadyScene::ReadyScene(void)");
}

ReadyScene::~ReadyScene(void)
{

	
}
// on "init" you need to initialize your instance

bool ReadyScene::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !Layer::init() )
	{
		return false;
	}
	CCLOG(" ReadyScene::init()");
	Ui_Ready *_ready = Ui_Ready::create();
	addChild(_ready);
	//���ֻ����ؼ��ļ��� 
	auto listener = EventListenerKeyboard::create(); 
	//�ͻص������� 
	listener->onKeyReleased = CC_CALLBACK_2(ReadyScene::onKeyReleased,this); 
	//��ӵ��¼��ַ����� 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this); 

	return true;
}

void ReadyScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		__Dictionary* text = __Dictionary::createWithContentsOfFile("plist/text.plist");
		__String* connect = static_cast<__String*>(text->objectForKey("1014"));
		__String* title = static_cast<__String*>(text->objectForKey("1015"));
		MessageBox(connect->getCString(),title->getCString());
	}
}
