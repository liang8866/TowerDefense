#include "PlistManager.h"
#include "UserRecord.h"
const int enckey[]={2,6,9,4,8};//加密字符
static PlistManager* mPlistManager = nullptr;
PlistManager* PlistManager::getInstance()
{
	if (!mPlistManager)
	{
		mPlistManager = new PlistManager();
		mPlistManager->initialise();
	}
	return mPlistManager;
}

PlistManager::PlistManager(void)
{
	
}
PlistManager::~PlistManager(void)
{
	clearConfigBossDeadValue();
	clearConfigBossFloorValue();
	mConfig->writeToFile(FILE_CONFIG_PLIST);
	mConfig->release();
}

bool PlistManager::initialise()
{

	//mPlistMapFileData.clear();
	mPlistMapData.clear();
	mConfig = nullptr;

	// 加载创建配置文件
	initConfig();
	// 加载配置表
	addPlistMap("BossData",		"plist/boss.plist");
	addPlistMap("WarriorData",	"plist/warrior.plist");
	addPlistMap("TrapData",		"plist/trap.plist");
	addPlistMap("TowerData",	"plist/tower.plist");
	addPlistMap("EnemyData",	"plist/enemy.plist");
	addPlistMap("SceneData",	"plist/scene.plist");
	addPlistMap("Text",	"plist/text.plist");
	addPlistMap("Items",		"plist/items.plist");
	addPlistMap("EventData",	"plist/event.plist");

	std::string path = FileUtils::getInstance()->getWritablePath()+"upgrades.plist";
	bool ret = FileUtils::getInstance()->isFileExist(path.c_str());
	if (!ret)
		path = "plist/upgrades.plist";
	addPlistMap("Upgrades",path.c_str());

	return true;
}

bool PlistManager::reload()
{
	/*mPlistMapFileData.clear();
	mPlistMapData.clear();
	PlistMapFileType::iterator iter;
	for( iter = mPlistMapFileData.begin(); iter != mPlistMapFileData.end(); ++iter)
	{
	addPlist(iter->first, iter->second);
	}*/

	return true;
}

cocos2d::__Dictionary* PlistManager::addPlistMap(const char* _plistName, const char* _plistFile)
{
	auto _curPlist = __Dictionary::createWithContentsOfFile(_plistFile);

	CCAssert(_curPlist != nullptr, "add plist error!!!");    

	if (!_curPlist)
	{
		
	}else
	{
		mPlistMapData.insert(StringUtils::toString(_plistName), _curPlist);
		return _curPlist;
	}
	return nullptr;
}

cocos2d::__Dictionary* PlistManager::getPlistMap(const char* _plistName)
{
	auto _curPlist = mPlistMapData.at(StringUtils::toString(_plistName));
	CCAssert(_curPlist != nullptr, "get plist is error!");
	if (!_curPlist)
	{
		
	}else
	{
		return _curPlist;
	}
	return nullptr;

}

__String PlistManager::getTypeName(size_t _type, size_t _typeIndex)
{
	const char* _curTypeName = "";
	if (_type == TYPE_BOSS){
		_curTypeName = "boss%d";
	}else if (_type == TYPE_ENEMY){
		_curTypeName = "enemy%d";
	}else if (_type == TYPE_TRAP){
		_curTypeName = "trap%d";
	}else if (_type == TYPE_TOWER){
		_curTypeName = "tower%d";
	}else if (_type == TYPE_WARRIOR){
		_curTypeName = "war%d";
	}else if (_type == TYPE_NPC){
		_curTypeName = "npc%d";
	}else if (_type == TYPE_DOOR){
		_curTypeName = "door%d";
	}else if (_type == TYPE_ALTAR){
		_curTypeName = "altar%d";
	}
	__String _newTypeName = StringUtils::format(_curTypeName, _typeIndex);
	return _newTypeName;
}
__String PlistManager::getTypeDataName(size_t _type)
{
	const char* _curName = "WarriorData";
	if (_type == TYPE_BOSS){
		_curName = "BossData";
	}else if (_type == TYPE_ENEMY){
		_curName = "EnemyData";
	}else if (_type == TYPE_TRAP){
		_curName = "TrapData";
	}else if (_type == TYPE_TOWER){
		_curName = "TowerData";
	}else if (_type == TYPE_WARRIOR){
		_curName = "WarriorData";
	}else if (_type == TYPE_NPC){
		_curName = "NpcData";
	}else if (_type == TYPE_DOOR){
		_curName = "DoorData";
	}else if (_type == TYPE_ALTAR){
		_curName = "AltarData";
	}
	__String _newName = StringUtils::format("%s", _curName);
	return _newName;
}

