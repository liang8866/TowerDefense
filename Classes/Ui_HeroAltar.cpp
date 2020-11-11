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

	//添加广播
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

// 显示
void Ui_HeroAltar::showView()
{
	updateHeroList();
	// 默认选中第一个
	updateHeroView(0);
	
	//mMainWidget->setPositionY(mDefaultPos.y);
	this->setVisible(true);
	mRootPanel->setTouchEnabled(true);

	//Director::getInstance()->pause();
	audioPlayer::getInstance()->pauseAllEffects();
	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,(Ref*)0);
}
// 显示
void Ui_HeroAltar::createView(bool _isAutoRemove)
{
	updateHeroList();
	// 默认选中第一个
	updateHeroView(0);

	//mMainWidget->setPositionY(mDefaultPos.y);
	this->setVisible(true);
	mRootPanel->setTouchEnabled(true);
	mIsAutoRemove = true;

	NotificationCenter::sharedNotificationCenter()->postNotification(PAUSEKEY,(Ref*)0);
}
// 隐藏
void Ui_HeroAltar::hideView()
{
	//mMainWidget->setPositionY(-2000);
	this->setVisible(false);
	mRootPanel->setTouchEnabled(false);
	//Director::getInstance()->resume();
	NotificationCenter::sharedNotificationCenter()->postNotification(RESUMEKEY, (Ref*)0);
	audioPlayer::getInstance()->resumeAllEffects();

	//通知ui_sceneselect开启 touch 事件
	NotificationCenter::getInstance()->postNotification("SCENE_OPEN_TOUCH");
	if (mIsAutoRemove)
	{
		removeFromParentAndCleanup(true);
	}
}
// 更新list头像
void Ui_HeroAltar::updateHeroList()
{
	// 先删除所有的数据
	mListViewHeroSelect->removeAllItems();
	mHeroItems.clear();
	// 从本地列表中取出所有的英雄
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
					// 解锁过
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
					// 没解锁
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
				// 更新基础成功率
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
				// 更新头像是否灰色
				ImageView* _imageBack = static_cast<ImageView*>(Helper::seekWidgetByName(_thisItemData.item, "ImageView_Back"));
				if (_imageBack)
				{
					setWidgetGray(_imageBack, _thisItemData.unLock == NO);
				}	
				// 更新头像图标显示
				ImageView* _imageIcon = static_cast<ImageView*>(Helper::seekWidgetByName(_thisItemData.item, "Image_Icon"));
				if (_imageIcon)
				{
					_imageIcon->loadTexture(_thisItemData.iconName.c_str(),UI_TEX_TYPE_LOCAL);
				}
			}
		}
	}
}
// 更新指定英雄的view界面
void Ui_HeroAltar::updateHeroView(int _listHeroIndex)
{
	if (_listHeroIndex < 0)
		return;
	// 取当前选中的英雄数据
	HeroItemInfo _curHeroData = mHeroItems.at(_listHeroIndex);
	if (_curHeroData.id <= 0)
		return;
	mPanelRevive->setVisible(false);
	// 取现有的数据
	int _curGold = UserRecord::getInstance()->getGold();
	int _curSoul = UserRecord::getInstance()->getHeroSoul();
	int _curSweet = UserRecord::getInstance()->getSweet();
	// 更新选中框
	Widget* _imageSelectCtrl = Helper::seekWidgetByName(_curHeroData.item, "Image_Select");
	_imageSelectCtrl->setVisible(true);
	// 改写锁定状态
	// 更新头像是否灰色
	ImageView* _imageBack = static_cast<ImageView*>(Helper::seekWidgetByName(_curHeroData.item, "ImageView_Back"));
	if (_imageBack)
	{
		setWidgetGray(_imageBack, _curHeroData.unLock == NO, 0);
	}	
	// 更新名称
	mTextName->setText(_curHeroData.name);
	// 更新等级
	mTextLevel->setText(StringUtils::format(" Lv.%d", _curHeroData.level));
	
	// 更新头像图标
	string str = _curHeroData.iconName;
	size_t pos = str.find_first_of("/");
	str = "roleIcon" + str.substr(pos);
	mImageStarIcon1->loadTexture(str.c_str(),UI_TEX_TYPE_LOCAL);
	mImageStarIcon1->setScale(0.8f);
	mImageStarIcon2->loadTexture(str.c_str(),UI_TEX_TYPE_LOCAL);
	mImageStarIcon2->setScale(0.8f);
	// 更新待机头像
	if (_curHeroData.isDead == YES)
	{
		// 死亡墓碑
		mImageHeroPhoto->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon1->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon1->setScale(0.8f);
		mImageStarIcon2->loadTexture("image/YXJH_MuBei.png",UI_TEX_TYPE_LOCAL);
		mImageStarIcon2->setScale(0.8f);
	}else
	{
		// 头像
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
	
	// 更新星级
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
	// 更新升级显示
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
	// 是否能点击升级按钮
	bool isClick_Level = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.level < TYPE_BOSS_MAX_LEVEL && _curGold >= _costGold_upLevel && mCurPageViewIndex == 1)
	{
		isClick_Level = true;
	}
	mButtonUpLevel->setTouchEnabled(isClick_Level);
	setWidgetGray(mButtonUpLevel, !isClick_Level);
	// 更新解锁选项
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
		// 糖果复活数值
		int _curSweetCost = UserRecord::getInstance()->getReviveSweet();
		auto _curSweetCostString = StringUtils::format("%d", _curSweetCost);
		mTextStarUseMoney->setText(_curSweetCostString);
		// 英雄之魂复活数值
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
		// 没解锁显示解锁选项
		mButtonUnLock->setVisible(true);
		mImageUnLockDescBack->setVisible(true);
		// 更新备注解释
		auto _curUnLockDesc = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), 0, "unlockDesc")._string;
		string str = _curUnLockDesc;
		while ( str.find_first_of("_n")!=string::npos )
		{
			size_t pos1 = str.find_first_of("_n"); 
			str = str.substr(0,pos1) + "\n" + str.substr(pos1+2);
		}
		mBitmapLabelUnLockDesc->setText(str);
		// 更新解锁金钱消耗
		auto _curUnLockTypeValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockType").intValue();
		if(_curUnLockTypeValue <=0)
			_curUnLockTypeValue = 0;
		auto _curUnLockCostValue = PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "unlockMoney").intValue();
		if(_curUnLockCostValue <=0)
			_curUnLockCostValue = 0;
		if (_curUnLockTypeValue == 1)
		{
			mImageUnLockIcon->loadTexture("image/UI_zjm_007.png");	// 金币
			if (_curGold >= _curUnLockCostValue)
			{
				isClick_Unlock = true;
			}
		}else if (_curUnLockTypeValue == 2)
		{
			mImageUnLockIcon->loadTexture("image/UI_zjm_008.png");	// 英雄之魂
			if (_curSoul >= _curUnLockCostValue)
			{
				isClick_Unlock = true;
			}
		}else if (_curUnLockTypeValue == 3)	
		{
			mImageUnLockIcon->loadTexture("image/RMB.png");			// 人民币
			isClick_Unlock = true;
		}
		auto _curUnLockCostString = StringUtils::format("%d", _curUnLockCostValue);
		mTextUnLockValue->setText(_curUnLockCostString);
	}
	// 解锁按钮
	mButtonUnLock->setTouchEnabled(isClick_Unlock);
	setWidgetGray(mButtonUnLock, !isClick_Unlock);	
	// 复活按钮
	mButtonStarUseMoney->setTouchEnabled(isClick_Revive_Gold);
	setWidgetGray(mButtonStarUseMoney, !isClick_Revive_Gold);	
	mButtonStarUseRmb->setTouchEnabled(isClick_Revive_Soul);
	setWidgetGray(mButtonStarUseRmb, !isClick_Revive_Soul);	
	
	// 更新分页面
	updateHeroViewPage(mCurPageViewIndex);
}
// 更新指定英雄的view界面, page 1, 2
void Ui_HeroAltar::updateHeroViewPage(int _pageIndex)
{
	if (_pageIndex < 0)
		return;
	// 取当前选中的英雄数据
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
		// 更新属性
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

		// 更新第1个技能等级
		mTextSkillLevel1->setText(StringUtils::toString(_curHeroData.skill1_level));
		// 更新第2个技能等级
		mTextSkillLevel2->setText(StringUtils::toString(_curHeroData.skill2_level));
		// 更新第3个技能等级
		mTextSkillLevel3->setText(StringUtils::toString(_curHeroData.skill3_level));
		// 当前选择技能
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
	// 更新各按钮是否可以点击
	mButtonUpLevel->setTouchEnabled(mButtonUpLevel->isTouchEnabled() && mCurPageViewIndex == 1);
	mButtonSkillUse->setTouchEnabled(mButtonSkillUse->isTouchEnabled() && mCurPageViewIndex == 1);
	mButtonUseStar->setTouchEnabled(mButtonUseStar->isTouchEnabled() && mCurPageViewIndex == 2);
	mButtonZhuFuTang->setTouchEnabled(mButtonZhuFuTang->isTouchEnabled() && mCurPageViewIndex == 2);
	mSliderStar->setTouchEnabled(mSliderStar->isTouchEnabled() && mCurPageViewIndex == 2);

	// 更新祝福值
	auto _curBlessingString = StringUtils::format("%d", _curHeroData.tempBlessing);
	mTextZhuFuValue->setText(_curBlessingString);
	updateSlider(_curHeroData.tempBlessing);
	// 更新进阶蛋和祝福蛋数量
	updateNeedStarCount(_curHeroData.tempBlessing);
	updateLevelUpTag();
}
// 更新选中某个技能
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
	// 取当前技能等级的介绍
	auto _curDesc = PlistManager::getInstance()->getHeroSkill(_curHeroData.keyName.c_str(), _newIndex, _curSkillLevel, "desc");
	mTextSkillDesc->setText(_curDesc.getCString());
	// 取当前技能等级的消耗
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
	// 更新按钮是否可点击
	bool isClick_upSkillLevel = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curSkillLevel < HeroSkillMaxLevel && _curGold >= _curCostGold && mCurPageViewIndex == 1)
	{
		isClick_upSkillLevel = true;
	}
	mButtonSkillUse->setTouchEnabled(isClick_upSkillLevel);
	setWidgetGray(mButtonSkillUse, !isClick_upSkillLevel);
}
// 更新进阶蛋需求数量
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
	// 取当前进阶蛋数量
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
	int _curCostStarItemCount = getValueB(_curHeroData.star);
	float _tempSuccessRate = _curHeroData.successRate;
	float _curSuccessRate = UserRecord::getInstance()->getHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate);
	int _onceSlideAddProp = getSlideAddProp(_curHeroData.star);					// 拖动滑竿每次增加材料
	int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);	// 拖动滑竿每次增加成功率

	// 计算增加的材料和成功率
	if (_toValue == _curHeroData.blessing)
	{
		// 不改变
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
	// 计算失败次数添加的成功率
	//CCLOG("_curSuccessRate :%f", _curSuccessRate, _curHeroData.tempBlessing);
	// 更新进阶蛋显示
	auto _curStarSweetString = StringUtils::format("%d/%d", _curCostStarItemCount, _curCount);
	mTextUseStarValue->setText(_curStarSweetString);
	// 更新进阶按钮
	bool isClick_StarItem = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.star < HeroSkillMaxLevel && _curCount >= _curCostStarItemCount && mCurPageViewIndex == 2)
	{
		isClick_StarItem = true;
	}
	mButtonUseStar->setTouchEnabled(isClick_StarItem);
	setWidgetGray(mButtonUseStar, !isClick_StarItem);
	// 更新祝福蛋
	int _curBlessingSweet = UserRecord::getInstance()->getGoodsCountByID("30002");
	if (_curBlessingSweet < 0)
		_curBlessingSweet = 0;
	auto _curBlessingSweetString = StringUtils::format("%d", _curBlessingSweet);
	mTextZhuFuTangValue->setText(_curBlessingSweetString);
	// 更新祝福蛋点击状态
	bool isClick_BlessingSweet = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.tempBlessing < 1000 && _curHeroData.blessing < 1000 && _curBlessingSweet > 0 && _curHeroData.star < HeroSkillMaxLevel && mCurPageViewIndex == 2)
	{
		isClick_BlessingSweet = true;
	}
	mButtonZhuFuTang->setTouchEnabled(isClick_BlessingSweet);
	setWidgetGray(mButtonZhuFuTang, !isClick_BlessingSweet);
	// 更新滑块条状态
	bool isClick_BlessingSlider = false;
	if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.star < HeroSkillMaxLevel && mCurPageViewIndex == 2)
	{
		isClick_BlessingSlider = true;
	}
	mSliderStar->setTouchEnabled(isClick_BlessingSlider);
	setWidgetGray(mSliderStar, !isClick_BlessingSlider);
	// 保存数据
	mHeroItems.at(mCurListViewIndex).isHeroUpStar = isClick_StarItem;
	mHeroItems.at(mCurListViewIndex).successRate = _curSuccessRate;
	mHeroItems.at(mCurListViewIndex).successCost = _curCostStarItemCount;
}
// 添加1个hero格子
Widget* Ui_HeroAltar::addHeroItem()
{
	const char* jsonFile = "HeroAltar_HeroItem/HeroAltar_HeroItem.ExportJson";
	Layout* _newItemCell = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile(jsonFile));
	if (!_newItemCell)
		return nullptr;
	mListViewHeroSelect->setItemModel(_newItemCell);
	// 选择红框
	Widget* _itemSelectCtrl = Helper::seekWidgetByName(_newItemCell, "Image_Select");
	if(_itemSelectCtrl)
		_itemSelectCtrl->setVisible(false);
	// 是否锁定状态
	Widget* _panelIsLock = Helper::seekWidgetByName(_newItemCell, "Panel_IsLock");
	if(_panelIsLock)
		_panelIsLock->setVisible(false);

	// 头像icon图标
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

			//新手引导教学(进阶页签)
			if (UserRecord::getInstance()->getIsFreshMan()){
				UserRecord::getInstance()->setFreshManMutex(false);
			}
		}else if (_curButton->getName() == mButtonStarUseMoney->getName())
		{
			buttonCallBack_ReviveSweet();		// 使用糖果复活
		}else if (_curButton->getName() == mButtonStarUseRmb->getName())
		{
			buttonCallBack_ReviveSoul();		// 使用英雄之混复活
		}else if (_curButton->getName() == mButtonUpLevel->getName())
		{	
			buttonCallBack_UpHeroLevel();		// 升级英雄等级
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
			buttonCallBack_UpHeroSkillLevel();	// 升级英雄技能等级
		}else if (_curButton->getName() == mButtonUnLock->getName())
		{	
			buttonCallBack_UnLockHero();		// 解锁英雄
		}else if (_curButton->getName() == mButtonZhuFuTang->getName())
		{
			buttonCallBack_UseBlessingItem();	// 使用祝福蛋
		}
		else if (_curButton->getName() ==  mButtonUseStar->getName())
		{
			buttonCallBack_UpHeroStar();		// 升级英雄星级
		}else if (_curButton->getName() == mButtonUseStarAdd->getName())
		{
			// 点击到商店去买进阶蛋
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
		// 取当前祝福值
		int _curBlessing = _curHeroData.blessing;
		// 转换成滑竿值 对比
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
				// 更新进阶蛋数量
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
				// 改变临时祝福值
				mHeroItems.at(mCurListViewIndex).tempBlessing = _curResultBlessing;
				// 更新进阶蛋数量
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

// 查找子控件
Widget* Ui_HeroAltar::findChild(const char* _widgetName)
{
	if (!mMainWidget)
		return nullptr;
	Widget* _thisChild = Helper::seekWidgetByName(mMainWidget, _widgetName);
	if (nullptr == _thisChild)
		return nullptr;
	return _thisChild;
}
// 更新星阶星星显示
void Ui_HeroAltar::updateStar(int _starNum, const char* _name, bool _isBlue)
{
	if (_starNum > HeroSkillMaxLevel)
	{
		_starNum = HeroSkillMaxLevel;
	}
	int			_maxCount = 5;
	const char*	_starTexture1 = "image/YXW_006.png";	// 半星
	const char*	_starTexture4 = "image/XYW_XX_02.png";	// 半星 灰色
	
	const char*	_starTexture2 = "image/GK_002.png";		// 实星 非蓝色

	const char*	_starTexture0 = "image/YXW_005.png";	// 空心 蓝色
	const char*	_starTexture3 = "image/XYW_XX_01.png";	// 空心 非蓝色

	// 偶数
	if (_starNum % 2 == 0)
	{
		int _count = _starNum / 2;
		for (int i = 1; i <= _maxCount; ++i)
		{
			auto _curIndex = StringUtils::format("%s%d",_name, i);
			ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
			if (i <= _count)
			{
				_curStar->loadTexture(_starTexture2);	// 实星 非蓝色
			}else
			{
				if (_isBlue)
				{
					_curStar->loadTexture(_starTexture0);	// 空心 蓝色
				}else
				{
					_curStar->loadTexture(_starTexture3);	// 空心 蓝色
				}
			}
		}
	}
	// 奇数
	else
	{
		int _count = int(_starNum / 2);
		for (int i = 1; i <= _maxCount; ++i)
		{
			auto _curIndex = StringUtils::format("%s%d",_name, i);
			ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
			if (i <= _count)
			{
				_curStar->loadTexture(_starTexture2);	// 实心
			}else
			{
				if (_isBlue)
				{
					_curStar->loadTexture(_starTexture0);	// 空心 蓝色
				}else
				{
					_curStar->loadTexture(_starTexture3);	// 空心 蓝色
				}
			}
		}
		auto _curIndex = StringUtils::format("%s%d",_name, _count + 1);
		ImageView* _curStar = static_cast<ImageView*>(findChild(_curIndex.c_str()));
		if (_isBlue)
		{
			_curStar->loadTexture(_starTexture1);			// 半心 蓝色
		}else
		{
			_curStar->loadTexture(_starTexture4);			// 半心 灰色
		}
		
	}
}
// 更新祝福值滑竿位置
void Ui_HeroAltar::updateSlider(int _value)
{
	int _maxValue = 1000;	// 最大值
	int _doCount = 10;		// 只能拖动10次
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
// 祝福值转换成 滑竿值
int Ui_HeroAltar::toPercentValue(int _value)
{
	int _maxValue = 1000;
	int _curPercenValue =(int)(((float)_value/(float)_maxValue)*100.0);
	return _curPercenValue;
}
// 滑竿值转换成 祝福值
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


// 取满祝福材料需求数量		A
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
// 取当前进阶级原定的消耗进阶蛋数量		B
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
// 取星级成功率		成功率=(B/A)*100
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
// 取失败增加的祝福值	1000/(A/B)
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
// 取失败增加的成功率	(100%-成功率) / (A/B)
float Ui_HeroAltar::getFailureAddSuccessRate(int _star)
{
	int _valueA = getValueA(_star);
	int _valueB = getValueB(_star);
	float _baseSuccessRate = getSuccessRate(_star);	// 取基础成功率
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
// 拖动滑竿每次增加材料		A/10
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
// 拖动滑竿每次增加成功率	(100%-成功率)/10
int Ui_HeroAltar::getSlideAddSuccessRate(int _star)
{
	float _baseSuccessRate = getSuccessRate(_star);	// 取基础成功率
	int _result = -1;
	if (_baseSuccessRate > 0)
	{
		float _floatResult = (100 - _baseSuccessRate) / 10;
		_result = Value(_floatResult).asInt();
		return _result;
	}
	return _result;
} 



// 按钮功能		使用糖果复活
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

	// 确认之后开始倒计时
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
// 复活倒计时回调
void Ui_HeroAltar::actionCallBack_ReviveSweet(Ref* sender)
{
	//HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	//if (_curHeroData.id <= 0)
	//	return;
	//mPanelRevive->setVisible(false);
	//auto _curCostSweet = ALTAR_REVIVE_SWEET_COST;
	//int _curSweet = UserRecord::getInstance()->getSweet();
	//// 扣除
	//UserRecord::getInstance()->alterSweet(-_curCostSweet);
	//UserRecord::getInstance()->flush();
	//// 将复活状态写入文件
	//auto _deadValue = StringUtils::format("%d", NO);
	//PlistManager::getInstance()->setHeroDeadKey(_curHeroData.keyName.c_str(), _deadValue);
	//// 改写状态
	//mHeroItems.at(mCurListViewIndex).isDead = NO;
	//// 删除墓碑，复活英雄
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
// 按钮功能		使用英雄之魂复活
void Ui_HeroAltar::buttonCallBack_ReviveSoul()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curCostSoul = ALTAR_REVIVE_SOUL_COST;
	int _curSoul = UserRecord::getInstance()->getHeroSoul();
	if (_curCostSoul > _curSoul)
		return;
	// 发送复活消息到服务器
	Connect* handle = Connect::getInstance();
	handle->loadSend("c=hero&a=revive", this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_ReviveSoul));
}
// 服务器回调>>>>	英雄之混复活回调
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
		// 扣除
		auto _curCostSoul = ALTAR_REVIVE_SOUL_COST;
		int _curSoul = UserRecord::getInstance()->getHeroSoul();
		UserRecord::getInstance()->alterHeroSoul(-_curCostSoul);
		UserRecord::getInstance()->flush();
		// 将复活状态写入文件
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_isDead, NO);
		// 改写状态
		mHeroItems.at(mCurListViewIndex).isDead = NO;
		// 删除墓碑，复活英雄
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
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so ReviveSoul Failure!!!");

		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 按钮功能		解锁英雄
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
	// 扣除
	if (_curUnLockTypeValue == 1)
	{
		auto _curGold = UserRecord::getInstance()->getGold();
		if (_curUnLockCostValue > _curGold)	// 金钱不足
			return;
	}else if (_curUnLockTypeValue == 2)	
	{
		auto _curSoul = UserRecord::getInstance()->getHeroSoul();
		if (_curUnLockCostValue > _curSoul)	// 英雄之魂不足
			return;
	}else if (_curUnLockTypeValue == 3)		// 人民币
	{
		__Dictionary* heroDic = PlistManager::getInstance()->getObjectPlist("BossData", _curHeroData.keyName.c_str());
		auto payid   = static_cast<__String*>(heroDic->objectForKey("payid"))->getCString();
		if (strcmp(payid,"0")!=0){
			mobilePay(payid,StringUtils::format("%d",UserRecord::getInstance()->getUid()).c_str());
		}
		return;
	}
	// 发送成功消息到服务器
	auto postDatas = __String::createWithFormat("c=hero&a=unlock&hero=%d",_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UnLockHero));
}

