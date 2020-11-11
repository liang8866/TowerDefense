#include "WindowManager.h"
#include "Ui_SwallowAllTouchEvents.h"


static WindowManager* mWindowManager = nullptr;

WindowManager* WindowManager::getInstance()
{
	if (!mWindowManager)
	{
		mWindowManager = new WindowManager();
	}
	return mWindowManager;
}
void WindowManager::destroyInstance()
{
	CC_SAFE_DELETE(mWindowManager);
};
WindowManager::WindowManager(void)
{
	
}
WindowManager::~WindowManager(void)
{
	
}

bool WindowManager::initialise(Layer* _gameLayer)
{
	if (!_gameLayer)
		return false;

	mGameLayer = _gameLayer;

	if (UserRecord::getInstance()->getSectionNum() == 1)
	{
		addWindow("BackGround", Ui_MainBack::create("Back/Back.ExportJson"));			// 主游戏背景
	} else if(UserRecord::getInstance()->getSectionNum() == 2) {
		addWindow("BackGround", Ui_MainBack::create("Back_2/Back_2.ExportJson"));			// 主游戏背景
	} else if(UserRecord::getInstance()->getSectionNum() == 3) {
		addWindow("BackGround", Ui_MainBack::create("Back_3/Back_3.ExportJson"));			// 主游戏背景
	}else{
		addWindow("BackGround", Ui_MainBack::create("Back_3/Back_3.ExportJson"));			// 主游戏背景
	}
	addWindow("Ui_MainLayerWnd",Ui_MainLayer::create("Ui_MainScene_1/Ui_MainScene_1.ExportJson"));       
	reloadWindow();
	return true;
}
void WindowManager::setGameLayer(Layer* _gameLayer)
{
	if (!_gameLayer)
		return;
	mGameLayer = _gameLayer;
}
Layer* WindowManager::getGameLayer()
{
	return mGameLayer;
}

WindowMapType WindowManager::getWindowMap()
{
	return mWindowsMap;
}
Window* WindowManager::getWindow(const char* _windowName)
{
	return mWindowsMap.at(StringUtils::toString(_windowName));
}
void WindowManager::addWindow(const char* _windowName, Window* _window)
{
	mWindowsMap.insert(StringUtils::toString(_windowName), _window);
}
void WindowManager::removeWindow(const char* _windowName)
{
	mWindowsMap.erase(StringUtils::toString(_windowName));
}

