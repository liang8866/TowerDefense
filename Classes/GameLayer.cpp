#include "GameLayer.h"
   USING_NS_CC;

static WindowManager* thisWindowManager = nullptr;
static SceneData* thisSceneData			= nullptr;

GameLayer::GameLayer(){

 }
GameLayer::~GameLayer()
{
	WindowManager::getInstance()->destroyInstance();
	SceneData::getInstance()->destroyInstance();
}

bool GameLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }


	 thisWindowManager = WindowManager::getInstance();
	 thisWindowManager->initialise(this);
	 thisSceneData = SceneData::getInstance();
	
	 this->schedule(schedule_selector(GameLayer::Gameupdate),(float) 1.0/60);

    return true;
}




//Ö÷¶¨Ê±Æ÷
void GameLayer::Gameupdate(float  dt)
{   
	
		thisWindowManager->update(dt);
		thisSceneData->update(dt);

	
		

	/*Vector<Node*> sp_vec=this->getChildren();
	Vector<Node*>::iterator Rank_IT = sp_vec.begin();
	while ( Rank_IT != sp_vec.end() ) 
	{    
	Node* sp = (*Rank_IT);
	GameObjects *obj=(GameObjects*)sp;
	int ptype=obj->getobjectType();
	if (ptype==TYPE_WARRIOR||ptype==TYPE_ENEMY||ptype==TYPE_BOSS){
	if (obj->gethp()<=0)
	{
	this->CreateAnimationForDeathObject(obj->getPosition());
	this->removeChild(obj);
	obj=nullptr;	
	}
	}
	if( Rank_IT == sp_vec.end() || sp_vec.size() == 0 )
	{
	break;
	}
	Rank_IT++;
	}*/
}





