#include "Ui_HeroAltar.h"
#include "PlistManager.h"
#include "UserRecord.h"
#include "WindowManager.h"
#include "Ui_MainBack.h"
#include "Ui_Prompt.h"
#include "headstoneObjects.h"
#include "Ui_ProgressBar.h"
#include "audioPlayer.h"
#include "Ui_SwallowAllTouchEvents.h"

string DefaultStarString = "";
Ui_HeroAltar::Ui_HeroAltar(void)
{
	audioPlayer::getInstance()->pauseAllEffects();
	NotificationCenter::getInstance()->postNotification(PAUSEKEY,(Ref*)0);
}
Ui_HeroAltar::~Ui_HeroAltar(void)
{
	NotificationCenter::getInstance()->postNotification(RESUMEKEY, (Ref*)0);
	audioPlayer::getInstance()->resumeAllEffects();
	NotificationCenter::getInstance()->removeObserver(this,BUYHEROSUCCESS);
}

bool Ui_HeroAltar::init()
{
	if (!Widget::init())
		return false;

	mIsViewClick = true;
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Ui_HeroAltar/GameUI0.plist","Ui_HeroAltar/GameUI0.png");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("HeroAltar_HeroItem/GameUI0.plist","HeroAltar_HeroItem/GameUI0.png");
	mMainWidget  = static_cast<Widget*>(GUIReader::getInstance()->widgetFromJsonFile("Ui_HeroAltar/Ui_HeroAltar.ExportJson"));
	this->setZOrder(6);
	mHeroItems.clear();
	mCurListViewIndex = 0;
	mCurPageViewIndex = 1;
	mCurSkillIndex = 0;
	mRootPanel = findChild("Panel_1286");

	this->setVisible(false);
	mRootPanel->setTouchEnabled(false);
	mIsAutoRemove = false;
	mIsSliderAnim = false;
	mSliderArmature = nullptr;
	mLevelArmature = nullptr;
	mSkillLevelSprite = nullptr;
	mStarArmature = nullptr;

	// -- 
	mButtonListUp	= static_cast<Button*>(findChild("Button_Up"));
	mButtonListDown	= static_cast<Button*>(findChild("Button_Down"));
	mButtonPage1	= static_cast<Button*>(findChild("Button_A"));
	mButtonPage2	= static_cast<Button*>(findChild("Button_B"));
	mButtonListUp->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonListDown->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonPage1->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonPage2->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mListViewHeroSelect = static_cast<ListView*>(findChild("ListView_HeroSelect"));
	mListViewHeroSelect->addEventListenerListView(this, listvieweventselector(Ui_HeroAltar::onListViewEventHero));

	mButtonClose	= static_cast<Button*>(findChild("Button_Close"));	
	mButtonClose->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	// -- 
	mPanelPageView1	= findChild("Panel_Page1");
	mPanelPageView2 = findChild("Panel_Page2");

	mImageUnLockIcon = static_cast<ImageView*>(findChild("Image_UnLockIcon"));
	mTextUnLockValue = static_cast<TextBMFont*>(findChild("Text_UnLockValue"));
	
	// -- 
	mTextLevel	= static_cast<TextBMFont*>(findChild("Text_Level"));
	mTextName	= static_cast<TextBMFont*>(findChild("Text_Name"));
	mTextStarUseMoney = static_cast<TextBMFont*>(findChild("Text_UseMoneyValue"));
	mTextStarUseRmb = static_cast<TextBMFont*>(findChild("Text_UseRmbValue"));
	mImageHeroPhoto	= static_cast<ImageView*>(findChild("Image_HeroImage"));
	mImageStar1	= static_cast<ImageView*>(findChild("Image_Star1"));
	mImageStar2	= static_cast<ImageView*>(findChild("Image_Star2"));
	mImageStar3	= static_cast<ImageView*>(findChild("Image_Star3"));
	mImageStar4	= static_cast<ImageView*>(findChild("Image_Star4"));
	mImageStar5	= static_cast<ImageView*>(findChild("Image_Star5"));
	mButtonStarUseMoney	= static_cast<Button*>(findChild("Button_StarUseMoney"));
	mButtonStarUseRmb	= static_cast<Button*>(findChild("Button_StarUseRmb"));
	mButtonStarUseMoney->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonStarUseRmb->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	mTextHp1			= static_cast<TextBMFont*>(findChild("Text_Hp1"));
	mTextHp2			= static_cast<TextBMFont*>(findChild("Text_Hp2"));
	mTextAtk1			= static_cast<TextBMFont*>(findChild("Text_Atk1"));
	mTextAtk2			= static_cast<TextBMFont*>(findChild("Text_Atk2"));
	mTextDef1			= static_cast<TextBMFont*>(findChild("Text_Def1"));
	mTextDef2			= static_cast<TextBMFont*>(findChild("Text_Def2"));
	mTextCrit1			= static_cast<TextBMFont*>(findChild("Text_Crit1"));
	mTextCrit2			= static_cast<TextBMFont*>(findChild("Text_Crit2"));
	mTextUpLevelMoney	= static_cast<TextBMFont*>(findChild("Text_UpLevelMoney"));
	mButtonUpLevel		= static_cast<Button*>(findChild("Button_UpLevel"));
	mButtonUpLevel->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	mImageSkillSelect	= static_cast<ImageView*>(findChild("Image_SkillSelect"));
	mTextSkillDesc		= static_cast<TextBMFont*>(findChild("Text_SkillDesc"));
	mTextSkillLevel1	= static_cast<TextBMFont*>(findChild("Text_SkillLevel1"));
	mTextSkillLevel2	= static_cast<TextBMFont*>(findChild("Text_SkillLevel2"));
	mTextSkillLevel3	= static_cast<TextBMFont*>(findChild("Text_SkillLevel3"));
	mImageSkillIcon1	= static_cast<ImageView*>(findChild("Image_SkillIcon1"));
	mImageSkillIcon2	= static_cast<ImageView*>(findChild("Image_SkillIcon2"));
	mImageSkillIcon3	= static_cast<ImageView*>(findChild("Image_SkillIcon3"));
	mButtonSkillUse		= static_cast<Button*>(findChild("Button_SkillUse"));
	mTextSkillUseValue	= static_cast<TextBMFont*>(findChild("Text_SkillUseValue"));
	mImageSkillIcon1->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mImageSkillIcon2->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mImageSkillIcon3->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonSkillUse->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	mSliderStar = static_cast<Slider*>(findChild("Slider_Star"));
	mImageStarIcon1	= static_cast<ImageView*>(findChild("Image_StarIcon1"));
	mImageStarIcon2	= static_cast<ImageView*>(findChild("Image_StarIcon2"));

	mImageUpStar1 = static_cast<ImageView*>(findChild("Image_UpStar1"));
	mImageUpStar2 = static_cast<ImageView*>(findChild("Image_UpStar2"));
	mImageUpStar3 = static_cast<ImageView*>(findChild("Image_UpStar3"));
	mImageUpStar4 = static_cast<ImageView*>(findChild("Image_UpStar4"));
	mImageUpStar5 = static_cast<ImageView*>(findChild("Image_UpStar5"));
	mImageUpStar2_1 = static_cast<ImageView*>(findChild("Image_UpStar2_1"));
	mImageUpStar2_2 = static_cast<ImageView*>(findChild("Image_UpStar2_2"));
	mImageUpStar2_3 = static_cast<ImageView*>(findChild("Image_UpStar2_3"));
	mImageUpStar2_4 = static_cast<ImageView*>(findChild("Image_UpStar2_4"));
	mImageUpStar2_5 = static_cast<ImageView*>(findChild("Image_UpStar2_5"));

	mButtonUseStar = static_cast<Button*>(findChild("Button_UseStar"));
	mButtonUseStarAdd = static_cast<Button*>(findChild("Button_UseStarAdd"));
	mTextUseStarValue = static_cast<TextBMFont*>(findChild("Text_UseStarValue"));
	mButtonUseStar->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mButtonUseStarAdd->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	mPanelRevive = static_cast<Widget*>(findChild("Panel_Revive"));
	mPanelRevive->setVisible(false);

	mButtonUnLock = static_cast<Button*>(findChild("Button_UnLock"));
	mButtonUnLock->setVisible(false);
	mButtonUnLock->setTouchEnabled(false);
	mButtonUnLock->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));
	mImageUnLockDescBack = static_cast<ImageView*>(findChild("Image_UnLockDescBack"));
	mBitmapLabelUnLockDesc = static_cast<TextBMFont*>(findChild("BitmapLabel_UnLockDesc"));
	mImageUnLockDescBack->setVisible(false);

	mTextStarUseShowText = static_cast<Text*>(findChild("Image_StarUseShowText"));
	mPanelZheZhao = static_cast<Widget*>(findChild("Panel_ZheZhao"));
	mPanelZheZhao->setVisible(false);

	mTextSkillLevel1 = static_cast<TextBMFont*>(findChild("Text_SkillLevel1"));
	mTextSkillLevel2 = static_cast<TextBMFont*>(findChild("Text_SkillLevel2"));
	mTextSkillLevel3 = static_cast<TextBMFont*>(findChild("Text_SkillLevel3"));

	mButtonZhuFuTang = static_cast<Button*>(findChild("Button_ZFtang"));
	mTextZhuFuTangValue = static_cast<TextBMFont*>(findChild("Text_ZhuFuTangValue"));
	mButtonZhuFuTang->addTouchEventListener(this, toucheventselector(Ui_HeroAltar::onButtonEvent));

	mTextZhuFuValue = static_cast<Text*>(findChild("Text_ZhuFuValue"));
	mDescZhuFuValue = mTextZhuFuValue->getStringValue();

	mSliderStar->addEventListenerSlider(this, sliderpercentchangedselector(Ui_HeroAltar::onSliderEvent));
	this->addChild(mMainWidget);

	mBmFontCurStar = static_cast<TextBMFont*>(findChild("BitmapLabel_CurStar"));
	mBmFontNextStar = static_cast<TextBMFont*>(findChild("BitmapLabel_NextStar"));
	DefaultStarString = mBmFontCurStar->getStringValue();

	mLoadingBarRevive = static_cast<LoadingBar*>(findChild("LoadingBar_Revive"));
	
	Widget *bg = findChild("panel_heroAltar");
	bg->setAnchorPoint(Point::ANCHOR_MIDDLE);
	bg->setPosition(Point(mMainWidget->getSize().width/2,mMainWidget->getSize().height/2));
	bg->setScale(bg->getScale()*0.8f);
	bg->runAction(Sequence::create(ScaleBy::create(0.1f,1.02/0.8f),ScaleBy::create(0.05f,1.0/1.02f),NULL));

	this->schedule(schedule_selector(Ui_HeroAltar::updateRevive),(float)1.0/60);
	fitScreen();

	//��ӹ㲥
	NotificationCenter::getInstance()->addObserver( this,
													callfuncO_selector(Ui_HeroAltar::buyHeroSuccessCallFuc),
													BUYHEROSUCCESS,
													NULL);
	return true;
}

void Ui_HeroAltar::onEnter()
{
	Widget::onEnter();
}

void Ui_HeroAltar::onExit()
{
	Widget::onExit();
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("Ui_HeroAltar/GameUI0.plist");
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("HeroAltar_HeroItem/GameUI0.plist");
	TextureCache::getInstance()->removeTextureForKey("Ui_HeroAltar/GameUI0.png");
	TextureCache::getInstance()->removeTextureForKey("HeroAltar_HeroItem/GameUI0.png");
}