cocos2d::__Dictionary* PlistManager::getObjectPlist(const char* _plistName, const char* _objectName)
{
	auto _curPlist = getPlistMap(_plistName);
	if (_curPlist)
	{
		auto _curObjectName = StringUtils::toString(_objectName);
		auto _curResultPlist = dynamic_cast<cocos2d::__Dictionary*>(_curPlist->objectForKey(_curObjectName));	// "war1"
		if (_curResultPlist)
		{
			return _curResultPlist;
		}else
		{
			
		}
	}
	return nullptr;
}

cocos2d::__Dictionary* PlistManager::getObjectLevelPlist(const char* _plistName, const char* _objectName, size_t _objectLevel)
{
	auto _curPlist = getObjectPlist(_plistName, _objectName);
	if (_curPlist)
	{
		auto _curKey = StringUtils::format("level%d", _objectLevel);
		auto _curLevelPlist = dynamic_cast<cocos2d::__Dictionary*>(_curPlist->objectForKey(_curKey));
		if (_curLevelPlist)
		{
			return _curLevelPlist;
		}
	}
	return nullptr;
}

Ref* PlistManager::getAttributeObject(const char* _plistName, const char* _objectName, size_t _objectLevel, const char* _attributeName)
{

	auto _curPlist = getObjectLevelPlist(_plistName, _objectName, _objectLevel);
	if (_curPlist)
	{
		auto _curAttributeName = StringUtils::toString(_attributeName);
		auto _curValueObject = _curPlist->objectForKey(_curAttributeName);		
		if (_curValueObject)
		{
			return _curValueObject;
		}
	}
	return nullptr;
}

__String PlistManager::getAttributeCCString(const char* _plistName, const char* _objectName, size_t _objectLevel, const char* _attributeName)
{
	auto _curValueObject = getAttributeObject(_plistName, _objectName, _objectLevel, _attributeName);

	if (_curValueObject)
	{
		__String* _curValue = dynamic_cast<__String*>(_curValueObject);
		if (_curValue != NULL)
		{
			__String _curResult = StringUtils::toString(_curValue->getCString());
			return _curResult;
		}
	}
	return "";
}


bool PlistManager::initConfig()
{
	if (!mConfig)
	{
		Dictionary* _curPlist = nullptr;
		std::string path = FileUtils::getInstance()->getWritablePath()+FILE_CONFIG_PLIST;

		bool ret = FileUtils::getInstance()->isFileExist(path.c_str());
		if (!ret)
			path = "plist/config.plist";
		_curPlist = Dictionary::createWithContentsOfFile(path.c_str());
		if (_curPlist == nullptr || _curPlist->_elements == nullptr)
		{
			//解码
			Data  data = FileUtils::getInstance()->getDataFromFile(path.c_str());
			if (data.getSize() > 0 )
			{
				makeDecode(data,getKey());
				/*ValueMap Value = FileUtils::getInstance()->getValueMapFromString((const char*)data.getBytes(),data.getSize());
				_curPlist = __Dictionary::createWithContentsOfValueMap(Value);*/
			}
		}
		
		if (_curPlist != nullptr && _curPlist->_elements !=nullptr)
		{
			mConfig = _curPlist;
			mConfig->retain();
			return true;
		} else {
			mConfig = __Dictionary::create();
			mConfig->retain();
			return true;
		}
	
	}
	return false;
}

