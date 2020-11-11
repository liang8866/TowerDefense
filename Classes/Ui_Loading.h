#ifndef __UI_LOADING_H__
#define __UI_LOADING_H__
#include "GameConfig.h"
class Ui_Loading : public Widget
{
public:
	Ui_Loading(void);
	~Ui_Loading(void);
	CREATE_FUNC(Ui_Loading);
	virtual bool init();
	virtual void onEnter() override;
	virtual void onExit() override;

	void fitScreen();
	Widget* _widget; 
	
};
#endif 
