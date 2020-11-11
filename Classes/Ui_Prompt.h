#ifndef __UI_PROMPT_FRAME_H__
#define __UI_PROMPT_FRAME_H__


#include "GameConfig.h"

using namespace std;
//typedef void (Ref::*SEL_CONNECT)(float tag);
//#define comm_selector(_SELECTOR) static_cast<SEL_COMM>(&_SELECTOR)

typedef void (Ref::*SEL_CONNECT)(int tag);
#define connect_selector(_SELECTOR) (SEL_CONNECT)(&_SELECTOR)

class Ui_Prompt : public Widget
{
public:
	Ui_Prompt();
	~Ui_Prompt();
public:

	virtual bool init() override;
	CREATE_FUNC(Ui_Prompt);
	virtual void onEnter() override;
	virtual void onExit() override;

	void close();
	void eventTouchButton(Ref* sender, TouchEventType type);

	void initWithOneButton(const char* content , const char* title = NULL , const char* buttonTitle = NULL);
	void initWithTwoButtons(const char* content , const char* title =NULL , const char* rbuttonTitle= NULL , const char* lbuttonTitle=NULL);
	void initWithRetry(const char* content , const char* title =NULL , const char* rbuttonTitle= NULL , const char* lbuttonTitle=NULL); //гажиЪд
	Ref   *m_obj_r;
	Ref   *m_obj_l;
	SEL_COMM m_callfunc_r;
	SEL_CONNECT m_connect_r;
	SEL_COMM m_callfunc_l;
	int m_callbackTag;
	void setRightSelector(Ref *obj , SEL_COMM callfunc);
	void setLeftSelector(Ref *obj , SEL_COMM callfunc);


	void setRightConnectSelector(Ref *obj , SEL_CONNECT callfunc,int tag);
	void setLeftConnectSelector(Ref *obj , SEL_COMM callfunc);

	void fitScreen();
	
private:
	Widget*     m_widget; 
	Button*		mButtonRight;
	Button*		mButtonLeft;
	Text  *     mTitle;
	TextBMFont* mContent; 

	EventListenerTouchOneByOne *_listener;
};


#endif // !__UI_PROMPT_FRAME_H__
