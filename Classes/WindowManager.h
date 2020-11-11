#pragma once
#include "cocos2d.h"
#include "GameConfig.h"
#include "Ui_MainBack.h"
#include "Ui_MainLayer.h"
#include "Ui_NextRound.h"
#include "Ui_GameOver.h"
#include "Ui_Shop.h"
#include "Ui_Prompt.h"
#include "Ui_HeroAltar.h"
#include "Ui_HeroSelect.h"

typedef cocos2d::Map<std::string, Window*> WindowMapType;

class WindowManager : 
	public cocos2d::Ref
{
public:
	static WindowManager* getInstance();
	void destroyInstance();

	WindowManager(void);
	~WindowManager(void);

	bool			initialise(Layer* _gameLayer);

	void			setGameLayer(Layer* _gameLayer);
	Layer*			getGameLayer();

	WindowMapType	getWindowMap();
	Window*			getWindow(const char* _windowName);
	void			addWindow(const char* _windowName, Window* _window);
	void			removeWindow(const char* _windowName);
	void			reloadWindow();

	GameObjects*	createEnemyObject(int otype, Point pos);	// 创建敌人
	GameObjects*	createBossObject(int otype,Point pos);		// 创建BOSS
	GameObjects*	createTowerObject(int otype,Point pos);		// 创建塔
	GameObjects*	createWarriorObject(int otype,Point pos);	// 创建勇士
	GameObjects*	createDoorObject(DoorType otype,Point pos);		// 创建关卡门
	GameObjects*    createTrapObject(int otype,Point pos);      //创建陷阱
	
	void			update(float dt);

	cocostudio::Armature *	CreateAnimation(const std::string& _fileName, const std::string& _file2Name, const std::string& _actionName, Point _pos, bool _isLoop);

	// window event
	void onWindowTouchEvent(Touch* _touch, Event* _unusedEvent, int _touchType, Point _touchPos, Point _touchNodePos);

	void createGameOverFrame(bool win,float time=2.0f);

	void createShopFrame(ShopItem idx=SweetItem);
	void createHeroAltar();
	Widget* getHeroAltar();
	Widget* createHeroSelect(int _storeyIndex, bool _isFree = false, bool _isDoEvent = false);
	void createHeroToHeroSelect(int _storeyIndex, int _heroType, Point _heroPos, int _heroLevel);

	void setAllChildPause(Widget* _widget, bool _isPause);

	void setSceneViewEnable(bool _isPause, int _storeyIndex);

	float getMainLayerBottomHeight();

	void setSwallowAllUi(bool bvar);

	float getMainBackScrollViewHeight();		// 取MainBack滚动条滚动的高度
	Widget* getMainLayerMagicWidget();			// 取魔法按钮
	void updateMagicNum();						// 刷新魔法数量
	void setTextureAlias(ImageView* _image);	// 设置纹理采样方式
public:
	Layer*				mGameLayer;
	WindowMapType		mWindowsMap; 
};