void WindowManager::reloadWindow()
{
	if (!mGameLayer)
		return;
	
	WindowMapType::iterator iter;
	for( iter = mWindowsMap.begin(); iter != mWindowsMap.end(); ++iter)
	{
		mGameLayer->addChild(iter->second);
	}
}
// 创建敌人
GameObjects* WindowManager::createEnemyObject(int otype, Point pos)
{
	GameObjects* _object = nullptr;
	
	switch (otype)
	{
	case ENEMY_ITEM1:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_01.ExportJson","td_enemy_01", otype, mGameLayer));
		break;
	case ENEMY_ITEM2:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_02.ExportJson","td_enemy_02", otype, mGameLayer));
		break;
	case ENEMY_ITEM3:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_03.ExportJson","td_enemy_03", otype, mGameLayer));
		break;
	case ENEMY_ITEM4:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_04.ExportJson","td_enemy_04", otype, mGameLayer));
		break;
	case ENEMY_ITEM5:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_05.ExportJson","td_enemy_05", otype, mGameLayer));
		break;
	case ENEMY_ITEM6:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_06.ExportJson","td_enemy_06", otype, mGameLayer));
		break;
	case ENEMY_ITEM7:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_07.ExportJson","td_enemy_07", otype, mGameLayer));
		break;
	case ENEMY_ITEM8:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_08.ExportJson","td_enemy_08", otype, mGameLayer));
		break;
	case ENEMY_ITEM9:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_09.ExportJson","td_enemy_09", otype, mGameLayer));
		break;
	case ENEMY_ITEM10:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_10.ExportJson","td_enemy_10", otype, mGameLayer));
		break;
	case ENEMY_ITEM11:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_11.ExportJson","td_enemy_11", otype, mGameLayer));
		break;
	case ENEMY_ITEM12:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_12.ExportJson","td_enemy_12", otype, mGameLayer));
		break;
	case ENEMY_ITEM13:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_13.ExportJson","td_enemy_13", otype, mGameLayer));
		break;
	case ENEMY_ITEM14:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_14.ExportJson","td_enemy_14", otype, mGameLayer));
		break;
	case ENEMY_ITEM15:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_15.ExportJson","td_enemy_15", otype, mGameLayer));
		break;
	case ENEMY_ITEM16:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_16.ExportJson","td_enemy_16", otype, mGameLayer));
		break;
	case ENEMY_ITEM17:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_17.ExportJson","td_enemy_17", otype, mGameLayer));
		break;
	case ENEMY_ITEM18:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_18.ExportJson","td_enemy_18", otype, mGameLayer));
		break;
	case ENEMY_ITEM19:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_19.ExportJson","td_enemy_19", otype, mGameLayer));
		break;
	case ENEMY_ITEM20:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_20.ExportJson","td_enemy_20", otype, mGameLayer));
		break;
	case ENEMY_ITEM21:
		ArmatureDataManager::getInstance()->addArmatureFileInfo("td_enemy_210.png","td_enemy_210.plist","td_enemy_21.ExportJson");
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_21.ExportJson","td_enemy_21", otype, mGameLayer));
		break;
	default:
		_object = static_cast<GameObjects*>(enemyObjects::create("td_enemy_01.ExportJson","td_enemy_01", otype, mGameLayer));
		break;
	}
	
	_object->setPosition(pos);
	return _object;
}
// 创建BOSS
GameObjects* WindowManager::createBossObject(int otype,Point pos)
{
	GameObjects*  _object = nullptr;
	if (otype==BOSS_ITEM1)// 	    近程攻击的士兵 
	{  
		 _object = static_cast<GameObjects*>(bossObjects::create("td_boss_01.ExportJson","td_boss_01", otype, mGameLayer));
	} else if(otype==BOSS_ITEM2)//近程攻击
	{
		 _object = static_cast<GameObjects*>(bossObjects::create("td_boss_02.ExportJson","td_boss_02", otype, mGameLayer));
		 pos=Point(pos.x,pos.y);
	}
	if (otype==BOSS_ITEM3)//治疗
	{
		 _object = static_cast<GameObjects*>(bossObjects::create("td_boss_03.ExportJson","td_boss_03", otype, mGameLayer));
		  pos=Point(pos.x,pos.y);
	}
	if (otype==BOSS_ITEM4)//肉盾
	{
		 _object = static_cast<GameObjects*>(bossObjects::create("td_boss_04.ExportJson","td_boss_04", otype, mGameLayer));
		  pos=Point(pos.x,pos.y);
	}
	if (otype==BOSS_ITEM5)//召唤
	{
		_object = static_cast<GameObjects*>(bossObjects::create("td_boss_05.ExportJson","td_boss_05", otype, mGameLayer));
		pos=Point(pos.x,pos.y);
	}

	_object->setPosition(pos);
	return _object;
}


GameObjects*   WindowManager::createTrapObject(int otype,Point pos)
{
	GameObjects*  _object = nullptr;
	if (otype==TRAP_IETM1)//攻击陷阱
	{  
		_object=trapObjects::create("td_trap_01_a.ExportJson","td_trap_01_a",otype,"td_trap_01_");

		/*pos=Point(pos.x,pos.y-4);*/
	} else if(otype==TRAP_IETM2)//固定目标陷阱
	{
	_object=trapObjects::create("td_trap_02_a.ExportJson","td_trap_02_a",otype,"td_trap_02_");
	}
	else if (otype==TRAP_IETM3)//手动引爆陷阱
	{
		_object=trapObjects::create("td_trap_03_a.ExportJson","td_trap_03_a",otype,"td_trap_03_");
	}
	else if (otype==TRAP_IETM4)//弹到上一层
	{
		_object=trapObjects::create("td_trap_04_a.ExportJson","td_trap_04_a",otype,"td_trap_04_");
	}
	else if (otype==TRAP_IETM5)//减速陷阱
	{
		_object=trapObjects::create("td_trap_05_a.ExportJson","td_trap_05_a",otype,"td_trap_05_");
	}


	_object->setPosition(pos);
	return _object;

}

