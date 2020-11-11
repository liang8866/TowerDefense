#pragma once
#include "GameConfig.h"
#include "GameObjects.h"


class doorObject : public GameObjects
{
public:
	static doorObject* create(DoorType doorType); 
	bool init(DoorType doorType);
	void setProperty();
	virtual void reduceHpEffect(int t);
	Action* CurrentAction;
	void ActionDone(Node* sender);
};




