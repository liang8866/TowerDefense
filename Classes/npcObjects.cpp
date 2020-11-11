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
	isArmature=false;// Ĭ������Ϊfalse 
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

  


//��������������غ��� 
void npcObjects::CreateArmature(const std::string& filename,const std::string&name ){
	isArmature=true;//����Ϊtrue
	ArmatureDataManager::getInstance()->addArmatureFileInfo(filename);  //ֱ����ExportJson�ļ���ʼ����ArmatureDataManager���Լ��ҵ������ļ�  
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
			//��
			if (_armature->getPositionX() < randPositionX) {
				_armature->setRotationSkewY(180);
			} else {
				_armature->setRotationSkewY(0);
				//��
			}
			_armature->getAnimation()->play("run");
		}),
		MoveTo::create(abs((randPositionX-_armature->getPositionX())/30),Point(randPositionX,_armature->getPositionY())),
		CallFunc::create( CC_CALLBACK_0(npcObjects::ActionDone,this)),
		NULL
	);
	_armature->runAction(_action);
}

//// ��ȡarmature
//Armature* npcObjects::getArmature(){
// 
//	return _armature;
//}
//���������Ĵ����¼�
void npcObjects::CreateListenerTouch_Armature(){
	 listener = EventListenerTouchOneByOne::create();//����һ����������
	 CC_SAFE_RETAIN(listener);
	 //3.0 �����ֱ����touchBegan���������ʵ�ִ��룬����������ȥдһ��touchBegan�ĺ���
    listener->onTouchBegan = [](Touch* touch, Event* event){
		auto target = static_cast<Armature*>(event->getCurrentTarget());//��ȡ�ĵ�ǰ������Ŀ��
		Point locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width*target->getScale(), s.height*target->getScale());
		if (rect.containsPoint(locationInNode)){//�жϴ������Ƿ���Ŀ��ķ�Χ��
			
			return true;
		} else {
       		  return false;
		}   

		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener ,_armature);  
}


//�����ɾ�������¼���
void npcObjects::RemoveListenerTouch()
{
	_eventDispatcher->removeEventListener(listener);  
}

