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
	// ����ui ����ȡ����
	StoreyData* getSceneDataMapStorey(size_t _index);
	// ����ʵ�ʲ����� ȡ����
	StoreyData* getSceneDataMapStoreyFormStoreyIndex(size_t _stroeyIndex);
	
	// ��Ӳ�
	void addNewStorey(size_t _index, size_t _storeyIndex);
	void addStorey(size_t _index, StoreyData* _newStorey);
	
	// ��Ӳ����߼����
	void addStoreyObject(size_t _index, GameObjects* _obj);

	// ɾ�����(�����������)
	void removeStorey(size_t _index);
	// ɾ������(ָ�����)
	void removeStoreyObject(size_t _index, GameObjects* _obj);
	void removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj);
	// ɾ������(�������)
	void removeStoreyAllObject(size_t _index);

	// ���Ҳ���ָ�����
	bool findStoreyObject(size_t _index, GameObjects* _obj);

	// ������в��(�����������)
	void clearScene();
	// ������в���(�������)
	void clearSceneAllObject();

	void update(float dt);

	int getNextCreateStoreyIndex();					// ������ʱ��ȡ��һ��
	int getNextStoreyIndex(size_t _storeyIndex);	// ȡ���ƶ�������һ��
	int  getUpStoreyIndex(size_t _storeyIndex);     //��ȡ��һ��
	int getLastStoreyIndex();						// ȡ��ǰ���һ��

	bool isClearAllMonster();						// ���������Ƿ��ޱ�(��)
	bool isRepeatHero(int _selfType);				// �Ƿ��ظ���boss


	

public:
	// ÿ������������
	size_t mMaxStoreyCount;

	// ��ǰ����������
	cocos2d::Map<size_t, StoreyData*> mSceneDataMap;

	void initGorundData(Widget* _widget);

	StoreyData*	mGroundData; // �������ݴ洢
	Widget* mGroundItemWidget;

	bool isExistObjSelect();

	GameObjects *findExistObjSelect();

	bool isYuJing();	// �Ƿ���Ԥ����ʾ

	bool isFinishBatch(int _batch);	// �Ƿ����һ������
/*
	plist ��������
*/
public:
	cocos2d::__Dictionary*	mScenePlistConfig;
	//cocos2d::__Dictionary*	getScenePist(const char* _sceneName);

	//// ȡ����ࣴ�����
	//cocos2d::__Dictionary*	getSceneMapPlist(const char* _sceneName, const char* _keyName);
	//CCString				getSceneMapPlistCString(const char* _sceneName, const char* _keyName, const char* _boKeyName, const char* _boAtrName);
	//// ȡ������������
	//CCString				getSceneMapCString(const char* _sceneName, const char* _keyName);
	
};

