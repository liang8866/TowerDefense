#ifndef __UserRecord__UserRecord__
#define __UserRecord__UserRecord__
#include "cocos2d.h"
#include "PlistManager.h"
using namespace std;
struct ActivityBeginTime
{
	int hour1;
	int min1;

	int hour2;
	int min2;
};

struct ActivityEndTime
{
	int hour1;
	int min1;

	int hour2;
	int min2;
};

class UserRecord : public cocos2d::CCObject
{
public:
	static UserRecord* getInstance();
	static void destroyInstance();
	CC_SYNTHESIZE(int , _gold, Gold); //金币
	CC_SYNTHESIZE(int , _maxGold, maxGold); //最大金币
	CC_SYNTHESIZE(int , _sweet, Sweet); //糖果数
	CC_SYNTHESIZE(int , _maxSweet, MaxSweet); //最大糖果数
	CC_SYNTHESIZE(int , _heroSoul, HeroSoul); //英雄之魂
	CC_SYNTHESIZE(int , _advance_egg, AdvanceEgg); //进阶蛋
	CC_SYNTHESIZE(int , _wish_egg, WishEgg); //祝福蛋
	CC_SYNTHESIZE(int , _CoolieCount, CoolieCount); //苦力可用位置
	CC_SYNTHESIZE(int , _hp , HP); //生命值
	CC_SYNTHESIZE(bool, _isBeginner, IsBeginner); 
	CC_SYNTHESIZE(bool, _isLogin, IsLogin); 
	CC_SYNTHESIZE(int , _uid, Uid);	 //用户ID	
	CC_SYNTHESIZE(std::string , _uUid, Uuid); //手机ID	
	CC_SYNTHESIZE(int, _loginDayNum, LoginDayNum);  //连续登录天数
	CC_SYNTHESIZE(bool, _isAward, IsAward); //是否领取了登录奖励
	CC_SYNTHESIZE(int, _platform, Platform); //平台ID
	CC_SYNTHESIZE(int, _modifytime, ModifyTime);  //最近操作时间
	CC_SYNTHESIZE(int, _lastlogin, LastLogin);  //最近一次登录时间
	CC_SYNTHESIZE(int, _birthday, Birthday); //注册时间
	CC_SYNTHESIZE(std::string, _seid, Seid); //session_id
	CC_SYNTHESIZE(int , _round, Round);       //回合
	CC_SYNTHESIZE(int , _maxRound, MaxRound);    //最大回合数
	CC_SYNTHESIZE(bool , _isGameStart, IsGameStart);  //游戏是否开始
	CC_SYNTHESIZE(bool , _isGameOver, IsGameOver);  //游戏是否结束
	CC_SYNTHESIZE(bool , _isGamePause, IsGamePause);  //游戏是否暂停
	CC_SYNTHESIZE(int , _sectionNum, SectionNum);  //当前游戏章节
	CC_SYNTHESIZE(int , _sceneNum, SceneNum);		//当前游戏关卡
	CC_SYNTHESIZE(int , _seatCount, SeatCount);    //携带单位数
	CC_SYNTHESIZE(int , _starCount, StarCount);    //星星数
	CC_SYNTHESIZE(bool , _isEvent104, IsEvent104);  //是否104event 特殊道具关卡
	
	std::string BulidUUID(); //生成用户唯一ID;

	std::map<std::string,std::map<std::string,int>> _upgrades;  //用户天赋数据
	std::map<std::string,std::map<std::string,int>> getAllUpgrades(); //取所有天赋数据

	int getUpgradesItemLevel(std::string type,std::string itemid); //取单个分类下某条目等级 如 type=tech itemid=item1
	float getUpgradesItemAllAddValue(std::string type,std::string itemid);//只对减少或者增加相同属性的有用
	void setUpgradesItemLevel(std::string type,std::string itemid,const int); //设置某一条目等级
	void removeUpgradesType(std::string type); //删除某一分类下所有数据
	float getUpgradesItemIsSelect(std::string type, std::string itemid, int _itemLevel);

	//每日随机关数据

	CC_SYNTHESIZE(int,_musicLock,musicLock);//音乐是否锁了
	CC_SYNTHESIZE(int,_effectLock,effectLock);//音效是否锁了
	map<int,int> _randomScene;
	void setRandomScene(const int chapter_id,const int scene_id);
	int getRandomScene(const int chapter_id);


	map<int,int> _goodsLimit; //限制商品数据
	void setGoodsLimit(const int goodsId ,const int limitCount);
	int getGoodsLimit(const int goodsId);
	void alterGooldLimit(const int goodsId ,const int limitCount);


	map<string,string> _objects;
	bool getObjectsIsUnlock(std::string objectKey);
	void setObjectsUnLock(std::string objectKey,std::string objectId);

	map<string,string> _goodsInfo;  //用户物品数据
	void setGoodsCount(string id , string num);
	void alterGoodsCount(std::string id , const int num);
	int getGoodsCountByID(string id);
	

	map<int,map<int,int>> chapter; //用户过关记录
	void setChapterLog(int chapterNum,int sceneNum,int starNum);
	int getSceneStarCount(int chapterNum,int sceneNum);
	void setSceneStarCount(int chapterNum,int sceneNum,int starNum);

