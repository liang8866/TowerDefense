#include "npcObjects.h"
USING_NS_CC;
npcObjects::npcObjects(void)
{
	
}

npcObjects::~npcObjects(void)
{
}
// on "init" you need to initialize your instance

bool npcObjects:: npcObjectsInit()
{
    //////////////////////////////
	isArmature=false;// 默认设置为false 
	_objectType=TYPE_NPC;
	string filePath = getFilePathByClothes();
	string temp = filePath.substr(filePath.find("/")+1); 
	string name = temp.substr(0,temp.find("."));
	this->CreateArmature(filePath,name);
	this->CreateListenerTouch_Armature();
	_maxX = 0;
	_minX = 0;
    return true;
}

string npcObjects::getFilePathByClothes()
{
	if (UserRecord::getInstance()->getClothesItemId()-70001 > 0)
		return StringUtils::format("npc/td_npc_clothes_%02d.ExportJson",UserRecord::getInstance()->getClothesItemId()-70001); 
	else{
		return "npc/td_npc.ExportJson";
	}
}


 npcObjects* npcObjects::create()
{
	npcObjects *sprite = new npcObjects();
    if (sprite && sprite->init())
    {
		sprite-> npcObjectsInit();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

  


//创建骨骼动画相关函数 
void npcObjects::CreateArmature(const std::string& filename,const std::string&name ){
	isArmature=true;//设置为true
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //直接用ExportJson文件初始化，ArmatureDataManager会自己找到其他文件  
	_armature = Armature::create(name);   
	this->addChild(_armature,1);
	_armature->setScale(1.1);
}
void npcObjects::ActionDone()
{
	_armature->getAnimation()->play("load");
	int randPositionX = rand()%(_maxX+1-_minX)+_minX;
	
	auto _action = Sequence::create(
		DelayTime::create(3+rand()%6),
		CallFunc::create(
		[=](){
			//左
			if (_armature->getPositionX() < randPositionX) {
				_armature->setRotationSkewY(180);
			} else {
				_armature->setRotationSkewY(0);
				//右
			}
			_armature->getAnimation()->play("run");
		}),
		MoveTo::create(abs((randPositionX-_armature->getPositionX())/30),Point(randPositionX,_armature->getPositionY())),
		CallFunc::create( CC_CALLBACK_0(npcObjects::ActionDone,this)),
		NULL
	);
	_armature->runAction(_action);
}

//// 获取armature
//Armature* npcObjects::getArmature(){
// 
//	return _armature;
//}
//骨骼动画的触摸事件
void npcObjects::CreateListenerTouch_Armature(){
	 listener = EventListenerTouchOneByOne::create();//创建一个触摸监听
	 CC_SAFE_RETAIN(listener);
	 //3.0 后可以直接在touchBegan后添加它的实现代码，而不用特意去写一个touchBegan的函数
    listener->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Armature*>(event->getCurrentTarget());//获取的当前触摸的目标
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width*target->getScale(), s.height*target->getScale());
		if (rect.containsPoint(locationInNode)){//判断触摸点是否在目标的范围内
			
			return true;
		} else {
       		  return false;
		}   

		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener ,_armature);  
}


//这个是删除触摸事件的
void npcObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
}

