#include "UserRecord.h"
#include "PlistManager.h"
#include "SceneData.h"
#include "GameConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) //判断当前是否为Android平台 
#include <jni.h> 
#include "platform/android/jni/JniHelper.h" 
#include <android/log.h> 
#endif
USING_NS_CC;
static UserRecord* _UserRecord = nullptr;
 UserRecord* UserRecord::getInstance()
{
	if (_UserRecord == nullptr)
	{
		_UserRecord = new UserRecord();
		_UserRecord->init();
	}
	return _UserRecord;
}
void UserRecord::destroyInstance()
{
	CC_SAFE_DELETE(_UserRecord);
};

bool UserRecord::init()
{
	_isEvent104 = false;
	mTemporaryMagic.clear();
	_musicFile.clear();
	auto pm = PlistManager::getInstance();
	auto isbeginner = pm->getConfigTypeCString("account","beginner");
	_isBeginner = isbeginner.boolValue();
	if(_isBeginner == false){
		this->reset();
		this->flush();
	}else{
		_isLogin = false;
		_hp = _defaultHp = 10;
		_sweet = _defaultSweet = pm->getConfigTypeCString("base","baseGold").intValue();;
		_gold = 0;
		_heroSoul = 0;
		_isAward  = 0;
		_maxSweet = pm->getConfigTypeCString("base","baseMaxGold").intValue();
		_musicLock=pm->getConfigTypeCString("base","musicLock").intValue();
		_effectLock=pm->getConfigTypeCString("base","effectLock").intValue();
		_starCount =0;
		_round = 0;
		_maxRound = 3;
		_isGameOver = false;
		_CoolieCount = 3;
		_sectionNum = Value(pm->getConfigTypeCString("account","sectionNum").getCString()).asInt();
		_sceneNum = 1;
		_goodsInfo.clear();
		_goodsLimit.clear();
		_uid =  Value(pm->getConfigTypeCString("account","uid").getCString()).asInt();
		_uUid = pm->getConfigTypeCString("account","UUID").getCString();
		_seid = pm->getConfigTypeCString("account","ssd").getCString();
		_modifytime = Value(pm->getConfigTypeCString("account","modifytime").getCString()).asInt();
		
		_isHeroGuide   =  pm->getConfigTypeCString("account","hasguide").boolValue();
		_isFourthScene =  pm->getConfigTypeCString("account","isFourthScene").boolValue();
		_isGradeGuide  =  pm->getConfigTypeCString("account","isGradeGuide").boolValue();
		_indexGuide    =  pm->getConfigTypeCString("account","guideIndex").intValue();
		_indexPost     =  pm->getConfigTypeCString("account","indexPost").intValue();
		_isFreshMan = false;
		_isFirstScene  =  false;
		_seatCount = pm->getConfigTypeCString("account","seatnum").intValue();
	}
	_pixelShift.insert(make_pair(1,0.0f));
	_pixelShift.insert(make_pair(2,0.0f));
	_pixelShift.insert(make_pair(3,0.0f));
	_pixelShift.insert(make_pair(4,0.0f));
	setTitlePos(0);
	setBoxEnemyDieMethod(0);

	setClothesItemId(70001);

	_isGameStart = _isGamePause  = false;
	_isGameOver = true;
	return true;
}

void UserRecord::setMusicLockValue(int v)//设置音乐的值
{
	if (v!=_musicLock)
	{
		_musicLock=v;
		auto pm = PlistManager::getInstance();
		if (_musicLock==YES)
		{
			pm->setConfigTypeCString("base","musicLock","1");
		}else{
			pm->setConfigTypeCString("base","musicLock","0");
		}
		
	}


}
void UserRecord::setEffectLockValue(int v)//设置音效的值
{
	if (v!=_effectLock)
	{
	_effectLock=v;
	auto pm = PlistManager::getInstance();
	if (_effectLock==YES)
	{
		pm->setConfigTypeCString("base","effectLock","1");
	}else{
		pm->setConfigTypeCString("base","effectLock","0");
	     }
	}
}



