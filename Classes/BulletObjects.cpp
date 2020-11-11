#include "BulletObjects.h"
bool BulletObjects::init() 
{
	if (!Sprite::init())
	{
		return false;
	}
	this->initWithFile("tower/bullet.png");
	_armature = nullptr;
	this->scheduleUpdate();
	this->_atk = 0;
	return true;
}
void BulletObjects::run(Point pos,float timer)
{
	this->runAction(Sequence::create(
		MoveTo::create(
		timer, pos), 
		CallFuncN::create(this, callfuncN_selector(BulletObjects::MoveFinished)),
		NULL));
}
void BulletObjects::MoveFinished(Node* sender)
{
	this->removeFromParentAndCleanup(true);
	//parent->removeNode(sprite);
}
void BulletObjects::update(float dt) 
{
	auto parent = this->getParent();
	Rect bulletObject = this->getBoundingBox();
	Vector<BulletObjects*> BulletToDelete;
	auto sdata = SceneData::getInstance();
	StoreyData* story  = sdata->getSceneDataMapStoreyFormStoreyIndex(this->_floorNum);
	if (story == nullptr)
	{
		return;
	}
	for (GameObjects* sp:story->mObjectData)
	{
		if (sp!=nullptr)
		{
			int objType = sp->getobjectType();
			if (objType != TYPE_ENEMY)
			{
				continue;
			}
			Rect spRect = Rect(
				sp->getPositionX()-sp->_armature->getContentSize().width,
				sp->getPositionY(),
				sp->_armature->getContentSize().width,
				sp->_armature->getContentSize().height
				);
			if (bulletObject.intersectsRect(spRect)) 
			{
				auto effects = effectsObject::create();
				effects->Bullet();
				effects->setPosition(this->getPosition());
				auto parent = static_cast<Widget*>(this->getParent());
				parent->addChild(effects,8);
				//parent->addNode(effects,1);
				CCLOG("bulletObject reducehp");
				sp->setReduceHp(this->_atkOption,this->_atk,0,0);

				this->removeFromParentAndCleanup(true);
				break;
			}

		}
		
	}
}

void BulletObjects::setAtk(int atk)
{
	this->_atk = atk;
}
void BulletObjects::setFloorNum(int floorNum)
{
	this->_floorNum = floorNum;
}

