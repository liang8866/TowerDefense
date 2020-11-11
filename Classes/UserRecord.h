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
	CC_SYNTHESIZE(int , _gold, Gold); //���
	CC_SYNTHESIZE(int , _maxGold, maxGold); //�����
	CC_SYNTHESIZE(int , _sweet, Sweet); //�ǹ���
	CC_SYNTHESIZE(int , _maxSweet, MaxSweet); //����ǹ���
	CC_SYNTHESIZE(int , _heroSoul, HeroSoul); //Ӣ��֮��
	CC_SYNTHESIZE(int , _advance_egg, AdvanceEgg); //���׵�
	CC_SYNTHESIZE(int , _wish_egg, WishEgg); //ף����
	CC_SYNTHESIZE(int , _CoolieCount, CoolieCount); //��������λ��
	CC_SYNTHESIZE(int , _hp , HP); //����ֵ
	CC_SYNTHESIZE(bool, _isBeginner, IsBeginner); 
	CC_SYNTHESIZE(bool, _isLogin, IsLogin); 
	CC_SYNTHESIZE(int , _uid, Uid);	 //�û�ID	
	CC_SYNTHESIZE(std::string , _uUid, Uuid); //�ֻ�ID	
	CC_SYNTHESIZE(int, _loginDayNum, LoginDayNum);  //������¼����
	CC_SYNTHESIZE(bool, _isAward, IsAward); //�Ƿ���ȡ�˵�¼����
	CC_SYNTHESIZE(int, _platform, Platform); //ƽ̨ID
	CC_SYNTHESIZE(int, _modifytime, ModifyTime);  //�������ʱ��
	CC_SYNTHESIZE(int, _lastlogin, LastLogin);  //���һ�ε�¼ʱ��
	CC_SYNTHESIZE(int, _birthday, Birthday); //ע��ʱ��
	CC_SYNTHESIZE(std::string, _seid, Seid); //session_id
	CC_SYNTHESIZE(int , _round, Round);       //�غ�
	CC_SYNTHESIZE(int , _maxRound, MaxRound);    //���غ���
	CC_SYNTHESIZE(bool , _isGameStart, IsGameStart);  //��Ϸ�Ƿ�ʼ
	CC_SYNTHESIZE(bool , _isGameOver, IsGameOver);  //��Ϸ�Ƿ����
	CC_SYNTHESIZE(bool , _isGamePause, IsGamePause);  //��Ϸ�Ƿ���ͣ
	CC_SYNTHESIZE(int , _sectionNum, SectionNum);  //��ǰ��Ϸ�½�
	CC_SYNTHESIZE(int , _sceneNum, SceneNum);		//��ǰ��Ϸ�ؿ�
	CC_SYNTHESIZE(int , _seatCount, SeatCount);    //Я����λ��
	CC_SYNTHESIZE(int , _starCount, StarCount);    //������
	CC_SYNTHESIZE(bool , _isEvent104, IsEvent104);  //�Ƿ�104event ������߹ؿ�
	
	std::string BulidUUID(); //�����û�ΨһID;

	std::map<std::string,std::map<std::string,int>> _upgrades;  //�û��츳����
	std::map<std::string,std::map<std::string,int>> getAllUpgrades(); //ȡ�����츳����

	int getUpgradesItemLevel(std::string type,std::string itemid); //ȡ����������ĳ��Ŀ�ȼ� �� type=tech itemid=item1
	float getUpgradesItemAllAddValue(std::string type,std::string itemid);//ֻ�Լ��ٻ���������ͬ���Ե�����
	void setUpgradesItemLevel(std::string type,std::string itemid,const int); //����ĳһ��Ŀ�ȼ�
	void removeUpgradesType(std::string type); //ɾ��ĳһ��������������
	float getUpgradesItemIsSelect(std::string type, std::string itemid, int _itemLevel);

	//ÿ�����������

	CC_SYNTHESIZE(int,_musicLock,musicLock);//�����Ƿ�����
	CC_SYNTHESIZE(int,_effectLock,effectLock);//��Ч�Ƿ�����
	map<int,int> _randomScene;
	void setRandomScene(const int chapter_id,const int scene_id);
	int getRandomScene(const int chapter_id);


	map<int,int> _goodsLimit; //������Ʒ����
	void setGoodsLimit(const int goodsId ,const int limitCount);
	int getGoodsLimit(const int goodsId);
	void alterGooldLimit(const int goodsId ,const int limitCount);


	map<string,string> _objects;
	bool getObjectsIsUnlock(std::string objectKey);
	void setObjectsUnLock(std::string objectKey,std::string objectId);

	map<string,string> _goodsInfo;  //�û���Ʒ����
	void setGoodsCount(string id , string num);
	void alterGoodsCount(std::string id , const int num);
	int getGoodsCountByID(string id);
	

	map<int,map<int,int>> chapter; //�û����ؼ�¼
	void setChapterLog(int chapterNum,int sceneNum,int starNum);
	int getSceneStarCount(int chapterNum,int sceneNum);
	void setSceneStarCount(int chapterNum,int sceneNum,int starNum);

	int getReviveSweet();
	int getReviveSweetTime();

	void initPostData();

	//�غ�������
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

	//�Ƿ�ʹ���������ǣ�ʹ�����CD���۸�Ϊ�̶�ֵ
	CC_SYNTHESIZE(bool,_isObjsNoCD,IsObjsNoCD);

	//��������ƫ��
	std::map<int,float> _pixelShift;

	//����������������ֹ��ťδ�ɹ���������������������󣬳�Ϊ������
	CC_SYNTHESIZE(bool,_freshmanMutex,FreshManMutex);
	CC_SYNTHESIZE(bool,_isFreshMan,IsFreshMan);
	CC_SYNTHESIZE(int,_freshManCurrentID,FreshManCurrentID);
	CC_SYNTHESIZE(float,_storeyScale,StoreyScale);

	CC_SYNTHESIZE(int,_indexPost,IndexPost);           //����ܹ��ڼ���(�����ڱ�ǵ��Ĺأ������)
	CC_SYNTHESIZE(int,_indexGuide,IndexGuide);         //����ܹ��ڼ��ؽ�ѧ
	CC_SYNTHESIZE(bool,_isFirstScene,IsFirstScene);    //�Ƿ�����һ��
	CC_SYNTHESIZE(bool,_isFourthScene,IsFourthScene);  //�Ƿ������Ĺ�
	CC_SYNTHESIZE(bool ,_isHeroGuide, IsHeroGuide);    //�Ƿ����Ӣ��������
	CC_SYNTHESIZE(bool,_isGradeGuide,IsGradeGuide);    //�Ƿ����Ƽ�������
	CC_SYNTHESIZE(int,_titlePos,TitlePos);

	//����ÿ��BOSS�����������������Boss�ؿ� 
	int getBoxEnemySceneBySection(int section);
	void setBoxEnemySceneBySection(int chapter_num,int section);
	CC_SYNTHESIZE(int,_boxEnemyDieMethod,BoxEnemyDieMethod);  //�������ֱ�������ķ�ʽ��0����ʾû�б���� 1����ʾ������ 2����ʾ���Ž��� 3����ʾ���� ��2,3��û���� ��

	//���湫��װ����ʱװID
	CC_SYNTHESIZE(int,_clothesItemId,ClothesItemId);

	CC_SYNTHESIZE(string, _musicFile, MuiscFile);
