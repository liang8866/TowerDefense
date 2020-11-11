#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "tinyxml2/tinyxml2.h"
#include "platform/CCSAXParser.h"
#include "platform/CCFileUtils.h"

typedef cocos2d::Map<std::string, cocos2d::__Dictionary*>	PlistMapType;

class PlistManager : public cocos2d::Ref
{
public:
	static PlistManager* getInstance();
	static void destroyInstance();

	PlistManager(void);
	~PlistManager(void);

	bool initialise();
	bool reload();

	//const char* getTypeDataName(size_t _type);			// ȡ��������
	__String getTypeDataName(size_t _type);					// ȡ��������
	__String getTypeName(size_t _type, size_t _typeIndex);	// ȡ���ͱ�������

	cocos2d::__Dictionary* addPlistMap(const char* _plistName, const char* _plistFile);
	cocos2d::__Dictionary* getPlistMap(const char* _plistName);
	cocos2d::__Dictionary* getObjectPlist(const char* _plistName, const char* _objectName);
	cocos2d::__Dictionary* getObjectLevelPlist(const char* _plistName, const char* _objectName, size_t _objectLevel);
	
	Ref*		getAttributeObject(const char* _plistName, const char* _objectName, size_t _objectLevel, const char* _attributeName);
	__String	getAttributeCCString(const char* _plistName, const char* _objectName, size_t _objectLevel, const char* _attributeName);

	void		setConfigTypeCString(const char* _type, const char* _key, const char* _value);
	void        setConfigTypeObj(const char* _type, const char* _key, __Dictionary*);
	Ref*		getConfigTypeObject(const char* _type, const char* _key);
	__String	getConfigTypeCString(const char* _type, const char* _key);

	PlistMapType			mPlistMapData;


	// plist����
public: 
	cocos2d::__Dictionary*	getPlist(cocos2d::__Dictionary* _plist, const char* _keyName);
	Ref*					getPlistObject(cocos2d::__Dictionary* _plist, const char* _key);
	__String				getPlistCString(cocos2d::__Dictionary* _plist, const char* _key);
	void					setPlistObject(cocos2d::__Dictionary* _plist, const char* _key, Ref* _obj);
	void					setPlistCString(cocos2d::__Dictionary* _plist, const char* _key, const char* _value);
	

	cocos2d::__Dictionary*	toDictionary(Ref* _object);
	__String				toCString(Ref* _object);
	bool					toBool(Ref* _object);

	__String				getHeroSkill(std::string _heroKey, int _index, int _level, std::string _key);//Ӣ���������ܼ����ȼ�����ȡ�Ĺؼ���


public:
	// Config�����ļ�
	cocos2d::__Dictionary*	mConfig;

	bool					initConfig();						// ��ʼ�����������ļ�
	void					resetConfig();						// �ָ�Ĭ���ļ�
	cocos2d::__Dictionary*	getConfig();						// ȡ�����ļ�

	cocos2d::__Dictionary*  getGoodsDictionary();
	int                     getGoodsCount(std::string id);
	void                    setGoodsCount(std::string id,std::string count);
	
	bool					writeToFile(ValueMap& dict, const std::string &fullPath);
	void					makeEncode(char* buff,const int *key);
	void					makeDecode(Data  &data,const int *key);
	const int*				getKey();

	bool					getUnLockHeroKey(std::string _key);							// ȡӢ�۽���plist��
	void					setUnLockHeroKey(std::string _key, std::string _value);		// ���Ӣ�۽���plist��

	bool					getHeroDeadKey(std::string _key);							// ȡӢ������plist��
	void					setHeroDeadKey(std::string _key, std::string _value);		// ���Ӣ������plist��


	int						getConfigBossKeyValue(std::string _bossName, std::string _keyName);
	void					setConfigBossKeyValue(std::string _bossName, std::string _keyName, std::string _keyValue);
	void					clearConfigBossDeadValue();
	void					clearConfigBossFloorValue();


	// ȡ�Ƽ���������
	Ref*					getUpgradesConfigKeyValue(std::string _typeName, std::string _itemName, std::string _itemIndex, std::string _key);
	int						getUpgradesSelect(std::string _typeName, std::string _itemName);


	// ȡ��Ʒ���� item plist
	_BagItemInfo getItemData(int _itemId);

	// ȡ�������� item box plist
	vector<_ItemBoxData> getItem_BoxPlist_Data(int _boxId);

	// ȡ��������ƥ��
	__String getStringFormat(int _num);
};

