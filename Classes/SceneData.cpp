#include "SceneData.h"
#include "PlistManager.h"
#include "AltarObject.h"
SceneData::SceneData(void)
{
}


SceneData::~SceneData(void)
{
}

static SceneData* mSceneData = nullptr;

SceneData* SceneData::getInstance()
{
	if (!mSceneData)
	{
		mSceneData = new SceneData();
		mSceneData->__init();
	}
	return mSceneData;
}
void SceneData::destroyInstance()
{
	CC_SAFE_DELETE(mSceneData);
};
bool SceneData::__init()
{
	mMaxStoreyCount = SCENE_MAXCOUNT;
	mSceneDataMap.clear();
	mGroundItemWidget = nullptr;
	//mScenePlistConfig = nullptr;

	// 读取plist配置文件
	//mScenePlistConfig = Dictionary::createWithContentsOfFile(FILE_SCENE_PLIST);
	//mScenePlistConfig->retain();
	//CCAssert(mScenePlistConfig != nullptr, "mScenePlistConfig:>> add plist error!!!");
	return true;
}

void SceneData::initGorundData(Widget* _widget)
{
	if (!_widget)
		return;
	mGroundItemWidget = _widget;
	Widget* _newItemCell = static_cast<Widget*>(mGroundItemWidget);
	StoreyData* _newStorey = new StoreyData(SCENE_STOREY_GROUND_INDEX, mGroundItemWidget);
	mGroundData = _newStorey;
}

void SceneData::addNewStorey(size_t _index, size_t _storeyIndex)
{
	if (_index == SCENE_STOREY_GROUND_INDEX || _storeyIndex == SCENE_STOREY_GROUND)
		return;
	StoreyData* _newStorey = new StoreyData(_index, _storeyIndex);
	addStorey(_index, _newStorey);
}
void SceneData::addStorey(size_t _index, StoreyData* _newStorey)
{
	if (_index == SCENE_STOREY_GROUND)
		return;
	mSceneDataMap.insert(_index, _newStorey);
}

void SceneData::addStoreyObject(size_t _index, GameObjects* _obj)
{
	if (!_obj)
		return;
	StoreyData* _thisStorey = nullptr;
	if (_index != SCENE_STOREY_GROUND_INDEX)
		_thisStorey = getSceneDataMapStorey(_index);
	else
		_thisStorey = mGroundData;

	_thisStorey->addObject(_obj);
}

void SceneData::removeStorey(size_t _index)
{
	StoreyData* _thisStorey = nullptr;
	if (_index != SCENE_STOREY_GROUND_INDEX)
		_thisStorey = getSceneDataMapStorey(_index);
	else
		_thisStorey = mGroundData;

	if (_thisStorey->getStoreyIndex() != SCENE_STOREY_GROUND)
	{
		_thisStorey->clearObject();
		mSceneDataMap.erase(_index);
	}else
	{
		_thisStorey->clearObject();
	}
}
void SceneData::removeStoreyObject(size_t _index, GameObjects* _obj)
{
	StoreyData* _thisStorey = nullptr;
	if (_index != SCENE_STOREY_GROUND_INDEX)
		_thisStorey = getSceneDataMapStorey(_index);
	else
		_thisStorey = mGroundData;

	_thisStorey->removeObject(_obj);
}
void SceneData::removeStoreyObjectFormStoreyIndex(size_t _storeyIndex, GameObjects* _obj)
{
	StoreyData* _thisStorey = nullptr;
	if (_storeyIndex != SCENE_STOREY_GROUND)
		_thisStorey = getSceneDataMapStoreyFormStoreyIndex(_storeyIndex);
	else
		_thisStorey = mGroundData;

	_thisStorey->removeObject(_obj);
}
void SceneData::removeStoreyAllObject(size_t _index)
{
	StoreyData* _thisStorey = nullptr;
	if (_index != SCENE_STOREY_GROUND_INDEX)
		_thisStorey = getSceneDataMapStorey(_index);
	else
		_thisStorey = mGroundData;

	_thisStorey->clearObject();
}

void SceneData::clearScene()
{
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		_thisStorey->clearObject();
	}
	mSceneDataMap.clear();
	//mGroundData->clearObject();
}