void UserRecord::reset()
{
	_sweet = _defaultSweet = PlistManager::getInstance()->getConfigTypeCString("base","baseGold").intValue();
	_maxSweet = PlistManager::getInstance()->getConfigTypeCString("base","baseMaxGold").intValue();
	_round = 0;
	_sectionNum = 1;
	_sceneNum = 1;
	_starCount = 0;
	_heroSoul = 0;
	_gold =0;
	_hp = _defaultHp = 10;
	_isGameOver = false;
	_isAward = 0;
	_CoolieCount = 3;
	_isLogin = false;
	_uid =  0;
	time_t timer = time(0);
	_uUid = BulidUUID();
	_seid = "";
	_musicLock=NO;
	_effectLock=NO;
	_modifytime = 0;
	_goodsInfo.clear();
	_goodsLimit.clear();
	this->setIsHeroGuide(false);
	this->setIsFourthScene(false);
	this->setIsGradeGuide(false);
	this->setIndexGuide(0);
	this->setIndexPost(0);
	this->setIsFirstScene(false);
	setSeatCount(5);
	//_goodsInfo.insert(make_pair("10001","0"));
	//_goodsInfo.insert(make_pair("10002","0"));
	//_goodsInfo.insert(make_pair("10003","0"));
	//_goodsInfo.insert(make_pair("20001","0"));
	//_goodsInfo.insert(make_pair("20002","0"));
	//_goodsInfo.insert(make_pair("20003","0"));
	//_goodsInfo.insert(make_pair("20004","0"));
	//_goodsInfo.insert(make_pair("30001","0"));
	//_goodsInfo.insert(make_pair("40001","0"));
	//_goodsInfo.insert(make_pair("40002","0"));

}
void UserRecord::alterGold(int delta)
{
	this->setGold(this->getGold()+delta);
}

void UserRecord::alterSweet(int delta)
{
	this->setSweet(this->getSweet()+delta);
}
void UserRecord::alterHeroSoul(int delta)
{
	this->setHeroSoul(this->getHeroSoul()+delta);
}
void UserRecord::alterStarCount(int delta)
{
	this->setStarCount(this->getStarCount()+delta);
}

void UserRecord::resetHp()
{
	_hp = _defaultHp;
}
void UserRecord::resetSweet()
{
	_sweet = _defaultSweet;
}

void UserRecord::resetGameData()
{
	_sweet = _defaultSweet;
	_maxSweet = PlistManager::getInstance()->getConfigTypeCString("base","baseMaxGold").intValue();
	_hp = _defaultHp;
	_round = 0;

	// 恢复英雄所在层级, 死亡状态
	clearHeroValue_isDead_StoreyIndex();
}

/************************************************************************/
/* 保存玩家数据                                                          */
/************************************************************************/
void UserRecord::flush()
{
	auto pm = PlistManager::getInstance();
	pm->setConfigTypeCString("account","beginner","true");
	pm->setConfigTypeCString("account","UUID", _uUid.c_str() ); //用户唯一ID
	pm->setConfigTypeCString("account","ssd", _seid.c_str() ); //session id
	pm->setConfigTypeCString("account","uid", Value(_uid).asString().c_str() ); //用户ID
	pm->setConfigTypeCString("account","modifytime", Value(_modifytime).asString().c_str() ); //用户ID
	pm->setConfigTypeCString("account","sectionNum", Value(_sectionNum).asString().c_str() ); //用户所在地图
	string temp;
	if (getIsHeroGuide()){
		temp = "true";
	}else
	{
		temp = "false";
	}
	pm->setConfigTypeCString("account","hasguide", temp.c_str() );
	if (getIsFourthScene()){
		temp = "true";
	}else{
		temp = "false";
	}
	pm->setConfigTypeCString("account","isFourthScene", temp.c_str() );
	if (getIsGradeGuide()){
		temp = "true";
	}else{
		temp = "false";
	}
	pm->setConfigTypeCString("account","isGradeGuide", temp.c_str() );
	pm->setConfigTypeCString("account","guideIndex",Value(_indexGuide).asString().c_str());
	pm->setConfigTypeCString("account","indexPost",Value(_indexPost).asString().c_str());
}
void UserRecord::alterGoodsCount(std::string id , const int num)
{
	std::map<std::string,string>::iterator itr;
	itr = _goodsInfo.find(id);
	if (itr != _goodsInfo.end())
	{
		itr->second  = Value(atoi(itr->second.c_str())+num).asString();
	}
}
//设置用户的限制商品数量
void UserRecord::setGoodsLimit(const int goodsId ,const int limitCount)
{
	std::map<int,int>::iterator itr;
	itr = _goodsLimit.find(goodsId);
	if (itr == _goodsLimit.end())
	{
		_goodsLimit.insert(std::make_pair(goodsId, limitCount));
	} else {
		itr->second = limitCount;
	}
}
int UserRecord::getGoodsLimit(const int goodsId)
{
	std::map<int,int>::iterator itr;
	itr = _goodsLimit.find(goodsId);
	int ret = -1;
	if (itr != _goodsLimit.end())
	{
		ret = itr->second;
	}
	return ret;
}
void UserRecord::alterGooldLimit(const int goodsId ,const int limitCount)
{
	std::map<int,int>::iterator itr;
	itr = _goodsLimit.find(goodsId);
	if (itr != _goodsLimit.end())
	{
		itr->second += limitCount;
	}
}