// 创建塔
GameObjects* WindowManager::createTowerObject(int otype,Point pos)
{
	GameObjects* _object;
	switch (otype)
	{
	case 1:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_01_a.ExportJson","td_tower_01_a",otype,mGameLayer));
		break;
	case 2:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_02_a.ExportJson","td_tower_02_a",otype,mGameLayer));
		break;
	case 3:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_03_a.ExportJson","td_tower_03_a",otype,mGameLayer));
		break;
	case 4:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_04.ExportJson","td_tower_04",otype,mGameLayer));
		break;
	case 5:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_05_a.ExportJson","td_tower_05_a",otype,mGameLayer));
		break;
	default:
		_object = static_cast<GameObjects*>(towerObjects::create("tower/td_tower_01_a.ExportJson","td_tower_01_a",otype,mGameLayer));
		break;
		break;
	}
	
	_object->setAnchorPoint(Point::ZERO);
	_object->setPosition(pos);
	return _object;

	//GameObjects* _object = static_cast<GameObjects*>(bossObjects::create("td_boss_01.ExportJson","td_boss_01", otype, mGameLayer));
	//_object->setPosition(pos);
	//return _object;
}
// 创建勇士
GameObjects* WindowManager::createWarriorObject(int otype,Point pos)
{
// 	    近程攻击的士兵 1
// 		远程攻击的士兵 2
// 		飞行攻击的士兵 3
// 		自爆攻击的士兵 4
//       吓人的狗      5
	
	GameObjects*_object=nullptr;
	if (otype==WAR_ITEM1)// 	    近程攻击的士兵 
	{  
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_01_a.ExportJson","td_war_01_a",otype,mGameLayer));
		
	}else if (otype==WAR_ITEM2)// 		远程攻击的士兵 2
	{
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_02_a.ExportJson","td_war_02_a",otype,mGameLayer));
	}
	else if (otype==WAR_ITEM3)// 		飞行攻击的士兵 4
	{
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_03_a.ExportJson","td_war_03_a",otype,mGameLayer));
	}else if (otype==WAR_ITEM4)// 		自爆攻击的士兵 5
	{
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_04.ExportJson","td_war_04",otype,mGameLayer));
	}
	else if (otype==WAR_ITEM5)// 		吓人狗
	{
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_05_a.ExportJson","td_war_05_a",otype,mGameLayer));
	}

	else {		
		_object = static_cast<GameObjects*>(warriorObjects::create("td_war_01_a.ExportJson","td_war_01_a",otype,mGameLayer));
	}

	_object->setPosition(pos);
	_object->setfloorNum(FLOOR_NUM1);
	return _object;
}
// 创建关卡门
GameObjects* WindowManager::createDoorObject(DoorType otype, Point pos)
{
	GameObjects* _object = static_cast<GameObjects*>(doorObject::create(otype));
	if (_object)
	{
		_object->setPosition(pos);
		return _object;
	}
	return nullptr;
}

cocostudio::Armature* WindowManager::CreateAnimation(const std::string& _fileName, const std::string& _file2Name, const std::string& _actionName, Point _pos, bool _isLoop)
{
	ArmatureDataManager::getInstance()->addArmatureFileInfo(_fileName); 
	cocostudio::Armature* _armature = Armature::create(_file2Name);
	if (_armature)
	{
		int _curLoop = -1;
		if (_isLoop)
			_curLoop = 1;

		_armature->setPosition(_pos);
		_armature->getAnimation()->play(_actionName, -1, _curLoop);
		return _armature;
	}
	return nullptr;
}
void WindowManager::update(float dt)
{

}