void SceneData::clearSceneAllObject()
{
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		_thisStorey->clearObject();
	}
	mGroundData->clearObject();
}
bool SceneData::findStoreyObject(size_t _index, GameObjects* _obj)
{
	if (!_obj)
		return false;

	StoreyData* _thisStorey = nullptr;
	if (_index != SCENE_STOREY_GROUND_INDEX)
		_thisStorey = getSceneDataMapStorey(_index);
	else
		_thisStorey = mGroundData;

	return _thisStorey->findObject(_obj);
}

StoreyData* SceneData::getSceneDataMapStorey(size_t _index)
{
	if (_index != SCENE_STOREY_GROUND_INDEX)
		return mSceneDataMap.at(_index); 
	else
		return mGroundData;
	return nullptr;
}


bool SceneData::isExistObjSelect(){

	bool selectFlag=false;
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _storeydata = getSceneDataMapStorey(i);
		for (GameObjects *sp:_storeydata->getObjectData())
		{
			int type=sp->getobjectType();
			if (type==TYPE_TOWER||type==TYPE_BOSS||type==TYPE_WARRIOR||type==TYPE_TRAP)
			{
				if (sp->getisSelect()==true)
				{
					selectFlag=true;
					break;
				}
			}

		}
	}

	//判断糖果机哪里的
	StoreyData* _homeStorey = getSceneDataMapStorey(SCENE_STOREY_HOME);
	auto _homeStoreyItem = _homeStorey->mItem;

	Widget *sweet_warp=(Widget*)_homeStoreyItem->getChildByTag(AlTAR_PARENT_TAG);
	 AltarObject *altar=(AltarObject*)sweet_warp->getChildByTag(AlTAR_MY_TAG);
	if (altar->getisSelect()==YES)
	{
		selectFlag=true;
	}

	return selectFlag;
}


GameObjects *SceneData::findExistObjSelect(){

	GameObjects *obj=nullptr;
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _storeydata = getSceneDataMapStorey(i);
		for (GameObjects *sp:_storeydata->getObjectData())
		{
			int type=sp->getobjectType();
			if (type==TYPE_TOWER||type==TYPE_BOSS||type==TYPE_WARRIOR||type==TYPE_TRAP)
			{
				if (sp->getisSelect()==true)
				{
					obj=sp;
					break;
				}
			}

		}
	}


	

	return obj;

}



StoreyData* SceneData::getSceneDataMapStoreyFormStoreyIndex(size_t _stroeyIndex)
{
	if (_stroeyIndex == SCENE_STOREY_GROUND)
		return mGroundData;
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		if (_thisStorey->getStoreyIndex() == _stroeyIndex)
		{
			return _thisStorey;
		}
	}
	return nullptr;
}

int SceneData::getNextCreateStoreyIndex()
{
	if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME))
	{
		if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP))
		{
			if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_FOUR))
			{
				if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_THREE))
				{
					if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TWO))
					{
						if (getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_ONE))
						{
							return -1;
						}else
							return SCENE_STOREY_ONE;
					}else
						return SCENE_STOREY_TWO;
				} else
					return SCENE_STOREY_THREE;
			} else
				return SCENE_STOREY_FOUR;
		} else
			return SCENE_STOREY_TOP;
	}else
		return SCENE_STOREY_HOME;
	return -1;
}

int SceneData::getNextStoreyIndex(size_t _storeyIndex)
{
	if (_storeyIndex == SCENE_STOREY_HOME)
	{
		return -1;
	}else if (_storeyIndex == SCENE_STOREY_GROUND)
	{
		if( getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP))
			return SCENE_STOREY_TOP;
	}else if (_storeyIndex == SCENE_STOREY_TOP)
	{
		if (getSceneDataMapStoreyFormStoreyIndex(_storeyIndex - 1) )
		{
			return _storeyIndex - 1;
		}else
		{
			return SCENE_STOREY_HOME;
		}
	}else
	{
		if (getSceneDataMapStoreyFormStoreyIndex(_storeyIndex - 1) )
		{
			return _storeyIndex - 1;
		}
	}
	return -1;
}


