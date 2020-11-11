#ifndef __Ui_SwallowAllTouchEvents_H__
#define __Ui_SwallowAllTouchEvents_H__

#include "cocos2d.h"
#include "../extensions/cocos-ext.h"

#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace ui;
using namespace std;

class Ui_SwallowAllTouchEvents : public Widget
{
public:
	Ui_SwallowAllTouchEvents();
	~Ui_SwallowAllTouchEvents();
public:
	virtual bool init() override;
	CREATE_FUNC(Ui_SwallowAllTouchEvents);

	virtual void onEnter() override;
	virtual void onExit() override;

	void setBackGroundColorOpacity(GLubyte _opacity);		// …Ë÷√Õ∏√˜∂»
public:
	Widget*		mWidget;
	Layout*		mLayerBack;
};

#endif