void WindowManager::onWindowTouchEvent(Touch* _touch, Event* _unusedEvent, int _touchType, Point _touchPos, Point _touchNodePos)
{
	switch (_touchType)
	{
	case WINDOW_TOUCH_TYPE_BEGIN:
		break;
	case WINDOW_TOUCH_TYPE_MOVE:
		break;
	case WINDOW_TOUCH_TYPE_END:
		break;
	case WINDOW_TOUCH_TYPE_CANCEL:
		break;
	default:
		break;
	}


	/*WindowMapType::iterator iter;
	for( iter = mWindowsMap.begin(); iter != mWindowsMap.end(); ++iter)
	{
		mGameLayer->addChild(iter->second);
	}*/
}

void WindowManager::createGameOverFrame(bool win,float time)
{
	audioPlayer::getInstance()->stopAllEffects();
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(WindowManager::getInstance()->getWindow("Ui_MainLayerWnd"));
	mainLayer->setPauseBtnGray();

	if ( mGameLayer->getChildByTag(123) )
		return;

	// 清空临时魔法
	std::map<int, int>::iterator iter;
	iter = UserRecord::getInstance()->mTemporaryMagic.begin();
	while (iter != UserRecord::getInstance()->mTemporaryMagic.end())
	{
		if (iter->second > 0)
		{
			// 清空实时数据
			UserRecord::getInstance()->alterGoodsCount(Value(iter->first).asString(), -iter->second);
			// 清空临时数据
			iter->second = 0;
		}
		++iter;
	}
	UserRecord::getInstance()->clearTemporaryMagic();


	if (WindowManager::getInstance()->mGameLayer->getChildByTag(250)){
		WindowManager::getInstance()->mGameLayer->removeChildByTag(250);
	}

	Point originPoint = Director::getInstance()->getVisibleOrigin();
	auto gameOver = Ui_GameOver::create();
	gameOver->setPosition(Point(originPoint.x,originPoint.y));
	gameOver->setTag(123);
	mGameLayer -> addChild(gameOver,100);

	if (win)
	{
		gameOver->winTheGame(time);
	}else{
		gameOver->loseTheGame();
	}

	if (!win && UserRecord::getInstance()->getIsFreshMan()){
		auto parent = mGameLayer->getParent();
		auto child  = parent->getChildByTag(99999);
		if (child){
			parent->removeChild(child);
		}
	}
} 

void WindowManager::createShopFrame(ShopItem idx)
{
	if ( !mGameLayer || mGameLayer->getChildByTag(124) )
		return;
	
	Ui_Shop* shopLayer = Ui_Shop::create();
	shopLayer  -> setTag(124);
	mGameLayer -> addChild(shopLayer,101);
	shopLayer  -> selectIndex(idx);
}

void WindowManager::createHeroAltar()
{
	if ( mGameLayer->getChildByTag(125) )
		return;

	Ui_HeroAltar* _layer = Ui_HeroAltar::create();
	_layer  -> setTag(125);
	mGameLayer -> addChild(_layer,100);
	_layer->createView(true);
}
Widget* WindowManager::getHeroAltar()
{
	Widget* thisWnd = static_cast<Widget*>(mGameLayer->getChildByTag(125));
	if (thisWnd)
	{
		return thisWnd;
	}
	return nullptr;
}

void WindowManager::setAllChildPause(Widget* _widget, bool _isPause)
{
	for (auto _curChild : _widget->getChildren())
	{
		Widget* _curChildWidget = static_cast<Widget*>(_curChild);
		if (_curChildWidget)
		{
			if (_isPause)
			{
				_curChildWidget->pause();
			}else
			{
				_curChildWidget->resume();
			}
		}
		if (_curChildWidget->getChildrenCount() > 0)
		{
			setAllChildPause(_curChildWidget, _isPause);
		}
	}
}

