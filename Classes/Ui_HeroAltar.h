#ifndef __UI_HEROALTAR_H__
#define __UI_HEROALTAR_H__

#include "GameConfig.h"
#include "Window.h"
#include "Connect.h"
#include "Ui_ProgressBar.h"

using namespace std;
using namespace network;

// 英雄选择格子信息
class HeroItemInfo
{
public:
	int			index;
	int			id;					// id						-- 服务器数据
	int			unLock;				// 是否解锁
	int			level;				// 等级
	int			star;				// 星级
	int			blessing;			// 祝福值
	int			skill1_level;		// 技能1等级
	int			skill2_level;		// 技能2等级
	int			skill3_level;		// 技能3等级
	float		successRate;		// 当前成功率
	int			deletTime;			// 祝福值清空时间间隔

	int			isDead;				// 是否死亡					-- 本地数据
	int			tempBlessing;		// 可变动的临时祝福值		-- 本地数据
	string		name;
	string		keyName;			// boss1.2.3.4.5名称
	string		iconName;			// 图标名称
	Widget*		item;				// widget控件

	int			successCost;		// 消耗数量
	int			failureCount;		// 失败次数
	int			reviveTime;			// 复活倒计时

	int			heroLevelUpCost;
	int			skillLevelUpCost1;
	int			skillLevelUpCost2;
	int			skillLevelUpCost3;
	int			heroUnLockType;
	int			heroUnLockCost;
	bool		isHeroUpStar;
};

class Ui_HeroAltar:public Widget
{
public:
	Ui_HeroAltar();
	~Ui_HeroAltar();
public:
	virtual bool init();
	CREATE_FUNC(Ui_HeroAltar);

	virtual void onEnter() override;
	virtual void onExit() override;

public:
	Widget* findChild(const char* _widgetName);

	void showView();
	void createView(bool _isAutoRemove);
	void hideView();

	Widget*	addHeroItem();
	void	updateHeroList();
	void	updateHeroView(int _listHeroIndex);
	void	updateHeroViewPage(int _pageIndex);
	void	updateHeroSkill(int _skillIndex);
	void	updateStar(int _starNum, const char* _name, bool _isBlue);

	void	onButtonEvent(Ref* sender, TouchEventType type);
	void	onListViewEventHero(Ref* sender, ListViewEventType type);
	void	onSliderEvent(Ref* sender, SliderEventType type);

	void	updateSlider(int _value);
	int		toPercentValue(int _value);
	int		toBlessingValue(int _value);
	void	updateNeedStarCount(int _toValue);

	void    fitScreen();
	void	setWidgetGray(Widget* _widget, bool _isGray, float _value = 0.03f);
	void	setViewIsClick(bool _isClick);
	
	// 数据计算
	int		getValueA(int _star);					// 取满祝福材料需求数量		A
	int		getValueB(int _star);					// 取当前进阶级原定的消耗进阶蛋数量		B
	float	getSuccessRate(int _star);				// 取星级成功率		成功率=(B/A)*100
	int		getFailureAddBlessing(int _star);		// 取失败增加的祝福值	1000/(A/B)
	float	getFailureAddSuccessRate(int _star);	// 取失败增加的成功率	(100%-成功率) / (A/B)
	int		getSlideAddProp(int _star);				// 拖动滑竿每次增加材料		A/10
	int		getSlideAddSuccessRate(int _star);		// 拖动滑竿每次增加成功率	(100%-成功率)/10


	// 按钮功能
	void buttonCallBack_ReviveSweet();				// 使用糖果复活
	void buttonCallBack_ReviveSoul();				// 使用英雄之混复活
	void buttonCallBack_UpHeroLevel();				// 升级英雄等级
	void buttonCallBack_UpHeroSkillLevel();			// 升级英雄技能等级
	void buttonCallBack_UnLockHero();				// 解锁英雄
	void buttonCallBack_UseBlessingItem();			// 使用祝福蛋
	void buttonCallBack_UpHeroStar();				// 升级英雄星级

	// 服务器回调
	void onHttpRequestCompleted_ReviveSoul(HttpClient *sender, HttpResponse *response);				// 英雄之混复活回调
	void onHttpRequestCompleted_UnLockHero(HttpClient *sender, HttpResponse *response);				// 解锁英雄回调
	void onHttpRequestCompleted_UpHeroLevel(HttpClient *sender, HttpResponse *response);			// 升级英雄等级回调
	void onHttpRequestCompleted_UpHeroSkillLevel(HttpClient *sender, HttpResponse *response);		// 升级英雄技能等级回调
	void onHttpRequestCompleted_UseBlessingItem(HttpClient *sender, HttpResponse *response);		// 使用祝福蛋回调
	void onHttpRequestCompleted_UpHeroStar_Success(HttpClient *sender, HttpResponse *response);		// 升级英雄星级_成功回调
	void onHttpRequestCompleted_UpHeroStar_Failure(HttpClient *sender, HttpResponse *response);		// 升级英雄星级_失败回调