void PlistManager::resetConfig()
{
	if (mConfig)
	{
		mConfig->removeAllObjects();
		String* pValue1 = String::create("100");
		mConfig->setObject(pValue1, "Test");
	}
}

cocos2d::__Dictionary* PlistManager::getConfig()
{
	if (!mConfig)
		return nullptr;
	return mConfig;
}

static tinyxml2::XMLElement* mapToXmlElement(const ValueMap &dict, tinyxml2::XMLDocument *doc);
static tinyxml2::XMLElement* mapToXmlElement(const ValueMap &dict, tinyxml2::XMLDocument *doc)
{
	tinyxml2::XMLElement* rootNode = doc->NewElement("dict");
	for (auto iter = dict.begin(); iter != dict.end(); ++iter)
	{

		switch (iter->second.getType())
		{
		case Value::Type::STRING :{

		
			tinyxml2::XMLElement* tmpNode = doc->NewElement("key");
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* keycontent = doc->NewText(iter->first.c_str());
			tmpNode->LinkEndChild(keycontent);
			tinyxml2::XMLElement* node = doc->NewElement("string");
			tinyxml2::XMLText* content = doc->NewText(iter->second.asString().c_str());
			node->LinkEndChild(content);
			rootNode->LinkEndChild(node);
			break;
		}
		case Value::Type::MAP :{
			if (iter->second.asValueMap().size() == 0)
			{
				break;
			}
			tinyxml2::XMLElement* tmpNode = doc->NewElement("key");
			rootNode->LinkEndChild(tmpNode);
			tinyxml2::XMLText* content = doc->NewText(iter->first.c_str());
			tmpNode->LinkEndChild(content);
			auto dd =  mapToXmlElement(iter->second.asValueMap(),doc);
			rootNode->LinkEndChild(dd);
			break;
							   }
		default:
			break;	
		}
	}
	return rootNode;
}


/************************************************************************/
/* 解密                                                                     */
/************************************************************************/
void PlistManager::makeDecode(Data &data,const int *key)
{
	ssize_t len = data.getSize();
	for (ssize_t i=0;i<len;++i)
	{
		*(data.getBytes()+i) = *(data.getBytes()+i)^key[i%5];
	}
}

/************************************************************************/
/* 加密                                                                  */
/************************************************************************/
void PlistManager::makeEncode(char* buff,const int *key)
{
	int len = strlen(buff);
	int i=0;
	for (;i<len;++i)
	{
		buff[i] = buff[i]^key[i%5];
	}
	buff[i] = '\0';
}
bool PlistManager::writeToFile(ValueMap& dict, const std::string &fullPath)
{
	
	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument();

	std::string path = FileUtils::getInstance()->getWritablePath()+fullPath;

	if (nullptr == doc)
		return false;

	tinyxml2::XMLDeclaration *declaration = doc->NewDeclaration("xml version=\"1.0\" encoding=\"UTF-8\"");
	if (nullptr == declaration)
	{
		delete doc;
		return false;
	}

	doc->LinkEndChild(declaration);
	tinyxml2::XMLElement *docType = doc->NewElement("!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\"");
	doc->LinkEndChild(docType);
	tinyxml2::XMLElement *rootEle = doc->NewElement("plist");
	rootEle->SetAttribute("version", "1.0");
	if (nullptr == rootEle)
	{
		delete doc;
		return false;
	}
	doc->LinkEndChild(rootEle);
	tinyxml2::XMLElement *innerDict = mapToXmlElement(dict, doc);
	if (nullptr == innerDict )
	{
		delete doc;
		return false;
	}
	rootEle->LinkEndChild(innerDict);
	tinyxml2::XMLPrinter printer;
	doc->Accept(&printer);
	char* buff = new char[printer.CStrSize()];
	memcpy(buff,printer.CStr(),printer.CStrSize());
	//加密
	//makeEncode(buff,getKey());
	//std::string writablePath = FileUtils::getInstance()->getWritablePath();
	//std::string fileName = writablePath.append(fullPath.c_str());
	FILE* file = fopen(path.c_str(), "wb");  
	if (file) {  
		fwrite(buff, sizeof(char),printer.CStrSize(),file);  
		fclose(file);  
	}  
	delete buff;
	delete doc;
	return true;
}

