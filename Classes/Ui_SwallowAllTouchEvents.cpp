#include "Ui_SwallowAllTouchEvents.h"



Ui_SwallowAllTouchEvents::Ui_SwallowAllTouchEvents()
{

}

Ui_SwallowAllTouchEvents::~Ui_SwallowAllTouchEvents()
{

}

bool Ui_SwallowAllTouchEvents::init()
{
	if (!Widget::init())
		return false;
	mWidget = nullptr;
	mLayerBack = nullptr;

	mWidget = GUIReader::getInstance()->widgetFromJsonFile("Mask/Mask.ExportJson");
	mWidget->setScaleX(Director::getInstance()->getVisibleSize().width/960);
	mWidget->setScaleY(Director::getInstance()->getVisibleSize().height/640);

	mLayerBack  = static_cast<Layout*>(mWidget->getChildByName("Panel_1"));
	mLayerBack->setBackGroundColorOpacity(0);

	mWidget->setTouchEnabled(true);
	mWidget->setTouchPriority(-300);

	this->addChild(mWidget);
	return true;
}

void Ui_SwallowAllTouchEvents::onEnter()
{
	Widget::onEnter();
}

void Ui_SwallowAllTouchEvents::onExit()
{
	Widget::onExit();
}
// ÉèÖÃÍ¸Ã÷¶È
void Ui_SwallowAllTouchEvents::setBackGroundColorOpacity(GLubyte _opacity)
{
	mLayerBack->setBackGroundColorOpacity(_opacity);
}