	// 特效动画
	void beginAnim_Slider();						// Slider特效
	void endAnim_Slider(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);							
	void beginAnim_UpLevel(int _heroId);			// 升级升星级特效
	void endAnim_UpLevel(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	
	void beginAnim_UpSkillLevel(int _skillIndex);	// 技能升级特效
	void endAnim_UpSkillLevel(Ref *sender);	
	void beginAnim_UpStar();						// 升级星级特效
	void endAnim_UpStar(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	

	Animate* createAnimation(const char*FrameName,int max);
	void actionCallBack_ReviveSweet(Ref* sender);	// 复活倒计时回调

	void updateLevelUpTag();
	
	void updateRevive(float _time);	// 计时器更新
	Ui_ProgressBar* getReviveProgress();

	void buyHeroSuccessCallFuc(Ref* ref);
public:	
	Widget			*mMainWidget;

	bool				mIsAutoRemove;
	Point					mDefaultPos;
	vector<HeroItemInfo>	mHeroItems;

	Widget*				mRootPanel;
	ui::ListView*		mListViewHeroSelect;
	int					mCurListViewIndex;
	Button*				mButtonClose;

	Widget*				mPanelPageView1;
	Widget*				mPanelPageView2;
	int					mCurPageViewIndex;

	Button*				mButtonListUp;
	Button*				mButtonListDown;
	Button*				mButtonPage1;
	Button*				mButtonPage2;
	
	TextBMFont*			mTextLevel;
	TextBMFont*			mTextName;
	TextBMFont*				mTextStarUseMoney;
	TextBMFont*				mTextStarUseRmb;
	ImageView*			mImageHeroPhoto;
	ImageView*			mImageStar1;
	ImageView*			mImageStar2;
	ImageView*			mImageStar3;
	ImageView*			mImageStar4;
	ImageView*			mImageStar5;
	Button*				mButtonStarUseMoney;
	Button*				mButtonStarUseRmb;

	TextBMFont*				mTextHp1;
	TextBMFont*				mTextHp2;
	TextBMFont*				mTextAtk1;
	TextBMFont*				mTextAtk2;
	TextBMFont*				mTextDef1;
	TextBMFont*				mTextDef2;
	TextBMFont*				mTextCrit1;
	TextBMFont*				mTextCrit2;
	TextBMFont*				mTextUpLevelMoney;
	Button*				mButtonUpLevel;

	int					mCurSkillIndex;
	ImageView*			mImageSkillSelect;
	TextBMFont*			mTextSkillDesc;
	TextBMFont*			mTextSkillLevel1;
	TextBMFont*			mTextSkillLevel2;
	TextBMFont*			mTextSkillLevel3;
	ImageView*			mImageSkillIcon1;
	ImageView*			mImageSkillIcon2;
	ImageView*			mImageSkillIcon3;
	Button*				mButtonSkillUse;
	TextBMFont*			mTextSkillUseValue;

	Slider*				mSliderStar;
	ImageView*			mImageStarIcon1;
	ImageView*			mImageStarIcon2;
	ImageView*			mImageUpStar1;
	ImageView*			mImageUpStar2;
	ImageView*			mImageUpStar3;
	ImageView*			mImageUpStar4;
	ImageView*			mImageUpStar5;
	ImageView*			mImageUpStar2_1;
	ImageView*			mImageUpStar2_2;
	ImageView*			mImageUpStar2_3;
	ImageView*			mImageUpStar2_4;
	ImageView*			mImageUpStar2_5;
	Button*				mButtonUseStar;
	Button*				mButtonUseStarAdd;
	TextBMFont*				mTextUseStarValue;

	ImageView*			mImageUnLockIcon;
	TextBMFont*				mTextUnLockValue;
	
	Widget*				mPanelRevive;
	Button*				mButtonUnLock;
	ImageView*			mImageUnLockDescBack;
	TextBMFont*			mBitmapLabelUnLockDesc;
	Text*				mTextStarUseShowText;
	Widget*				mPanelZheZhao;


	Button*				mButtonZhuFuTang;
	TextBMFont*				mTextZhuFuTangValue;
	Text*				mTextZhuFuValue;
	std::string			mDescZhuFuValue;

	int					mCurUpStarRate;
	int					mCurUpStarCostCount;
	int					mCurUpStarFailureCount;

	bool				mIsViewClick;
	
	TextBMFont*			mBmFontCurStar;
	TextBMFont*			mBmFontNextStar;

	// 是否播放滑块条动画
	bool				mIsSliderAnim;	
	Armature*			mSliderArmature;
	Armature*			mLevelArmature;
	Sprite*				mSkillLevelSprite;
	LoadingBar*			mLoadingBarRevive;
	Armature*			mStarArmature;
};



#endif // !__UI_HEROALTAR_H__