void PlistManager::setConfigTypeObj(const char* _type, const char* _key, __Dictionary* pobj)
{
	//mConfig->setObject(pobj,_key,)
	__Dictionary* typeDict = dynamic_cast<__Dictionary*>(mConfig->objectForKey(_type));
	
	if (!typeDict)
	{
		typeDict = __Dictionary::create();
		mConfig->setObject(typeDict,_type);
	}
	typeDict->setObject(pobj,_key);
	//将dictionary转换成Valuemap
	ValueMap value = mConfig->dictionaryToValuemap();
	///写入文件
	writeToFile(value,FILE_CONFIG_PLIST);
}
void PlistManager::setConfigTypeCString(const char* _type, const char* _key, const char* _value)
{
	if (!mConfig || !_type || !_key)
		return;

	auto _curTypeObjectDict = toDictionary(getPlistObject(mConfig, _type));
	if (_curTypeObjectDict)
	{
		Ref* _curKeyObject = getPlistObject(_curTypeObjectDict, _type);
		if (_curKeyObject)
			_curTypeObjectDict->removeObjectForKey(_key);		// 子存在就删除子
		_curTypeObjectDict->setObject(__String::create(StringUtils::toString(_value)), StringUtils::toString(_key));
	}else
	{
		cocos2d::__Dictionary* _curNewDict = new cocos2d::__Dictionary();
		_curNewDict->retain();
		_curNewDict->setObject(__String::create(StringUtils::toString(_value)), StringUtils::toString(_key));
		mConfig->setObject(_curNewDict, StringUtils::toString(_type));
	}

	//将dictionary转换成Valuemap
	ValueMap value = mConfig->dictionaryToValuemap();

	///写入文件
	writeToFile(value,FILE_CONFIG_PLIST);
}

Ref* PlistManager::getConfigTypeObject(const char* _type, const char* _key)
{
	if (!mConfig || !_type || !_key)
		return nullptr;

	auto _curTypeObjectDict = toDictionary(getPlistObject(mConfig, _type));

	if (_curTypeObjectDict)
	{
		Ref* _curKeyObject = getPlistObject(_curTypeObjectDict, _key);
		if (_curKeyObject)
			return _curKeyObject;
	}
	return nullptr;
}
__String PlistManager::getConfigTypeCString(const char* _type, const char* _key)
{
	if (!mConfig || !_type || !_key)
		return "";
	auto _curObject = getConfigTypeObject(_type, _key);
	return toCString(_curObject);
}

cocos2d::__Dictionary* PlistManager::getPlist(cocos2d::__Dictionary* _plist, const char* _keyName)
{
	auto _curName = StringUtils::toString(_keyName);
	auto _curResultPlist = dynamic_cast<cocos2d::__Dictionary*>(_plist->objectForKey(_curName));
	CCAssert(_curResultPlist != nullptr, "PlistManager::getPlist:>> get Plist is null!!!");
	return _curResultPlist;
}

Ref* PlistManager::getPlistObject(cocos2d::__Dictionary* _plist, const char* _key)
{
	if (!_plist)
		return nullptr;

	auto _curValueObject = _plist->objectForKey(StringUtils::toString(_key));
	if (_curValueObject)
	{
		return _curValueObject;
	}
	return nullptr;
}