// ��ʾ
void Ui_HeroAltar::showView()
{
	updateHeroList();
	// Ĭ��ѡ�е�һ��
	updateHeroView(0);
	
	//mMainWidget->setPositionY(mDefaultPos.y);
	this->setVisible(true);
	mRootPanel->setTouchEnabled(true);

	//Director::getInstance()->pause();
	audioPlayer::getInstance()->pauseAllEffects();
	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,(Ref*)0);
}
// ��ʾ
void Ui_HeroAltar::createView(bool _isAutoRemove)
{
	updateHeroList();
	// Ĭ��ѡ�е�һ��
	updateHeroView(0);

	//mMainWidget->setPositionY(mDefaultPos.y);
	this->setVisible(true);
	mRootPanel->setTouchEnabled(true);
	mIsAutoRemove = true;

	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,(Ref*)0);
}
// ����
void Ui_HeroAltar::hideView()
{
	//mMainWidget->setPositionY(-2000);
	this->setVisible(false);
	mRootPanel->setTouchEnabled(false);
	//Director::getInstance()->resume();
	NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, (Ref*)0);
	audioPlayer::getInstance()->resumeAllEffects();

	//֪ͨui_sceneselect���� touch �¼�
	NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
	if (mIsAutoRemove)
	{
		removeFromParentAndCleanup(true);
	}
}
// ����listͷ��
void Ui_HeroAltar::updateHeroList()
{
	// ��ɾ�����е�����
	mListViewHeroSelect->removeAllItems();
	mHeroItems.clear();
	// �ӱ����б���ȡ�����е�Ӣ��
	auto _curPlist = PlistManager::getInstance()->getPlistMap("BossData");
	auto _curArray = _curPlist->allKeys();
	for (size_t i=0; i!=_curPlist->count(); ++i)
	{
		int _curTypeIndex = i + 1;
		auto _curName = StringUtils::format("boss%d", _curTypeIndex);
		auto _curNameObject = PlistManager::getInstance()->getPlistObject(_curPlist, _curName.c_str());
		if (_curNameObject)
		{
			addHeroItem();
			Widget* _itemCell = mListViewHeroSelect->getItem(i);
			if (_itemCell)
			{
				HeroItemInfo _thisItemData;
				_thisItemData.index = i;
				_thisItemData.id = _curTypeIndex;
				auto _haveData = UserRecord::getInstance()->getHeroData(_thisItemData.id);
				_thisItemData.keyName = _curName;
				if (_haveData.id > 0)
				{
					// ������
					_thisItemData.unLock		= _haveData.unLock;
					_thisItemData.level			= _haveData.level;
					_thisItemData.star			= _haveData.star;
					_thisItemData.blessing		= _haveData.blessing;
					_thisItemData.skill1_level	= _haveData.skill1_level;
					_thisItemData.skill2_level	= _haveData.skill2_level;
					_thisItemData.skill3_level	= _haveData.skill3_level;
					_thisItemData.successRate	= _haveData.successRate;
					_thisItemData.deletTime		= _haveData.deletTime;
					_thisItemData.reviveTime	= _haveData.isReviveTime;
				}else
				{
					// û����
					_thisItemData.unLock		= NO;
					_thisItemData.level			= 0;
					_thisItemData.star			= 0;
					_thisItemData.blessing		= 0;
					_thisItemData.skill1_level	= 1;
					_thisItemData.skill2_level	= 1;
					_thisItemData.skill3_level	= 1;
					_thisItemData.successRate	= 0;
					_thisItemData.deletTime		= 24;
					_thisItemData.reviveTime	= 0;
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_unLock, _thisItemData.unLock);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_level, _thisItemData.level);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_star, _thisItemData.star);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_blessing, _thisItemData.blessing);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_skill1_level, _thisItemData.skill1_level);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_skill2_level, _thisItemData.skill2_level);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_skill3_level, _thisItemData.skill3_level);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_deletTime, _thisItemData.deletTime);
					UserRecord::getInstance()->setHeroKeyValue(_thisItemData.id, _HeroDataKey_Index_revivieTime, _thisItemData.reviveTime);
				}
				_thisItemData.name			= _haveData.name;
				// ���»����ɹ���
				if (_thisItemData.successRate <= 0)
				{
					_thisItemData.successRate = getSuccessRate(_thisItemData.star);
					UserRecord::getInstance()->setHeroKeyValue_Float(_thisItemData.id, _HeroDataKey_Index_successRate, _thisItemData.successRate);
				}
				_thisItemData.tempBlessing = _thisItemData.blessing;
				_thisItemData.item = _itemCell;
				_thisItemData.isDead = _haveData.isDead;
					
				auto _iconName = StringUtils::format("heroAltar/icon_%s.png", _thisItemData.keyName.c_str());
				_thisItemData.iconName = _iconName;	
				_thisItemData.successCost = 0;
				_thisItemData.heroLevelUpCost = 0;
				_thisItemData.skillLevelUpCost1 = 0;
				_thisItemData.skillLevelUpCost2 = 0;
				_thisItemData.skillLevelUpCost3 = 0;
				_thisItemData.heroUnLockType = 0;
				_thisItemData.heroUnLockCost = 0;
				_thisItemData.isHeroUpStar = false;
				for (int i = 1; i <= 3; ++i)
				{
					if (i == 1)
					{
						int _curCostGold = PlistManager::getInstance()->getHeroSkill(_thisItemData.keyName.c_str(), i, _thisItemData.skill1_level, "skilllupgradeMoney").intValue();
						_thisItemData.skillLevelUpCost1 = _curCostGold;
					}else if (i == 2)
					{
						int _curCostGold = PlistManager::getInstance()->getHeroSkill(_thisItemData.keyName.c_str(), i, _thisItemData.skill2_level, "skilllupgradeMoney").intValue();
						_thisItemData.skillLevelUpCost2 = _curCostGold;
					}else if (i == 3)
					{
						int _curCostGold = PlistManager::getInstance()->getHeroSkill(_thisItemData.keyName.c_str(), i, _thisItemData.skill3_level, "skilllupgradeMoney").intValue();
						_thisItemData.skillLevelUpCost3 = _curCostGold;
					}			
				}	
				_thisItemData.heroUnLockType = PlistManager::getInstance()->getAttributeCCString("BossData", _thisItemData.keyName.c_str(), _thisItemData.level, "unlockType").intValue();
				_thisItemData.heroUnLockCost = PlistManager::getInstance()->getAttributeCCString("BossData", _thisItemData.keyName.c_str(), _thisItemData.level, "unlockMoney").intValue();
				mHeroItems.push_back(_thisItemData);
				// ����ͷ���Ƿ��ɫ
				ImageView* _imageBack = static_cast<ImageView*>(Helper::seekWidgetByName(_thisItemData.item, "ImageView_Back"));
				if (_imageBack)
				{
					setWidgetGray(_imageBack, _thisItemData.unLock == NO);
				}	
				// ����ͷ��ͼ����ʾ
				ImageView* _imageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_thisItemData.item, "Image_Icon"));
				if (_imageIcon)
				{
					_imageIcon->loadTexture(_thisItemData.iconName.c_str(),UI_TEX_TYPE_LOCAL);
				}
			}
		}
	}
}
// ����ָ��Ӣ�۵�view����
void Ui_HeroAltar::updateHeroView(int _listHeroIndex)
{
	if (_listHeroIndex < 0)
		return;
	// ȡ��ǰѡ�е�Ӣ������
	HeroItemInfo _curHeroData = mHeroItems.at(_listHeroIndex);
	if (_curHeroData.id <= 0)
		return;
	mPanelRevive->setVisible(false);
	// ȡ���е�����
	int _curGold = UserRecord::getInstance()->getGold();
	int _curSoul = UserRecord::getInstance()->getHeroSoul();
	int _curSweet = UserRecord::getInstance()->getSweet();
	// ����ѡ�п�
	Widget* _imageSelectCtrl = Helper::seekWidgetByName(_curHeroData.item, "Image_Select");
	_imageSelectCtrl->setVisible(true);
	// ��д����״̬
	// ����ͷ���Ƿ��ɫ
	ImageView* _imageBack = static_cast<ImageView*>(Helper::seekWidgetByName(_curHeroData.item, "ImageView_Back"));
	if (_imageBack)
	{
		setWidgetGray(_imageBack, _curHeroData.unLock == NO, 0);
	}	
	// ��������
	mTextName->setText(_curHeroData.name);
	// ���µȼ�
	mTextLevel->setText(StringUtils::format(" Lv.%d", _curHeroData.level));
	
	// ����ͷ��ͼ��
	string str = _curHeroData.iconName;
	size_t pos = str.find_first_of("/");
	str = "roleIcon" + str.substr(pos);
	mImageStarIcon1->loadTexture(str.c_str(),UI_TEX_TYPE_LOCAL);
	mImageStarIcon1->setScale(0.8f);
	mImageStarIcon2->loadTexture(str.c_str(),UI_TEX_TYPE_LOCAL);
	mImageStarIcon2->setScale(0.8f);
	// ���´���ͷ��
	if (_curHeroData.isDead == YES)
	{
		// ����Ĺ��
		mImageHeroPhoto->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon1->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon1->setScale(0.8f);
		mImageStarIcon2->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon2->setScale(0.8f);
	}else
	{
		// ͷ��
		mImageHeroPhoto->loadTexture("image/null.png");
		mImageHeroPhoto->removeAllChildrenWithCleanup(true);
		auto _animName = StringUtils::format("td_boss_0%d.ExportJson", _curHeroData.id);
		auto _animActName = StringUtils::format("td_boss_0%d", _curHeroData.id);
		Armature* _armature = nullptr;
		if (_curHeroData.id > 0)
		{
			ArmatureDataManager::getInstance()->addArmatureFileInfo(_animName);
			_armature = Armature::create(_animActName);
			_armature->setScale(1.5f);
			_armature->setPositionX(0);
			_armature->setPositionY(0);
			if (_curHeroData.id == BOSS_ITEM2)
			{
				_armature->setPositionX(20);
			}
		}
		_armature->getAnimation()->play("load", -1, 1);
		mImageHeroPhoto->addChild(_armature, 1);
	}
	
	// �����Ǽ�
	int _curStar = _curHeroData.star;
	int _nextStar = _curHeroData.star + 1;
	if (_nextStar > HeroSkillMaxLevel)
		_nextStar = HeroSkillMaxLevel;
	updateStar(_curStar, "Image_Star", true);
	updateStar(_curStar, "Image_UpStar", false);
	updateStar(_nextStar, "Image_UpStar2_", false);
	

	string _curStarString = "Max";
	if (_curStar < 10)
	{
		int _count = _curStar;
		string _countString = PlistManager::getInstance()->getStringFormat(_count)._string;
		_curStarString = StringUtils::format("%s%s", _countString.c_str(), DefaultStarString.c_str());
	}
	mBmFontCurStar->setText(_curStarString);

	string _nextStarString = "Max";
	if (_nextStar < 10)
	{
		int _count = _nextStar;
		string _countString = PlistManager::getInstance()->getStringFormat(_count)._string;
		_nextStarString = StringUtils::format("%s%s", _countString.c_str(), DefaultStarString.c_str());
	}
	mBmFontNextStar->setText(_nextStarString);


	/*mBmFontCurStar->setText();
	mBmFontNextStar->setText(PlistManager::getInstance()->getStringFormat(_nextStar).getCString());*/
	// ����������ʾ
	auto _costGold_upLevel =  PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "upgradeMoney").intValue();
	auto _costGold_upLevelString = StringUtils::format("%d", _costGold_upLevel);
	if (_curHeroData.level >= TYPE_BOSS_MAX_LEVEL)
	{
		_costGold_upLevelString = "     Max";
	}
	ImageView* _glodIcon = static_cast<ImageView*>(findChild("ImageView_1345_0"));
	if (_glodIcon)
	{
		_glodIcon->setVisible(_curHeroData.level < TYPE_BOSS_MAX_LEVEL);
	}
	mTextUpLevelMoney->setText(_costGold_upLevelString);
	mHeroItems.at(_listHeroIndex).heroLevelUpCost = _costGold_upLevel;
	// �Ƿ��ܵ��������ť
	bool isClick_Level = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.level < TYPE_BOSS_MAX_LEVEL && _curGold >= _costGold_upLevel && mCurPageViewIndex == 1)
	{
		isClick_Level = true;
	}
	mButtonUpLevel->setTouchEnabled(isClick_Level);
	setWidgetGray(mButtonUpLevel, !isClick_Level);
	// ���½���ѡ��
	bool isClick_Unlock = false;
	bool isClick_Revive_Gold = false;
	bool isClick_Revive_Soul = false;
	mTextStarUseShowText->setVisible(false);
	mButtonStarUseMoney->setVisible(false);
	mButtonStarUseRmb->setVisible(false);
	mButtonUnLock->setVisible(false);
	mImageUnLockDescBack->setVisible(false);
	if (_curHeroData.unLock == YES)
	{
		// �ǹ�������ֵ
		int _curSweetCost = UserRecord::getInstance()->getReviveSweet();
		auto _curSweetCostString = StringUtils::format("%d", _curSweetCost);
		mTextStarUseMoney->setText(_curSweetCostString);
		// Ӣ��֮�긴����ֵ
		int _curSoulCost = ALTAR_REVIVE_SOUL_COST;
		auto _curSoulCostString = StringUtils::format("%d", _curSoulCost);
		mTextStarUseRmb->setText(_curSoulCostString);
		mTextStarUseShowText->setVisible(true);
		mButtonStarUseMoney->setVisible(true);
		mButtonStarUseRmb->setVisible(true);
		if (_curHeroData.isDead == YES)
		{
			if (_curSweet >= _curSweetCost)
			{
				isClick_Revive_Gold = true;
			}
			if (_curSoul >= _curSoulCost)
			{
				isClick_Revive_Soul = true;
			}
		}
	}else if (_curHeroData.unLock == NO)
	{
		// û������ʾ����ѡ��
		mButtonUnLock->setVisible(true);
		mImageUnLockDescBack->setVisible(true);
		// ���±�ע����
		auto _curUnLockDesc = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), 0, "unlockDesc")._string;
		string str = _curUnLockDesc;
		while ( str.find_first_of("_n")!=string::npos )
		{
			size_t pos1 = str.find_first_of("_n"); 
			str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
		}
		mBitmapLabelUnLockDesc->setText(str);
		// ���½�����Ǯ����
		auto _curUnLockTypeValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockType").intValue();
		if(_curUnLockTypeValue <=0)
			_curUnLockTypeValue = 0;
		auto _curUnLockCostValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockMoney").intValue();
		if(_curUnLockCostValue <=0)
			_curUnLockCostValue = 0;
		if (_curUnLockTypeValue == 1)
		{
			mImageUnLockIcon->loadTexture("image/UI_zjm_007.png");	// ���
			if (_curGold >= _curUnLockCostValue)
			{
				isClick_Unlock = true;
			}
		}else if (_curUnLockTypeValue == 2)
		{
			mImageUnLockIcon->loadTexture("image/UI_zjm_008.png");	// Ӣ��֮��
			if (_curSoul >= _curUnLockCostValue)
			{
				isClick_Unlock = true;
			}
		}else if (_curUnLockTypeValue == 3)	
		{
			mImageUnLockIcon->loadTexture("image/RMB.png");			// �����
			isClick_Unlock = true;
		}
		auto _curUnLockCostString = StringUtils::format("%d", _curUnLockCostValue);
		mTextUnLockValue->setText(_curUnLockCostString);
	}
	// ������ť
	mButtonUnLock->setTouchEnabled(isClick_Unlock);
	setWidgetGray(mButtonUnLock, !isClick_Unlock);	
	// ���ť
	mButtonStarUseMoney->setTouchEnabled(isClick_Revive_Gold);
	setWidgetGray(mButtonStarUseMoney, !isClick_Revive_Gold);	
	mButtonStarUseRmb->setTouchEnabled(isClick_Revive_Soul);
	setWidgetGray(mButtonStarUseRmb, !isClick_Revive_Soul);	
	
	// ���·�ҳ��
	updateHeroViewPage(mCurPageViewIndex);
}
// ����ָ��Ӣ�۵�view����, page 1, 2
void Ui_HeroAltar::updateHeroViewPage(int _pageIndex)
{
	if (_pageIndex < 0)
		return;
	// ȡ��ǰѡ�е�Ӣ������
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	mButtonPage1->setLocalZOrder(2);
	mButtonPage2->setLocalZOrder(2);
	mButtonUseStarAdd->setTouchEnabled(false);
	if (_pageIndex == 1)
	{
		mButtonPage1->setLocalZOrder(3);
		mCurPageViewIndex = 1;
		mPanelPageView1->setVisible(true);
		//WindowManager::getInstance()->setAllChildPause(mPanelPageView1, false);
		mPanelPageView2->setVisible(false);
		//WindowManager::getInstance()->setAllChildPause(mPanelPageView2, true);
		mButtonPage1->setPositionY(548);
		mButtonPage2->setPositionY(542);
		// ��������
		int _curLevel = _curHeroData.level;
		int _nextLevel = _curHeroData.level + 1;
		int _maxLevel = 40;
		if (_nextLevel > TYPE_BOSS_MAX_LEVEL)
		{
			_nextLevel = TYPE_BOSS_MAX_LEVEL;
		}
		// Hp
		int _curHp1 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curLevel, "maxHp").intValue();
		int _curHp2 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _nextLevel, "maxHp").intValue();
		int _maxHp = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _maxLevel, "maxHp").intValue();
		int _disHp = _curHp2 - _curHp1;
		int _oncesAddHp = _maxHp * 0.1;
		int _addHp = _curHeroData.star * _oncesAddHp;
		_curHp1 = _curHp1 + _addHp;
		_curHp2 = _curHp1 + _disHp;
		mTextHp1->setText(StringUtils::format("%d", _curHp1));
		mTextHp2->setText(StringUtils::format("%d", _curHp2));
		ImageView* _imageValueBack1 = static_cast<ImageView*>(findChild("Image_ValueBack1"));
		_imageValueBack1->setVisible(_disHp > 0);
		// Atk
		int _curAtk1 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curLevel, "atk").intValue();
		int _curAtk2 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _nextLevel, "atk").intValue();
		int _maxAtk = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _maxLevel, "atk").intValue();
		int _disAtk = _curAtk2 - _curAtk1;
		int _oncesAddAtk = _maxAtk * 0.1;
		int _addAtk = _curHeroData.star * _oncesAddAtk;
		_curAtk1 = _curAtk1 + _addAtk;
		_curAtk2 = _curAtk1 + _disAtk;
		mTextAtk1->setText(StringUtils::format("%d", _curAtk1));
		mTextAtk2->setText(StringUtils::format("%d", _curAtk2));
		ImageView* _imageValueBack2 = static_cast<ImageView*>(findChild("Image_ValueBack2"));
		_imageValueBack2->setVisible(_disAtk > 0);
		// dfe
		float _curDef1 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curLevel, "dfe").floatValue();
		float _curDef2 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _nextLevel, "dfe").floatValue();
		float _maxDef = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _maxLevel, "dfe").floatValue();
		float _disDef = _curDef2 - _curDef1;
		float _oncesAddDef = 0.01;
		float _addDef = _curHeroData.star * _oncesAddDef;
		_curDef1 = _curDef1 + _addDef;
		_curDef2 = _curDef1 + _disDef;
		mTextDef1->setText(StringUtils::format("%d%%", (int)(_curDef1*100)));
		mTextDef2->setText(StringUtils::format("%d%%", (int)(_curDef2*100)));
		ImageView* _imageValueBack3 = static_cast<ImageView*>(findChild("Image_ValueBack3"));
		_imageValueBack3->setVisible(_disDef > 0);
		// crit
		float _curCrit1 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curLevel, "cri").floatValue();
		float _curCrit2 = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _nextLevel, "cri").floatValue();
		float _maxCrit = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _maxLevel, "cri").floatValue();
		float _disCrit = _curCrit2 - _curCrit1;
		float _oncesAddCrit = 0.01;
		float _addCrit = _curHeroData.star * _oncesAddCrit;
		_curCrit1 = _curCrit1 + _addCrit;
		_curCrit2 = _curCrit1 + _disCrit;
		mTextCrit1->setText(StringUtils::format("%d%%", (int)(_curCrit1*100)));
		mTextCrit2->setText(StringUtils::format("%d%%", (int)(_curCrit2*100)));
		ImageView* _imageValueBack4 = static_cast<ImageView*>(findChild("Image_ValueBack4"));
		_imageValueBack4->setVisible(_disCrit > 0);

		// ���µ�1�����ܵȼ�
		mTextSkillLevel1->setText(StringUtils::toString(_curHeroData.skill1_level));
		// ���µ�2�����ܵȼ�
		mTextSkillLevel2->setText(StringUtils::toString(_curHeroData.skill2_level));
		// ���µ�3�����ܵȼ�
		mTextSkillLevel3->setText(StringUtils::toString(_curHeroData.skill3_level));
		// ��ǰѡ����
		updateHeroSkill(mCurSkillIndex);
	}else if (_pageIndex == 2)
	 {
		mButtonUseStarAdd->setTouchEnabled(true);
		mButtonPage2->setLocalZOrder(3);
		mCurPageViewIndex = 2;
		mPanelPageView1->setVisible(false);
		//WindowManager::getInstance()->setAllChildPause(mPanelPageView1, true);
		mPanelPageView2->setVisible(true);
		//WindowManager::getInstance()->setAllChildPause(mPanelPageView2, false);
		mButtonPage2->setPositionY(548);
		mButtonPage1->setPositionY(542);
	}
	// ���¸���ť�Ƿ���Ե��
	mButtonUpLevel->setTouchEnabled(mButtonUpLevel->isTouchEnabled() && mCurPageViewIndex == 1);
	mButtonSkillUse->setTouchEnabled(mButtonSkillUse->isTouchEnabled() && mCurPageViewIndex == 1);
	mButtonUseStar->setTouchEnabled(mButtonUseStar->isTouchEnabled() && mCurPageViewIndex == 2);
	mButtonZhuFuTang->setTouchEnabled(mButtonZhuFuTang->isTouchEnabled() && mCurPageViewIndex == 2);
	mSliderStar->setTouchEnabled(mSliderStar->isTouchEnabled() && mCurPageViewIndex == 2);

	// ����ף��ֵ
	auto _curBlessingString = StringUtils::format("%d", _curHeroData.tempBlessing);
	mTextZhuFuValue->setText(_curBlessingString);
	updateSlider(_curHeroData.tempBlessing);
	// ���½��׵���ף��������
	updateNeedStarCount(_curHeroData.tempBlessing);
	updateLevelUpTag();
}
// ����ѡ��ĳ������
void Ui_HeroAltar::updateHeroSkill(int _skillIndex)
{
	//CCLOG("select updateHeroSkill >> %d !", _skillIndex);
	ImageView* _imageArrow = static_cast<ImageView*>(findChild("Image_ArrowSkill"));
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	mCurSkillIndex = _skillIndex;
	int _curSkillLevel = 0;
	if (mCurSkillIndex == 0)
	{
		_imageArrow->setPositionX(68);
		mImageSkillSelect->setPositionX(68);
		_curSkillLevel = _curHeroData.skill1_level;
	}else if (mCurSkillIndex == 1)
	{
		_imageArrow->setPositionX(178);
		mImageSkillSelect->setPositionX(178);
		_curSkillLevel = _curHeroData.skill2_level;
	}else if (mCurSkillIndex == 2)
	{
		_imageArrow->setPositionX(293);
		mImageSkillSelect->setPositionX(293);
		_curSkillLevel = _curHeroData.skill3_level;
	}
	WindowManager::getInstance()->setTextureAlias(_imageArrow);
	int _newIndex = mCurSkillIndex + 1;
	int _curGold = UserRecord::getInstance()->getGold();
	// ȡ��ǰ���ܵȼ��Ľ���
	auto _curDesc = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, _curSkillLevel, "desc");
	mTextSkillDesc->setText(_curDesc.getCString());
	// ȡ��ǰ���ܵȼ�������
	auto _curCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, _curSkillLevel, "skilllupgradeMoney").intValue();
	auto _curCostGoldString = StringUtils::format("%d", _curCostGold);
	if (_curSkillLevel >= HeroSkillMaxLevel)
	{
		_curCostGoldString = "     Max";
	}
	
	ImageView* _glodIcon2 = static_cast<ImageView*>(findChild("ImageView_2410"));
	if (_glodIcon2)
	{
		_glodIcon2->setVisible(_curSkillLevel < HeroSkillMaxLevel);
	}
	mTextSkillUseValue->setText(_curCostGoldString);
	// ���°�ť�Ƿ�ɵ��
	bool isClick_upSkillLevel = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curSkillLevel < HeroSkillMaxLevel && _curGold >= _curCostGold && mCurPageViewIndex == 1)
	{
		isClick_upSkillLevel = true;
	}
	mButtonSkillUse->setTouchEnabled(isClick_upSkillLevel);
	setWidgetGray(mButtonSkillUse, !isClick_upSkillLevel);
}
// ���½��׵���������
int _curSlideValue = 0;
void Ui_HeroAltar::updateNeedStarCount(int _toValue)
{
	if (_toValue < 0)
		return;
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	if (_toValue < _curHeroData.blessing)
		return;
	// ȡ��ǰ���׵�����
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
	int _curCostStarItemCount = getValueB(_curHeroData.star);
	float _tempSuccessRate = _curHeroData.successRate;
	float _curSuccessRate = UserRecord::getInstance()->getHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate);
	int _onceSlideAddProp = getSlideAddProp(_curHeroData.star);					// �϶�����ÿ�����Ӳ���
	int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);	// �϶�����ÿ�����ӳɹ���

	// �������ӵĲ��Ϻͳɹ���
	if (_toValue == _curHeroData.blessing)
	{
		// ���ı�
		/*_curCostStarItemCount = getValueB(_curHeroData.star);
		_curSuccessRate = getSuccessRate(_curHeroData.star);*/
		//_curSuccessRate = UserRecord::getInstance()->getHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate);
		//CCLOG("->> _toValue:%d, _curHeroData.blessing:%d, _curSuccessRate:%f, tempBlessing:%d ", _toValue, _curHeroData.blessing, _curSuccessRate, _curHeroData.tempBlessing);
	}else if (_toValue > _curHeroData.blessing)
	{
		int _dragCount = (int)((_toValue - _curHeroData.blessing)/100);
		_curCostStarItemCount = _curCostStarItemCount + _dragCount* (int)(_onceSlideAddProp);
		_curSuccessRate = _curSuccessRate + _dragCount * _onceSlideAddSuccessRate;
	}
	if (_toValue >= 1000)
	{
		_curSuccessRate = 100.0f;
	}
	// ����ʧ�ܴ�����ӵĳɹ���
	//CCLOG("_curSuccessRate :%f", _curSuccessRate, _curHeroData.tempBlessing);
	// ���½��׵���ʾ
	auto _curStarSweetString = StringUtils::format("%d/%d", _curCostStarItemCount, _curCount);
	mTextUseStarValue->setText(_curStarSweetString);
	// ���½��װ�ť
	bool isClick_StarItem = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.star < HeroSkillMaxLevel && _curCount >= _curCostStarItemCount && mCurPageViewIndex == 2)
	{
		isClick_StarItem = true;
	}
	mButtonUseStar->setTouchEnabled(isClick_StarItem);
	setWidgetGray(mButtonUseStar, !isClick_StarItem);
	// ����ף����
	int _curBlessingSweet = UserRecord::getInstance()->getGoodsCountByID("30002");
	if (_curBlessingSweet < 0)
		_curBlessingSweet = 0;
	auto _curBlessingSweetString = StringUtils::format("%d", _curBlessingSweet);
	mTextZhuFuTangValue->setText(_curBlessingSweetString);
	// ����ף�������״̬
	bool isClick_BlessingSweet = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.tempBlessing < 1000 && _curHeroData.blessing < 1000 && _curBlessingSweet > 0 && _curHeroData.star < HeroSkillMaxLevel && mCurPageViewIndex == 2)
	{
		isClick_BlessingSweet = true;
	}
	mButtonZhuFuTang->setTouchEnabled(isClick_BlessingSweet);
	setWidgetGray(mButtonZhuFuTang, !isClick_BlessingSweet);
	// ���»�����״̬
	bool isClick_BlessingSlider = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.star < HeroSkillMaxLevel && mCurPageViewIndex == 2)
	{
		isClick_BlessingSlider = true;
	}
	mSliderStar->setTouchEnabled(isClick_BlessingSlider);
	setWidgetGray(mSliderStar, !isClick_BlessingSlider);
	// ��������
	mHeroItems.at(mCurListViewIndex).isHeroUpStar = isClick_StarItem;
	mHeroItems.at(mCurListViewIndex).successRate = _curSuccessRate;
	mHeroItems.at(mCurListViewIndex).successCost = _curCostStarItemCount;
}
// ���1��hero����
Widget* Ui_HeroAltar::addHeroItem()
{
	const char* jsonFile = "HeroAltar_HeroItem/HeroAltar_HeroItem.ExportJson";
	Layout* _newItemCell = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonFile));
	if (!_newItemCell)
		return nullptr;
	mListViewHeroSelect->setItemModel(_newItemCell);
	// ѡ����
	Widget* _itemSelectCtrl = Helper::seekWidgetByName(_newItemCell, "Image_Select");
	if(_itemSelectCtrl)
		_itemSelectCtrl->setVisible(false);
	// �Ƿ�����״̬
	Widget* _panelIsLock = Helper::seekWidgetByName(_newItemCell, "Panel_IsLock");
	if(_panelIsLock)
		_panelIsLock->setVisible(false);

	// ͷ��iconͼ��
	Widget* _itemIconCtrl = Helper::seekWidgetByName(_newItemCell, "Image_Icon"); 
	mListViewHeroSelect->pushBackDefaultItem();
	return _itemIconCtrl;
}
void Ui_HeroAltar::onButtonEvent(Ref* sender, TouchEventType type)
{
	Button* _curButton = static_cast<Button*>(sender);	
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		if (_curButton->getName()!=mButtonClose->getName())
		{
			audioPlayer::getInstance()->playEffect("S1.mp3");
		}
		
		if (_curButton->getName() == mImageSkillIcon1->getName())
		{	
			mImageSkillIcon1->setScale(1.1f);
		}else if (_curButton->getName() == mImageSkillIcon2->getName())
		{	
			mImageSkillIcon2->setScale(1.1f);
		}else if (_curButton->getName() == mImageSkillIcon3->getName())
		{	
			mImageSkillIcon3->setScale(1.1f);
		}
		break;
	case TOUCH_EVENT_ENDED:
		audioPlayer::getInstance()->playEffect("S1.mp3");
		if (_curButton->getName() == mButtonListUp->getName())
		{
			
		}else if (_curButton->getName() == mButtonClose->getName())
		{
			hideView();
		}else if (_curButton->getName() == mButtonListDown->getName())
		{
	
		}else if (_curButton->getName() == mButtonPage1->getName())
		{
			mButtonPage1->setLocalZOrder(3);
			mButtonPage2->setLocalZOrder(4);
			updateHeroViewPage(1);
			
		}else if (_curButton->getName() == mButtonPage2->getName())
		{
			mButtonPage1->setLocalZOrder(4);
			mButtonPage2->setLocalZOrder(3);
			updateHeroViewPage(2);

			//����������ѧ(����ҳǩ)
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
		}else if (_curButton->getName() == mButtonStarUseMoney->getName())
		{
			buttonCallBack_ReviveSweet();		// ʹ���ǹ�����
		}else if (_curButton->getName() == mButtonStarUseRmb->getName())
		{
			buttonCallBack_ReviveSoul();		// ʹ��Ӣ��֮�츴��
		}else if (_curButton->getName() == mButtonUpLevel->getName())
		{	
			buttonCallBack_UpHeroLevel();		// ����Ӣ�۵ȼ�
		}else if (_curButton->getName() == mImageSkillIcon1->getName())
		{	
			mImageSkillIcon1->setScale(1.0f);
			updateHeroSkill(0);
		}else if (_curButton->getName() == mImageSkillIcon2->getName())
		{	
			mImageSkillIcon2->setScale(1.0f);
			updateHeroSkill(1);
		}else if (_curButton->getName() == mImageSkillIcon3->getName())
		{	
			mImageSkillIcon3->setScale(1.0f);
			updateHeroSkill(2);
		}else if (_curButton->getName() == mButtonSkillUse->getName())
		{	
			buttonCallBack_UpHeroSkillLevel();	// ����Ӣ�ۼ��ܵȼ�
		}else if (_curButton->getName() == mButtonUnLock->getName())
		{	
			buttonCallBack_UnLockHero();		// ����Ӣ��
		}else if (_curButton->getName() == mButtonZhuFuTang->getName())
		{
			buttonCallBack_UseBlessingItem();	// ʹ��ף����
		}
		else if (_curButton->getName() ==  mButtonUseStar->getName())
		{
			buttonCallBack_UpHeroStar();		// ����Ӣ���Ǽ�
		}else if (_curButton->getName() == mButtonUseStarAdd->getName())
		{
			// ������̵�ȥ����׵�
			if (this->getParent()->getChildByTag(124) )
				return;
			Ui_Shop* shopLayer = Ui_Shop::create();
			shopLayer  -> setTag(124);
			this->getParent() -> addChild(shopLayer,1000);
			shopLayer->selectIndex(ShopItem::MaterialItem);
			shopLayer->heroAltarUpdateCallBack(this);
		}
		break;
	}
}
void Ui_HeroAltar::onSliderEvent(Ref* sender, SliderEventType type)
{
	switch (type)
	{
	case SLIDER_PERCENTCHANGED:
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// ȡ��ǰף��ֵ
		int _curBlessing = _curHeroData.blessing;
		// ת���ɻ���ֵ �Ա�
		auto _curBlessingPercent = toPercentValue(_curBlessing);
		auto _curSliderPercent = mSliderStar->getPercent();
		if (_curSliderPercent <= _curBlessingPercent)
		{
			mSliderStar->setPercent(_curBlessingPercent);
			auto _curBlessingString = StringUtils::format("%d", _curBlessing);
			mTextZhuFuValue->setText(_curBlessingString);
			if (_curBlessing != mHeroItems.at(mCurListViewIndex).tempBlessing)
			{
				mHeroItems.at(mCurListViewIndex).tempBlessing = _curBlessing;
				// ���½��׵�����
				updateNeedStarCount(_curBlessing);
			}
			
		}else
		{
			int _curResultPercent = 0;
			if (_curSliderPercent >=0 && _curSliderPercent <= 10)
			{
				_curResultPercent = 10;
			}else if (_curSliderPercent > 10 && _curSliderPercent <= 20)
			{
				_curResultPercent = 20;
			}else if (_curSliderPercent > 20 && _curSliderPercent <= 30)
			{
				_curResultPercent = 30;
			}else if (_curSliderPercent > 30 && _curSliderPercent <= 40)
			{
				_curResultPercent = 40;
			}else if (_curSliderPercent > 40 && _curSliderPercent <= 50)
			{
				_curResultPercent = 50;
			}else if (_curSliderPercent > 50 && _curSliderPercent <= 60)
			{
				_curResultPercent = 60;
			}else if (_curSliderPercent > 60 && _curSliderPercent <= 70)
			{
				_curResultPercent = 70;
			}else if (_curSliderPercent > 70 && _curSliderPercent <= 80)
			{
				_curResultPercent = 80;
			}else if (_curSliderPercent > 80 && _curSliderPercent <= 90)
			{
				_curResultPercent = 90;
			}else if (_curSliderPercent > 90 && _curSliderPercent <= 100)
			{
				_curResultPercent = 100;
			}
			mSliderStar->setPercent(_curResultPercent);
			auto _curResultBlessing = toBlessingValue(_curResultPercent);
			if (_curResultBlessing != mHeroItems.at(mCurListViewIndex).tempBlessing)
			{
				auto _curResultBlessingString = StringUtils::format("%d", _curResultBlessing);
				mTextZhuFuValue->setText(_curResultBlessingString);
				// �ı���ʱף��ֵ
				mHeroItems.at(mCurListViewIndex).tempBlessing = _curResultBlessing;
				// ���½��׵�����
				updateNeedStarCount(_curResultBlessing);
			}
		}
		break;
	}
}
void Ui_HeroAltar::onListViewEventHero(Ref* sender, ListViewEventType type)
{
	Widget *clickedItem = mListViewHeroSelect->getItem(mListViewHeroSelect->getCurSelectedIndex());
	Widget *wg          = clickedItem->getChildByName("ImageView_Back");
	switch (type)
	{
	case LISTVIEW_ONSELECTEDITEM_START:
		wg->setScale(1.1f);
		break;
	case LISTVIEW_ONSELECTEDITEM_END:
		{
			audioPlayer::getInstance()->playEffect("S1.mp3");
			wg->setScale(1.0f);
			auto _curIndex = mListViewHeroSelect->getCurSelectedIndex();
			auto _curItem = mListViewHeroSelect->getItem(_curIndex);

			for (int i = 0; i< mListViewHeroSelect->getItems().size(); ++i)
			{
				auto _thisItem = mListViewHeroSelect->getItem(i);
				if (_thisItem)
				{
					Widget* _itemSelectCtrl = Helper::seekWidgetByName(_thisItem, "Image_Select");
					if (_itemSelectCtrl)
					{
						_itemSelectCtrl->setVisible(false);
					}
				}
			}
			mCurListViewIndex = _curIndex;
			updateHeroView(_curIndex);
			break;
		}
	default:
		break;
	}
}