void UserRecord::setGoodsCount(std::string id , string num)
{
	std::map<std::string,string>::iterator itr;
	itr = _goodsInfo.find(id);
	if (itr == _goodsInfo.end())
	{
		_goodsInfo.insert(std::make_pair(id, num));
	} else {
		itr->second = num;
	}

	if (!SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME))
		return;

	if (id.find("10001")!=string::npos){
		SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->strawberryCnt->setText(num);
		Button *btn = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->strawberryBtn;
		if (atoi(num.c_str())>0 && btn->_isgrey){
			btn->removeGrey();
		}

	}else if (id.find("10002")!=string::npos){
		SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mintCnt->setText(num);
		Button *btn = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->mintBtn;
		if (atoi(num.c_str())>0 && btn->_isgrey){
			btn->removeGrey();
		}

	}else if (id.find("10003")!=string::npos){
		SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->lemonCnt->setText(num);
		Button *btn = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(SCENE_STOREY_HOME)->lemonBtn;
		if (atoi(num.c_str())>0 && btn->_isgrey){
			btn->removeGrey();
		}
	}
}
int UserRecord::getGoodsCountByID(std::string id)
{
	map<string,string>::iterator iter=_goodsInfo.find(id);
	if (iter != _goodsInfo.end())
	{
		return atoi(iter->second.c_str());
	} else {
		return -1;
	}
}

void UserRecord::setRandomScene(const int chapter_id,const int scene_id)
{
	std::map<int,int>::iterator itr;
	itr = _randomScene.find(chapter_id);
	if (itr == _randomScene.end())
	{
		_randomScene.insert(std::make_pair(chapter_id, scene_id));
	} else {
		itr->second = scene_id;
	}
}
int UserRecord::getRandomScene(const int chapter_id)
{
	std::map<int,int>::iterator itr;
	int ret = 0;
	itr = _randomScene.find(chapter_id);
	if (itr != _randomScene.end())
	{
		ret = itr->second;
	} 
	return ret;
}

void UserRecord::setChapterLog(const int chapterNum,const int sceneNum,const int starNum)
{
	
	int starCount = starNum;
	//if (chapterNum == 1 && sceneNum <=3 && starNum > 0)
	//{
	//	starCount = 1;
	//}
	std::map<int,std::map<int,int>>::iterator itr;
	itr = chapter.find(chapterNum);
	//如果有此记录
	if (itr != chapter.end())
	{
		std::map<int,int>::iterator  itrr;
		itrr = itr->second.find(sceneNum);
		if (itrr != itr->second.end())
		{
			if (itrr->second >= starCount)
			{
				return;
			}
			itrr->second = starCount;
		}  else {
			itr->second.insert(make_pair(sceneNum,starCount));
		}


		if (sceneNum >= 4)
		{
			setIndexPost(sceneNum);
			setIsHeroGuide(true);
		}

		if (sceneNum >= 5)
		{
			setIsGradeGuide(true);
			setIndexGuide(sceneNum);
		}

		
		//开放下一关
		if (sceneNum < 15)
		{
			itrr = itr->second.find(sceneNum+1);
			if (itrr == itr->second.end())
			{
				itr->second.insert(make_pair(sceneNum+1,0));
			} 
		} else if (sceneNum == 15) { //开放下一章节
			itr = chapter.find(chapterNum+1);
			if (itr == chapter.end())
			{
				map<int,int> temp;
				temp.insert(std::make_pair(1, 0));
				chapter.insert(std::make_pair(chapterNum+1, temp ));
			} 
		}
	}  else {
		map<int,int> temp;
		temp.insert(std::make_pair(sceneNum, starCount));
		if (chapterNum == 1 && sceneNum == 1 && starCount > 0)
		{
			temp.insert(std::make_pair(2, 0));
		}
		chapter.insert(std::make_pair(chapterNum, temp ));
	}
}
/************************************************************************/
/* 设置用户过关信息 
/* @param ChapterNum 第几章
/* @param SceneNum  第几关
/* @param starNum   星级
/************************************************************************/
void UserRecord::setSceneStarCount(const int chapterNum,const int sceneNum,const int starNum)
{
	int starCount = starNum;
	if (chapterNum == 1 && sceneNum <=3 && starNum > 0)
	{
	//	starCount = 1;
	}
	std::map<int,std::map<int,int>>::iterator itr;
	itr = chapter.find(chapterNum);
	//如果有此记录
	if (itr != chapter.end())
	{
		std::map<int,int>::iterator  itrr;
		itrr = itr->second.find(sceneNum);
		if (itrr != itr->second.end())
		{
			if (itrr->second >= starCount)
			{
				return;
			}
			alterStarCount(starCount-itrr->second);
			itrr->second = starCount;
		}  else {
			itr->second.insert(make_pair(sceneNum,starCount));
			alterStarCount(starCount);
		}

		//开放下一关
		if (sceneNum < 15)
		{
			itrr = itr->second.find(sceneNum+1);
			if (itrr == itr->second.end())
			{
				itr->second.insert(make_pair(sceneNum+1,0));
			} 
		} else if (sceneNum == 15) { //开放下一章节
			itr = chapter.find(chapterNum+1);
			if (itr == chapter.end())
			{ 
				map<int,int> temp;
				temp.insert(std::make_pair(1, 0));
				chapter.insert(std::make_pair(chapterNum+1, temp ));
			} 
		}
	}  else {
		map<int,int> temp;
		temp.insert(std::make_pair(sceneNum, starCount));
		chapter.insert(std::make_pair(chapterNum, temp ));
		alterStarCount(starCount);
	}
}