__String PlistManager::getPlistCString(cocos2d::__Dictionary* _plist, const char* _key)
{
	__String _curResult = "";
	if (!_plist)
		return _curResult;

	auto _curObject = getPlistObject(_plist, _key);
	if (_curObject)
	{
		__String* _curValue = dynamic_cast<__String*>(_curObject);
		if (NULL != _curValue)
		{
			_curResult = StringUtils::toString(_curValue->getCString());
		}
	}
	return _curResult;
}

void PlistManager::setPlistObject(cocos2d::__Dictionary* _plist, const char* _key, Ref* _obj)
{
	if (!_plist || !_obj)
		return;
	Ref* _curObject = getPlistObject(_plist, _key);
	if (_curObject)
	{
		// 存在的话删除他
		_plist->removeObjectForKey(_key);
	}
	_plist->setObject(_obj, StringUtils::toString(_key));
}
void PlistManager::setPlistCString(cocos2d::__Dictionary* _plist, const char* _key, const char* _value)
{
	if (!_plist || !_key)
		return;
	Ref* _curObject = getPlistObject(_plist, _key);
	if (_curObject)
	{
		// 存在就删他
		_plist->removeObjectForKey(_key);
	}
	_plist->setObject(__String::create(StringUtils::toString(_value)), StringUtils::toString(_key));

}

cocos2d::__Dictionary* PlistManager::toDictionary(Ref* _object)
{
	if (!_object)
		return nullptr;
	auto _curResult = dynamic_cast<cocos2d::__Dictionary*>(_object);
	if (_curResult)
		return _curResult;
	return nullptr;
}
__String PlistManager::toCString(Ref* _object)
{
	__String _curResult = "";
	if (!_object)
		return _curResult;
	__String* _curValue = dynamic_cast<__String*>(_object);
	if (_curValue != NULL)
	{
		_curResult = StringUtils::toString(_curValue->getCString());
		return _curResult;
	}
	return _curResult;
}

const int* PlistManager::getKey()
{
	return enckey;
}

cocos2d::__Dictionary*  PlistManager::getGoodsDictionary()
{
	return getPlist(getConfig(), "GoodsItems");
}

int  PlistManager::getGoodsCount(std::string id)
{
	auto count = toCString(getConfigTypeObject("GoodsItems",id.c_str()));
	if (count.getCString())
		return count.intValue();

	return 0;
}

void PlistManager::setGoodsCount(std::string id,std::string count)
{
	setConfigTypeCString("GoodsItems", id.c_str() ,count.c_str());
}


 bool PlistManager::getUnLockHeroKey(std::string _key)
{
	auto _curObject = getConfigTypeObject(_key.c_str(), "UnLockBoss");
	if (_curObject)
	{
		auto _curValue = getConfigTypeCString( _key.c_str(), "UnLockBoss").intValue();
		if (_curValue == 1)
		{
			return true;
		}
	}
	return false;
}
void PlistManager::setUnLockHeroKey(std::string _key, std::string _value)
{
	auto _curObject = getConfigTypeObject(_key.c_str(), "UnLockBoss");
	if (_curObject)
		getConfig()->removeObjectForKey("UnLockBoss");
	setConfigTypeCString(_key.c_str(), "UnLockBoss" ,_value.c_str());
}

bool PlistManager::getHeroDeadKey(std::string _key)
{
	
	auto _curObject = getConfigTypeObject(_key.c_str(), "Dead");
	if (_curObject)
	{
		auto _curValue = getConfigTypeCString( _key.c_str(), "Dead").intValue();
		if (_curValue == 1)
		{
			return true;
		}
	}
	return false;
}
void PlistManager::setHeroDeadKey(std::string _key, std::string _value)
{
	auto _curObject = getConfigTypeObject(_key.c_str(), "Dead");
	if (_curObject)
		getConfig()->removeObjectForKey("Dead");
	setConfigTypeCString(_key.c_str(), "Dead", _value.c_str());
}