	int getReviveSweet();
	int getReviveSweetTime();

	void initPostData();

	//回合数数据
	CC_SYNTHESIZE(int,m_disTime,DisTime);
	CC_SYNTHESIZE(vector<int> ,m_enemy,Enemy);
	void dataResume();

	std::map<int,string> _takeUnitsID;

	void setTakeUnits(std::map<int,string> val){
		_takeUnitsID = val;
	};

	std::map<int,string> getTakeUnits(){
			return _takeUnitsID;
	};
	void takeUnitsInit();

	std::map<int,int> _typeWarLevel;
	int getWarLevelBySelfType(int selftype){return _typeWarLevel.at(selftype);};
	void setWarSelfTypeLevel(int selftype,int level){
		_typeWarLevel.at(selftype)=level;
	};

	//是否使用了柠檬糖，使造兵无CD，价格为固定值
	CC_SYNTHESIZE(bool,_isObjsNoCD,IsObjsNoCD);

	//保存像素偏移
	std::map<int,float> _pixelShift;

	//用于新手引导，防止按钮未成功点击，新手引导紊乱现象，称为界面锁
	CC_SYNTHESIZE(bool,_freshmanMutex,FreshManMutex);
	CC_SYNTHESIZE(bool,_isFreshMan,IsFreshMan);
	CC_SYNTHESIZE(int,_freshManCurrentID,FreshManCurrentID);
	CC_SYNTHESIZE(float,_storeyScale,StoreyScale);

	CC_SYNTHESIZE(int,_indexPost,IndexPost);           //标记跑过第几关(仅用于标记第四关，第五关)
	CC_SYNTHESIZE(int,_indexGuide,IndexGuide);         //标记跑过第几关教学
	CC_SYNTHESIZE(bool,_isFirstScene,IsFirstScene);    //是否进入第一关
	CC_SYNTHESIZE(bool,_isFourthScene,IsFourthScene);  //是否进入第四关
	CC_SYNTHESIZE(bool ,_isHeroGuide, IsHeroGuide);    //是否进入英雄屋引导
	CC_SYNTHESIZE(bool,_isGradeGuide,IsGradeGuide);    //是否进入科技树引导
	CC_SYNTHESIZE(int,_titlePos,TitlePos);

	//用于每日BOSS活动，保存服务器随机的Boss关卡 
	int getBoxEnemySceneBySection(int section);
	void setBoxEnemySceneBySection(int chapter_num,int section);
	CC_SYNTHESIZE(int,_boxEnemyDieMethod,BoxEnemyDieMethod);  //用于区分宝箱怪死的方式（0：表示没有宝箱怪 1：表示被打死 2：表示从门进入 3：表示逃跑 ，2,3都没奖励 ）

	//保存公主装备的时装ID
	CC_SYNTHESIZE(int,_clothesItemId,ClothesItemId);

	CC_SYNTHESIZE(string, _musicFile, MuiscFile);
public:
   /**
     *	@brief	 将数据从内存保存到外部文件中
     */
    void flush();
	/**
     *	@brief	 内存不足时调用
     */
    void purge();
	 /**
     *	@brief	 对Gold的操作
     *
     *	@param 	delta 	正负值均可
     */
    void alterGold(int delta);
    /**
     *	@brief	 重置游戏数据为默认数值
     */

	/**
     *	@brief	 对Sweet的操作
     *
     *	@param 	delta 	正负值均可
     */
    void alterSweet(int delta);


	/**
     *	@brief	 对HeroSoul的操作
     *
     *	@param 	delta 	正负值均可
     */
    void alterHeroSoul(int delta);

	void alterStarCount(int delta);
    /**
     *	@brief	 重置游戏数据为默认数值
     */
    void reset();
	void resetHp();
	void resetGameData();
	void resetSweet();

	int	getStarEgg();	// 获取进阶蛋数量  30001

	// 英雄数据处理
	void		setHeroKeyValue(int _id, int _key, int _value);		// 取所有英雄数据
	void		setHeroKeyValue_Float(int _id, int _key, float _value);

	int			getHeroKeyValue(int _id, int _key);					// 设置英雄数据
	float		getHeroKeyValue_Float(int _id, int _key);

	void		clearHeroValue_isDead_StoreyIndex();				// 清空英雄死亡状态 、 层级数据

	_HeroData	getHeroData(int _id);								// 取英雄数据
	vector<_HeroData>	getAllHeroData();							// 取英雄数据

	bool        isActivityTime();
	void setMusicLockValue(int v);//设置音乐的值
	void setEffectLockValue(int v);//设置音效的值

	std::map<int, int> mTemporaryMagic;		// 临时魔法
	void setTemporaryMagic(int _id, int _count);
	int  getTemporaryMagic(int _id);
	void alterTemporaryMagic(int _id, int _count);
	void clearTemporaryMagic();

	int getRandom(int _maxNumber);	// 取随机数

	void setMusicPlay(string _music);
	void resetMuiscPlay();
protected:
	UserRecord();
	~UserRecord();
	bool init();
	float _defaultHp;
	float _defaultSweet;

	vector<_HeroData> mHeroData;	// 从服务器过来的英雄数据
};

#endif /* defined(__UserRecord__UserRecord__) */