void Ui_HeroAltar::buyHeroSuccessCallFuc(Ref* ref)
{
	this->runAction(Sequence::create(DelayTime::create(0.5f),CallFunc::create([=](){
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		// 发送成功消息到服务器
		auto postDatas = __String::createWithFormat("c=hero&a=unlock&hero=%d",_curHeroData.id)->getCString();
		Connect* handle = Connect::getInstance();
		handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UnLockHero));
	}),NULL));
}

// 服务器回调>>>>	解锁英雄回调
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
		// 扣除金钱
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
		// 改写状态
		mHeroItems.at(mCurListViewIndex).unLock = YES;
		mHeroItems.at(mCurListViewIndex).isDead = NO;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_unLock, YES);
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_isDead, NO);
		
		
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
		//新手引导教学
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UnLockHero Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 升级英雄等级
void Ui_HeroAltar::buttonCallBack_UpHeroLevel()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	auto _curCostGold =  PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "upgradeMoney").intValue();
	if(_curCostGold < 0)
		return;
	int _curGold = UserRecord::getInstance()->getGold();
	if (_curCostGold > _curGold)	// 金钱不足
		return;
	if (_curHeroData.level >= TYPE_BOSS_MAX_LEVEL)	// 等级超出
		return;
	// 发送成功消息到服务器
	
	auto postDatas  = __String::createWithFormat("c=hero&a=uplevel&hero=%d",_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroLevel));
} 
// 服务器回调>>>>	升级英雄等级回调
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
		// 扣除
		auto _curCostGold =  PlistManager::getInstance()->getAttributeCCString("BossData", _curHeroData.keyName.c_str(), _curHeroData.level, "upgradeMoney").intValue();
		int _curGold = UserRecord::getInstance()->getGold();
		UserRecord::getInstance()->alterGold(-_curCostGold);
		UserRecord::getInstance()->flush();
		// 改写状态
		int _nowLevel = mHeroItems.at(mCurListViewIndex).level;
		mHeroItems.at(mCurListViewIndex).level = _nowLevel + 1;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_level, mHeroItems.at(mCurListViewIndex).level);
		// 播放特效
		beginAnim_UpLevel(_curHeroData.id);
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
		//新手引导教学
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroLevel Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 升级英雄技能等级
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
	if (_curCostGold > _curGold)	// 金钱不足
		return;
	if (_curSkillLevel >= HeroSkillMaxLevel)	// 等级超出
		return;
	// 发送成功消息到服务器
	auto postDatas = __String::createWithFormat("c=hero&a=upskill&skill=%d&hero=%d",
		_newIndex,
		_curHeroData.id)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UpHeroSkillLevel));
} 
// 服务器回调>>>>	升级英雄技能等级回调
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
		// 扣除
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
		// 改写状态
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
		// 播放特效
		beginAnim_UpSkillLevel(mCurSkillIndex);
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
		//新手引导教学
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroSkillLevel Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 使用祝福蛋
void Ui_HeroAltar::buttonCallBack_UseBlessingItem()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _curCostBlessingItem = 1;
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30002");
	if (_curCostBlessingItem > _curCount)	// 祝福蛋不足
		return;
	if (_curHeroData.tempBlessing >= 1000)		// 临时祝福值满值
		return;
	if (_curHeroData.blessing >= 1000)			// 祝福值满值
		return;
	// 计算新成功率
	int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);
	float _newSuccessRate = _curHeroData.successRate + _onceSlideAddSuccessRate;
	// 发送成功消息到服务器

	auto postDatas = __String::createWithFormat("c=hero&a=blessing&hero=%d&successRate=%2f",
		_curHeroData.id,
		_newSuccessRate
		)->getCString();
	Connect* handle = Connect::getInstance();
	handle->loadSend(postDatas, this, httpresponse_selector(Ui_HeroAltar::onHttpRequestCompleted_UseBlessingItem));
} 
// 服务器回调>>>>	使用祝福蛋回调
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
		// 扣除
		int _curCostBlessingItem = 1;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30002");
		UserRecord::getInstance()->alterGoodsCount("30002", -_curCostBlessingItem);
		UserRecord::getInstance()->flush();
		// 改写状态祝福值
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		mHeroItems.at(mCurListViewIndex).blessing = _nowBlessing + 100;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		// 改写状态成功率
		int _onceSlideAddSuccessRate = getSlideAddSuccessRate(_curHeroData.star);
		float _newSuccessRate = _curHeroData.successRate + _onceSlideAddSuccessRate;
		mHeroItems.at(mCurListViewIndex).successRate = _newSuccessRate;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		// 改变特效状态
		mIsSliderAnim = true;
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UseBlessingItem Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 升级英雄星级
void Ui_HeroAltar::buttonCallBack_UpHeroStar()
{
	HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
	if (_curHeroData.id <= 0)
		return;
	int _curCostCount = _curHeroData.successCost;
	int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
	if (_curCostCount > _curCount)			// 进阶蛋不足
		return;
	if (_curHeroData.star >= HeroSkillMaxLevel)
		return;

	// 几率计算
	int _curMaxRand = _curHeroData.successRate * 100;
	int _curRandSize = 100000;
	int _curRand = UserRecord::getInstance()->getRandom(_curRandSize);                     
	bool isOk = false;       
	if (_curRand > 0 && _curRand <= _curMaxRand)
	{
		isOk = true;
	}
	if (_curMaxRand >= 10000)	// 如果成功率是100，就把成功概率提升到100%
	{
		isOk = true;
	}
	if (UserRecord::getInstance()->getIsFreshMan()){//如果是教学则将成功概率提升到100%
		isOk = true;
	}
	//CCLOG(">>> _curHeroData.successRate:%f, _curRand %d < %d",_curHeroData.successRate, _curRand, _curMaxRand);
	// 成功
	if (isOk)
	{
		int nextStar = _curHeroData.star + 1;
		float nextStarSuccess = 0;
		if (nextStar <= HeroSkillMaxLevel)
		{
			nextStarSuccess = getSuccessRate(_curHeroData.star + 1);
		}	
		// 发送成功消息到服务器
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
	// 失败
	else
	{
		// 失败增加祝福值
		int _addBlessing = getFailureAddBlessing(_curHeroData.star);
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		int _resultBlessing = _nowBlessing + _addBlessing;
		if (_resultBlessing > 1000)
			_resultBlessing = 1000;
		// 失败增加成功率
		float _addFailureAddSuccessRate = getFailureAddSuccessRate(_curHeroData.star);
		float _curSuccessRate = _curHeroData.successRate + _addFailureAddSuccessRate;
		// 发送成功消息到服务器
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
// 服务器回调>>>>	升级英雄星级_成功回调
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
		// 播放音效_成功
		audioPlayer::getInstance()->playEffect("S11.mp3");
		int nextStar = _curHeroData.star + 1;
		float nextStarSuccess = 0;
		if (nextStar <= HeroSkillMaxLevel)
		{
			nextStarSuccess = getSuccessRate(_curHeroData.star + 1);
		}
		// 扣除
		int _curCostCount = _curHeroData.successCost;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
		UserRecord::getInstance()->alterGoodsCount("30001", -_curCostCount);
		UserRecord::getInstance()->flush();
		// 改写状态 祝福值
		mHeroItems.at(mCurListViewIndex).blessing = 0;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		// 改写状态 星级
		int _nowStar = mHeroItems.at(mCurListViewIndex).star;
		mHeroItems.at(mCurListViewIndex).star = _nowStar + 1;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_star, mHeroItems.at(mCurListViewIndex).star);
		// 改写状态 成功率
		mHeroItems.at(mCurListViewIndex).successCost = 0;
		mHeroItems.at(mCurListViewIndex).successRate = nextStarSuccess;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		// 播放特效
		beginAnim_UpLevel(_curHeroData.id);
		// 播放升星特效
		beginAnim_UpStar();

		//新手引导教学(进阶按钮)
		if (UserRecord::getInstance()->getIsFreshMan()){
			UserRecord::getInstance()->setFreshManMutex(false);
		}
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroStar_Success Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}
// 服务器回调>>>>	升级英雄星级_失败回调
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
		// 播放音效_失败
		audioPlayer::getInstance()->playEffect("S11_2.mp3");
		// 扣除
		int _curCostCount = _curHeroData.successCost;
		int _curCount = UserRecord::getInstance()->getGoodsCountByID("30001");
		UserRecord::getInstance()->alterGoodsCount("30001", -_curCostCount);
		UserRecord::getInstance()->flush();
		// 改写状态 失败增加祝福值
		int _addBlessing = getFailureAddBlessing(_curHeroData.star);
		int _nowBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		int _resultBlessing = _nowBlessing + _addBlessing;
		if (_resultBlessing > 1000)
			_resultBlessing = 1000;
		mHeroItems.at(mCurListViewIndex).blessing = _resultBlessing;
		mHeroItems.at(mCurListViewIndex).tempBlessing = mHeroItems.at(mCurListViewIndex).blessing;
		UserRecord::getInstance()->setHeroKeyValue(_curHeroData.id, _HeroDataKey_Index_blessing, mHeroItems.at(mCurListViewIndex).blessing);
		//CCLOG("shibai add _resultBlessing:%d, tempBlessing:%d", _resultBlessing, mHeroItems.at(mCurListViewIndex).blessing);
		// 改写状态 失败增加成功率	
		float _addFailureAddSuccessRate = getFailureAddSuccessRate(_curHeroData.star);
		float _curSuccessRate = _curHeroData.successRate + _addFailureAddSuccessRate;
		mHeroItems.at(mCurListViewIndex).successRate = _curSuccessRate;
		UserRecord::getInstance()->setHeroKeyValue_Float(_curHeroData.id, _HeroDataKey_Index_successRate, mHeroItems.at(mCurListViewIndex).successRate);
		//CCLOG("shibai add successRate:%f", mHeroItems.at(mCurListViewIndex).successRate);
		// 改变特效状态
		mIsSliderAnim = true;
		// 更新显示
		updateHeroView(mCurListViewIndex);
		NotificationCenter::sharedNotificationCenter()->postNotification(RESETBOSSSKILL, NULL);//更新英雄的属性
	}else
	{
		// 失败
		CCLOG("beacuse you RenPin < 0 !!!  so UpHeroStar_Failure Failure!!!");
		// 失败
		Ui_Prompt *prompt = Ui_Prompt::create();
		prompt->setTag(251);
		prompt->setPosition(Director::getInstance()->getVisibleOrigin());
		prompt->initWithOneButton(v["info"].GetString() );
		//prompt->setRightSelector(this,comm_selector(Ui_Shop::goldNoEnoughCallFunc));
		Director::getInstance()->getRunningScene()->addChild(prompt,300);
	}
}


