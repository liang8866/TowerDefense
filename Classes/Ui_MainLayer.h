#ifndef __UI_MAINLAYER_H__
#define __UI_MAINLAYER_H__ 


#include "cocos2d.h"
#include "Window.h"
#include "GameConfig.h"
#include "bossObjects.h"


class ItemInfo
{
public:
	ItemInfo();
	~ItemInfo();
public:
	string unit_id;      
	float totalCDtime;
	float leftCDtime;
	int   price;
	int   objType;
	int   selfType;
};

class Ui_MainLayer : public Window
{

public:
	Ui_MainLayer();
	~Ui_MainLayer();

	static Ui_MainLayer * create(const char *fileName);
	virtual bool initialise(const char* fileName);

	void initData();

	void eventTouchUiButton(Ref* sender, TouchEventType type);      //处理暂停，商店按钮点击事件
	void eventTouchMagicButton(Ref* sender, TouchEventType type);   //处理魔法按钮点击事件  
	void eventTouchItemObjButton(Ref* sender, TouchEventType type); //处理条目内对象点击事件

	void hideMagics();                                 //隐藏魔法
	void showMagics();                                 //显示魔法

	void updateItemObjs();                             //更新条目数据

	void setRoundNum(int currRound , int totalRound); //设置回合数
	void setLifeNum(int life);                        //设置生命值 
	void setSweetNum(int addsweet , int limit);            //设置糖果数
	void setDynamicSweetNum(int sweet , Point pos);               //设置糖果数　+　动态效果
	void setSoulNum(int soul);                        //设置英雄之魂数量
	void setMagicNum(int index ,int num);             //设置魔法数量
	void setItemObjPrice(Widget *obj ,int price);     //设置菜单条目内对象价格
	void updateAllObjsPrice();                        //更新所有条目内所有对象价格
	void createObjCost(Ref *obj, int _indexId = 0);  //创建单位成功后需要扣除对应的糖果

	void updateObjsIcon();                            //更新条目上的Icon
	void FindObjToNoCd(int oType,int sType);
	void hideProgress(Ref *pSender);
	void showProgress(Ref *pSender, int _indexId = 0);
	void progressActionCallFunc(Ref *pSender,const int key);

	void showHeroInfo(int selfType,int Level,bossObjects *obj);
	void hideHeroInfo();
	void setHeroHp(int hp,int maxHp);
	void eventTouchHeroBtn(Ref* sender, TouchEventType type); //处理英雄头像点击事件
	void bossCdPrgCallfunc(Ref *sender);//冷却技能进度条的回调

	void onFrameEvent(Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);
	//门被打的时候的显示
	Sprite *waring;
	void SetAtkDoorWarning();
    void WarningCallback(Ref *f);

	int getSweetNum(){                         //获取糖果数 
		return m_sweet;
	};                  
	int getMaxSweet(){                         //获取糖果最大限制 
		return m_totalSweet;
	};

	int getCurrRound(){                        //获取当前波次
		return m_round;
	};
	int getMaxRound(){                         //获取最大波次
		return m_totalRound;                    
	}

	int getLifeNum(){                          //获取生命值HP
		return m_life;     
	}

	int getSoulNum(){                          //获取英雄灵魂
		return m_soul;         
	}

	void enterObjsNoCDstatus();               //进入造兵无CD状态
	void leaveObjsNoCDstatus();               //离开造兵无CD状态
	Animate* noCDeffect();                     //造兵无CD特效

	void update(float dt);

	ItemInfo& getItemInfoByID(int id); 
	void fitScreen();
	
	void changePrice(Ref *obj);                        //单位升级后造价更改 

	void myPauseAllAction(Ref *obj); //暂停
	void myResumeAllAction(Ref *obj);//取消暂停

	// 魔法释放监听
	EventListenerTouchOneByOne* _magicListener;
	void createMagicListenerTouch();
	void removeMagicListenerTouch();
	bool onMagicTouchBegan(Touch* _touch, Event* _unusedEvent);  
	void onMagicTouchMove(Touch* _touch, Event* _unusedEvent);
	void onMagicTouchEnd(Touch* _touch, Event* _unusedEvent);

	void updateYuJing(bool isVisible = false, bool isUp = false);

	void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);

	void setItemSelect(int _index, bool _isHideAll = false);	// 设置选中

	void eventTouch_Debug(Ref* sender, TouchEventType type);
public:
	std::map<int,ItemInfo> m_itemInfo;  //存储菜单栏对象的信息
	int noCDprice;
	

	static const int nextRoundArrivedTime = 10;

	int _disTime;
	//暂定计时
	int _lastKey;
	Map<int,ProgressTimer*> loadBars;


	void setPauseBtnGray();//把暂停按钮灰掉
	void removePauseBtnGray();//暂停按钮恢复
public:
	Widget*     currPanel;
	Armature*   _armature;
	Button*	    m_PauseBtn;
	Button*     m_ShopBtn;
	//20001攻击    20002 变羊  20003 传送门   20004 哥斯拉
	Button*     HeroBtn;
	Button*     m_AddMagicBtn;
	Button*     m_MagicBtn_1;//20001攻击  
	Button*     m_MagicBtn_2;//20002 变羊 
	Button*     m_MagicBtn_3;//20003 传送门
	Button*     m_MagicBtn_4;//20004 哥斯拉
	Widget*     magic_bk; 
	ImageView*  _soulIcon;

	ProgressTimer *skillPrg;//技能冷却动画进度条
	bossObjects *targetBoss;//英雄对象

	int         m_sweet;
	int         m_totalSweet;
	int         m_round;
	int         m_totalRound;
	int         m_life;
	int         m_soul;

public://魔法技能处理
    

	int magicIndex;// 是释放第几个技能'
	void reduceOneMagic(std::string id,Button *btn);
	void refreshMagic(std::string id,Button *btn = nullptr);
	
	void magic1SkillAction(Point pos,int floor);//20001攻击 
	void magic2SkillAction(Point pos,int floor);//20002 变羊
	void magic3SkillAction(Point pos,int floor);// 20003 传送门
	void magic4SkillAction(Point pos,int floor);//20004 哥斯拉

	//20001攻击    20002 变羊  20003 传送门   20004 哥斯拉

	void magicCdAction(Button* btn,float cdtime);
	void magicCdCallback(Ref *b);

	float m_magicCdTime_1;//20001 攻击冷却时间
	float m_magicCdTime_2;//20002 变羊冷却时间
	float m_magicCdTime_3;//20003 传送门却时间
	float m_magicCdTime_4;//20004 哥斯拉冷却时间
	void cdBtnProgess(Button*btn,int isPause);

	bool  skillCDstatus;

private:
	Point clickPos;
	int   clickStoreyIndex;
};


#endif // !__UI_MAINLAYER_H__
