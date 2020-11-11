#include "Window.h"

Window::Window(void)
{
}
Window::~Window(void)
{
}

bool Window::initialise(const char* fileName)
{
	// ¼ÓÔØjson
	mMainWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile(fileName);
	if (nullptr == mMainWidget)
		return false;
	mJsonName = fileName;
	return true;
}

Widget* Window::findChild(const char* _widgetName)
{
	if (!mMainWidget)
		return nullptr;
	Widget* _thisChild = Helper::seekWidgetByName(mMainWidget, _widgetName);
	if (nullptr == _thisChild)
		return nullptr;
	return _thisChild;
}

void Window::fitScreen()
{
	Point orginPt = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mMainWidget->setScale(_scale);
	mMainWidget->setPosition(Point(visibleSize.width/2  - mMainWidget->getSize().width/2*_scale ,
								   visibleSize.height/2 - mMainWidget->getSize().height/2*_scale 
		                           ));
}