// �����ӿؼ�
Widget* Ui_HeroAltar::findChild(const char* _widgetName)
{
	if (!mMainWidget)
		return nullptr;
	Widget* _thisChild = Helper::seekWidgetByName(mMainWidget, _widgetName);
	if (nullptr == _thisChild)
		return nullptr;
	return _thisChild;
}
// �����ǽ�������ʾ
void Ui_HeroAltar::updateStar(int _starNum, const char* _name, bool _isBlue)
{
	if (_starNum > HeroSkillMaxLevel)
	{
		_starNum = HeroSkillMaxLevel;
	}
	int			_maxCount = 5;
	const char*	_starTexture1 = "image/YXW_006.png";	// ����
	const char*	_starTexture4 = "image/XYW_XX_02.png";	// ���� ��ɫ
	
	const char*	_starTexture2 = "image/GK_002.png";		// ʵ�� ����ɫ

	const char*	_starTexture0 = "image/YXW_005.png";	// ���� ��ɫ
	const char*	_starTexture3 = "image/XYW_XX_01.png";	// ���� ����ɫ

	// ż��
	if (_starNum % 2 == 0)
	{
		int _count = _starNum / 2;
		for (int i = 1; i <= _maxCount; ++i)
		{
			auto _curIndex = StringUtils::format("%s%d",_name, i);
			ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
			if (i <= _count)
			{
				_curStar->loadTexture(_starTexture2);	// ʵ�� ����ɫ
			}else
			{
				if (_isBlue)
				{
					_curStar->loadTexture(_starTexture0);	// ���� ��ɫ
				}else
				{
					_curStar->loadTexture(_starTexture3);	// ���� ��ɫ
				}
			}
		}
	}
	// ����
	else
	{
		int _count = int(_starNum / 2);
		for (int i = 1; i <= _maxCount; ++i)
		{
			auto _curIndex = StringUtils::format("%s%d",_name, i);
			ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
			if (i <= _count)
			{
				_curStar->loadTexture(_starTexture2);	// ʵ��
			}else
			{
				if (_isBlue)
				{
					_curStar->loadTexture(_starTexture0);	// ���� ��ɫ
				}else
				{
					_curStar->loadTexture(_starTexture3);	// ���� ��ɫ
				}
			}
		}
		auto _curIndex = StringUtils::format("%s%d",_name, _count + 1);
		ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
		if (_isBlue)
		{
			_curStar->loadTexture(_starTexture1);			// ���� ��ɫ
		}else
		{
			_curStar->loadTexture(_starTexture4);			// ���� ��ɫ
		}
		
	}
}
// ����ף��ֵ����λ��
void Ui_HeroAltar::updateSlider(int _value)
{
	int _maxValue = 1000;	// ���ֵ
	int _doCount = 10;		// ֻ���϶�10��
	if (_value == 0)
	{
		_maxValue = _doCount;
	}
	int _curPercenValue = toPercentValue(_value);
	mSliderStar->setPercent(_curPercenValue);
	if (mIsSliderAnim)
	{
		beginAnim_Slider();
	}
}
// ף��ֵת���� ����ֵ
int Ui_HeroAltar::toPercentValue(int _value)
{
	int _maxValue = 1000;
	int _curPercenValue =(int)(((float)_value/(float)_maxValue)*100.0);
	return _curPercenValue;
}
// ����ֵת���� ף��ֵ
int Ui_HeroAltar::toBlessingValue(int _value)
{
	int _curValue = (int)(((float)_value/100.0f) * 1000.0f);
	return _curValue;
}