__String PlistManager::getHeroSkill(std::string _heroKey, int _index, int _level, std::string _key)
{
	__String _curPlistFile = StringUtils::format("plist/%sSkill.plist", _heroKey.c_str());
	auto _curPlist = Dictionary::createWithContentsOfFile(_curPlistFile.getCString());
	if (_curPlist)
	{
		auto _curSkillName = StringUtils::format("skill%d", _index);
		auto _curSkillPlist = getPlist(_curPlist, _curSkillName.c_str());
		if (_curSkillPlist)
		{
			auto _curLevelKey = StringUtils::format("level%d", _level);
			auto _curLevelPlist = getPlist(_curSkillPlist, _curLevelKey.c_str());
			if (_curLevelPlist)
			{
				auto _curValue = getPlistCString(_curLevelPlist, _key.c_str());
				return _curValue;
			}
		}
	}
	return "";
}


bool PlistManager::toBool(Ref* _object)
{
	auto _curValue = toCString(_object).intValue();
	if (_curValue == 1)
	{
		return true;
	}
	return false;
}

int PlistManager::getConfigBossKeyValue(std::string _bossName, std::string _keyName)
{
	auto _curObject = getConfigTypeObject(_bossName.c_str(), _keyName.c_str());
	if (_curObject)
	{
		auto _curValue = toCString(_curObject).intValue();
		return _curValue;
	}
	return -1;
}
void PlistManager::setConfigBossKeyValue(std::string _bossName, std::string _keyName, std::string _keyValue)
{
	auto _curObject = getConfigTypeObject(_bossName.c_str(), _keyName.c_str());
	if (_curObject)
	{
		auto _curBossPlist = getPlist(getConfig(), _bossName.c_str());
		if (_curBossPlist)
		{
			_curBossPlist->removeObjectForKey(_keyName);
		}
	}
	setConfigTypeCString(_bossName.c_str(), _keyName.c_str(), _keyValue.c_str());
}

void PlistManager::clearConfigBossDeadValue()
{
	// 从列表里取出所有的英雄
	auto _curPlist = getPlistMap("BossData");
	auto _curArray = _curPlist->allKeys();
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		auto _bossName = dynamic_cast<__String*>(_curArray->objectAtIndex(i))->getCString();
		setHeroDeadKey(_bossName, "0");
	}
}
void PlistManager::clearConfigBossFloorValue()
{
	// 从列表里取出所有的英雄
	auto _curPlist = getPlistMap("BossData");
	auto _curArray = _curPlist->allKeys();
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		auto _bossName = dynamic_cast<__String*>(_curArray->objectAtIndex(i))->getCString();
		auto _curValue = StringUtils::format("%d", -1);
		setConfigBossKeyValue(_bossName, "floor", _curValue);
	}
}


Ref* PlistManager::getUpgradesConfigKeyValue(std::string _typeName, std::string _itemName, std::string _itemIndex, std::string _key)
{
	auto _typePist = getObjectPlist("Upgrades", _typeName.c_str());
	if (_typePist)
	{
		auto _itemPlist = getPlist(_typePist, _itemName.c_str());
		if (_itemPlist)
		{
			auto _levelPlist = getPlist(_itemPlist, _itemIndex.c_str());
			if (_levelPlist)
			{
				Ref* _object = getPlistObject(_levelPlist, _key.c_str()); 
				if (_object)
				{
					return _object;
				}
			}
		}
	}
	return nullptr;
}