void WindowManager::setSceneViewEnable(bool _isPause, int _storeyIndex)
{
	Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(getWindow("BackGround"));
	if (mainBack)
	{
		if (_storeyIndex && _storeyIndex > 0)
		{
			mainBack->updateCenterStorey(_storeyIndex);
		}
		if (_isPause)
		{
			mainBack->mScrollViewWidget->getInnerContainer()->setTouchEnabled(false);
			mainBack->mScrollViewWidget->setInertiaScrollEnabled(false);
			mainBack->mScrollViewWidget->setScrollEnbled(false);

			mainBack->mListViewWidget->pause();
			mainBack->mListViewWidget->getInnerContainer()->pause();
			mainBack->mListViewWidget->getInnerContainer()->setTouchEnabled(false);
			mainBack->mListViewWidget->setInertiaScrollEnabled(false);
		}else
		{
			mainBack->mScrollViewWidget->getInnerContainer()->setTouchEnabled(true);
			mainBack->mScrollViewWidget->setInertiaScrollEnabled(true);
			mainBack->mScrollViewWidget->setScrollEnbled(true);

			mainBack->mListViewWidget->getInnerContainer()->setTouchEnabled(true);
			mainBack->mListViewWidget->setInertiaScrollEnabled(true);
		}
	}
}


Widget* WindowManager::createHeroSelect(int _storeyIndex, bool _isFree, bool _isDoEvent)
{
	if ( mGameLayer->getChildByTag(126) )
		return nullptr;

	Ui_HeroSelect* _layer = Ui_HeroSelect::create();
	_layer->setTag(126);
	mGameLayer -> addChild(_layer,99);
	_layer->showView(_storeyIndex, true, _isFree);
	if (_isDoEvent)
	{
		_layer->setIsDoEvent(true);
	}
	return _layer;
}
void WindowManager::createHeroToHeroSelect(int _storeyIndex, int _heroType, Point _heroPos, int _heroLevel)
{
	Ui_MainBack *mainBack = static_cast<Ui_MainBack*>(getWindow("BackGround"));
	if (mainBack)
	{
		mainBack->createHeroToStorey(_storeyIndex, _heroType, _heroPos, _heroLevel);
	}
}

float WindowManager::getMainLayerBottomHeight()
{
	Ui_MainLayer *mainLayer = static_cast<Ui_MainLayer*>(getWindow("Ui_MainLayerWnd"));
	Widget* bottom          = Helper::seekWidgetByName(mainLayer->mMainWidget, "bottom_bg");

	return bottom->getSize().height; //112 pix
}


void WindowManager::setSwallowAllUi(bool bvar)
{
	if (bvar){
		if (mGameLayer->getChildByTag(123654))
			return;

		Ui_SwallowAllTouchEvents* swallowUi = Ui_SwallowAllTouchEvents::create();
		swallowUi->setPosition(Director::getInstance()->getVisibleOrigin());
		swallowUi->setTag(123654);
		mGameLayer->addChild(swallowUi);

	}else{
		if (mGameLayer->getChildByTag(123654))
			mGameLayer->removeChildByTag(123654);
	}
}
// 取MainBack滚动条滚动的高度
float WindowManager::getMainBackScrollViewHeight()
{
	float _height = 0;
	Ui_MainBack* _window = static_cast<Ui_MainBack*>(getWindow("BackGround"));
	if (_window)
	{
		_height = _window->mScrollViewWidget->getInnerContainer()->getPositionY();
	}
	return _height;
}
// 取魔法按钮
Widget* WindowManager::getMainLayerMagicWidget()
{
	Widget* _widget = nullptr;
	Ui_MainLayer* _window = static_cast<Ui_MainLayer*>(getWindow("Ui_MainLayerWnd"));
	if (_window)
	{
		_widget = _window->m_AddMagicBtn;
	}
	return _widget;
}
// 刷新魔法数量
void WindowManager::updateMagicNum()
{
	Widget* _widget = nullptr;
	Ui_MainLayer* _window = static_cast<Ui_MainLayer*>(getWindow("Ui_MainLayerWnd"));
	if (_window)
	{
		_window->refreshMagic("20001");
		_window->refreshMagic("20002");
		_window->refreshMagic("20003");
		_window->refreshMagic("20004");
	}
}
// 设置纹理采样方式
void WindowManager::setTextureAlias(ImageView* _image)
{
	if (!_image)
		return;
	Sprite* _imageSp = static_cast<Sprite*>(_image->getVirtualRenderer());
	if (_imageSp)
	{
		_imageSp->getTexture()->setAliasTexParameters();
	}
}