int UserRecord::getSceneStarCount(int chapterNum,int sceneNum)
{
	std::map<int,std::map<int,int>>::iterator itr;
	itr = chapter.find(chapterNum);

	if (itr != chapter.end())
	{
		std::map<int,int>::iterator  itrr;
		itrr = itr->second.find(sceneNum);
		if (itrr != itr->second.end())
		{
			return itrr->second;
		}  else {
			return -1;
		}
	} else {
		return -1;
	}
}

void UserRecord::purge()
{
	 CC_SAFE_RELEASE_NULL(_UserRecord);
}
UserRecord::UserRecord()
{

}

UserRecord::~UserRecord()
{
	this->flush();
}


int UserRecord::getStarEgg()
{
	auto _curId = StringUtils::format("%d", 30001);
	int _curCount = getGoodsCountByID(_curId);
	return _curCount;
}

void  UserRecord::initPostData()
{
	__Dictionary* data = PlistManager::getInstance()->getPlistMap("SceneData");

	m_enemy.clear();

	Value currentSection(getSectionNum());
	int currSceneCount = dynamic_cast<__Dictionary*>(data->objectForKey(currentSection.asString()))->count();
	if (getSceneNum()>currSceneCount && getSectionNum()<data->count())
	{
		setSceneNum(1);
		setSectionNum(getSectionNum()+1);
	}else if (getSceneNum()>currSceneCount && getSectionNum()>=data->count()){
		return;
	}


	Value currentSections(getSectionNum());
	Value currentScene(getSceneNum());
	Value currentRound(getRound());
	__Dictionary* curr_section = dynamic_cast<__Dictionary*>(data->objectForKey(currentSections.asString()));
	__Dictionary* allScene = dynamic_cast<__Dictionary*>(curr_section->objectForKey("scene"));
	__Dictionary* curr_scene = dynamic_cast<__Dictionary*>(allScene->objectForKey(currentScene.asString()));
	__Dictionary* monster = dynamic_cast<__Dictionary*>(curr_scene->objectForKey("monster"));
	setMaxRound(monster->count());
	__Dictionary* round = dynamic_cast<__Dictionary*>(monster->objectForKey(currentRound.asString()));
	__String* disTime  = dynamic_cast<__String*>(round->objectForKey("disTime"));
	__Dictionary* enemys = dynamic_cast<__Dictionary*>(round->objectForKey("enemy"));

	setDisTime(disTime->intValue());

	vector<int> enemyType;
	for (int i=1;i<=enemys->count();++i)
	{
		__Dictionary* item = dynamic_cast<__Dictionary*>(enemys->objectForKey(Value(i).asString()));
		__String* e_type = dynamic_cast<__String*>(item->objectForKey("type"));
		__String* e_num = dynamic_cast<__String*>(item->objectForKey("num"));
		for (int j=1;j<=e_num->intValue();++j){
			enemyType.push_back(e_type->intValue());
		}
	}

	setEnemy(enemyType);
}

