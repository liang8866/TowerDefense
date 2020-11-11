#include "Ui_Loading.h"
Ui_Loading::Ui_Loading(void)
{
}
Ui_Loading::~Ui_Loading(void)
{
}

bool Ui_Loading::init()
{
	if (!Widget::init())
	{
		return false;
	}
	bool Ret = false;
	do{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_loading/GameUI0.plist","Ui_loading/GameUI0.png");
		_widget = GUIReader::getInstance()->widgetFromJsonFile("Ui_loading/Ui_loading.ExportJson");
		this->addChild(_widget);
		auto repeat = RepeatForever::create( 
			Sequence::create(
			RotateTo::create(0.6f,180.0f),
			DelayTime::create(0.4f),
			RotateTo::create(0.6f,360.0f),
			DelayTime::create(0.4f),
			NULL));
			//RotateBy::create(3.0f, 360) );
		_widget->getChildByName("Image")->runAction(repeat);
		setTouchEnabled(true);
		setTouchPriority(-1000);
		fitScreen();
		
		Ret = true;
	}while(0);
	return Ret;
}

void Ui_Loading::onEnter()
{
	Widget::onEnter();
}

void Ui_Loading::onExit()
{
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_loading/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_loading/GameUI0.png");
	Widget::onExit();
}

void Ui_Loading::fitScreen()
{
	Point originPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	_widget->setSize(visibleSize);
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	//_widget->setScale(_scale);
	_widget->setPosition(Point(originPt.x,originPt.y));
	_widget->getChildByName("Image")->setPosition(Point(originPt.x+visibleSize.width/2,originPt.y+visibleSize.height/2));
	_widget->getChildByName("Image")->setScale(_scale);
	//Image
}