int PlistManager::getUpgradesSelect(std::string _typeName, std::string _itemName)
{

	auto userData=UserRecord::getInstance();
	return userData->getUpgradesItemLevel(_typeName,_itemName);

// 	auto _typePist = getObjectPlist("Upgrades", _typeName.c_str());
// 	if (_typePist)
// 	{
// 		auto _itemPlist = getPlist(_typePist, _itemName.c_str());
// 		if (_itemPlist)
// 		{
// 			int _select = 0;
// 			for (int i = 1; i != _itemPlist->count(); ++i)
// 			{
// 				auto _curLevelString = StringUtils::format("level%d", i);
// 				auto _levelPlist = getPlist(_itemPlist, _curLevelString.c_str());
// 				if (_levelPlist)
// 				{
// 					Ref* _object = getPlistObject(_levelPlist, "isSelect"); 
// 					if (_object)
// 					{
// 						auto _objectString = toCString(_object);
// 						auto _objectValue = _objectString.intValue();
// 						if (_objectValue == 1)
// 						{
// 							_select++;
// 						}
// 					}
// 				}
// 			}
// 			return _select;
// 		}
// 	}
// 	return 0;
}



// 取item 配置
_BagItemInfo PlistManager::getItemData(int _itemId)
{
	_BagItemInfo _curItemData;
	_curItemData._id = -1;
	if (_itemId <= 0)
		return _curItemData;

	auto _allItemPlist =  getPlistMap("Items");
	if (_allItemPlist)
	{
		auto _idPlistString = StringUtils::format("%d", _itemId);
		auto _idPlist = getPlist(_allItemPlist, _idPlistString.c_str());
		if (_idPlist)
		{
			_curItemData._id = _itemId;
			_curItemData._type = getPlistCString(_idPlist, "type").intValue();
			_curItemData._isUse = getPlistCString(_idPlist, "isUse").intValue();
			_curItemData._value = getPlistCString(_idPlist, "value").floatValue();
			_curItemData._desc = getPlistCString(_idPlist, "desc")._string;
			_curItemData._name = getPlistCString(_idPlist, "name")._string;
			_curItemData._icon = getPlistCString(_idPlist, "icon")._string;
		}
	}
	return _curItemData;
}

// 取item_box随机掉落盒内容配置
vector<_ItemBoxData> PlistManager::getItem_BoxPlist_Data(int _boxId)
{
	vector<_ItemBoxData> mBoxItems;
	mBoxItems.clear();
	if (_boxId <= 0)
		return mBoxItems;

	auto _itemBoxPlist = Dictionary::createWithContentsOfFile("plist/item_box.plist");
	if (_itemBoxPlist)
	{
		auto _idPlistString = StringUtils::format("%d", _boxId);
		auto _idPlist = getPlist(_itemBoxPlist, _idPlistString.c_str());
		if (_idPlist)
		{
			for (size_t i=0; i!=_idPlist->count(); ++i)
			{
				int _curIndex = i + 1;
				auto _oneItemPlistString = StringUtils::format("item%d", _curIndex);
				auto _oneItemPlist = getPlist(_idPlist, _oneItemPlistString.c_str());
				if (_oneItemPlist)
				{
					auto _curItemId = getPlistCString(_oneItemPlist, "id").intValue();
					if (_curItemId > 0)
					{
						_ItemBoxData _curItemBoxData;
						_curItemBoxData.id = _curItemId;
						_curItemBoxData.key = _oneItemPlistString;
						_curItemBoxData.name = getPlistCString(_oneItemPlist, "name")._string;
						_curItemBoxData.count = getPlistCString(_oneItemPlist, "count").intValue();
						_curItemBoxData.percent = getPlistCString(_oneItemPlist, "percent").floatValue();	
						mBoxItems.push_back(_curItemBoxData);
					}
				}
			}
		}
	}
	return mBoxItems;
}
// 取中文数字匹配
__String PlistManager::getStringFormat(int _num)
{
	if (_num < 0 || _num > 10)
		return "";
	auto keyString = StringUtils::format("%d", _num);
	auto _valueObject = getConfigTypeObject("NumberFormat", keyString.c_str());
	if (_valueObject)
	{
		return toCString(_valueObject);
	}
	return "";
}