void UserRecord::dataResume()
{
	UserRecord::getInstance()->setRound(0);
	UserRecord::getInstance()->setHP(20);
	UserRecord::getInstance()->setSweet(1000);
	UserRecord::getInstance()->setMaxSweet(2000);
	//initPostData();
}

void UserRecord::takeUnitsInit()
{
	_takeUnitsID.clear();
}

void UserRecord::removeUpgradesType(std::string type)
{
	if (_upgrades.size() == 0)
	{
		return;
	}
	std::map<std::string,std::map<std::string,int>>::iterator itr;
	itr = _upgrades.find(type);
	if (itr != _upgrades.end())
	{
		_upgrades.erase(itr);
	} 
}
int UserRecord::getUpgradesItemLevel(std::string type,std::string itemid)
{
	int num = 0;
	std::map<std::string,std::map<std::string,int>>::iterator itr;
	itr = _upgrades.find(type);
	if (itr != _upgrades.end())
	{
		std::map<std::string,int>::iterator itrr;
		itrr = itr->second.find(itemid);
		if (itrr !=  itr->second.end())
		{
			num =  itrr->second;
		} 
	} 
	return num;
}
float UserRecord::getUpgradesItemAllAddValue(std::string type,std::string itemid){//只对减少或者增加相同属性的有用
	float valuenum=0.0;
	int item1Level = UserRecord::getInstance()->getUpgradesItemLevel(type,itemid);
	if (item1Level > 0)
	{
		for (int i = 1;i<=item1Level;++i)
		{
			auto _curLevelString = StringUtils::format("level%d", i);
			auto _thisScalObject = PlistManager::getInstance()->getUpgradesConfigKeyValue(type, itemid, _curLevelString, "value");
			valuenum+= PlistManager::getInstance()->toCString(_thisScalObject).floatValue();		
		}
	}
	
	valuenum=( (double)( (int)( (valuenum+0.00)*100 ) ) )/100;
	
	return valuenum;
}
void UserRecord::setUpgradesItemLevel(std::string type,std::string itemid,const int level)
{
	std::map<std::string,std::map<std::string,int>>::iterator itr;
	itr = _upgrades.find(type);
	//找出指定天赋分类
	if (itr != _upgrades.end()) 
	{
		std::map<std::string,int>::iterator itrr;
		itrr = itr->second.find(itemid);
		if (itrr != itr->second.end())
		{
			itrr->second = level;
		} else {
			itr->second.insert(std::make_pair(itemid, level));
		}
	} else {
		//_upgrades.insert(std::make_pair(type, map<std::string,int> ));
		map<std::string,int> temp;
		temp.insert(std::make_pair(itemid, level));
		_upgrades.insert(std::make_pair(type, temp ));
	}
}
// 取天赋某项是否是激活
float UserRecord::getUpgradesItemIsSelect(std::string type, std::string itemid, int _itemLevel)
{
	int _curLevel = UserRecord::getInstance()->getUpgradesItemLevel(type, itemid);
	if (_curLevel > 0 && _curLevel >= _itemLevel)
	{
		auto _curLevelString = StringUtils::format("level%d", _itemLevel);
		auto _thisScalObject = PlistManager::getInstance()->getUpgradesConfigKeyValue(type, itemid, _curLevelString, "value");
		float _value = PlistManager::getInstance()->toCString(_thisScalObject).floatValue();	
		return _value;
	}
	return 0.0;
}