void Ui_HeroAltar::fitScreen()
{
	Point originPt   = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	float _scale = MIN(visibleSize.width/960 , visibleSize.height/640);
	mMainWidget->setScale(_scale);
	mMainWidget->setPosition(Point(visibleSize.width/2  - mMainWidget->getSize().width/2*_scale  +originPt.x ,
								   visibleSize.height/2 - mMainWidget->getSize().height/2*_scale +originPt.y
		));

	Widget *win_bg  = static_cast<Button*>(Helper::seekWidgetByName(mMainWidget, "Image_2"));
	win_bg->setScaleX(2);
	win_bg->setScaleY(2);

	win_bg->setTouchEnabled(true);
}
void Ui_HeroAltar::setWidgetGray(Widget* _widget, bool _isGray, float _value)
{
	if (_widget)
	{
		if (_isGray)
		{
			_widget->setGrey(0);
		}else
		{
			_widget->removeGrey();
		}
	}
}
void Ui_HeroAltar::setViewIsClick(bool _isClick)
{

}


// ȡ��ף��������������		A
int Ui_HeroAltar::getValueA(int _star)
{
	if (_star == 0)
	{
		return 10;
	}else if (_star == 1)
	{
		return 15;
	}else if (_star == 2)
	{
		return 20;
	}else if (_star == 3)
	{
		return 50;
	}else if (_star == 4)
	{
		return 100;
	}else if (_star == 5)
	{
		return 150;
	}else if (_star == 6)
	{
		return 250;
	}else if (_star == 7)
	{
		return 650;
	}else if (_star == 8)
	{
		return 1700;
	}else if (_star == 9)
	{
		return 2500;
	}
	return 0;
} 
// ȡ��ǰ���׼�ԭ�������Ľ��׵�����		B
int Ui_HeroAltar::getValueB(int _star)
{
	if (_star == 0 || _star == 1 || _star == 2)
	{
		return 2;
	}else if (_star == 3 || _star == 4 || _star == 5)
	{
		return 4;
	}else if (_star == 6 || _star == 7 || _star == 8)
	{
		return 6;
	}else if (_star == 9)
	{
		return 8;
	}
	return 0;
} 
// ȡ�Ǽ��ɹ���		�ɹ���=(B/A)*100
float Ui_HeroAltar::getSuccessRate(int _star)
{
	int _valueA = getValueA(_star);
	int _valueB = getValueB(_star);
	float _result = 0;

	if (_valueA > 0 && _valueB > 0)
	{
		float _floatValueA = Value(_valueA).asFloat();
		float _floatValueB = Value(_valueB).asFloat();
		float _floatResult = (_floatValueB / _floatValueA) * 100;
		return _floatResult;
	}
	return _result;
} 
// ȡʧ�����ӵ�ף��ֵ	1000/(A/B)
int Ui_HeroAltar::getFailureAddBlessing(int _star)
{
	int _valueA = getValueA(_star);
	int _valueB = getValueB(_star);
	int _result = -1;
	if (_valueA > 0 && _valueB > 0)
	{
		float _float1000 = Value(1000).asFloat();
		float _floatValueA = Value(_valueA).asFloat();
		float _floatValueB = Value(_valueB).asFloat();
		float _floatResult = _float1000 / ( _floatValueA/ _floatValueB);
		_result = Value(_floatResult).asInt();
		return _result;
	}
	return _result;
} 
// ȡʧ�����ӵĳɹ���	(100%-�ɹ���) / (A/B)
float Ui_HeroAltar::getFailureAddSuccessRate(int _star)
{
	int _valueA = getValueA(_star);
	int _valueB = getValueB(_star);
	float _baseSuccessRate = getSuccessRate(_star);	// ȡ�����ɹ���
	float _result = 0;
	if (_valueA > 0 && _valueB > 0 && _baseSuccessRate > 0)
	{
		float _floatValueA = Value(_valueA).asFloat();
		float _floatValueB = Value(_valueB).asFloat();
		float _floatResult = (100 - _baseSuccessRate) / ( _floatValueA / _floatValueB );
		return _floatResult;
	}
	return _result;
} 
// �϶�����ÿ�����Ӳ���		A/10
int Ui_HeroAltar::getSlideAddProp(int _star)
{
	int _valueA = getValueA(_star);
	int _result = -1;
	if (_valueA > 0)
	{
		float _floatValueA = Value(_valueA).asFloat();
		float _floatResult = _floatValueA / 10;
		_result = Value(_floatResult).asInt();
		return _result;
	}
	return _result;
} 
// �϶�����ÿ�����ӳɹ���	(100%-�ɹ���)/10
int Ui_HeroAltar::getSlideAddSuccessRate(int _star)
{
	float _baseSuccessRate = getSuccessRate(_star);	// ȡ�����ɹ���
	int _result = -1;
	if (_baseSuccessRate > 0)
	{
		float _floatResult = (100 - _baseSuccessRate) / 10;
		_result = Value(_floatResult).asInt();
		return _result;
	}
	return _result;
} 



