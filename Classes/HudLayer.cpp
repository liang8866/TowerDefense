#include "HudLayer.h"
#include "WindowManager.h"
#include "Ui_MessageBox.h"

   USING_NS_CC;
bool HudLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
     visibleSize = Director::getInstance()->getVisibleSize();
     origin = Director::getInstance()->getVisibleOrigin();
     winSize=Director::getInstance()->getWinSize();
	 //this-> fun_createPauseMenu();

     scheduleUpdate();//定时器
    return true;
}
void HudLayer::labelForCallback(Ref* pSender)
{
	this->removeChild(static_cast<LabelTTF*>(pSender),true);

}


//主定时器
void HudLayer::update(float  dt)
{
	

}

///////////////////////////////////////////////////////////////
void HudLayer::fun_createPauseMenu()//创建暂停按钮
{
	
	var_pauseItem=MenuItemImage::create("pasue.png","pasue.png",CC_CALLBACK_0(HudLayer::fun_pauseMenuCallback, this));
	var_pauseItem->setPosition(Point(visibleSize.width + origin.x-50, visibleSize.height + origin.y-50));
	Menu *pauseMenu=Menu::create(var_pauseItem,NULL);
	pauseMenu->setPosition(Point::ZERO);
	this->addChild(pauseMenu,10);
}
void HudLayer::fun_pauseMenuCallback()//暂停按钮的回调
{
	
}