// ---------------- 特效动画 ---------------------
// Slider特效开始
void Ui_HeroAltar::beginAnim_Slider()
{
	if(!mSliderArmature)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("hero_wu_Effects_020.png","hero_wu_Effects_020.plist","hero_wu_Effects_02.ExportJson"); 
		mSliderArmature = cocostudio::Armature::create("hero_wu_Effects_02"); 
		mSliderArmature->setScale(1/0.95);//在导出骨骼动画的时候被缩放到0.95
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
// Slider特效结束
void Ui_HeroAltar::endAnim_Slider(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		Widget* _thisParent = static_cast<Widget*>(mSliderArmature->getParent());
		if (_thisParent)
		{
			_thisParent->removeChildByTag(35);
		}
		// 播放动画2
		mSliderArmature->getAnimation()->stop();
		mSliderArmature->setVisible(false);
		mIsSliderAnim = false;
	}
}
// 升级特效开始
void Ui_HeroAltar::beginAnim_UpLevel(int _heroId)
{
	if(!mLevelArmature)
	{
		ArmatureDataManager::getInstance()->addArmatureFileInfo("hero_wu_Effects_010.png","hero_wu_Effects_010.plist","hero_wu_Effects_01.ExportJson"); 
		mLevelArmature = cocostudio::Armature::create("hero_wu_Effects_01");   
		mLevelArmature->setScale(1.25);//在导出骨骼的时候缩小到了0.8
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
// 升级特效结束
void Ui_HeroAltar::endAnim_UpLevel(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"finish")==0)
	{
		mLevelArmature->getAnimation()->stop();
		mLevelArmature->setVisible(false);
	}
}
// 技能升级特效开始
void Ui_HeroAltar::beginAnim_UpSkillLevel(int _skillIndex)
{
	if(!mSkillLevelSprite)
	{
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("hero_wu_Effects_03.plist");
		mSkillLevelSprite = Sprite::createWithSpriteFrameName("hero_wu_Effects_03_update_01.png");
		mSkillLevelSprite->setScale(1.25);//因为合成大图的时候缩小到0.8
		mImageSkillSelect->addChild(mSkillLevelSprite, 30);
		mSkillLevelSprite->setPositionX(mImageSkillSelect->getSize().width/2);
		mSkillLevelSprite->setPositionY(mImageSkillSelect->getSize().height/2);
	}
	if (mSkillLevelSprite)
	{
		// 播放动画
		mSkillLevelSprite->setVisible(true);
		Animate* ani = createAnimation("hero_wu_Effects_03_update_", 12);
		auto action = Sequence::create(ani,
			CallFuncN::create(CC_CALLBACK_1(Ui_HeroAltar::endAnim_UpSkillLevel, this)),
			NULL);
		mSkillLevelSprite->runAction(action);
	}
}
// 技能升级特效结束
void Ui_HeroAltar::endAnim_UpSkillLevel(Ref *sender)
{
	if (mSkillLevelSprite)
	{
		mSkillLevelSprite->setVisible(false);
	}
}
// 创建序列帧动画
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
// 升级星级特效 开始
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
		// 显示挡板
		Ui_SwallowAllTouchEvents* swallowUi = Ui_SwallowAllTouchEvents::create();
		swallowUi->setPosition(Director::getInstance()->getVisibleOrigin());
		swallowUi->setTag(123);
		swallowUi->setBackGroundColorOpacity(200);
		this->addChild(swallowUi, 10);
		// 播放动画
		mStarArmature->getAnimation()->play("play",-1,0);
		mStarArmature->getAnimation()->setFrameEventCallFunc(this,frameEvent_selector(Ui_HeroAltar::endAnim_UpStar));
	}
}
// 升级星级特效 结束
void Ui_HeroAltar::endAnim_UpStar(cocostudio::Bone *bone, const std::string& evt, int originFrameIndex, int currentFrameIndex)
{
	if (strcmp(evt.c_str(),"comeout")==0)
	{
		// 取当前选中的英雄数据
		HeroItemInfo _curHeroData = mHeroItems.at(mCurListViewIndex);
		if (_curHeroData.id <= 0)
			return;
		// 显示头像
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

	// 同步当前位置
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

// 更新升级提示标签
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
				// 英雄升级 是否提示
				bool isTag_Level = false;
				auto _costGold_upLevel =  _curHeroData.heroLevelUpCost;
				if (_curHeroData.unLock == YES && _curHeroData.isDead == NO && _curHeroData.level < TYPE_BOSS_MAX_LEVEL && _curGold >= _costGold_upLevel)
				{
					isTag_Level = true;
				}
				// 英雄技能升级 是否提示
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
				// 解锁提示
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
					// 显示提示
					_tagCtrl->setVisible(true);
				}else
				{
					// 不显示
					_tagCtrl->setVisible(false);
				}
			}
		}
	}
}