// 取所有英雄数据
vector<_HeroData> UserRecord::getAllHeroData()
{ 
	return mHeroData;
}
// 取英雄数据
_HeroData UserRecord::getHeroData(int _id)
{ 
	_HeroData _thisData;
	_thisData.id = -1;
	_thisData.unLock = NO;
	_thisData.level = 0;
	_thisData.star = 0;
	_thisData.blessing = 0;
	_thisData.skill1_level = 0;
	_thisData.skill2_level = 0;
	_thisData.skill3_level = 0;
	_thisData.successRate = 0;
	_thisData.deletTime = 0;
	_thisData.isReviveTime = 0;
	_thisData.revivePosX = 0;
	_thisData.isDead = NO;
	_thisData.storeyIndex - 1;
	if (_id <= 0)
	{
		return _thisData;
	}
	auto _typeName = StringUtils::format("boss%d", _id);
	auto _curName = PlistManager::getInstance()->getAttributeCCString("BossData", _typeName.c_str(), 0, "name")._string;
	_thisData.name = _curName;

	for (int i = 0; i != mHeroData.size(); i++)
	{
		if (mHeroData.at(i).id == _id)
		{
			_thisData.id = _id;
			mHeroData.at(i).name = _thisData.name;
			_thisData = mHeroData.at(i);
			break;
		}
	}
	return _thisData;
}
// 设置英雄数据
void UserRecord::setHeroKeyValue(int _id, int _key, int _value)
{ 
	if (_id <= 0 || _key < 0 || _value <= -1000)
	{
		CCLOG("int _id, int _key, int _value is null");
		return;
	}

	int _isFindIndex = -1;
	for (int i = 0; i != mHeroData.size(); i++)
	{
		if (mHeroData.at(i).id == _id)
		{
			_isFindIndex = i;
			break;
		}
	}
	_HeroData _thisData;
	_thisData.id = -1;
	if (_isFindIndex >= 0)
	{
		// 找到就赋值
		_thisData = mHeroData.at(_isFindIndex);
	}else
	{
		// 没找到就创建新的
		_thisData.id = _id;
	}

	if (_key == _HeroDataKey_Index_id)
	{
		_thisData.id = _value;
	}else if (_key == _HeroDataKey_Index_unLock)
	{
		_thisData.unLock = _value;
	}else if (_key == _HeroDataKey_Index_level)
	{
		_thisData.level = _value;
	}else if (_key == _HeroDataKey_Index_star)
	{
		_thisData.star = _value;
	}else if (_key == _HeroDataKey_Index_blessing)
	{
		_thisData.blessing = _value;
	}else if (_key == _HeroDataKey_Index_skill1_level)
	{
		_thisData.skill1_level = _value;
	}else if (_key == _HeroDataKey_Index_skill2_level)
	{
		_thisData.skill2_level = _value;
	}else if (_key == _HeroDataKey_Index_skill3_level)
	{
		_thisData.skill3_level = _value;
	}else if (_key == _HeroDataKey_Index_successRate)
	{
		_thisData.successRate = _value;
	}else if (_key == _HeroDataKey_Index_deletTime)
	{
		_thisData.deletTime = _value;
	}else if (_key == _HeroDataKey_Index_revivieTime)
	{
		_thisData.isReviveTime = _value;
	}else if (_key == _HeroDataKey_Index_reviviePosX)
	{
		_thisData.revivePosX = _value;
	}else if (_key == _HeroDataKey_Index_isDead)
	{
		_thisData.isDead = _value;
	}else if (_key == _HeroDataKey_Index_storeyIndex)
	{
		_thisData.storeyIndex = _value;
	}

	if (_isFindIndex >= 0)
	{
		// 将改变的数据赋值回去
		mHeroData.at(_isFindIndex) = _thisData;
	}else
	{
		// 将新数据赋值添加进去
		mHeroData.push_back(_thisData);
	}
}
// 设置英雄数据
void UserRecord::setHeroKeyValue_Float(int _id, int _key, float _value)
{ 
	if (_id <= 0 || _key < 0 || _value < -999)
	{
		CCLOG("int _id, int _key, int _value is null");
		return;
	}

	int _isFindIndex = -1;
	for (int i = 0; i != mHeroData.size(); i++)
	{
		if (mHeroData.at(i).id == _id)
		{
			_isFindIndex = i;
			break;
		}
	}
	_HeroData _thisData;
	_thisData.id = -1;
	if (_isFindIndex >= 0)
	{
		// 找到就赋值
		_thisData = mHeroData.at(_isFindIndex);
	}else
	{
		// 没找到就创建新的
		_thisData.id = _id;
	}

	if (_key == _HeroDataKey_Index_successRate)
	{
		_thisData.successRate = _value;
	}else if (_key == _HeroDataKey_Index_reviviePosX)
	{
		_thisData.revivePosX = _value;
	}

	if (_isFindIndex >= 0)
	{
		// 将改变的数据赋值回去
		mHeroData.at(_isFindIndex) = _thisData;
	}else
	{
		// 将新数据赋值添加进去
		mHeroData.push_back(_thisData);
	}
}
// 取英雄数据
int UserRecord::getHeroKeyValue(int _id, int _key)
{ 
	if (_id <= 0 || _key < 0)
	{
		CCLOG("int _id, int _key is null");
		return -1;
	}
	
	int _isFindIndex = -1;
	for (int i = 0; i != mHeroData.size(); i++)
	{
		if (mHeroData.at(i).id == _id)
		{
			_isFindIndex = i;
			break;
		}
	}
	int _value = -1;
	if (_isFindIndex >= 0)
	{
		// 找到的数据返回
		if (_key == _HeroDataKey_Index_id)
		{
			_value = mHeroData.at(_isFindIndex).id;
		}else if (_key == _HeroDataKey_Index_unLock)
		{
			_value = mHeroData.at(_isFindIndex).unLock;
		}else if (_key == _HeroDataKey_Index_level)
		{
			_value = mHeroData.at(_isFindIndex).level;
		}else if (_key == _HeroDataKey_Index_star)
		{
			_value = mHeroData.at(_isFindIndex).star;
		}else if (_key == _HeroDataKey_Index_blessing)
		{
			_value = mHeroData.at(_isFindIndex).blessing;
		}else if (_key == _HeroDataKey_Index_skill1_level)
		{
			_value = mHeroData.at(_isFindIndex).skill1_level;
		}else if (_key == _HeroDataKey_Index_skill2_level)
		{
			_value = mHeroData.at(_isFindIndex).skill2_level;
		}else if (_key == _HeroDataKey_Index_skill3_level)
		{
			_value = mHeroData.at(_isFindIndex).skill3_level;
		}else if (_key == _HeroDataKey_Index_deletTime)
		{
			_value = mHeroData.at(_isFindIndex).deletTime;
		}else if (_key == _HeroDataKey_Index_revivieTime)
		{
			_value = mHeroData.at(_isFindIndex).isReviveTime;
		}else if (_key == _HeroDataKey_Index_isDead)
		{
			_value = mHeroData.at(_isFindIndex).isDead;
		}else if (_key == _HeroDataKey_Index_storeyIndex)
		{
			_value = mHeroData.at(_isFindIndex).storeyIndex;
		}
	}
	return _value;
}
// 取英雄数据
float UserRecord::getHeroKeyValue_Float(int _id, int _key)
{
	if (_id <= 0 || _key < 0)
	{
		CCLOG("int _id, int _key is null");
		return -1;
	}

	int _isFindIndex = -1;
	for (int i = 0; i != mHeroData.size(); i++)
	{
		if (mHeroData.at(i).id == _id)
		{
			_isFindIndex = i;
			break;
		}
	}
	float _value = -1000;
	if (_isFindIndex >= 0)
	{
		if (_key == _HeroDataKey_Index_successRate)
		{
			_value = mHeroData.at(_isFindIndex).successRate;
		}else if (_key == _HeroDataKey_Index_reviviePosX)
		{
			_value = mHeroData.at(_isFindIndex).revivePosX;
		}
	}
	return _value;
}
// 清空英雄死亡状态 、 层级数据
void UserRecord::clearHeroValue_isDead_StoreyIndex()
{
	for (int i = 0; i != mHeroData.size(); i++)
	{
		mHeroData.at(i).isDead = NO;
		mHeroData.at(i).storeyIndex = -1;
	}
}