// ��ť����		ʹ���ǹ�����
void Ui_HeroAltar::buttonCallBack_ReviveSweet()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curCostSweet = UserRecord::getInstance()->getReviveSweet();
	int _curSweet = UserRecord::getInstance()->getSweet();
	if (_curCostSweet > _curSweet)
		return;
	UserRecord::getInstance()->alterSweet(-_curCostSweet);

	// ȷ��֮��ʼ����ʱ
	auto _curFloorValue = UserRecord::getInstance()->getHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_storeyIndex);
	if (_curFloorValue > 0)
	{
		auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_curFloorValue);
		if (_curStoreyData)
		{
			auto _obj = _curStoreyData->getHeadstone();
			if (_obj)
			{
				headstoneObjects* _headstoneObj = static_cast<headstoneObjects*>(_obj);
				if (_headstoneObj)
				{
					_headstoneObj->beginProgressBar(0);
					UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_revivieTime, YES);
				}
			}
		}
	}
}
// �����ʱ�ص�
void Ui_HeroAltar::actionCallBack_ReviveSweet(Ref* sender)
{
	//HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	//if (_curHeroData.id <= 0)
	//	return;
	//mPanelRevive->setVisible(false);
	//auto _curCostSweet = ALTAR_REVIVE_SWEET_COST;
	//int _curSweet = UserRecord::getInstance()->getSweet();
	//// �۳�
	//UserRecord::getInstance()->alterSweet(-_curCostSweet);
	//UserRecord::getInstance()->flush();
	//// ������״̬д���ļ�
	//auto _deadValue = StringUtils::format("%d", NO);
	//PlistManager::getInstance()->setHeroDeadKey(_curHeroData.keyName.c_str(), _deadValue);
	//// ��д״̬
	//mHeroItems.at(mCurListViewIndex).isDead = NO;
	//// ɾ��Ĺ��������Ӣ��
	//auto _curFloorValue = PlistManager::getInstance()->getConfigBossKeyValue(_curHeroData.keyName.c_str(), "floor");
	//if (_curFloorValue > 0)
	//{
	//	auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_curFloorValue);
	//	if (_curStoreyData)
	//	{
	//		_curStoreyData->mItem->removeChildByTag(55);
	//		WindowManager::getInstance()->createHeroToHeroSelect(_curFloorValue, _curHeroData.id, Point(0,0), _curHeroData.level);
	//	}
	//}
	//updateHeroView(mCurListViewIndex);
}
// ��ť����		ʹ��Ӣ��֮�긴��
void Ui_HeroAltar::buttonCallBack_ReviveSoul()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curCostSoul = ALTAR_REVIVE_SOUL_COST;
	int _curSoul = UserRecord::getInstance()->getHeroSoul();
	if (_curCostSoul > _curSoul)
		return;
	// ���͸�����Ϣ��������
	Connect* handle = Connect::getInstance();
	handle->loadSend("c=hero&a=revive", this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_ReviveSoul));
}
// �������ص�>>>>	Ӣ��֮�츴��ص�
void Ui_HeroAltar::onHttpRequestCompleted_ReviveSoul(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// �۳�
		auto _curCostSoul = ALTAR_REVIVE_SOUL_COST;
		int _curSoul = UserRecord::getInstance()->getHeroSoul();
		UserRecord::getInstance()->alterHeroSoul(-_curCostSoul);
		UserRecord::getInstance()->flush();
		// ������״̬д���ļ�
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_isDead, NO);
		// ��д״̬
		mHeroItems.at(mCurListViewIndex).isDead = NO;
		// ɾ��Ĺ��������Ӣ��
		auto _curFloorValue = UserRecord::getInstance()->getHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_storeyIndex);
		if (_curFloorValue > 0)
		{
			auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_curFloorValue);
			if (_curStoreyData)
			{
				GameObjects* _obj = _curStoreyData->getHeadstone();
				SceneData::getInstance()->removeStoreyObjectFormStoreyIndex(_curFloorValue, _obj);
				_obj->removeFromParentAndCleanup(true);
				Point _newPos;
				_newPos.x = UserRecord::getInstance()->getHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_reviviePosX);
				_newPos.y = 0;
				WindowManager::getInstance()->createHeroToHeroSelect(_curFloorValue, _curHeroData.id, _newPos, _curHeroData.level);
				UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_revivieTime, NO);
			}
		}
		updateHeroView(mCurListViewIndex);
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so ReviveSoul Failure!!!");

		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// ��ť����		����Ӣ��
void Ui_HeroAltar::buttonCallBack_UnLockHero()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curUnLockTypeValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockType").intValue();
	if(_curUnLockTypeValue <=0)
		_curUnLockTypeValue = 0;
	auto _curUnLockCostValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockMoney").intValue();
	if(_curUnLockCostValue <=0)
		_curUnLockCostValue = 0;
	// �۳�
	if (_curUnLockTypeValue == 1)
	{
		auto _curGold = UserRecord::getInstance()->getGold();
		if (_curUnLockCostValue > _curGold)	// ��Ǯ����
			return;
	}else if (_curUnLockTypeValue == 2)	
	{
		auto _curSoul = UserRecord::getInstance()->getHeroSoul();
		if (_curUnLockCostValue > _curSoul)	// Ӣ��֮�겻��
			return;
	}else if (_curUnLockTypeValue == 3)		// �����
	{
		__Dictionary* heroDic = PlistManager::getInstance()->getObjectPlist("BossData", _curHeroData.keyName.c_str());
		auto payid   = static_cast<__String*>(heroDic->objectForKey("payid"))->getCString();
		if (strcmp(payid,"0")!=0){
			mobilePay(payid,StringUtils::format("%d",UserRecord::getInstance()->getUid()).c_str());
		}
		return;
	}
	// ���ͳɹ���Ϣ��������
	auto postDatas = __String::createWithFormat("c=hero&a=unlock&hero=%d",_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UnLockHero));
}

void Ui_HeroAltar::buyHeroSuccessCallFuc(Ref* ref)
{
	this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		// ���ͳɹ���Ϣ��������
		auto postDatas = __String::createWithFormat("c=hero&a=unlock&hero=%d",_curHeroData.id)->getCString();
		Connect* handle = Connect::getInstance();
		handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UnLockHero));
	}),NULL));
}