public:
   /**
     *	@brief	 �����ݴ��ڴ汣�浽�ⲿ�ļ���
     */
    void flush();
	/**
     *	@brief	 �ڴ治��ʱ����
     */
    void purge();
	 /**
     *	@brief	 ��Gold�Ĳ���
     *
     *	@param 	delta 	����ֵ����
     */
    void alterGold(int delta);
    /**
     *	@brief	 ������Ϸ����ΪĬ����ֵ
     */

	/**
     *	@brief	 ��Sweet�Ĳ���
     *
     *	@param 	delta 	����ֵ����
     */
    void alterSweet(int delta);


	/**
     *	@brief	 ��HeroSoul�Ĳ���
     *
     *	@param 	delta 	����ֵ����
     */
    void alterHeroSoul(int delta);

	void alterStarCount(int delta);
    /**
     *	@brief	 ������Ϸ����ΪĬ����ֵ
     */
    void reset();
	void resetHp();
	void resetGameData();
	void resetSweet();

	int	getStarEgg();	// ��ȡ���׵�����  30001

	// Ӣ�����ݴ���
	void		setHeroKeyValue(int _id, int _key, int _value);		// ȡ����Ӣ������
	void		setHeroKeyValue_Float(int _id, int _key, float _value);

	int			getHeroKeyValue(int _id, int _key);					// ����Ӣ������
	float		getHeroKeyValue_Float(int _id, int _key);

	void		clearHeroValue_isDead_StoreyIndex();				// ���Ӣ������״̬ �� �㼶����

	_HeroData	getHeroData(int _id);								// ȡӢ������
	vector<_HeroData>	getAllHeroData();							// ȡӢ������

	bool        isActivityTime();
	void setMusicLockValue(int v);//�������ֵ�ֵ
	void setEffectLockValue(int v);//������Ч��ֵ

	std::map<int, int> mTemporaryMagic;		// ��ʱħ��
	void setTemporaryMagic(int _id, int _count);
	int  getTemporaryMagic(int _id);
	void alterTemporaryMagic(int _id, int _count);
	void clearTemporaryMagic();

	int getRandom(int _maxNumber);	// ȡ�����

	void setMusicPlay(string _music);
	void resetMuiscPlay();
protected:
	UserRecord();
	~UserRecord();
	bool init();
	float _defaultHp;
	float _defaultSweet;

	vector<_HeroData> mHeroData;	// �ӷ�����������Ӣ������
};

#endif /* defined(__UserRecord__UserRecord__) */