bool UserRecord::getObjectsIsUnlock(std::string objectId)
{
	std::map<std::string,std::string>::iterator itr;
	itr = _objects.find(objectId);
	if (itr == _objects.end())
	{
		return false;
	}  else {
		return true;
	}
}
void  UserRecord::setObjectsUnLock(std::string objectKey,std::string objectId)
{

	std::map<std::string,std::string>::iterator itr;
	itr = _objects.find(objectKey);
	if (itr == _objects.end())
	{
		_objects.insert(make_pair(objectKey,objectId));
	}  else {
		itr->second=objectId;
	}
}

int UserRecord::getReviveSweet()
{
	// 科技树，英雄复活价格减少,只限糖果
	float _level2 = getUpgradesItemIsSelect("tech", "item5", 2);
	float _level4 =	getUpgradesItemIsSelect("tech", "item5", 4);
	float _resultValue = (1 - _level2 - _level4) * ALTAR_REVIVE_SWEET_COST;
	return _resultValue;
}
int UserRecord::getReviveSweetTime()
{
	// 科技树，英雄复活时间减少
	float _level1 = getUpgradesItemIsSelect("tech", "item5", 1);
	float _level5 = getUpgradesItemIsSelect("tech", "item5", 5);
	float _resultValue = ALTAR_REVIVE_SWEET_TIME - _level1 - _level5;
	return (int)_resultValue;
}