// �������ص�>>>>	����Ӣ�ۻص�
void Ui_HeroAltar::onHttpRequestCompleted_UnLockHero(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		auto _curUnLockTypeValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockType").intValue();
		if(_curUnLockTypeValue <=0)
			_curUnLockTypeValue = 0;
		auto _curUnLockCostValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockMoney").intValue();
		if(_curUnLockCostValue <=0)
			_curUnLockCostValue = 0;
		// �۳���Ǯ
		if (_curUnLockTypeValue == 1)
		{
			auto _curGold = UserRecord::getInstance()->getGold();
			UserRecord::getInstance()->alterGold(-_curUnLockCostValue);
		}else if (_curUnLockTypeValue == 2)	
		{
			auto _curSoul = UserRecord::getInstance()->getHeroSoul();
			UserRecord::getInstance()->alterHeroSoul(-_curUnLockCostValue);
		}else if (_curUnLockTypeValue == 3)		
		{
		}
		UserRecord::getInstance()->flush();
		// ��д״̬
		mHeroItems.at(mCurListViewIndex).unLock = YES;
		mHeroItems.at(mCurListViewIndex).isDead = NO;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_unLock, YES);
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_isDead, NO);
		
		
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
		//����������ѧ
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UnLockHero Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// ����Ӣ�۵ȼ�
void Ui_HeroAltar::buttonCallBack_UpHeroLevel()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curCostGold =  PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "upgradeMoney").intValue();
	if(_curCostGold < 0)
		return;
	int _curGold = UserRecord::getInstance()->getGold();
	if (_curCostGold > _curGold)	// ��Ǯ����
		return;
	if (_curHeroData.level >= TYPE_BOSS_MAX_LEVEL)	// �ȼ�����
		return;
	// ���ͳɹ���Ϣ��������
	
	auto postDatas  = __String::createWithFormat("c=hero&a=uplevel&hero=%d",_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroLevel));
} 
// �������ص�>>>>	����Ӣ�۵ȼ��ص�
void Ui_HeroAltar::onHttpRequestCompleted_UpHeroLevel(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		audioPlayer::getInstance()->playEffect("S11.mp3");
		// �۳�
		auto _curCostGold =  PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "upgradeMoney").intValue();
		int _curGold = UserRecord::getInstance()->getGold();
		UserRecord::getInstance()->alterGold(-_curCostGold);
		UserRecord::getInstance()->flush();
		// ��д״̬
		int _nowLevel = mHeroItems.at(mCurListViewIndex).level;
		mHeroItems.at(mCurListViewIndex).level = _nowLevel + 1;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_level, mHeroItems.at(mCurListViewIndex).level);
		// ������Ч
		beginAnim_UpLevel(_curHeroData.id);
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
		//����������ѧ
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroLevel Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// ����Ӣ�ۼ��ܵȼ�
void Ui_HeroAltar::buttonCallBack_UpHeroSkillLevel()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _curSkillLevel = 0;
	if (mCurSkillIndex == 0)
	{
		_curSkillLevel = _curHeroData.skill1_level;
	}else if (mCurSkillIndex == 1)
	{
		_curSkillLevel = _curHeroData.skill2_level;
	}else if (mCurSkillIndex == 2)
	{
		_curSkillLevel = _curHeroData.skill3_level;
	}
	int _newIndex = mCurSkillIndex + 1;
	auto _curCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, _curSkillLevel, "skilllupgradeMoney").intValue();
	if(_curCostGold < 0)
		return;
	int _curGold = UserRecord::getInstance()->getGold();
	if (_curCostGold > _curGold)	// ��Ǯ����
		return;
	if (_curSkillLevel >= HeroSkillMaxLevel)	// �ȼ�����
		return;
	// ���ͳɹ���Ϣ��������
	auto postDatas = __String::createWithFormat("c=hero&a=upskill&skill=%d&hero=%d",
		_newIndex,
		_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroSkillLevel));
} 
// �������ص�>>>>	����Ӣ�ۼ��ܵȼ��ص�
void Ui_HeroAltar::onHttpRequestCompleted_UpHeroSkillLevel(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		audioPlayer::getInstance()->playEffect("S11.mp3");
		// �۳�
		int _curSkillLevel = 0;
		if (mCurSkillIndex == 0)
		{
			_curSkillLevel = _curHeroData.skill1_level;
		}else if (mCurSkillIndex == 1)
		{
			_curSkillLevel = _curHeroData.skill2_level;
		}else if (mCurSkillIndex == 2)
		{
			_curSkillLevel = _curHeroData.skill3_level;
		}
		int _newIndex = mCurSkillIndex + 1;
		auto _curCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, _curSkillLevel, "skilllupgradeMoney").intValue();
		int _curGold = UserRecord::getInstance()->getGold();
		UserRecord::getInstance()->alterGold(-_curCostGold);
		UserRecord::getInstance()->flush();
		// ��д״̬
		if (mCurSkillIndex == 0)
		{
			int _nowLevel = mHeroItems.at(mCurListViewIndex).skill1_level;
			mHeroItems.at(mCurListViewIndex).skill1_level = _nowLevel + 1;
			UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_skill1_level, mHeroItems.at(mCurListViewIndex).skill1_level);
			int _newCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, mHeroItems.at(mCurListViewIndex).skill1_level, "skilllupgradeMoney").intValue();
			mHeroItems.at(mCurListViewIndex).skillLevelUpCost1 = _newCostGold;
		}else if (mCurSkillIndex == 1)
		{
			int _nowLevel = mHeroItems.at(mCurListViewIndex).skill2_level;
			mHeroItems.at(mCurListViewIndex).skill2_level = _nowLevel + 1;
			UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_skill2_level, mHeroItems.at(mCurListViewIndex).skill2_level);
			int _newCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, mHeroItems.at(mCurListViewIndex).skill2_level, "skilllupgradeMoney").intValue();
			mHeroItems.at(mCurListViewIndex).skillLevelUpCost2 = _newCostGold;
		}else if (mCurSkillIndex == 2)
		{
			int _nowLevel = mHeroItems.at(mCurListViewIndex).skill3_level;
			mHeroItems.at(mCurListViewIndex).skill3_level = _nowLevel + 1;
			UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_skill3_level, mHeroItems.at(mCurListViewIndex).skill3_level);
			int _newCostGold = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, mHeroItems.at(mCurListViewIndex).skill3_level, "skilllupgradeMoney").intValue();
			mHeroItems.at(mCurListViewIndex).skillLevelUpCost3 = _newCostGold;
		}
		// ������Ч
		beginAnim_UpSkillLevel(mCurSkillIndex);
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
		//����������ѧ
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroSkillLevel Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// ʹ��ף����
void Ui_HeroAltar::buttonCallBack_UseBlessingItem()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _curCostBlessingItem = 1;
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30002");
	if (_curCostBlessingItem > _curCount)	// ף��������
		return;
	if (_curHeroData.tempBlessing >= 1000)		// ��ʱף��ֵ��ֵ
		return;
	if (_curHeroData.blessing >= 1000)			// ף��ֵ��ֵ
		return;
	// �����³ɹ���
	int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);
	float _newSuccessRate = _curHeroData.successRate + _onceSlideAddSuccessRate;
	// ���ͳɹ���Ϣ��������

	auto postDatas = __String::createWithFormat("c=hero&a=blessing&hero=%d&successRate=%2f",
		_curHeroData.id,
		_newSuccessRate
		)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UseBlessingItem));
} 
// �������ص�>>>>	ʹ��ף�����ص�
void Ui_HeroAltar::onHttpRequestCompleted_UseBlessingItem(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		audioPlayer::getInstance()->playEffect("S11.mp3");
		// �۳�
		int _curCostBlessingItem = 1;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30002");
		UserRecord::getInstance()->alterGoodsCount("30002", -_curCostBlessingItem);
		UserRecord::getInstance()->flush();
		// ��д״̬ף��ֵ
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		mHeroItems.at(mCurListViewIndex).blessing = _nowBlessing + 100;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		// ��д״̬�ɹ���
		int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);
		float _newSuccessRate = _curHeroData.successRate + _onceSlideAddSuccessRate;
		mHeroItems.at(mCurListViewIndex).successRate = _newSuccessRate;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		// �ı���Ч״̬
		mIsSliderAnim = true;
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UseBlessingItem Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// ����Ӣ���Ǽ�
void Ui_HeroAltar::buttonCallBack_UpHeroStar()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _curCostCount = _curHeroData.successCost;
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
	if (_curCostCount > _curCount)			// ���׵�����
		return;
	if (_curHeroData.star >= HeroSkillMaxLevel)
		return;

	// ���ʼ���
	int _curMaxRand = _curHeroData.successRate * 100;
	int _curRandSize = 100000;
	int _curRand = UserRecord::getInstance()->getRandom(_curRandSize);                     
	bool isOk = false;       
	if (_curRand > 0 && _curRand <= _curMaxRand)
	{
		isOk = true;
	}
	if (_curMaxRand >= 10000)	// ����ɹ�����100���Ͱѳɹ�����������100%
	{
		isOk = true;
	}
	if (UserRecord::getInstance()->getIsFreshMan()){//����ǽ�ѧ�򽫳ɹ�����������100%
		isOk = true;
	}
	//CCLOG(">>> _curHeroData.successRate:%f, _curRand %d < %d",_curHeroData.successRate, _curRand, _curMaxRand);
	// �ɹ�
	if (isOk)
	{
		int nextStar = _curHeroData.star + 1;
		float nextStarSuccess = 0;
		if (nextStar <= HeroSkillMaxLevel)
		{
			nextStarSuccess = getSuccessRate(_curHeroData.star + 1);
		}	
		// ���ͳɹ���Ϣ��������
		auto postDatas = __String::createWithFormat("c=hero&a=advance&hero=%d&advance_egg=%d&success=%d&successRate=%2f",
			_curHeroData.id,
			_curHeroData.successCost,
			YES,
			nextStarSuccess
			)->getCString();

		Connect* handle = Connect::getInstance();
		handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroStar_Success));
		//CCLOG(" --------- true ---------- ");
	}
	// ʧ��
	else
	{
		// ʧ������ף��ֵ
		int _addBlessing = getFailureAddBlessing(_curHeroData.star);
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		int _resultBlessing = _nowBlessing + _addBlessing;
		if (_resultBlessing > 1000)
			_resultBlessing = 1000;
		// ʧ�����ӳɹ���
		float _addFailureAddSuccessRate = getFailureAddSuccessRate(_curHeroData.star);
		float _curSuccessRate = _curHeroData.successRate + _addFailureAddSuccessRate;
		// ���ͳɹ���Ϣ��������
		auto postDatas = __String::createWithFormat("c=hero&a=advance&hero=%d&advance_egg=%d&success=%d&blessing=%d&successRate=%2f",
			_curHeroData.id,
			_curHeroData.successCost,
			NO,
			_resultBlessing,
			_curSuccessRate
			)->getCString();
		Connect* handle = Connect::getInstance();
		handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroStar_Failure));
		//CCLOG(" --- false --");
	}
} 
// �������ص�>>>>	����Ӣ���Ǽ�_�ɹ��ص�
void Ui_HeroAltar::onHttpRequestCompleted_UpHeroStar_Success(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// ������Ч_�ɹ�
		audioPlayer::getInstance()->playEffect("S11.mp3");
		int nextStar = _curHeroData.star + 1;
		float nextStarSuccess = 0;
		if (nextStar <= HeroSkillMaxLevel)
		{
			nextStarSuccess = getSuccessRate(_curHeroData.star + 1);
		}
		// �۳�
		int _curCostCount = _curHeroData.successCost;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
		UserRecord::getInstance()->alterGoodsCount("30001", -_curCostCount);
		UserRecord::getInstance()->flush();
		// ��д״̬ ף��ֵ
		mHeroItems.at(mCurListViewIndex).blessing = 0;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		// ��д״̬ �Ǽ�
		int _nowStar = mHeroItems.at(mCurListViewIndex).star;
		mHeroItems.at(mCurListViewIndex).star = _nowStar + 1;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_star, mHeroItems.at(mCurListViewIndex).star);
		// ��д״̬ �ɹ���
		mHeroItems.at(mCurListViewIndex).successCost = 0;
		mHeroItems.at(mCurListViewIndex).successRate = nextStarSuccess;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		// ������Ч
		beginAnim_UpLevel(_curHeroData.id);
		// ����������Ч
		beginAnim_UpStar();

		//����������ѧ(���װ�ť)
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroStar_Success Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// �������ص�>>>>	����Ӣ���Ǽ�_ʧ�ܻص�
void Ui_HeroAltar::onHttpRequestCompleted_UpHeroStar_Failure(HttpClient *sender, HttpResponse *response)
{
	if (!response)
		return;
	std::vector<char> *buffer = response->getResponseData();
	buffer->push_back('\0');
	rapidjson::Document doc; 
	doc.Parse<rapidjson::kParseDefaultFlags>(&(*buffer->begin()));
	if (doc.HasParseError())  
		return ;   
	rapidjson::Value &v=doc; 
	if (v["status"].GetInt() == 1)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// ������Ч_ʧ��
		audioPlayer::getInstance()->playEffect("S11_2.mp3");
		// �۳�
		int _curCostCount = _curHeroData.successCost;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
		UserRecord::getInstance()->alterGoodsCount("30001", -_curCostCount);
		UserRecord::getInstance()->flush();
		// ��д״̬ ʧ������ף��ֵ
		int _addBlessing = getFailureAddBlessing(_curHeroData.star);
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		int _resultBlessing = _nowBlessing + _addBlessing;
		if (_resultBlessing > 1000)
			_resultBlessing = 1000;
		mHeroItems.at(mCurListViewIndex).blessing = _resultBlessing;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		//CCLOG("shibai add _resultBlessing:%d, tempBlessing:%d", _resultBlessing, mHeroItems.at(mCurListViewIndex).blessing);
		// ��д״̬ ʧ�����ӳɹ���	
		float _addFailureAddSuccessRate = getFailureAddSuccessRate(_curHeroData.star);
		float _curSuccessRate = _curHeroData.successRate + _addFailureAddSuccessRate;
		mHeroItems.at(mCurListViewIndex).successRate = _curSuccessRate;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		//CCLOG("shibai add successRate:%f", mHeroItems.at(mCurListViewIndex).successRate);
		// �ı���Ч״̬
		mIsSliderAnim = true;
		// ������ʾ
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//����Ӣ�۵�����
	}else
	{
		// ʧ��
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroStar_Failure Failure!!!");
		// ʧ��
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}


