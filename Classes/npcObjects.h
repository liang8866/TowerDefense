#ifndef __NPC_OBJECTS_H__  
#define __NPC_OBJECTS_H__  
#include "cocos2d.h"
#include "GameObjects.h"
USING_NS_CC;

//CCStandardTouchDelegate 
 class npcObjects : public GameObjects
{
public:
	npcObjects(void);
   ~npcObjects(void);
public:
	//---------
	static npcObjects* create();
	 
	virtual bool npcObjectsInit(); //自定义的初始化

	string getFilePathByClothes();
	
	CC_SYNTHESIZE(int,ttag,TTag);//备用

	bool isArmature;
	EventListenerTouchOneByOne *listener;
	//骨骼动画的实现和获取
	
	void CreateArmature(const std::string& filename,const std::string&name );//创建骨骼动画，参数:文件目录 参数2:创建的骨骼动画
	//Armature *_armature;
	//Armature* getArmature();//获取 骨骼动画
	 
	//创建触摸事件监听
	void CreateListenerTouch_Armature();//对于骨骼的

	//去掉触摸监听
	void RemoveListenerTouch();
	void ActionDone();
	int _maxX;
	int _minX;
};

#endif  