bool UserRecord::isActivityTime()
{
	ActivityBeginTime activityBeginTime;
	activityBeginTime.hour1 = 0;
	activityBeginTime.min1  = 0;
	activityBeginTime.hour2 = 12;
	activityBeginTime.min2  = 0;

	ActivityEndTime activityEndTime;
	activityEndTime.hour1 = 12;
	activityEndTime.min1  = 0;
	activityEndTime.hour2 = 24;
	activityEndTime.min2  = 0;

	bool isActivity = false;
	if (getTm(getServerTime())->tm_hour>=activityBeginTime.hour1 && getTm(getServerTime())->tm_min>=activityBeginTime.min1 && getTm(getServerTime())->tm_hour<activityEndTime.hour1){
		isActivity = true;
	}else if (getTm(getServerTime())->tm_hour==activityEndTime.hour1 && getTm(getServerTime())->tm_min<=activityEndTime.min1){
		isActivity = true;
	}else if (getTm(getServerTime())->tm_hour>=activityBeginTime.hour2 && getTm(getServerTime())->tm_min>=activityBeginTime.min2 && getTm(getServerTime())->tm_hour<activityEndTime.hour2){
		isActivity = true;
	}else if (getTm(getServerTime())->tm_hour==activityEndTime.hour2 && getTm(getServerTime())->tm_min<=activityEndTime.min2){
		isActivity = true;
	}

	return isActivity;
}

int UserRecord::getBoxEnemySceneBySection(int section)
{
	return getRandomScene(section);
}

void UserRecord::setBoxEnemySceneBySection(int scene_num,int section)
{
	setRandomScene(section,scene_num);
}

std::string UserRecord::BulidUUID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	JniMethodInfo minfo;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"test/Test","getUUID", "()Ljava/lang/String;");  
	if (!isHave) { 
		CCLog("jni:no"); 
	}else{ 
		jstring jstr = (jstring) minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);
		return JniHelper::jstring2string(jstr);
	}
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	return  Value((int)time(0)).asString();
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) 
	return  Value((int)time(0)).asString();
#endif
}
// 设置临时魔法道具数量
void UserRecord::setTemporaryMagic(int _id, int _count)
{
	std::map<int, int>::iterator iter;
	iter = mTemporaryMagic.find(_id);
	if (iter == mTemporaryMagic.end())
	{
		if (_count >= 0)
		{
			mTemporaryMagic.insert(std::make_pair(_id, _count));
		}
	} else {
		iter->second = _count;
	}
}
// 取临时魔法道具数量
int UserRecord::getTemporaryMagic(int _id)
{
	int _count = 0;
	std::map<int, int>::iterator iter;
	iter = mTemporaryMagic.find(_id);
	if (iter != mTemporaryMagic.end())
	{
		_count = iter->second; 
	}
	return _count;
}
// 增减魔法道具的数量
void UserRecord::alterTemporaryMagic(int _id, int _count)
{
	std::map<int, int>::iterator iter;
	iter = mTemporaryMagic.find(_id);
	if (iter == mTemporaryMagic.end())
	{
		if (_count >= 0)
		{
			mTemporaryMagic.insert(std::make_pair(_id, _count));
		}
	} else {
		iter->second += _count;
		if (iter->second <= 0)
		{
			iter->second = 0;
		}
	}
}
// 清空魔法道具数量
void UserRecord::clearTemporaryMagic()
{
	int _count = 0;
	std::map<int, int>::iterator iter;
	iter = mTemporaryMagic.begin();
	while (iter != mTemporaryMagic.end())
	{
		iter->second = 0;
		++iter;
	}
	mTemporaryMagic.clear();
}
// 取随机数
int UserRecord::getRandom(int _maxNumber)
{
	//srand(time(0));
	// 区间 1 - max
	int _number = 0;
	if (_maxNumber <= 1)
	{
		_number = _maxNumber;
	}
	if (_maxNumber > 1)
	{
		//_number = rand()%_maxNumber + 1;
		_number = (int)(CCRANDOM_0_1()*_maxNumber) + 1;
	}
	return _number;
}

// 设置音乐播放
void UserRecord::setMusicPlay(string _music)
{
	CCLOG("getmusicLock()=%d",getmusicLock());
	setMuiscFile(_music);
	if (_musicFile.empty())
	{
		audioPlayer::getInstance()->stopBackgroundMusic();
	}else
	{
		if (getmusicLock() == NO)
		{
			audioPlayer::getInstance()->playBackgroundMusic(_musicFile.c_str(), true);
		}
	}
}

// 恢复音乐播放
void UserRecord::resetMuiscPlay()
{
	if (_musicFile.empty())
	{
		audioPlayer::getInstance()->stopBackgroundMusic();
	}else
	{
		if (getmusicLock() == NO)
		{
			audioPlayer::getInstance()->playBackgroundMusic(_musicFile.c_str(), true);
		}
	}
}