// ---------------- ��Ч���� ---------------------
// Slider��Ч��ʼ
void Ui_HeroAltar::beginAnim_Slider()
{
	if(!mSliderArmature)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("hero_wu_Effects_020.png","hero_wu_Effects_020.plist","hero_wu_Effects_02.ExportJson"); 
		mSliderArmature = cocostudio::Armature::create("hero_wu_Effects_02"); 
		mSliderArmature->setScale(1/0.95);//�ڵ�������������ʱ�����ŵ�0.95
		if (mSliderArmature)
		{
			ImageView* panelCtrl2	= static_cast<ImageView*>(findChild("Panel_Page2"));
			if (panelCtrl2)
			{	
				mSliderArmature->setScaleX(0.95);
				mSliderArmature->setScaleY(0.98);
				mSliderArmature->setTag(3000);
				panelCtrl2->addChild(mSliderArmature, 30);
				mSliderArmature->setPositionX(mSliderStar->getPositionX());
				mSliderArmature->setPositionY(mSliderStar->getPositionY() - mSliderStar->getSize().height/2 - 5);
			}
		}
	}
	if (mSliderArmature)
	{
		mSliderArmature->setVisible(true);
		mSliderArmature->getAnimation()->play("Animation1", -1, 0);
		mSliderArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(Ui_HeroAltar::endAnim_Slider));
		// --- 
		if (!mSliderArmature->getParent()->getChildByTag(35))
		{
			Widget* _slidWidget = static_cast<Widget*>(mSliderStar->get_slidBallRenderer());
			if (_slidWidget)
			{
				ImageView* _imageSlid = ImageView::create();
				_imageSlid->loadTexture("image/YXW_027.png");
				_imageSlid->setTag(35);
				mSliderArmature->getParent()->addChild(_imageSlid, 35);
				Point _newPos = mSliderArmature->getParent()->convertToNodeSpace(_slidWidget->getWorldPosition());
				_imageSlid->setPosition(_newPos);
			}
		}else
		{
			ImageView* _imageSlid = static_cast<ImageView*>(mSliderArmature->getParent()->getChildByTag(35));
			Widget* _slidWidget = static_cast<Widget*>(mSliderStar->get_slidBallRenderer());
			if (_imageSlid && _slidWidget)
			{
				Point _newPos = mSliderArmature->getParent()->convertToNodeSpace(_slidWidget->getWorldPosition());
				_imageSlid->setPosition(_newPos);
			}
		}
	}
}
// Slider��Ч����
void Ui_HeroAltar::endAnim_Slider(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		Widget* _thisParent = static_cast<Widget*>(mSliderArmature->getParent());
		if (_thisParent)
		{
			_thisParent->removeChildByTag(35);
		}
		// ���Ŷ���2
		mSliderArmature->getAnimation()->stop();
		mSliderArmature->setVisible(false);
		mIsSliderAnim = false;
	}
}
// ������Ч��ʼ
void Ui_HeroAltar::beginAnim_UpLevel(int _heroId)
{
	if(!mLevelArmature)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("hero_wu_Effects_010.png","hero_wu_Effects_010.plist","hero_wu_Effects_01.ExportJson"); 
		mLevelArmature = cocostudio::Armature::create("hero_wu_Effects_01");   
		mLevelArmature->setScale(1.25);//�ڵ���������ʱ����С����0.8
		if (mLevelArmature)
		{
			ImageView* panelCtrl2	= static_cast<ImageView*>(findChild("Panel_Page2"));
			if (panelCtrl2)
			{	
				mLevelArmature->setTag(3000);
				auto _parentCtrl = mImageHeroPhoto->getParent();
				_parentCtrl->addChild(mLevelArmature, 30);
			}
		}
	}
	if (mLevelArmature)
	{
		if (_heroId == 1)
		{
			mLevelArmature->setPositionX(mImageHeroPhoto->getPositionX() + 10);
			mLevelArmature->setPositionY(mImageHeroPhoto->getPositionY() - mImageHeroPhoto->getSize().height + 50);
		}else
		{
			mLevelArmature->setPositionX(mImageHeroPhoto->getPositionX() + 10);
			mLevelArmature->setPositionY(mImageHeroPhoto->getPositionY() - mImageHeroPhoto->getSize().height + 20);
		}
		mLevelArmature->setVisible(true);
		mLevelArmature->getAnimation()->play("Animation1", -1, 0);
		mLevelArmature->getAnimation()->setFrameEventCallFunc(this, frameEvent_selector(Ui_HeroAltar::endAnim_UpLevel));
	}
}
// ������Ч����
void Ui_HeroAltar::endAnim_UpLevel(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		mLevelArmature->getAnimation()->stop();
		mLevelArmature->setVisible(false);
	}
}
// ����������Ч��ʼ
void Ui_HeroAltar::beginAnim_UpSkillLevel(int _skillIndex)
{
	if(!mSkillLevelSprite)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero_wu_Effects_03.plist");
		mSkillLevelSprite = Sprite::createWithSpriteFrameName("hero_wu_Effects_03_update_01.png");
		mSkillLevelSprite->setScale(1.25);//��Ϊ�ϳɴ�ͼ��ʱ����С��0.8
		mImageSkillSelect->addChild(mSkillLevelSprite, 30);
		mSkillLevelSprite->setPositionX(mImageSkillSelect->getSize().width/2);
		mSkillLevelSprite->setPositionY(mImageSkillSelect->getSize().height/2);
	}
	if (mSkillLevelSprite)
	{
		// ���Ŷ���
		mSkillLevelSprite->setVisible(true);
		Animate* ani = createAnimation("hero_wu_Effects_03_update_", 12);
		auto action = Sequence::create(ani,
			CallFuncN::create(CC_CALLBACK_1(Ui_HeroAltar::endAnim_UpSkillLevel, this)),
			NULL);
		mSkillLevelSprite->runAction(action);
	}
}
// ����������Ч����
void Ui_HeroAltar::endAnim_UpSkillLevel(Ref *sender)
{
	if (mSkillLevelSprite)
	{
		mSkillLevelSprite->setVisible(false);
	}
}
// ��������֡����
Animate* Ui_HeroAltar::createAnimation(const char*FrameName,int max)
{
	Vector<SpriteFrame*> aniFrames(max);
	char str[100] = {0};
	for(int i = 1; i <= max; i++)
	{
		SpriteFrame *frame = SpriteFrameCache::getInstance()->spriteFrameByName(__String::createWithFormat("%s%02d.png",FrameName,i)->getCString());
		aniFrames.pushBack(frame);
	}
	Animation *animation = Animation::createWithSpriteFrames(aniFrames, 0.05f);
	Animate *animate = CCAnimate::create(animation);
	return animate;
}
// �����Ǽ���Ч ��ʼ
void Ui_HeroAltar::beginAnim_UpStar()
{
	if(!mStarArmature)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("td_boss_updatelevel.ExportJson");
		mStarArmature = cocostudio::Armature::create("td_boss_updatelevel"); 
		if (mStarArmature)
		{
			ImageView* panelCtrl2	= static_cast<ImageView*>(findChild("Panel_Page2"));
			if (panelCtrl2)
			{	
				mStarArmature->setScale(1.0/0.75f);
				mStarArmature->setTag(122);
				this->addChild(mStarArmature, 11);
				Size _winSize = Director::getInstance()->getWinSize();
				mStarArmature->setPosition(_winSize.width/2, _winSize.height/2);
			}
		}
	}
	if (mStarArmature)
	{
		// ��ʾ����
		Ui_SwallowAllTouchEvents* swallowUi = Ui_SwallowAllTouchEvents::create();
		swallowUi->setPosition(Director::getInstance()->getVisibleOrigin());
		swallowUi->setTag(123);
		swallowUi->setBackGroundColorOpacity(200);
		this->addChild(swallowUi, 10);
		// ���Ŷ���
		mStarArmature->getAnimation()->play("play",-1,0);
		mStarArmature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_HeroAltar::endAnim_UpStar));
	}
}
// �����Ǽ���Ч ����
void Ui_HeroAltar::endAnim_UpStar(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"comeout")==0)
	{
		// ȡ��ǰѡ�е�Ӣ������
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// ��ʾͷ��
		string str = _curHeroData.iconName;
		size_t pos = str.find_first_of("/");
		str = "roleIcon" + str.substr(pos);
		ImageView* _imageIcon = ImageView::create();
		_imageIcon->setTag(124);
		_imageIcon->loadTexture(str.c_str(),UI_TEX_TYPE_LOCAL);
		_imageIcon->setPosition(mStarArmature->getPosition());
		_imageIcon->setPositionY(mStarArmature->getPosition().y + 20);
		this->addChild(_imageIcon,13);
	}
	else if (strcmp(evt.c_str(),"finish")==0)
	{
		this->runAction(Sequence::create(DelayTime::create(0.1f),CallFunc::create([=](){
			mStarArmature->removeFromParentAndCleanup(true);
			mStarArmature = nullptr;
			this->removeChildByTag(123);
			this->removeChildByTag(124);
		}),NULL));
	}
}

void Ui_HeroAltar::updateRevive(float _time)
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _value = UserRecord::getInstance()->getHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_revivieTime);
	if (_value <= 0)
		return;
	auto _progress = getReviveProgress();
	if (_progress)
	{
		mPanelRevive->setVisible(true);
		int _curPercent = _progress->getProgressBarPercent();
		mLoadingBarRevive->setPercent(_curPercent);
	}else
	{
		mPanelRevive->setVisible(false);
	}

	int _curSweet = UserRecord::getInstance()->getSweet();
	int _curSweetCost = 200;
	mButtonStarUseMoney->setGrey();
	if (_curHeroData.unLock == YES && _curHeroData.isDead == YES && _curSweet >= _curSweetCost && !_progress)
	{
		mButtonStarUseMoney->removeGrey();
	}
}
Ui_ProgressBar* Ui_HeroAltar::getReviveProgress()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return nullptr;

	// ͬ����ǰλ��
	auto _curFloorValue = UserRecord::getInstance()->getHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_storeyIndex);
	if (_curFloorValue > 0)
	{
		auto _curStoreyData = SceneData::getInstance()->getSceneDataMapStoreyFormStoreyIndex(_curFloorValue);
		if (_curStoreyData)
		{
			auto _obj = _curStoreyData->getHeadstone();
			if (_obj)
			{
				auto _loadingWidget = _obj->getChildByTag(11);			
				if (_loadingWidget)
				{
					Ui_ProgressBar* _loadingBar = static_cast<Ui_ProgressBar*>(_loadingWidget);
					if (_loadingBar)
					{
						return _loadingBar;
					}
				}
			}
		}
	}
	return nullptr;
}

// ����������ʾ��ǩ
void Ui_HeroAltar::updateLevelUpTag()
{
	for (int i = 0; i != mHeroItems.size(); ++i)
	{
		HeroItemInfo _curHeroData = mHeroItems.at(i);
		if (_curHeroData.id > 0)
		{
			Widget* _tagCtrl = _curHeroData.item->getChildByName("Image_Tag");
			if (_tagCtrl)
			{
				auto _curGold = UserRecord::getInstance()->getGold();
				auto _curSoul = UserRecord::getInstance()->getHeroSoul();
				// Ӣ������ �Ƿ���ʾ
				bool isTag_Level = false;
				auto _costGold_upLevel =  _curHeroData.heroLevelUpCost;
				if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.level < TYPE_BOSS_MAX_LEVEL && _curGold >= _costGold_upLevel)
				{
					isTag_Level = true;
				}
				// Ӣ�ۼ������� �Ƿ���ʾ
				bool isTag_SkillLevel = false;
				for (int i = 1; i <= 3; ++i)
				{
					int _curSkillLevel = 1;
					int _curCostGold = 0;
					if (i == 1)
					{
						_curSkillLevel = _curHeroData.skill1_level;
						_curCostGold = _curHeroData.skillLevelUpCost1;
					}else if (i == 2)
					{
						_curSkillLevel = _curHeroData.skill2_level;
						_curCostGold = _curHeroData.skillLevelUpCost2;
					}else if (i == 3)
					{
						_curSkillLevel = _curHeroData.skill3_level;
						_curCostGold = _curHeroData.skillLevelUpCost3;
					}	
					if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curSkillLevel < HeroSkillMaxLevel && _curGold >= _curCostGold)
					{
						isTag_SkillLevel = true;
						break;
					}
				}
				// ������ʾ
				bool isTag_unLock = false;
				if (_curHeroData.unLock == NO)
				{
					if (_curHeroData.heroUnLockType == 1)
					{
						if (_curGold >= _curHeroData.heroUnLockCost)
						{
							isTag_unLock = true;
						}
					}else if (_curHeroData.heroUnLockType == 2)
					{
						if (_curSoul >= _curHeroData.heroUnLockCost)
						{
							isTag_unLock = true;
						}
					}
				}

				if (isTag_Level || isTag_SkillLevel || isTag_unLock || _curHeroData.isHeroUpStar)
				{
					// ��ʾ��ʾ
					_tagCtrl->setVisible(true);
				}else
				{
					// ����ʾ
					_tagCtrl->setVisible(false);
				}
			}
		}
	}
}
