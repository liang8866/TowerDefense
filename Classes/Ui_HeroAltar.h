#ifndef __UI_HEROALTAR_H__
#define __UI_HEROALTAR_H__

#include "GameConfig.h"
#include "Window.h"
#include "Connect.h"
#include "Ui_ProgressBar.h"

using namespace std;
using namespace network;

// Ӣ��ѡ�������Ϣ
class HeroItemInfo
{
public:
	int			index;
	int			id;					// id						-- ����������
	int			unLock;				// �Ƿ����
	int			level;				// �ȼ�
	int			star;				// �Ǽ�
	int			blessing;			// ף��ֵ
	int			skill1_level;		// ����1�ȼ�
	int			skill2_level;		// ����2�ȼ�
	int			skill3_level;		// ����3�ȼ�
	float		successRate;		// ��ǰ�ɹ���
	int			deletTime;			// ף��ֵ���ʱ����

	int			isDead;				// �Ƿ�����					-- ��������
	int			tempBlessing;		// �ɱ䶯����ʱף��ֵ		-- ��������
	string		name;
	string		keyName;			// boss1.2.3.4.5����
	string		iconName;			// ͼ������
	Widget*		item;				// widget�ؼ�

	int			successCost;		// ��������
	int			failureCount;		// ʧ�ܴ���
	int			reviveTime;			// �����ʱ

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
	
	// ���ݼ���
	int		getValueA(int _star);					// ȡ��ף��������������		A
	int		getValueB(int _star);					// ȡ��ǰ���׼�ԭ�������Ľ��׵�����		B
	float	getSuccessRate(int _star);				// ȡ�Ǽ��ɹ���		�ɹ���=(B/A)*100
	int		getFailureAddBlessing(int _star);		// ȡʧ�����ӵ�ף��ֵ	1000/(A/B)
	float	getFailureAddSuccessRate(int _star);	// ȡʧ�����ӵĳɹ���	(100%-�ɹ���) / (A/B)
	int		getSlideAddProp(int _star);				// �϶�����ÿ�����Ӳ���		A/10
	int		getSlideAddSuccessRate(int _star);		// �϶�����ÿ�����ӳɹ���	(100%-�ɹ���)/10


	// ��ť����
	void buttonCallBack_ReviveSweet();				// ʹ���ǹ�����
	void buttonCallBack_ReviveSoul();				// ʹ��Ӣ��֮�츴��
	void buttonCallBack_UpHeroLevel();				// ����Ӣ�۵ȼ�
	void buttonCallBack_UpHeroSkillLevel();			// ����Ӣ�ۼ��ܵȼ�
	void buttonCallBack_UnLockHero();				// ����Ӣ��
	void buttonCallBack_UseBlessingItem();			// ʹ��ף����
	void buttonCallBack_UpHeroStar();				// ����Ӣ���Ǽ�

	// �������ص�
	void onHttpRequestCompleted_ReviveSoul(HttpClient *sender, HttpResponse *response);				// Ӣ��֮�츴��ص�
	void onHttpRequestCompleted_UnLockHero(HttpClient *sender, HttpResponse *response);				// ����Ӣ�ۻص�
	void onHttpRequestCompleted_UpHeroLevel(HttpClient *sender, HttpResponse *response);			// ����Ӣ�۵ȼ��ص�
	void onHttpRequestCompleted_UpHeroSkillLevel(HttpClient *sender, HttpResponse *response);		// ����Ӣ�ۼ��ܵȼ��ص�
	void onHttpRequestCompleted_UseBlessingItem(HttpClient *sender, HttpResponse *response);		// ʹ��ף�����ص�
	void onHttpRequestCompleted_UpHeroStar_Success(HttpClient *sender, HttpResponse *response);		// ����Ӣ���Ǽ�_�ɹ��ص�
	void onHttpRequestCompleted_UpHeroStar_Failure(HttpClient *sender, HttpResponse *response);		// ����Ӣ���Ǽ�_ʧ�ܻص�

	// ��Ч����
	void beginAnim_Slider();						// Slider��Ч
	void endAnim_Slider(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);							
	void beginAnim_UpLevel(int _heroId);			// �������Ǽ���Ч
	void endAnim_UpLevel(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	
	void beginAnim_UpSkillLevel(int _skillIndex);	// ����������Ч
	void endAnim_UpSkillLevel(Ref *sender);	
	void beginAnim_UpStar();						// �����Ǽ���Ч
	void endAnim_UpStar(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex);	

	Animate* createAnimation(const char*FrameName,int max);
	void actionCallBack_ReviveSweet(Ref* sender);	// �����ʱ�ص�

	void updateLevelUpTag();
	
	void updateRevive(float _time);	// ��ʱ������
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

	// �Ƿ񲥷Ż���������
	bool				mIsSliderAnim;	
	Armature*			mSliderArmature;
	Armature*			mLevelArmature;
	Sprite*				mSkillLevelSprite;
	LoadingBar*			mLoadingBarRevive;
	Armature*			mStarArmature;
};



#endif // !__UI_HEROALTAR_H__
