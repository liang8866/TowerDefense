#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "StoreyData.h"


enum StoreyType
{
	eHome = 0,
	eLast
};


class SceneData : public cocos2d::Ref
{
public:
	static SceneData* getInstance();
	void destroyInstance();

	SceneData(void);
	~SceneData(void);
public:
	bool __init();

	cocos2d::Map<size_t, StoreyData*> getSceneDataMap(){ return mSceneDataMap; };
	// 根据ui 索引取数据
	StoreyData* getSceneDataMapStorey(size_t _index);
	// 根据实际层索引 取数据
	StoreyData* getSceneDataMapStoreyFormStoreyIndex(size_t _stroeyIndex);
	
	// 添加层
	void addNewStorey(size_t _index, size_t _storeyIndex);
	void addStorey(size_t _index, StoreyData* _newStorey);
	
	// 添加层上逻辑物件
	void addStoreyObject(size_t _index, GameObjects* _obj);

	// 删除层和(层上所有物件)
	void removeStorey(size_t _index);
	// 删除层上(指定物件)
	void removeStoreyObject(size_t _index, GameObjects* _obj);
	void removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);
	// 删除层上(所有物件)
	void removeStoreyAllObject(size_t _index);

	// 查找层上指定物件
	bool findStoreyObject(size_t _index, GameObjects* _obj);

	// 清空所有层和(层上所有物件)
	void clearScene();
	// 清空所有层上(所有物件)
	void clearSceneAllObject();

	void update(float dt);

	int getNextCreateStoreyIndex();					// 创建的时候取下一层
	int getNextStoreyIndex(size_t _storeyIndex);	// 取可移动到的下一层
	int  getUpStoreyIndex(size_t _storeyIndex);     //获取上一层
	int getLastStoreyIndex();						// 取当前最后一层

	bool isClearAllMonster();						// 整个场景是否无兵(怪)
	bool isRepeatHero(int _selfType);				// 是否重复的boss


	

public:
	// 每个场景最大层数
	size_t mMaxStoreyCount;

	// 当前场景层数据
	cocos2d::Map<size_t, StoreyData*> mSceneDataMap;

	void initGorundData(Widget* _widget);

	StoreyData*	mGroundData; // 地面数据存储
	Widget* mGroundItemWidget;

	bool isExistObjSelect();

	GameObjects *findExistObjSelect();

	bool isYuJing();	// 是否有预警提示

	bool isFinishBatch(int _batch);	// 是否打完一个波次
/*
	plist 配置数据
*/
public:
	cocos2d::__Dictionary*	mScenePlistConfig;
	//cocos2d::__Dictionary*	getScenePist(const char* _sceneName);

	//// 取出兵啵次数据
	//cocos2d::__Dictionary*	getSceneMapPlist(const char* _sceneName, const char* _keyName);
	//CCString				getSceneMapPlistCString(const char* _sceneName, const char* _keyName, const char* _boKeyName, const char* _boAtrName);
	//// 取场景其他数据
	//CCString				getSceneMapCString(const char* _sceneName, const char* _keyName);
	
};