int  SceneData::getUpStoreyIndex(size_t _storeyIndex)//获取上一层
{
	if (getSceneDataMapStoreyFormStoreyIndex(_storeyIndex + 1) )
	{
		return _storeyIndex + 1;
	}
	return -1;

}

int SceneData::getLastStoreyIndex()
{
	int _lastStoreyIndex = -1;
	if (!getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_TOP))
		return _lastStoreyIndex;

	for (int i = SCENE_STOREY_TOP; i != SCENE_STOREY_ONE; i-- )
	{
		if (getSceneDataMapStoreyFormStoreyIndex(i))
		{
			_lastStoreyIndex = i;
		}
	}
	return _lastStoreyIndex;
}


void SceneData::update(float dt)
{
	
}

// 整个场景是否无兵(怪)
bool SceneData::isClearAllMonster()
{
	bool _isClear = true;
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		if (!_thisStorey->isClearMonster())
		{
			_isClear = false;
			break;
		}
	}

	// 如果场景内无兵，判断下地面是否也无兵
	if (_isClear)
	{
		if (!mGroundData->isClearMonster())
		{
			_isClear = false;
		}
	}
	return _isClear;
}

// 是否重复的boss
bool SceneData::isRepeatHero(int _selfType)
{
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		if (_thisStorey)
		{
			auto _curObject = _thisStorey->getHero();
			if (_curObject)
			{
				if (_curObject->getSelfType() == _selfType)
				{
					return true;
				}
			}
		}
	}
	return false;
}

// 是否有预警提示
bool SceneData::isYuJing()
{
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		if (_thisStorey)
		{
			auto _isYujing = _thisStorey->isYuJing();
			if (_isYujing)
			{
				return true;
			}
		}
	}
	return false;
}

// 是否打完一个波次
bool SceneData::isFinishBatch(int _batch)
{
	for (int i = 0; i < mSceneDataMap.size(); ++i)
	{
		StoreyData* _thisStorey = getSceneDataMapStorey(i);
		if (!_thisStorey->isFinishBatch(_batch))
		{
			return false;
		}
	}
	return true;
}

//cocos2d::__Dictionary* SceneData::getScenePist(const char* _sceneName)
//{
//	if (!mScenePlistConfig)
//		return nullptr;
//	auto _curName = StringUtils::toString(_sceneName);
//	auto _curResult = thisPlistMgr->getPlist(mScenePlistConfig, _sceneName);
//	if (_curResult)
//		return _curResult;
//	return nullptr;
//}
//
//
//cocos2d::__Dictionary* SceneData::getSceneMapPlist(const char* _sceneName, const char* _keyName)
//{
//	if (!mScenePlistConfig)
//	return nullptr;
//	auto _curScenePlist = getScenePist(_sceneName);
//	if (!_curScenePlist)
//	return nullptr;
//	auto _curResult = thisPlistMgr->getPlist(_curScenePlist, _keyName);
//	if (_curResult)
//	return _curResult;
//	return nullptr;
//}
//CCString SceneData::getSceneMapPlistCString(const char* _sceneName, const char* _keyName, const char* _boKeyName, const char* _boAtrName)
//{
//	if (!mScenePlistConfig)
//		return nullptr;
//	auto _curMapPlist = getSceneMapPlist(_sceneName, _keyName);
//	if (!_curMapPlist)
//		return nullptr;
//	auto _curMonsterPlist = thisPlistMgr->getPlist(_curMapPlist, _boKeyName);
//	if (!_curMonsterPlist)
//		return nullptr;
//	auto _curResult = thisPlistMgr->getPlistCString(_curMonsterPlist, _boAtrName);
//	return _curResult;
//	
//}
//
//CCString SceneData::getSceneMapCString(const char* _sceneName, const char* _keyName)
//{
//	if (!mScenePlistConfig)
//		return nullptr;
//	auto _curScenePlist = getScenePist(_sceneName);
//	if (!_curScenePlist)
//		return nullptr;
//	auto _curResult = thisPlistMgr->getPlistCString(_curScenePlist, _keyName);
//	return _curResult;
//}

